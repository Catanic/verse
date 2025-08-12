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

#ifndef GUI_BUTTONINFO_H
#define GUI_BUTTONINFO_H

#include "enums/input/inputaction.h"

#include "enums/simpletypes/visible.h"

#include "localconsts.h"

class Button;

struct ButtonInfo final
{
    ButtonInfo(Button *const button0,
               const InputActionT key0,
               const Visible visible0) :
        button(button0),
        key(key0),
        visible(visible0)
    {
    }

    A_DELETE_COPY(ButtonInfo)

    Button *button;
    InputActionT key;
    Visible visible;
};
#endif  // GUI_BUTTONINFO_H
