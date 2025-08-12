/*
 *  The ManaVerse Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/popups/textpopup.h"

#include "gui/widgets/label.h"

#include "gui/fonts/font.h"

#include "debug.h"

TextPopup *textPopup = nullptr;

TextPopup::TextPopup() :
    Popup("TextPopup", "textpopup.xml"),
    mText()
{
}

void TextPopup::postInit()
{
    Popup::postInit();
    const int fontHeight = getFont()->getHeight();
    int y = 0;
    for (int f = 0; f < TEXTPOPUPCOUNT; f ++)
    {
        Label *const label = new Label(this);
        mText[f] = label;
        label->setPosition(0, y);
        label->setForegroundColorAll(
        getThemeColor(ThemeColorId::POPUP, 255U),
        getThemeColor(ThemeColorId::POPUP_OUTLINE, 255U));
        add(label);
        y += fontHeight;
    }
    addMouseListener(this);
}

TextPopup::~TextPopup()
{
}

void TextPopup::show(const int x, const int y, const std::string &str1,
                     const std::string &str2, const std::string &str3)
{
    mText[0]->setCaption(str1);
    mText[1]->setCaption(str2);
    mText[2]->setCaption(str3);

    int minWidth = 0;
    for (int f = 0; f < TEXTPOPUPCOUNT; f ++)
    {
        Label *const label = mText[f];
        label->adjustSize();
        const int width = label->getWidth();
        if (width > minWidth)
            minWidth = width;
    }

    const int pad2 = 2 * mPadding;
    minWidth += pad2;
    setWidth(minWidth);

    int cnt = 1;
    if (!str2.empty())
        cnt ++;
    if (!str3.empty())
        cnt ++;

    setHeight(pad2 + mText[0]->getFont()->getHeight() * cnt);
    const int distance = 20;

    const Rect &rect = mDimension;
    int posX = std::max(0, x - rect.width / 2);
    int posY = y + distance;

    if (posX + rect.width > mainGraphics->mWidth)
        posX = mainGraphics->mWidth - rect.width;
    if (posY + rect.height > mainGraphics->mHeight)
        posY = y - rect.height - distance;

    setPosition(posX, posY);
    setVisible(Visible_true);
    requestMoveToTop();
}

void TextPopup::mouseMoved(MouseEvent &event)
{
    Popup::mouseMoved(event);

    // When the mouse moved on top of the popup, hide it
    setVisible(Visible_false);
}
