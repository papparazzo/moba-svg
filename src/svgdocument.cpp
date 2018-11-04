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

/**
     <?xml version="1.0"?>

<svg xmlns="http://www.w3.org/2000/svg" version="1.2" xml:lang="en">

              <desc>Gleise</desc>

              <path d="m30,30 h90 l100,100 v100" fill="white" stroke="rgb(100,100,100)" stroke-width="10" />

              <polygon id="tri" points="20,35 31,35 31,25 30,25" fill="rgb(100,100,100)"/>



  <!--

  <path d="m80,0 h40 v80 h180 v40h-180 v80 h-40 v-80 h-80 v-40 h80z" style="stroke-width:1" />

              <line x1="100" y1="100" x2="400" y2="100" style="stroke:rgb(100,100,100);stroke-width:9" />
              <line x1="100" y1="100" x2="400" y2="100" stroke-dasharray="100,10" style="stroke:red;stroke-width:5" />
        -->
</svg>
 */

void SvgDocument::create(const std::string &fileName, LineVector &lines, size_t height, size_t width) {
    out.open(fileName);
    createDocument("auto-generated", lines, height, width);
    out.close();
}

size_t SvgDocument::getRealPosition(size_t pos, size_t offset) {
    return (pos * factor) + (factor / 2) + offset;
}

void SvgDocument::createDocument(const std::string &title, LineVector &lines, size_t height, size_t width) {
    out <<
        "<?xml version=\"1.0\"?>\n" <<
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" xml:lang=\"en\" " <<
        "width=\"" << getRealPosition(width) << "\" height=\"" << getRealPosition(height) << "\">\n" << // viewbox="0 0 3 2"
            "<title>" << title << "</title>\n";

    for(auto line : lines) {
        createPath(line);
    }

    out << "</svg>";
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
    out << "\" fill=\"white\" stroke=\"rgb(100,100,100)\" stroke-width=\"10\" />\n";
}



