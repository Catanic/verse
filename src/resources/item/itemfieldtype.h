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

#ifndef RESOURCES_ITEM_ITEMFIELDTYPE_H
#define RESOURCES_ITEM_ITEMFIELDTYPE_H

#include <string>

#include "localconsts.h"

struct ItemFieldType final
{
    ItemFieldType(const std::string &name0,
                  const std::string &description0,
                  const bool sign0) :
        name(name0),
        description(description0),
        sign(sign0)
    { }

    A_DELETE_COPY(ItemFieldType)

    const std::string name;
    const std::string description;
    const bool sign;
};

#endif  // RESOURCES_ITEM_ITEMFIELDTYPE_H
