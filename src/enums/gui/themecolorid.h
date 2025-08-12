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

#ifndef ENUMS_GUI_THEMECOLORID_H
#define ENUMS_GUI_THEMECOLORID_H

#include "enums/simpletypes/enumdefines.h"

#define themeEnumStart(name) name = 0,
#define themeEnum(name) name,
#define themeEnumEnd(name) name

enumStart(ThemeColorId)
{
#include "gui/themecolortype.inc"
}
enumEnd(ThemeColorId);

THEMECOLORTYPE_VOID

#undef themeEnumStart
#undef themeEnum
#undef themeEnumEnd
#undef THEMECOLORTYPE_VOID

#endif  // ENUMS_GUI_THEMECOLORID_H
