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

#include <vector>
#include <list>
#include <exception>

#include "position.h"
#include "container.h"
#include "symbol.h"
#include "direction.h"
#include "common.h"

class LayoutParserException : public std::exception {
    public:
        explicit LayoutParserException(const std::string &err) throw() : what_{err} {
        }

        LayoutParserException() throw() : what_{"Unknown error"} {
        }

        virtual ~LayoutParserException() throw() {
        }

        virtual const char *what() const throw() {
            return this->what_.c_str();
        }

    private:
        std::string what_;
};

class LayoutParser {

    LineVector lines;

    LayoutContainer layout;

    /**
     * Enthält sämtliche Weichen und Kreuzungen. Also Startpunkte von denen man aus
     * den Gleisplan parsen kann.
     */
    std::list<Position> pointsOfInterest;

    void collectTrackPoints(Position pos, Direction dir);
    Position getNextBendPosition(Position pos, Direction dir);
    Position getRealStartPosition();

public:
    LayoutParser() {

    }
    virtual ~LayoutParser() {

    }

    LineVector parse(LayoutContainer layout);
};



