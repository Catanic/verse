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

#ifndef GUI_WIDGETS_TABSTRIP_H
#define GUI_WIDGETS_TABSTRIP_H

#include "gui/widgets/widgetgroup.h"

class TabStrip final : public WidgetGroup
{
    public:
        TabStrip(const Widget2 *const widget,
                 const std::string &group,
                 const int height,
                 const int spacing);

        TabStrip(const Widget2 *const widget,
                 const int height,
                 const int spacing);

        A_DELETE_COPY(TabStrip)

        Widget *createWidget(const std::string &name,
                             const bool pressed)
                             const override final A_WARN_UNUSED;

        void action(const ActionEvent &event) override final;

        void nextTab();

        void prevTab();

        void setPressFirst(const bool press)
        { mPressFirst = press; }

    protected:
        bool mPressFirst;
};

#endif  // GUI_WIDGETS_TABSTRIP_H
