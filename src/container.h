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

#include <exception>
#include <string>

#include <modules/lib-tracklayout/src/position.h>

class ContainerException : public std::exception {
    public:
        explicit ContainerException(const std::string &err) throw() : what_{err} {
        }

        ContainerException() throw() : what_{"Unknown error"} {
        }

        virtual ~ContainerException() throw() {
        }

        virtual const char *what() const throw() {
            return this->what_.c_str();
        }

    private:
        std::string what_;
};

template<typename T>
class Container {
    // TODO: Hier besser eine Map verwenden!!

    public:
        Container(size_t width, size_t height):
            width{width}, height{height}
        {
            if(height == 0 || width == 0) {
                throw ContainerException{"Dimensions out of range!"};
            }

            items = new T*[width];

            for(size_t i = 0; i < width; ++i) {
                items[i] = new T[height];
            }
        }

        virtual ~Container() {
            for(size_t i = 0; i < width; ++i) {
                delete[] items[i];
            }
            delete[] items;
        }

        size_t getHeight() const {
            return height;
        }

        size_t getWidth() const {
            return width;
        }

        Position getCurrentPosition() const {
            return position;
        }

        void setCurrentPosition(const Position &pos) {
            checkPosition(pos);
            position = pos;
        }

        void addItem(const Position &pos, T item) const {
            checkPosition(pos);
            items[pos.x][pos.y] = item;
        }

        T get(const Position &pos) const {
            this->checkPosition(pos);
            if(items[pos.x][pos.y] == nullptr) {
                throw ContainerException{"no valid item"};
            }
            return items[pos.x][pos.y];
        }

        T getCurrent() const {
            return this->get(position.x, position.y);
        }

        size_t itemsCount() const {
            size_t cnt = 0;
            for(size_t y = 0; y < height; ++y) {
                for(size_t x = 0; x < width; ++x) {
                    if(static_cast<bool>(this->items[x][y])) {
                        cnt++;;
                    }
                }
            }
            return cnt;
        }

        Position getNextFreePosition(const Position &pos) {
            for(size_t y = pos.y; y < height; ++y) {
                for(size_t x = pos.x; x < width; ++x) {
                    if((bool)items[x][y]) {
                        continue;
                    }
                    return {x, y};
                }
            }
            throw ContainerException{"No position found!"};
        }

        Position getNextBoundPosition(const Position &pos) {
            for(size_t y = pos.y; y < height; ++y) {
                for(size_t x = pos.x; x < this->width; ++x) {
                    if(!(bool)items[x][y]) {
                        continue;
                    }
                    return {x, y};
                }
            }
            throw ContainerException{"No position found!"};
        }

    protected:
        void checkPosition(const Position &pos) const {
            if(pos.x >= width || pos.y >= height) {
                throw ContainerException{"Dimensions out of range!"};
            }
        }

        size_t height;
        size_t width;
        Position position = {0, 0};

        T **items;
};



