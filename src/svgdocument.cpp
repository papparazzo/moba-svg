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

#include "svgdocument.h"
#include <sstream>

SvgDocument::SvgDocument(const std::string &fileName, size_t height, size_t width) {
    out.open(fileName);
    out <<
        "<?xml version=\"1.0\"?>\n" <<
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" xml:lang=\"en\" " <<
        "viewBox=\"0 0 " << getRealPosition(width) << " " << getRealPosition(height) << "\" " <<
        "width=\"100%\" height=\"100%\">\n" <<
        "<title>auto-generated</title>\n";
}

SvgDocument::~SvgDocument() {
    out << "</svg>";
    out.close();
}

size_t SvgDocument::getRealPosition(size_t pos, size_t offset) {
    return (pos * factor) + (factor / 2) + offset;
}

void SvgDocument::addLayout(LineVector &lines) {
    for(auto line : lines) {
        createPath(line);
    }
}

void SvgDocument::addSwitch(size_t x, size_t y, int id) {
    out <<
        "<g id=\"switch_" << id << "\">" <<
//        "<path class=\"bend\" d="M125,87 L137,87 L149,75 \" fill=\"transparent\" stroke=\"white\" stroke-width=\"3\" />" <<
//        "<line class="straight" x1="125" x2="150" y1="87" y2="87" fill="transparent" stroke="white" stroke-width="3"/>
        "</g>";

}

void SvgDocument::addText(size_t x, size_t y, const std::string &id, const std::string &text) {
    out <<
        "<text id=\"" << id << "\" x=\"" << getRealPosition(x, -4) << "\" y=\"" << getRealPosition(y, 4) << "\">" <<
        "<tspan font-weight=\"bold\" fill=\"red\">" << text << "</tspan></text>";
}

void SvgDocument::createPath(const std::vector<Position> &line) {
    out << "<path d=\"";

    bool first = true;

    for(auto pos : line) {
        if(first) {
            first = false;
            out << "M" << getRealPosition(pos.x) << "," << getRealPosition(pos.y)<< " ";
            continue;
        }
        out << "L" << getRealPosition(pos.x) << "," << getRealPosition(pos.y)<< " ";

    }
    out << "\" fill=\"transparent\" stroke=\"rgb(100,100,100)\" stroke-width=\"" << strokeWidth << "\" />\n";
}
