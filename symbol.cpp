#include "symbol.h"

#include <exception>
#include <stdexcept>

Symbol::operator bool() const {
    return isSymbol();
}

bool Symbol::isSymbol() const {
    if(symbolFix != 0) {
        return true;
    }
    return false;
}

bool Symbol::isStartSymbol() const {
    if(symbolFix & Direction::LEFT) {
        return false;
    }
    if(symbolFix & Direction::TOP_LEFT) {
        return false;
    }
    if(symbolFix & Direction::TOP) {
        return false;
    }
    if(symbolFix & Direction::TOP_RIGHT) {
        return false;
    }
    return true;
}

/**
 * Prüft, ob ein bestimmtes Bit-Muster (z.B. für gerades / gebogenes Gleis)
 * gesetzt ist. Durch rotieren der Bits (@see rotate) kann jede Position durch
 * probiert werden. Simples Beispiel: Bei einem geraden Gleis sind die jeweiligen
 * Gegenüberliegende Verbindungspunkte gesetzt. Mit 4maligen rotieren hat man
 * sämtliche Kombinationen von geraden Gleisen durch
 *
 * @param int i anzahl iteration (2, 4, 8)
 * @param int b start bitmaske -> Das Startsymbol, was i-mal gedreht wird
 * @return bool true -> Symbol stimmt mit Vorlage überein, sonst false
 */
bool Symbol::check(std::uint8_t i, std::uint8_t b) const {
    while(i--) {
        if(symbolFix == b) {
            return true;
        }
        b = Symbol::rotate(b);
    }
    return false;
}

/**
 * Prüft, ob das Symbol ein Endsymbol (Prellbock) ist
 */
bool Symbol::isEnd() const {
    return check(8, Direction::TOP);
}

/**
 * Prüft, ob das Symbol ein gerades Gleis ist
 */
bool Symbol::isStraight() const {
    return check(4, Direction::TOP | Direction::BOTTOM);
}

/**
 * Prüft, ob das Symobl eine einfache Kreuzung ist
 */
bool Symbol::isCrossOver() const {
    return check(2, Direction::TOP | Direction::BOTTOM | Direction::RIGHT | Direction::LEFT);
}

/**
 * Prüft, ob das Symbol gebogen ist
 */
bool Symbol::isBend() const {
    return check(8, Direction::RIGHT | Direction::BOTTOM_LEFT);
}

/**
 * Prüft, ob das Symbol ein einfaches Gleis ist (keine Weiche...)
 */
bool Symbol::isTrack() const {
    if(isStraight()) {
        return true;
    }
    if(isCrossOver()) {
        return true;
    }
    if(isBend()) {
        return true;
    }
    if(isEnd()) {
        return true;
    }
    return false;
}

/**
 * Prüft, ob das Symob eine Doppelte Kreuzungsweiche ist
 */
bool Symbol::isCrossOverSwitch() const {
    return check(4, Direction::RIGHT | Direction::LEFT | Direction::TOP_RIGHT | Direction::BOTTOM_LEFT);
}

/**
 * Prüft, ob Symbol eine Linksweiche ist
 */
bool Symbol::isLeftSwitch() const {
    return check(8, Direction::RIGHT | Direction::LEFT | Direction::TOP_RIGHT);
}

/**
 * Prüft, ob Symbol eine Rechtsweiche ist
 */
bool Symbol::isRightSwitch() const {
    return check(8, Direction::RIGHT | Direction::LEFT | Direction::BOTTOM_RIGHT);
}

/**
 * Prüft, ob Symbol eine Weiche ist
 */
bool Symbol::isJunktionSwitch() const {
    if(isRightSwitch()) {
        return true;
    }
    if(isLeftSwitch()) {
        return true;
    }
    return false;
}

/**
 * Prüft, ob Symbol eine Dreiwegweiche ist
 */
bool Symbol::isThreeWaySwitch() const {
    return check(8, Direction::TOP | Direction::BOTTOM | Direction::TOP_RIGHT | Direction::TOP_LEFT);
}

/**
 * Prüft, ob Symbol kein einfaches Gleis ist
 */
bool Symbol::isSwitch() const {
    if(isCrossOverSwitch()) {
        return true;
    }
    if(isJunktionSwitch()) {
        return true;
    }
    if(isThreeWaySwitch()) {
        return true;
    }
    return false;
}

/**
 * Prüft, ob Symbol ein gültiges Symbol ist
 */
bool Symbol::isValidSymbol() const {
    if(isTrack()) {
        return true;
    }
    if(isSwitch()) {
        return true;
    }
    return false;
}

int Symbol::getJunktionsCount() const {
    return countJunktions(symbolFix);
}

int Symbol::getOpenJunktionsCount() const {
    return countJunktions(symbolDyn);
}

Direction Symbol::getNextJunktion(Direction start) const {
    return nextJunktion(symbolFix, start);
}

bool Symbol::hasOpenJunctionsLeft() const {
    return static_cast<bool>(symbolDyn);
}

Direction Symbol::getNextOpenJunktion(Direction start) const {
    return nextJunktion(symbolDyn, start);
}

void Symbol::reset() {
    symbolDyn = symbolFix;
}

bool Symbol::isJunctionSet(Direction dir) const {
    return symbolDyn & dir;
}

bool Symbol::isOpenJunctionSet(Direction dir) const {
    return symbolFix & dir;
}

void Symbol::removeJunktion(Direction dir) {
     if(!(symbolDyn & dir)) {
         throw std::out_of_range("junction not set");
     }
     symbolDyn &= ~dir;
}

/**
 * rotiert ein Symbol mit "Überschlag" nach links. D.h. das letzte gesetzte Bit
 * rotiert wieder zum Anfang.
 *
 * @param int symbol
 * @return
 */
std::uint8_t Symbol::rotate(std::uint8_t symbol) {
    if(symbol & 0x80) { // if last bit (Most significant bit) is set rotate it to bit 0
        return (symbol << 1) | 0x1;
    }
    return symbol << 1;
}

/**
 * Gibt die Anzahl der Verbindungspunkte zurück
 */
int Symbol::countJunktions(std::uint8_t symbol) const {
    int counter = 0;
    std::uint8_t b = Direction::TOP;
    for(int i = 0; i < 8; ++i) {
        if(symbol & b) {
            ++counter;
        }
        b = rotate(b);
    }
    return counter;
}

/**
 * Gibt die nächste offene Verbindung zurück
 */
Direction Symbol::nextJunktion(std::uint8_t symbol, Direction start) const {
    std::uint8_t b = start;
    for(int i = 0; i < 8; ++i) {
        if(symbol & b) {
            return static_cast<Direction>(b);
        }
        b = rotate(b);
    }
    return Direction::UNSET;
}