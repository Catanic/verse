/*
 *  The ManaVerse Client
 *  Copyright (C) 2013-2020  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_COLORPAGE_H
#define GUI_WIDGETS_COLORPAGE_H

#include "gui/widgets/listbox.h"

#include "localconsts.h"

class ColorPage final : public ListBox
{
    public:
        ColorPage(const Widget2 *const widget,
                  ListModel *const listModel,
                  const std::string &skin);

        A_DELETE_COPY(ColorPage)

        ~ColorPage() override final;

        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void resetAction();

        void adjustSize() override final;
};

#endif  // GUI_WIDGETS_COLORPAGE_H
