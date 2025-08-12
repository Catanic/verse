/*
 *  The ManaVerse Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef GUI_WIDGETS_TABS_SETUP_PERFOMANCE_H
#define GUI_WIDGETS_TABS_SETUP_PERFOMANCE_H

#include "gui/widgets/tabs/setuptabscroll.h"

class NamesModel;

class Setup_Perfomance final : public SetupTabScroll
{
    public:
        explicit Setup_Perfomance(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Perfomance)

        ~Setup_Perfomance() override final;

    private:
#ifdef USE_SDL2
        NamesModel *mSdlDriversList;
#endif  // USE_SDL2
        NamesModel *mTexturesList;
};

#endif  // GUI_WIDGETS_TABS_SETUP_PERFOMANCE_H
