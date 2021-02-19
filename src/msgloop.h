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

#pragma once

#include <boost/noncopyable.hpp>

#include "moba/layoutmessages.h"
#include "moba/interfacemessages.h"
#include "moba/controlmessages.h"
#include "moba/endpoint.h"
#include <memory>
#include "common.h"

class MessageLoop : private boost::noncopyable {

    struct GetLayout : public LayoutMessage {
        static constexpr std::uint32_t MESSAGE_ID = LAYOUT_GET_LAYOUT_RES;

        GetLayout(const rapidjson::Document &d) {
            symbols = std::make_shared<Container<SymbolPtr>>();
            for(auto &iter : d["symbols"].GetArray()) {
                symbols->addItem(
                    {
                        static_cast<std::size_t>(iter["xPos"].GetInt()),
                        static_cast<std::size_t>(iter["yPos"].GetInt())
                    },
                    std::make_shared<LayoutSymbol>(iter["id"].GetInt(), iter["symbol"].GetInt())
                );
            }
        }

        LayoutContainerPtr symbols;
    };

    struct GetBlockingContacts : public ControlMessage {
        static constexpr std::uint32_t MESSAGE_ID = CONTROL_GET_CONTACT_LIST_RES;

        GetBlockingContacts(const rapidjson::Document &d) {
            blockContacts = std::make_shared<std::map<Position, BlockContactDataPtr>>();

            for(auto &iter : d.GetArray()) {
                (*blockContacts)[{
                    static_cast<std::size_t>(iter["xPos"].GetInt()),
                    static_cast<std::size_t>(iter["yPos"].GetInt())
                }] = std::make_shared<BlockContactData>(iter);
            }
        }

        BlockContactDataMapPtr blockContacts;
    };

    struct GetSwitchStates : public ControlMessage {
        static constexpr std::uint32_t MESSAGE_ID = CONTROL_GET_SWITCH_STAND_LIST_RES;
        GetSwitchStates(const rapidjson::Document &d) {
            switchstates = std::make_shared<std::map<Position, SwitchStandData>>();

            for(auto &iter : d.GetArray()) {
                (*switchstates)[{
                    static_cast<std::size_t>(iter["xPos"].GetInt()),
                    static_cast<std::size_t>(iter["yPos"].GetInt())
                }] = SwitchStandData{iter};
            }
        }

        SwitchStandMapPtr switchstates;
    };

    EndpointPtr endpoint;
    BlockContactDataMapPtr blockContacts;
    SwitchStandMapPtr switchstates;
    
    void parseLayout(const GetLayout &d);
    void getFeedbackContactList(const GetBlockingContacts &d);
    void getSwitchStates(const GetSwitchStates &d);

    bool closing;

public:
    MessageLoop(EndpointPtr endpoint);
    void run();
};
