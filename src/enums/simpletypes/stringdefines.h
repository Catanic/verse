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

#ifndef ENUMS_SIMPLETYPES_STRINGDEFINES_H
#define ENUMS_SIMPLETYPES_STRINGDEFINES_H

#include "localconsts.h"

#define stringEnumStart(type) \
    std::map<std::string, type> mStringsArray; \
    bool contains(const std::string &key) \
    { \
        return mStringsArray.find(key) != mStringsArray.end(); \
    } \
    type get(const std::string &key) \
    { \
        return mStringsArray[key]; \
    } \
    void init() \
    { \
        mStringsArray.clear();

#define strEnumDef(prefix, name) \
        mStringsArray[#name] = prefix::name; \

#define strEnumDef2(prefix, name, str) \
        mStringsArray[#name] = prefix::name; \
        mStringsArray[str] = prefix::name;

#define stringEnumEnd \
    }


#endif  // ENUMS_SIMPLETYPES_STRINGDEFINES_H
