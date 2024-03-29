/*
 *  Project:    moba-svg
 *
 *  Copyright (C) 2016 Stefan Paproth <pappi-@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include <iostream>

#include "layoutparser.h"
#include "common.h"

void LayoutParser::collectTrackPoints(Position pos, Direction dir) {
    std::vector<Position> posVector;

    // Aktuelle Position speichern (ist quasi der Startpunkt)
    posVector.push_back(pos);

    while(true) {
        // Nächsten Koordinaten der Richtung
        pos.setNewPosition(dir);
        auto currSymbol = layout->get(pos);

        if(currSymbol->symbol.isSwitch() && switches->find(pos) == switches->end()) {
            (*switches)[pos] = *currSymbol;
        }

        // Verbindugspunkt entfernen (verhindert eine Endlosschleife)
        Direction compDir = dir.getComplementaryDirection();

        currSymbol->symbol.removeJunction(compDir);

        // Wie viele Verbindungspunkte sind noch offen?
        switch(currSymbol->symbol.getOpenJunctionsCount()) {
            case 0: // Endgleis -> Funktion verlassen
                posVector.push_back(pos);
                lines->push_back(std::move(posVector));
                return;

            case 1: {
                auto openDir = currSymbol->symbol.getNextOpenJunction();
                switch(openDir.getDistanceType(compDir)) {
                    // Teil einer Weiche (kann z.B. bei einer Kehrschleife passieren)
                    case Direction::INVALID:
                        posVector.push_back(pos);
                        lines->push_back(std::move(posVector));
                        currSymbol->symbol.removeJunction(openDir);
                        collectTrackPoints(pos, openDir);
                        return;

                    case Direction::LEFT_BEND:
                    case Direction::RIGHT_BEND:
                        posVector.push_back(pos);
                        dir = openDir;
                    case Direction::STRAIGHT:
                        currSymbol->symbol.removeJunction(dir);
                        continue; // einfaches Gleis -> weitermachen
                }
            }

            case 2:
            case 3: {
                if(currSymbol->symbol.isCrossOver() || currSymbol->symbol.isCrossOverSwitch()) {
                    pointsOfInterest.push_back(getNextBendPosition(pos, currSymbol->symbol.getNextOpenJunction()));
                } else {
                    pointsOfInterest.push_back(pos);
                }
                if(currSymbol->symbol.isOpenJunctionSet(dir)) {
                    currSymbol->symbol.removeJunction(dir);
                    continue; // einfaches Gleis -> weitermachen
                }
                posVector.push_back(pos);

                dir--;
                if(currSymbol->symbol.isOpenJunctionSet(dir)) {
                    currSymbol->symbol.removeJunction(dir);
                    continue;
                }
                dir += 2;
                if(currSymbol->symbol.isOpenJunctionSet(dir)) {
                    currSymbol->symbol.removeJunction(dir);
                    continue;
                }
                throw LayoutParserException{"invalid"};
            }

            default:
                throw LayoutParserException{"invalid case"};
        }
    }
}

Position LayoutParser::getRealStartPosition() {
    Position pos = layout->getNextBoundPosition();

    if(!layout->get(pos)->symbol.isStartSymbol()) {
        throw LayoutParserException{"first symbol is not a start symbol"};
    }

    return pos;
}

LayoutParser::~LayoutParser() {
}

void LayoutParser::parse(LayoutContainerPtr layout) {

    this->layout = layout;
    lines = std::make_shared<LineVector>();
    switches = std::make_shared<SwitchMap>();

    // Startpunkt ermitteln
    pointsOfInterest.push_back(getRealStartPosition());

    // So lange parsen, bis die Liste mit Startpunkten leer ist.
    while(!pointsOfInterest.empty()) {
        // Das erste Symbol der Liste holen und die Startrichtung ermitteln
        auto pos = pointsOfInterest.front();
        auto currSymbol = layout->get(pos);

        Direction dir = currSymbol->symbol.getNextOpenJunction(Direction::TOP);

        // Falls das Symobl keine offenen Verbindungspunkte mehr aufweist dann
        // das Element aus der Liste entfernen und mit dem nächsten Symbol fortfahren
        if(dir == Direction::UNSET) {
            pointsOfInterest.pop_front();
            continue;
        }
        currSymbol->symbol.removeJunction(dir);
        collectTrackPoints(pos, dir);
        if(currSymbol->symbol.hasOpenJunctionsLeft()) {
            pointsOfInterest.push_back(pos);
        }
        pointsOfInterest.pop_front();
    }
}

Position LayoutParser::getNextBendPosition(Position pos, Direction dir) {
    SymbolPtr currSymbol;
    do {
        // Nächstes Symobl der aus Richtung "dir" ermitteln
        pos.setNewPosition(dir);
        currSymbol = layout->get(pos);
    } while(currSymbol->symbol.isOpenJunctionSet(dir));
    return pos;
}

LineVectorPtr LayoutParser::getLineVector() {
    return lines;
}

SwitchMapPtr LayoutParser::getSwitchMap() {
    return switches;
}