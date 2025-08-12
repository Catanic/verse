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

#ifndef NET_CLANHANDLER_H
#define NET_CLANHANDLER_H

#include <string>

#include "localconsts.h"

namespace Net
{

class ClanHandler notfinal
{
    public:
        ClanHandler()
        { }

        A_DELETE_COPY(ClanHandler)

        virtual ~ClanHandler()
        { }

        virtual void chat(const std::string &restrict text) const = 0;
};

}  // namespace Net

extern Net::ClanHandler *clanHandler;

#endif  // NET_CLANHANDLER_H
