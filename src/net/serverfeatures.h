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

#ifndef NET_SERVERFEATURES_H
#define NET_SERVERFEATURES_H

#include "localconsts.h"

namespace Net
{
class ServerFeatures notfinal
{
    public:
        ServerFeatures()
        { }

        A_DELETE_COPY(ServerFeatures)

        virtual ~ServerFeatures()
        { }

        virtual bool haveServerOnlineList() const = 0;

        virtual bool haveServerHp() const = 0;

        virtual bool havePlayerStatusUpdate() const = 0;

        virtual bool haveIncompleteChatMessages() const = 0;

        virtual bool haveRaceSelection() const = 0;

        virtual bool haveLookSelection() const = 0;

        virtual bool haveMove3() const = 0;

        virtual bool haveCharHiddenGender() const = 0;

        virtual bool haveMonsterAttackRange() const = 0;

        virtual bool haveEmailOnRegister() const = 0;

        virtual bool haveEmailOnDelete() const = 0;

        virtual bool haveServerVersion() const = 0;

        virtual bool haveMapServerVersion() const = 0;

        virtual bool haveNpcGender() const = 0;

        virtual bool haveJoinChannel() const = 0;

        virtual bool haveCreateCharGender() const = 0;

        virtual bool haveTalkPet() const = 0;

        virtual bool haveMovePet() const = 0;

        virtual bool haveServerWarpNames() const = 0;

        virtual bool haveExtendedRiding() const = 0;

        virtual bool haveExtendedDropsPosition() const = 0;

        virtual bool haveMoveWhileSit() const = 0;

        virtual bool haveKillerId() const = 0;

        virtual bool havePincode() const = 0;
};

}  // namespace Net

extern Net::ServerFeatures *serverFeatures;

#endif  // NET_SERVERFEATURES_H
