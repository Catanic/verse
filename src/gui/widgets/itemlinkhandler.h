/*
 *  The ManaVerse Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_ITEMLINKHANDLER_H
#define GUI_WIDGETS_ITEMLINKHANDLER_H

#include "gui/widgets/linkhandler.h"

#include "localconsts.h"

class ItemLinkHandler final : public LinkHandler
{
    public:
        ItemLinkHandler();

        A_DELETE_COPY(ItemLinkHandler)

        ~ItemLinkHandler() override final;

        void handleLink(const std::string &link,
                        MouseEvent *event) override final;

        void setAllowCommands(const bool b)
        { mAllowCommands = b; }

    private:
        void handleCommandLink(const std::string &link,
                               const std::string &prefix);

        static void handleHelpLink(const std::string &link);

        static void handleHttpLink(const std::string &link,
                                   const MouseEvent *const event);

        static void handleItemLink(const std::string &link);

        static void handleSearchLink(const std::string &link);

        bool mAllowCommands;
};

#endif  // GUI_WIDGETS_ITEMLINKHANDLER_H
