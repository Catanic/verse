/*
 *  The ManaVerse Client
 *  Copyright (C) 2011-2020  The ManaPlus Developers
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

#include "net/eathena/maphandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

MapHandler::MapHandler() :
    Net::MapHandler()
{
    mapHandler = this;
}

MapHandler::~MapHandler()
{
    mapHandler = nullptr;
}

void MapHandler::blockPlayCancel() const
{
    createOutPacket(CMSG_MAP_BLOCK_PLAY_CANCEL);
}

void MapHandler::destroyInstance() const
{
    createOutPacket(CMSG_INSTANCE_COMMAND);
    outMsg.writeInt32(3, "command");
}

void MapHandler::camera(const int action,
                        const float range,
                        const float rotation,
                        const float latitude) const
{
    if (packetVersion < 20160525)
        return;
    createOutPacket(CMSG_CAMERA_INFO);
    outMsg.writeInt8(action, "action");
    outMsg.writeFloat(range, "range");
    outMsg.writeFloat(rotation, "rotation");
    outMsg.writeFloat(latitude, "latitude");
}

void MapHandler::clientVersion(const int version) const
{
    if (packetVersion < 20090406)
        return;
    createOutPacket(CMSG_CLIENT_VERSION);
    outMsg.writeInt32(version, "version");
}

}  // namespace EAthena
