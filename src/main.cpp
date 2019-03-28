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

#include <memory>

#include "moba/container.h"
#include "moba/symbol.h"
#include "layoutparser.h"
#include "common.h"
#include "svgdocument.h"

#include <iostream>

int main(int argc, char** argv) {
    std::shared_ptr<Container<std::shared_ptr<Symbol>>> layout{new Container<std::shared_ptr<Symbol>>{20, 20}} ;

    layout->addItem({10, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::RIGHT)});
    layout->addItem({11, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::RIGHT | Direction::LEFT)});
    layout->addItem({12, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::RIGHT | Direction::LEFT | Direction::TOP | Direction::BOTTOM)});
    layout->addItem({13, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::LEFT)});

    layout->addItem({12, 9}, std::shared_ptr<Symbol>{new Symbol(Direction::BOTTOM)});
    layout->addItem({12, 11}, std::shared_ptr<Symbol>{new Symbol(Direction::TOP)});
    LayoutParser parser;
    LineVector lines = parser.parse(layout);
    SvgDocument svg;
    svg.create("/home/stefan/Desktop/test.svg", lines, layout->getHeight(), layout->getWidth());

    return 0;
}
