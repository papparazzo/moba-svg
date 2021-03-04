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
    return (pos * FACTOR) + MIDDLE + offset;
}

void SvgDocument::addLayout(LineVectorPtr lines) {
    for(auto line : *lines) {
        createPath(line);
    }
}

void SvgDocument::addSwitch(size_t x, size_t y, std::uint8_t offset, int id, bool right) {
    if(offset % 2) {
        auto rotate = 45 * (offset + 1);
        out <<
            "<g id=\"switch_" << id << "\" fill=\"transparent\" stroke=\"yellow\" stroke-width=\"" << STROKE_WIDTH_INNER << "\" " <<
            "transform=\"translate(" << FACTOR * x << "," << FACTOR * y << ") rotate(" << rotate << ", 13, 13)\">\n" <<
            "    <path class=\"bend1\" d=\"M" << FACTOR << "," << FACTOR << " L" << MIDDLE << "," << MIDDLE << " L" << MIDDLE << "," << (right ? 0 : FACTOR) << "\" />\n" <<
            "    <line class=\"straight1\" x1=\"0\" x2=\"" << FACTOR << "\" y1=\"0\" y2=\"" << FACTOR << "\" />\n" <<
            "</g>\n";
    } else {
        auto rotate = 45 * offset;
        out <<
            "<g id=\"switch_" << id << "\" fill=\"transparent\" stroke=\"yellow\" stroke-width=\"" << STROKE_WIDTH_INNER << "\" " <<
            "transform=\"translate(" << FACTOR * x << "," << FACTOR * y << ") rotate(" << rotate << ", 13, 13)\">\n" <<
            "    <path class=\"bend1\" d=\"M" << MIDDLE << "," << FACTOR << " L" << MIDDLE << "," << MIDDLE << " L" << (right ? FACTOR : 0) << ",0\" />\n" <<
            "    <line class=\"straight1\" x1=\"" << MIDDLE << "\" x2=\"" << MIDDLE << "\" y1=\"0\" y2=\"" << FACTOR << "\" />\n" <<
            "</g>\n";
    }
}

void SvgDocument::addCrossOverSwitch(size_t x, size_t y, std::uint8_t offset, int id) {



}

void SvgDocument::addRightSwitch(size_t x, size_t y, std::uint8_t offset, int id) {
    addSwitch(x, y, offset, id, true);
}

void SvgDocument::addLeftSwitch(size_t x, size_t y, std::uint8_t offset, int id) {
    addSwitch(x, y, offset, id, false);
}

void SvgDocument::addThreeWaySwitch(size_t x, size_t y, std::uint8_t offset, int id) {
    out <<
        "<g id=\"switch_" << id << "\" fill=\"transparent\" stroke=\"yellow\" stroke-width=\"" << STROKE_WIDTH_INNER << "\" " <<
        "transform=\"translate(" << FACTOR * x << "," << FACTOR * y << ") rotate(" << 45 * offset << ", 13, 13)\">\n" <<
        "    <path class=\"bend1\" d=\"M" << MIDDLE << "," << FACTOR << " L" << MIDDLE << "," << MIDDLE << " L" << FACTOR << ",0\" />\n" <<
        "    <path class=\"bend2\" d=\"M" << MIDDLE << "," << FACTOR << " L" << MIDDLE << "," << MIDDLE << " L0,0\" />\n" <<
        "    <line class=\"straight1\" x1=\"" << MIDDLE << "\" x2=\"" << MIDDLE << "\" y1=\"0\" y2=\"" << FACTOR << "\" />\n" <<
        "    <line class=\"straight2\" x1=\"" << MIDDLE << "\" x2=\"" << MIDDLE << "\" y1=\"0\" y2=\"" << FACTOR << "\" />\n" <<
        "</g>\n";
}

void SvgDocument::addText(size_t x, size_t y, const std::string &id, const std::string &text) {
    out <<
        "<text id=\"" << id << "\" x=\"" << getRealPosition(x, -4) << "\" y=\"" << getRealPosition(y, 4) << "\">" <<
        "<tspan font-weight=\"bold\" fill=\"red\">" << text << "</tspan></text>\n";
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
    out << "\" fill=\"transparent\" stroke=\"rgb(100,100,100)\" stroke-width=\"" << STROKE_WIDTH_OUTER << "\" />\n";
}
