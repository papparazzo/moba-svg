#pragma once

#include <cstdint>
#include "direction.h"

class Symbol {
    public:
        Symbol(std::uint8_t symbol) : symbolFix{symbol}, symbolDyn{symbol} {
        }

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

        int getJunktionsCount() const;
        int getOpenJunktionsCount() const;

        Direction getNextJunktion(Direction start = Direction::TOP) const;
        Direction getNextOpenJunktion() const;

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


