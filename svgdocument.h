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

#include <string>
#include <fstream>

#include "common.h"

class SvgDocument {
    public:
        SvgDocument() {
        }

        void create(const std::string &fileName, LineVector &lines, size_t height, size_t width);

    protected:
        size_t getRealPosition(size_t pos, size_t offset = 0);
        void createDocument(const std::string &title, LineVector &lines, size_t height, size_t width);
        void createPath(const std::vector<Position> &line);

        size_t factor = 25;
        std::ofstream out;
};

