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

#include <cstdint>
#include "direction.h"

class Symbol {
    public:
        Symbol(std::uint8_t symbol);
        
        virtual ~Symbol() {
        }

        std::uint8_t getType() const {
            return symbolFix;
        }

        std::uint8_t static rotate(std::uint8_t s);

        bool isSymbol() const;
        bool isStartSymbol() const;
        bool isEnd() const;
        bool isStraight() const;
        bool isCrossOver() const;
        bool isBend() const;
        bool isTrack() const;
        bool isCrossOverSwitch() const;
        bool isLeftSwitch() const;
        bool isRightSwitch() const;
        bool isJunktionSwitch() const;
        bool isThreeWaySwitch() const;
        bool isSwitch() const;
        bool isValidSymbol() const;

        bool isJunctionSet(Direction d) const;
        bool isOpenJunctionSet(Direction dir) const;

        int getJunktionsCount() const;
        int getOpenJunktionsCount() const;

        Direction getNextJunktion(Direction start = Direction::TOP) const;
        Direction getNextOpenJunktion(Direction start = Direction::TOP) const;

        Direction getComplementaryDirection(Direction dir);

        void removeJunktion(Direction curDir);

        operator bool() const;

        bool hasOpenJunctionsLeft() const;
        void reset();

    protected:
        std::uint8_t symbolFix; // Symbol mit festen Verbindungen
        std::uint8_t symbolDyn; // Symbol mit dynamischen Verbindungen
        bool check(std::uint8_t i, std::uint8_t b) const;

        int countJunktions(std::uint8_t symbol) const;
        Direction nextJunktion(std::uint8_t symbol, Direction start = Direction::TOP) const;
};


