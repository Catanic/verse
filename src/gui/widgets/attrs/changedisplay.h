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

#ifndef GUI_WIDGETS_ATTRS_CHANGEDISPLAY_H
#define GUI_WIDGETS_ATTRS_CHANGEDISPLAY_H

#include "gui/widgets/attrs/attrdisplay.h"

#include "listeners/actionlistener.h"

#include "localconsts.h"

class Button;

class ChangeDisplay final : public AttrDisplay,
                            public ActionListener
{
    public:
        ChangeDisplay(const Widget2 *const widget,
                      const AttributesT id,
                      const std::string &restrict name,
                      const std::string &restrict shortName);

        A_DELETE_COPY(ChangeDisplay)

        std::string update() override final;

        Type getType() const override final
        { return CHANGEABLE; }

        void setPointsNeeded(const int needed);

        void action(const ActionEvent &event) override final;

    private:
        int mNeeded;

        Label *mPoints;
        Button *mInc;
};

#endif  // GUI_WIDGETS_ATTRS_CHANGEDISPLAY_H
