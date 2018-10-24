#include "helper.h"

/**
 * rotiert ein Symbol mit "Überschlag" nach links. D.h. das letzte gesetzte Bit
 * rotiert wieder zum Anfang.
 *
 * @param int s symbol
 * @return
 */
std::uint8_t rotate(std::uint8_t s) {
    if(s & 0x80) { // if last bit (Most significant bit) is set rotate it to bit 0
        return (s << 1) | 0x1;
    }
    return s << 1;
}

/**
 * Gibt die Entgegengesetzt Richtung zurück. Beispiel: TOP liefert BOTTOM
 *
 * @param dir
 * @return
 */
Direction getComplementaryDirection(Direction dir) {
    std::uint8_t d = dir;
    d = rotate(d);
    d = rotate(d);
    d = rotate(d);
    return static_cast<Direction>(rotate(d));
}

/**
 * Die Distanz zwischen zwei Verbindungspunkte muss mindestens 3 Bit betragen, damit
 * zwei 2 Verbindungspunkte (auch als Teil einer Weiche) ein gültiges Gleis bilden.
 * Zu einem Verbindungspunkt dir1 kommen nur 3 mögliche Verbindungspunkte dir2 in Frage:
 * 1. Der komplemntäre Verbindungspunkt (also ein gerades Gleis)
 * 2. Der komplemntäre Verbindungspunkt + 1 Bit (also gebogenes Gleis)
 * 3. Der komplemntäre Verbindungspunkt - 1 Bit (also gebogenes Gleis)
 *
 * @param dir1
 * @param dir2
 * @return DistanceType
 */

DistanceType getDistanceType(Direction dir1, Direction dir2) {
    if(dir1 == dir2) {
        return DistanceType.INVALID;
    }

    Direction dirc = getComplementaryDirection(dir1);

    if(dir2 == dirc) {
        return DistanceType.STRAIGHT;
    }

    if(dir2 == (dirc * 2)) {
        return DistanceType.BEND;
    }

    // Sonderfall: TOP == 1 -> 1 / 2 = 0 -> müsste hier jedoch 128 sein!!
    if(dirc == TOP && dir2 == TOP_LEFT) {
        return DistanceType.BEND;
    }

    if(dir2 == (dirc / 2)) {
        return DistanceType.BEND;
    }

    return DistanceType.INVALID;
}
