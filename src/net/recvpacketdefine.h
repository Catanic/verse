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

#ifndef NET_RECVPACKETDEFINE_H
#define NET_RECVPACKETDEFINE_H

#define packet(pname, pid, sz, pfunc, ver) \
    mPackets[pid].name = #pname; \
    mPackets[pid].len = sz; \
    mPackets[pid].func = pfunc; \
    mPackets[pid].version = ver

#define packet2(pname, pid, sz, pfunc, ver) \
    mPackets[(pid) + (evolPacketOffset)].name = #pname; \
    mPackets[(pid) + (evolPacketOffset)].len = sz; \
    mPackets[(pid) + (evolPacketOffset)].func = pfunc; \
    mPackets[(pid) + (evolPacketOffset)].version = ver

#endif  // NET_RECVPACKETDEFINE_H
