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

#ifndef GUI_WIDGETS_CHARACTERDISPLAY_H
#define GUI_WIDGETS_CHARACTERDISPLAY_H

#include "gui/widgets/container.h"
#include "gui/widgets/playerbox.h"

#include "listeners/widgetlistener.h"

#include "localconsts.h"

class CharSelectDialog;
class Label;

namespace Net
{
    struct Character;
}  // namespace Net

class CharacterDisplay final : public Container,
                               public MouseListener,
                               public WidgetListener
{
    public:
        CharacterDisplay(const Widget2 *const widget,
                         CharSelectDialog *const charSelectDialog);

        A_DELETE_COPY(CharacterDisplay)

        ~CharacterDisplay() override final;

        void setCharacter(Net::Character *const character);

        Net::Character *getCharacter() const
        { return mCharacter; }

        void requestFocus() override final;

        void setActive(const bool active);

        bool isSelectFocused() const
        { return false; }

        bool isDeleteFocused() const
        { return false; }

        void focusSelect()
        { }

        void focusDelete()
        { }

        void setSelect(bool b)
        { mPlayerBox->setSelected(b); }

        void widgetHidden(const Event &event) override final;

        void mouseExited(MouseEvent &event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mousePressed(MouseEvent &event) override final;

        void update();

    private:
        Net::Character *mCharacter;
        PlayerBox *mPlayerBox A_NONNULLPOINTER;
        Label *mName A_NONNULLPOINTER;
};

#endif  // GUI_WIDGETS_CHARACTERDISPLAY_H
