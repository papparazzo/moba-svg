#include <memory>

#include "container.h"
#include "symbol.h"
#include "layoutparser.h"

#include <iostream>


int main(int argc, char** argv) {
    std::shared_ptr<Container<std::shared_ptr<Symbol>>> layout{new Container<std::shared_ptr<Symbol>>{20, 20}} ;

    layout->addItem({10, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::RIGHT)});
    layout->addItem({11, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::RIGHT | Direction::LEFT)});
    layout->addItem({12, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::RIGHT | Direction::LEFT | Direction::TOP | Direction::BOTTOM)});
    layout->addItem({13, 10}, std::shared_ptr<Symbol>{new Symbol(Direction::LEFT)});

    layout->addItem({12, 9}, std::shared_ptr<Symbol>{new Symbol(Direction::BOTTOM)});
    layout->addItem({12, 11}, std::shared_ptr<Symbol>{new Symbol(Direction::TOP)});
    LayoutParser parser{layout};
    parser.parse();
    parser.printTrackPoints();
}
