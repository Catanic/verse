/*
 *  The ManaVerse Client
 *  Copyright (C) 2015-2020  The ManaPlus Developers
 *  Copyright (C) 2020-2025  The ManaVerse Developers
 *
 *  This file is part of The ManaVerse Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "net/tmwa/updateprotocol.h"

#include "net/tmwa/network.h"
#include "net/tmwa/protocolout.h"

#include "debug.h"

extern int itemIdLen;

namespace TmwAthena
{

void updateProtocol()
{
    itemIdLen = 2;
#define PACKETS_UPDATE
#include "net/protocoloutupdate.h"
#include "net/tmwa/packetsout.inc"
#undef packet
    Network *const network = Network::mInstance;
    if (network != nullptr)
    {
        network->clearHandlers();
        network->registerHandlers();
    }
}

PACKETSOUT_VOID
PROTOCOLOUTUPDATE_VOID
PROTOCOLOUT_VOID

}  // namespace TmwAthena
