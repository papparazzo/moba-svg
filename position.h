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
