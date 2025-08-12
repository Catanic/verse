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

#ifndef GUI_DIALOGSMANAGER_H
#define GUI_DIALOGSMANAGER_H

#include "enums/simpletypes/modal.h"

#include "listeners/attributelistener.h"
#include "listeners/playerdeathlistener.h"

#include <string>

#include "localconsts.h"

class Window;

class DialogsManager final : public AttributeListener,
                             public PlayerDeathListener
{
    public:
        DialogsManager();

        A_DELETE_COPY(DialogsManager)

        static void closeDialogs();

        static void createUpdaterWindow();

        static Window *openErrorDialog(const std::string &header,
                                       const std::string &message,
                                       const Modal modal) RETURNS_NONNULL;

        void playerDeath() override final;

        void attributeChanged(const AttributesT id,
                              const int64_t oldVal,
                              const int64_t newVal) override final;
};

extern DialogsManager *dialogsManager;

#endif  // GUI_DIALOGSMANAGER_H
