/*
 *  Project:    moba-sniffer
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

#pragma once

enum Direction {
    UNSET        = 0,
    TOP          = 1,
    TOP_RIGHT    = 2,
    RIGHT        = 4,
    BOTTOM_RIGHT = 8,

    BOTTOM       = 16,
    BOTTOM_LEFT  = 32,
    LEFT         = 64,
    TOP_LEFT     = 128
};

inline Direction getNextLeftDirection(Direction dir) {
    if(dir == TOP) {
        return TOP_LEFT;
    }
    return static_cast<Direction>(dir / 2);
}

inline Direction getNextRightDirection(Direction dir) {
    if(dir == TOP_LEFT) {
        return TOP;
    }
    return static_cast<Direction>(dir * 2);
}

/**
 * Gibt die Entgegengesetzt Richtung zurück. Beispiel: TOP liefert BOTTOM
 *
 * @param dir
 * @return
 */
inline Direction getComplementaryDirection(Direction dir) {
    if(dir == Direction::UNSET) {
        return Direction::UNSET;
    }
    if(dir < BOTTOM) {
        return static_cast<Direction>(dir * 16);
    }
    return static_cast<Direction>(dir / 16);
}

enum class DistanceType {
    INVALID = 0,
    STRAIGHT = 1,
    BEND = 2
};

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
inline DistanceType getDistanceType(Direction dir1, Direction dir2) {
   if(dir1 == dir2) {
       return DistanceType::INVALID;
   }

   Direction dirc = getComplementaryDirection(dir1);

   if(dir2 == dirc) {
       return DistanceType::STRAIGHT;
   }

   if(dir2 == (dirc * 2)) {
       return DistanceType::BEND;
   }

   // Sonderfall: TOP == 1 -> 1 / 2 = 0 -> müsste hier jedoch 128 sein!!
   if(dirc == TOP && dir2 == TOP_LEFT) {
       return DistanceType::BEND;
   }

   if(dir2 == (dirc / 2)) {
       return DistanceType::BEND;
   }

   return DistanceType::INVALID;
}

