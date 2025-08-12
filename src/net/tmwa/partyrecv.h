/*
 *  The ManaVerse Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#ifndef NET_TMWA_PARTYRECV_H
#define NET_TMWA_PARTYRECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace TmwAthena
{
    namespace PartyRecv
    {
        void processPartySettings(Net::MessageIn &msg);
        void processPartyInfo(Net::MessageIn &msg);
        void processPartyMessage(Net::MessageIn &msg);
        void processPartyInviteResponse(Net::MessageIn &msg);
        void processPartyInvited(Net::MessageIn &msg);
        void processPartyMove(Net::MessageIn &msg);
        void processPartyUpdateHp(Net::MessageIn &msg);
    }  // namespace PartyRecv
}  // namespace TmwAthena

#endif  // NET_TMWA_PARTYRECV_H
