/*
 *  The ManaVerse Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_TMWA_CHARSERVERRECV_H
#define NET_TMWA_CHARSERVERRECV_H

namespace Net
{
    class MessageIn;

    struct Character;
}  // namespace Net

namespace TmwAthena
{
    namespace CharServerRecv
    {
        void readPlayerData(Net::MessageIn &msg,
                            Net::Character *const character);
        void processCharCreate(Net::MessageIn &msg);
        void processCharDeleteFailed(Net::MessageIn &msg);
        void processCharLogin(Net::MessageIn &msg);
        void processCharMapInfo(Net::MessageIn &msg);
        void processChangeMapServer(Net::MessageIn &msg);
    }  // namespace CharServerRecv
}  // namespace TmwAthena

#endif  // NET_TMWA_CHARSERVERRECV_H
