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

#ifndef UTILS_LANGS_H
#define UTILS_LANGS_H

#include "utils/vector.h"

#include <string>

#include "localconsts.h"

typedef STD_VECTOR<std::string> LangVect;
typedef LangVect::const_iterator LangIter;

LangVect getLang() A_WARN_UNUSED;

LangVect getServerLang() A_WARN_UNUSED;

std::string getLangSimple() A_WARN_UNUSED;

std::string getLangShort() A_WARN_UNUSED;

#endif  // UTILS_LANGS_H
