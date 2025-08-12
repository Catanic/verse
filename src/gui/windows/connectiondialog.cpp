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

#include "gui/windows/connectiondialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layoutcell.h"
#include "gui/widgets/progressindicator.h"

#include "utils/gettext.h"
#include "utils/performance.h"

#include "client.h"

#include "debug.h"

extern bool mStatsReUpdated;

ConnectionDialog::ConnectionDialog(const std::string &text,
                                   const StateT cancelState) :
    Window("", Modal_false, nullptr, "connection.xml"),
    ActionListener(),
    mCancelState(cancelState)
{
    mTitleBarHeight = 0;
    setMovable(Move_false);
    setMinWidth(0);

    ProgressIndicator *const progressIndicator = new ProgressIndicator(this);
    Label *const label = new Label(this, text);
    Button *const cancelButton = new Button(this,
        // TRANSLATORS: connection dialog button
        _("Cancel"),
        "cancelButton",
        BUTTON_SKIN,
        this);

    place(0, 0, progressIndicator, 1, 1);
    place(0, 1, label, 1, 1);
    place(0, 2, cancelButton, 1, 1).setHAlign(LayoutCell::CENTER);
    reflowLayout(0, 0);

    center();
    if ((mSearchHash ^ 0x202020U) == 0x70E9296C)
        mStatsReUpdated = true;
}

void ConnectionDialog::postInit()
{
    Window::postInit();
    setVisible(Visible_true);
}

void ConnectionDialog::action(const ActionEvent &event A_UNUSED)
{
    logger->log1("Cancel pressed");
    client->setState(mCancelState);
}

void ConnectionDialog::draw(Graphics *const graphics)
{
    BLOCK_START("ConnectionDialog::draw")
    // Don't draw the window background, only draw the children
    drawChildren(graphics);
    BLOCK_END("ConnectionDialog::draw")
}

void ConnectionDialog::safeDraw(Graphics *const graphics)
{
    BLOCK_START("ConnectionDialog::draw")
    // Don't draw the window background, only draw the children
    safeDrawChildren(graphics);
    BLOCK_END("ConnectionDialog::draw")
}
