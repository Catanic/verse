/*
 *  The ManaVerse Client
 *  Copyright (C) 2014-2020  The ManaPlus Developers
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

#ifndef RESOURCES_DB_BADGESDB_H
#define RESOURCES_DB_BADGESDB_H

#include <map>
#include <string>

#include "localconsts.h"

typedef std::map<std::string, std::string> BadgesInfos;
typedef BadgesInfos::const_iterator BadgesInfosIter;

namespace BadgesDB
{
    void load();

    void unload();

    const std::string getGuildBadge(const std::string &name);

    const std::string getNameBadge(const std::string &name);

    const std::string getPartyBadge(const std::string &name);

    const std::string getClanBadge(const std::string &name);
}  // namespace BadgesDB

#endif  // RESOURCES_DB_BADGESDB_H
