
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
        // Nächsten Koordinaten der Richtung
        pos.setNewPosition(dir);
        auto currSymbol = layout->get(pos);

        // Verbindugspunkt entfernen (verhindert eine Endlosschleife)
        Direction compDir = getComplementaryDirection(dir);
        currSymbol->removeJunktion(compDir);

        // Wie viele Verbindungspunkte sind noch offen?
        switch(currSymbol->getOpenJunktionsCount()) {
            case 0: // Endgleis -> Funktion verlassen
                posVector.push_back(pos);
                lines.push_back(posVector);
                return;

            case 1: {
               auto openDir = currSymbol->getNextOpenJunktion();
               switch(getDistanceType(openDir, compDir)) {
                   // Teil einer Weiche (kann z.B. bei einer Kehrschleife passieren)
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
                if(currSymbol->isCrossOver() || currSymbol->isCrossOverSwitch()) {
                    pointsOfInterest.push_back(getNextBendPosition(pos, currSymbol->getNextOpenJunktion()));
                } else {
                    pointsOfInterest.push_back(pos);
                }
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

// Ermittelt einen definierten Startpunkt. Beginnt oben links und gibt Position
// des ersten Symbols zurück, welches entweder Weiche oder ein Endstück ist.
Position LayoutParser::getRealStartPosition() {
    Position pos = layout->getNextBoundPosition({0, 0});

    auto currSymbol = layout->get(pos);

    if(!currSymbol->isStartSymbol()) {
        throw LayoutParserException("first symbol is not a start symbol");
    }

    return pos;
}

/**
 * Parst einen gesamten Gleisplan und liefert die Punkte von Weichen und gebogenen
 * Gleisen zurück.
 */
void LayoutParser::parse() {

    // Startpunkt ermitteln
    pointsOfInterest.push_back(getRealStartPosition());

    // So lange parsen, bis die Liste mit Startpunkten leer ist.
    while(!pointsOfInterest.empty()) {
        // Das erste Symbol der Liste holen und die Startrichtung ermitteln
        auto pos = pointsOfInterest.front();
        auto currSymbol = layout->get(pos);

        Direction dir = currSymbol->getNextOpenJunktion(Direction::TOP);

        // Falls das Symobl keine offenen Verbindungspunkte mehr aufweist dann
        // das Element aus der Liste entfernen und mit dem nächsten Symbol fortfahren
        if(dir == UNSET) {
            pointsOfInterest.pop_front();
            continue;
        }
        currSymbol->removeJunktion(dir);
        collectTrackPoints(pos, dir);
        if(currSymbol->hasOpenJunctionsLeft()) {
            pointsOfInterest.push_back(pos);
        }
        pointsOfInterest.pop_front();
    }
}
/**
 * Hangelt sich am Gleisplan entlang bis kein gerades Gleis mehr gefunden wurde,
 * Bzw. bis es kein Symbol mehr gibt, welches in die Richtung "dir" weiterführt
 * @param pos
 * @param dir
 * @return
 */
Position LayoutParser::getNextBendPosition(Position pos, Direction dir) {
    std::shared_ptr<Symbol> currSymbol;
    do {
        // Nächstes Symobl der aus Richtung "dir" ermitteln
        pos.setNewPosition(dir);
        currSymbol = layout->get(pos);
    } while(currSymbol->isOpenJunctionSet(dir));
    return pos;
}

void LayoutParser::printTrackPoints() {
    for(auto &iter : lines) {
        for(auto &iter2 : iter) {
            std::cout << iter2 << " - ";
        }
        std::cout << std::endl;
    }

    std::cout << "-------------------------------------------------------" << std::endl;

    for (auto &iter : pointsOfInterest) {
        std::cout << iter << std::endl;
    }
}
