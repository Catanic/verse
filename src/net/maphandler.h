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

#ifndef NET_MAPHANDLER_H
#define NET_MAPHANDLER_H

#include "localconsts.h"

namespace Net
{

class MapHandler notfinal
{
    public:
        MapHandler()
        { }

        A_DELETE_COPY(MapHandler)

        virtual ~MapHandler()
        { }

        virtual void blockPlayCancel() const = 0;

        virtual void destroyInstance() const = 0;

        virtual void camera(const int action,
                            const float range,
                            const float rotation,
                            const float latitude) const = 0;

        virtual void clientVersion(const int version) const = 0;
};

}  // namespace Net

extern Net::MapHandler *mapHandler;

#endif  // NET_MAPHANDLER_H
