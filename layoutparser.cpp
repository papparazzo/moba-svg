
#include "layoutparser.h"

#include <iostream>

/**
 * parst den Gleisplan bis zu einem Endgleis. Alle Positionen
 * von gebogenen Gleisen weden in einem Vektor gespeichert. Bei einem Endgleis wird
 * die Funktion beendet, bei einer Weiche wird beim komplimentären Punkt ganz normal
 * weiter gemacht und die Position im Vector pointsOfInterest gespeichert.
 *
 * @param pos Aktuelle Position beim Parsen
 * @param dir Richtung in die weiter geparst werden soll
 */
void LayoutParser::collectTrackPoints(Position pos, Direction dir) {
    std::vector<Position> posVector;

    // Aktuelle Position speichern (ist quasi der Startpunkt)
    posVector.push_back(pos);

    while(true) {
        pos.setNewPosition(dir);
        auto currSymbol = layout->get(pos);

        // Verbindugspunkt entfernen (verhindert eine Endlosschleife)
        Direction compDir = getComplementaryDirection(dir);
        currSymbol->removeJunktion(compDir);

        // Wie viele Verbindungspunkte sind noch offen?
        switch(currSymbol->getOpenJunktionsCount()) {
            case 0:
                posVector.push_back(pos);
                lines.push_back(posVector);
                return; // Endgleis -> Funktion verlassen

            case 1: {
               auto openDir = currSymbol->getNextOpenJunktion();
               switch(getDistanceType(openDir, compDir)) {
                   case DistanceType::INVALID:
                        posVector.push_back(pos);
                        lines.push_back(posVector);
                        currSymbol->removeJunktion(openDir);
                        collectTrackPoints(pos, openDir);
                        return;

                   case DistanceType::BEND:
                        posVector.push_back(pos);
                        dir = openDir;
                   case DistanceType::STRAIGHT:
                        currSymbol->removeJunktion(dir);
                        continue; // einfaches Gleis -> weitermachen
                }
            }
            case 2:
            case 3: {
                pointsOfInterest.push_back(pos);
                if(currSymbol->isOpenJunctionSet(dir)) {
                    currSymbol->removeJunktion(dir);
                    continue; // einfaches Gleis -> weitermachen
                }
                posVector.push_back(pos);
                auto ndir = getNextLeftDirection(dir);
                if(currSymbol->isOpenJunctionSet(ndir)) {
                    dir = ndir;
                    continue;
                }
                ndir = getNextRightDirection(dir);
                if(currSymbol->isOpenJunctionSet(ndir)) {
                    dir = ndir;
                    continue;
                }
                throw LayoutParserException("invalid");
            }

            default:
                throw LayoutParserException("invalid case");
        }
    }
}

// Ermittelt einen definierten Startpunkt. Begint oben links und gibt Position
// des ersten Symbols zurück, welches entweder Weiche oder ein Endstück ist.
Position LayoutParser::getRealStartPosition() {
    Position pos = layout->getNextBoundPosition({0, 0});

    auto currSymbol = layout->get(pos);

    if(!currSymbol->isStartSymbol()) {
        throw LayoutParserException("first symbol is not a start symbol");
    }

    return pos;
}

void LayoutParser::printTrackPoints() {
    for(auto iter = lines.begin(); iter != lines.end(); ++iter) {
        for(auto iter2 = iter->begin(); iter2 != iter->end(); ++iter2) {
            std::cout << *iter2 << std::endl;
        }
    }
}

void LayoutParser::parse() {

    Position pos = getRealStartPosition();
    auto currSymbol = layout->get(pos);

    // nächste Richtung bestimmen. Das Startsymbol kann nur ein Endpunkt oder ein
    // gerades Gleis sein. Beim geraden Gleis in beide Richtungen parsen
    Direction dir = currSymbol->getNextJunktion(Direction::TOP);
    collectTrackPoints(pos, dir);
    if(!currSymbol->isEnd()) {
        pointsOfInterest.push_back(pos);
    }

    


    //dir = currSymbol->getNextJunktion(dir);
    //collectTrackPoints(pos, dir);
}

