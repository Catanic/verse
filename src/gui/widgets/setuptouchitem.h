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

#ifndef GUI_WIDGETS_SETUPTOUCHITEM_H
#define GUI_WIDGETS_SETUPTOUCHITEM_H

#include "gui/widgets/setupitem.h"

class TouchActionsModel;

class SetupActionDropDown final : public SetupItem
{
    public:
        SetupActionDropDown(const std::string &restrict text,
                            const std::string &restrict description,
                            const std::string &restrict keyName,
                            SetupTabScroll *restrict const parent,
                            const std::string &restrict eventName,
                            TouchActionsModel *restrict const model,
                            const int width,
                            const MainConfig mainConfig);

        SetupActionDropDown(const std::string &restrict text,
                            const std::string &restrict description,
                            const std::string &restrict keyName,
                            SetupTabScroll *restrict const parent,
                            const std::string &restrict eventName,
                            TouchActionsModel *restrict const model,
                            const int width, const std::string &def,
                            const MainConfig mainConfig);

        A_DELETE_COPY(SetupActionDropDown)

        ~SetupActionDropDown() override final;

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        TouchActionsModel *mModel;
        DropDown *mDropDown;
        int mWidth;
};

#endif  // GUI_WIDGETS_SETUPTOUCHITEM_H
