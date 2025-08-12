/*
 *  The ManaVerse Client
 *  Copyright (C) 2012-2020  The ManaPlus Developers
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

#include "utils/translation/podict.h"

#include "debug.h"

std::string empty;

PoDict *translator = nullptr;
PoDict *dictionary = nullptr;
PoDict *reverseDictionary = nullptr;
#ifdef ENABLE_CUSTOMNLS
PoDict *mainTranslator = nullptr;
#endif  // ENABLE_CUSTOMNLS

PoDict::PoDict(const std::string &lang) :
    mPoLines(),
    mLang(lang)
{
}

PoDict::~PoDict()
{
}

const std::string PoDict::getStr(const std::string &str)
{
    if (mPoLines.find(str) == mPoLines.end())
        return str;
    return mPoLines[str];
}

const char *PoDict::getChar(const char *const str)
{
    if (mPoLines.find(str) == mPoLines.end())
        return str;
    return mPoLines[str].c_str();
}

bool PoDict::haveStr(const std::string &str) const
{
    return mPoLines.find(str) != mPoLines.end();
}
