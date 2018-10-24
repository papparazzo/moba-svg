#pragma once

#include <cstdint>
#include "helper.h"

class Symbol {
    public:
        Symbol(std::uint8_t s) : s{s}, t{s} {
        }

        virtual ~Symbol() {
        }

        std::uint8_t getType() const {
            return s;
        }

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
        int getJunktionsCount() const;
        Direction getComplementaryDirection(Direction dir);
        Direction getNextJunktion(Direction start = Direction::TOP) const;
        Direction getNextOpenJunktion() const;

        void removeJunktion(Direction curDir);

        operator bool() const;

        bool hasOpenJunctionsLeft() const;
        void reset();

    protected:
        std::uint8_t s;
        std::uint8_t t;
        bool check(std::uint8_t i, std::uint8_t b) const;

};


