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

#ifndef UTILS_GETTEXTHELPER_H
#define UTILS_GETTEXTHELPER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

#include "localconsts.h"

class GettextHelper final
{
    public:
        A_DELETE_COPY(GettextHelper)

        static void initLang();

    private:
#ifdef ENABLE_NLS
        static void bindTextDomain(const char *const path);
#else  // ENABLE_NLS
        static void bindTextDomain(const char *const path A_UNUSED);
#endif  // ENABLE_NLS
};

#endif  // UTILS_GETTEXTHELPER_H
