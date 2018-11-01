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

#pragma once

#include <cstddef>
#include <iostream>

#include "direction.h"

struct Position {
    Position(size_t x, size_t y) : x{x}, y{y} {
    }
    size_t x;
    size_t y;

    friend std::ostream& operator<< (std::ostream& os, const Position& pos) {
        os << "x: " << pos.x << " y: " << pos.y;
        return os;
    }

    /**
     * setzt den Cursor (Position) in die Richtung welche mit Direction
     * angegeben ist. Beispiel: Direction RIGHT -> x einen weiter nach rechts
     * @param d
     */
    void setNewPosition(Direction d) {
        switch(d) {
            case UNSET:
                return;

            case TOP_RIGHT:
                x++;  // fall-through

            case TOP:
                y--;
                break;

            case BOTTOM_RIGHT:
                y++; // fall-through

            case RIGHT:
                x++;
                return;

            case BOTTOM:
                y++;
                return;

            case BOTTOM_LEFT:
                y++;  // fall-through

            case LEFT:
                x--;
                break;

            case TOP_LEFT:
                y--;
                x--;
                break;
        }
        if(y < 0 || !x < 0) {
            throw -1; // TODO: Out of range exception
        }
    }
};
