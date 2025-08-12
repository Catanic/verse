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

#ifndef RESOURCES_DB_DEADDB_H
#define RESOURCES_DB_DEADDB_H

#include "enums/simpletypes/skiperror.h"

#include <string>

/**
 * Char information database.
 */
namespace DeadDB
{
    /**
     * Loads the chars data.
     */
    void load();

    void loadXmlFile(const std::string &fileName,
                     const SkipError skipError);

    /**
     * Clear the chars data
     */
    void unload();

    std::string getRandomString();
}  // namespace DeadDB

#endif  // RESOURCES_DB_DEADDB_H
