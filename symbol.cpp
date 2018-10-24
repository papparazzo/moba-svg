#include "symbol.h"
#include "helper.h"

#include <exception>
#include <stdexcept>

Symbol::operator bool() const {
    return isSymbol();
}

bool Symbol::isSymbol() const {
    if(s != 0) {
        return true;
    }
    return false;
}

bool Symbol::isStartSymbol() const {
    if(s & Direction::LEFT) {
        return false;
    }
    if(s & Direction::TOP_LEFT) {
        return false;
    }
    if(s & Direction::TOP) {
        return false;
    }
    if(s & Direction::TOP_RIGHT) {
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
        if(s == b) {
            return true;
        }
        b = rotate(b);
    }
    return false;
}

/**
 * Prüft, ob das Symbol ein Endsymbol (Prellbock) ist
 *
 * @return bool
 */
bool Symbol::isEnd() const {
    return check(8, Direction::TOP);
}

/**
 * Prüft, ob das Symbol ein gerades Gleis ist
 *
 * @return
 */
bool Symbol::isStraight() const {
    return check(4, Direction::TOP | Direction::BOTTOM);
}

/**
 * Prüft, ob das Symobl eine einfache Kreuzung ist
 * @return
 */
bool Symbol::isCrossOver() const {
    return check(2, Direction::TOP | Direction::BOTTOM | Direction::RIGHT | Direction::LEFT);
}

/**
 * Prüft, ob das Symbol gebogen ist
 *
 * @return
 */
bool Symbol::isBend() const {
    return check(8, Direction::RIGHT | Direction::BOTTOM_LEFT);
}

/**
 * Prüft, ob das Symbol ein einfaches Gleis ist (keine Weiche...)
 *
 * @return
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
 *
 * @return
 */
bool Symbol::isCrossOverSwitch() const {
    return check(4, Direction::RIGHT | Direction::LEFT | Direction::TOP_RIGHT | Direction::BOTTOM_LEFT);
}

/**
 * Prüft, ob Symbol eine Linksweiche ist
 *
 * @return
 */
bool Symbol::isLeftSwitch() const {
    return check(8, Direction::RIGHT | Direction::LEFT | Direction::TOP_RIGHT);
}

/**
 * Prüft, ob Symbol eine Rechtsweiche ist
 *
 * @return
 */
bool Symbol::isRightSwitch() const {
    return check(8, Direction::RIGHT | Direction::LEFT | Direction::BOTTOM_RIGHT);
}

/**
 * Prüft, ob Symbol eine Weiche ist
 *
 * @return
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
 *
 * @return
 */
bool Symbol::isThreeWaySwitch() const {
    return check(8, Direction::TOP | Direction::BOTTOM | Direction::TOP_RIGHT | Direction::TOP_LEFT);
}

/**
 * Prüft, ob Symbol kein einfaches Gleis ist
 *
 * @return
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
 *
 * @return
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

/**
 * Gibt die Anzahl der Verbindungspunkte zurück
 *
 * @return
 */
int Symbol::getJunktionsCount() const {
    int c = 0;
    std::uint8_t b = Direction::TOP;
    for(int i = 0; i < 8; ++i) {
        if(s & b) {
            ++c;
        }
        b = rotate(b);
    }
    return c;
}

/**
 * Gibt den nächsten Verbindungspunkt des Symobls zurück
 *
 * @param start
 * @return
 */
Direction Symbol::getNextJunktion(Direction start) const {
    std::uint8_t b = start;
    while(b) {
        if(s & b) {
            return static_cast<Direction>(b);
        }
        b <<= 1;
    }
    return Direction::UNSET;
}

/**
 * Gibt zurück, ob noch offene Verbindungen existieren
 *
 * @return
 */
bool Symbol::hasOpenJunctionsLeft() const {
    return static_cast<bool>(t);
}

/**
 * Gibt die nächste offene Verbindung zurück
 *
 * @return
 */
Direction Symbol::getNextOpenJunktion() const {
    std::uint8_t b = 1;
    while(b) {
        if(t & b) {
            return static_cast<Direction>(b);
        }
        b <<= 1;
    }
    return Direction::UNSET;
}

void Symbol::reset() {
    t = s;
}

bool Symbol::isJunctionSet(Direction d) const {
    return t & d;
}

void Symbol::removeJunktion(Direction dir) {
     if(!(t & dir)) {
         throw std::out_of_range("junction not set");
     }
     t |= ~dir;
}


