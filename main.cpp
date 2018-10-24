#include <memory>

#include "container.h"
#include "symbol.h"

#include <iostream>
#include <vector>


Container<std::shared_ptr<Symbol>> layout{20, 20};

// Vector enthält alle SVG-Linien
std::vector<std::vector<Position> > svgLine;


// Ermittelt einen definierten Startpunkt. Begint oben links und gibt Position
// des ersten Symbols zurück, welches entweder Weiche oder ein Endstück ist.
Position getRealStartPosition() {
    Position pos = layout.getNextBoundPosition({0, 0});

    std::shared_ptr<Symbol> currSymbol = layout.get(pos);

    if(!currSymbol->isStartSymbol()) {
        throw -1;
    }

    return pos;
}

/**
 * parst den Gleisplan bis zu einem Endgleis oder einer Weiche. Alle Positionen
 * von gebogenen Gleisen weden in einem Vektor gespeichert. Bei einem Endgleis wird
 * die Funktion beendet, bei einer Weiche wird beim komplimentären Punkt ganz normal
 * weiter gemacht und mit dem Abzeigenden Punkt die Funktion rekursiv aufgerufen.
 *
 * @param pos Aktuelle Position beim Parsen
 * @param dir Richtung in die weiter geparst werden soll
 */
void collectTrackPoints(Position pos, Direction dir) {
    std::vector<Position> posVector;
    // Aktuelle Position speichern (ist quasi der Startpunkt)
    posVector.push_back(pos);

    auto currSymbol = layout.get(pos);

    // Eventuell wurde die Verbindung bereits gelöscht
    if(currSymbol->isJunctionSet(dir)) {
        return;
    }

    // Verbindugspunkt entfernen (verhindert eine Endlosschleife)
    currSymbol->removeJunktion(dir);

    while(true) {
        // Curser entsprechend der Richtung weitersetzen
        pos.setNewPosition(dir);
        currSymbol = layout.get(pos);

        // Verbindugspunkt entfernen (verhindert eine Endlosschleife)
        currSymbol->removeJunktion(getComplementaryDirection(dir));

        // Wie viele Verbindungspunkte hat das Symbol?
        switch(currSymbol->getJunktionsCount()) {
            case 1:
                posVector.push_back(pos);
                svgLine.push_back(posVector);
                return; // Endgleis -> Funktion verlassen

            case 2:
                if(currSymbol->isBend()) {
                    // bei gebogenen Gleisen die Position speichern
                    posVector.push_back(pos);
                    dir = currSymbol->getNextOpenJunktion();
                }
                currSymbol->removeJunktion(dir);
                continue; // einfaches Gleis -> weitermachen

            case 3: {
                Direction dir1 = currSymbol->getNextOpenJunktion();
                switch(getDistanceType(dir1, dir)) {
                    case DistanceType::BEND:
                    case DistanceType::INVALID:
                    case DistanceType::STRAIGHT:
                    break;
                }


                //currSymbol->removeJunktion(dir);
                //currSymbol->removeJunktion(dir);
                //collectTrackPoints();
                //if() {

                //}
            }
            case 4:
                break;

            default:
                throw -1; // invalid Symbol given
        }
    }
}

void parseTracklayout() {
    Position pos = getRealStartPosition();
    auto currSymbol = layout.get(pos);

    // nächste Richtung bestimmen. Das Startsymbol kann nur ein Endpunkt oder ein
    // gerades Gleis sein. Beim geraden Gleis in beide Richtungen parsen
    Direction dir = currSymbol->getNextJunktion(Direction::TOP);
    collectTrackPoints(pos, dir);
    if(currSymbol->isEnd()) {
        return;
    }
    dir = currSymbol->getNextJunktion(dir);
    collectTrackPoints(pos, dir);
}

int main(int argc, char** argv) {


    layout.addItem({10, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::RIGHT)});
    layout.addItem({11, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::RIGHT | Direction::LEFT)});
    layout.addItem({12, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::LEFT)});

    parseTracklayout();

    for(auto iter = svgLine.begin(); iter != svgLine.end(); ++iter) {
        for(auto iter2 = iter->begin(); iter2 != iter->end(); ++iter2) {
            std::cout << *iter2 << std::endl;
        }
    }


}
