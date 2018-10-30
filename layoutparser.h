
#pragma once

#include <vector>
#include <list>
#include <memory>
#include <exception>

#include "position.h"
#include "container.h"
#include "symbol.h"
#include "direction.h"

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

    typedef std::shared_ptr<Container<std::shared_ptr<Symbol> > > LayoutContainer;

    std::vector<std::vector<Position> > lines;

    LayoutContainer layout;

    /**
     * Enthält sämtliche Weichen und Kreuzungen
     */
    std::list<Position> pointsOfInterest;

    void collectTrackPoints(Position pos, Direction dir);

    Position getRealStartPosition();

public:
    LayoutParser(LayoutContainer layout) : layout{layout} {}
    virtual ~LayoutParser() {
    }

    void printTrackPoints();
    void parse();
};



