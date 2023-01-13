/*
 *  Project:    moba-blocksystem
 *
 *  Copyright (C) 2019 Stefan Paproth <pappi-@gmx.de>
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

#include <thread>
#include <string>

#include "msgloop.h"
#include "moba/registry.h"
#include "layoutparser.h"
#include "svgdocument.h"
#include "moba/symbol.h"

MessageLoop::MessageLoop(EndpointPtr endpoint) : endpoint{endpoint}, closing{false} {
}

void MessageLoop::run() {
    while(!closing) {
        try {
            Registry registry;
            registry.registerHandler<GetLayout>([this](const GetLayout &d){parseLayout(d);});
            registry.registerHandler<GetBlockingContacts>([this](const GetBlockingContacts &d){getFeedbackContactList(d);});
            registry.registerHandler<GetSwitchStates>([this](const GetSwitchStates &d) {getSwitchStates(d);});

            endpoint->connect();
            endpoint->sendMsg(ControlGetContactListReq{});

            while(true) {
                registry.handleMsg(endpoint->waitForNewMsg());
            }
        } catch(const std::exception &e) {
            std::cerr << "exception occured! <" << e.what() << ">" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }
}

void MessageLoop::getFeedbackContactList(const GetBlockingContacts &d) {
    blockContacts = d.blockContacts;
    endpoint->sendMsg(ControlGetSwitchStateListReq{});
}

void MessageLoop::getSwitchStates(const GetSwitchStates &d) {
    switchstates = d.switchstates;
    endpoint->sendMsg(LayoutGetLayoutReadOnlyReq{});
}

void MessageLoop::parseLayout(const MessageLoop::GetLayout &d) {
    LayoutParser parser;
    parser.parse(d.symbols);

    SvgDocument svg{"/home/stefan/Documents/moba/quellcode/moba-display/src/www-data/img/test.svg", d.symbols->getHeight() + 2, d.symbols->getWidth() + 2};
    svg.addLayout(parser.getLineVector());

    for(auto &iter : *blockContacts) {
        svg.addText(iter.first.x, iter.first.y, "b" + std::to_string(iter.second->id), "AA");
    }

    for(auto &iter : *(parser.getSwitchMap())) {
        auto &pos = iter.first;
        auto &sym = iter.second.symbol;
        auto &id  = iter.second.id;

        if(sym.isLeftSwitch()) {
            svg.addLeftSwitch(pos.x, pos.y, sym.getDistance(Symbol::LEFT_SWITCH), id);
        } else if(sym.isRightSwitch()) {
            svg.addRightSwitch(pos.x, pos.y, sym.getDistance(Symbol::RIGHT_SWITCH), id);
        } else if(sym.isCrossOverSwitch()) {
//            svg.addCrossOverSwitch(pos.x, pos.y, sym.getDistance(Symbol::CROSS_OVER_SWITCH), id);
        } else if(sym.isThreeWaySwitch()) {
//            svg.addThreeWaySwitch(pos.x, pos.y, sym.getDistance(Symbol::THREE_WAY_SWITCH), id);
        }
    }
}