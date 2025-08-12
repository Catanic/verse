/*
 *  The ManaVerse Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef LISTENERS_WEIGHTLISTENER_H
#define LISTENERS_WEIGHTLISTENER_H

#include "gui/windows/okdialog.h"

#include "localconsts.h"

extern OkDialog *weightNotice;

/**
  * Listener used for handling the overweight message.
  */
struct WeightListener final : public ActionListener
{
    public:
        WeightListener() :
            ActionListener()
        { }

        A_DELETE_COPY(WeightListener)

        void action(const ActionEvent &event A_UNUSED) override final
        {
            weightNotice = nullptr;
        }
};

#endif  // LISTENERS_WEIGHTLISTENER_H
