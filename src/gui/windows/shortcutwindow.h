/*
 *  The ManaVerse Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_SHORTCUTWINDOW_H
#define GUI_WINDOWS_SHORTCUTWINDOW_H

#include "gui/widgets/window.h"

class ScrollArea;
class ShortcutContainer;
class TabbedArea;

/**
 * A window around a ShortcutContainer.
 *
 * \ingroup Interface
 */
class ShortcutWindow final : public Window
{
    public:
        /**
         * Constructor.
         */
        ShortcutWindow(const std::string &restrict title,
                       ShortcutContainer *restrict const content,
                       const std::string &restrict skinFile,
                       int width,
                       int height);

        ShortcutWindow(const std::string &restrict title,
                       const std::string &restrict skinFile,
                       const int width,
                       const int height);

        A_DELETE_COPY(ShortcutWindow)

        /**
         * Destructor.
         */
        ~ShortcutWindow() override final;

        void addTab(const std::string &name, ShortcutContainer *const content);

        int getTabIndex() const A_WARN_UNUSED;

        void widgetHidden(const Event &event) override final;

        void widgetMoved(const Event& event) override final;

        void mousePressed(MouseEvent &event) override final;

        void mouseDragged(MouseEvent &event) override final;

        void nextTab();

        void prevTab();

        void addButton(const std::string &text,
                       const std::string &eventName,
                       ActionListener *const listener);

#ifdef USE_PROFILER
        void logicChildren();
#endif  // USE_PROFILER

    private:
        ShortcutWindow();
        ShortcutContainer *mItems;

        ScrollArea *mScrollArea A_NONNULLPOINTER;
        TabbedArea *mTabs;
        STD_VECTOR<ShortcutContainer*> mPages;
        int mButtonIndex;

        static int mBoxesWidth;
};

extern ShortcutWindow *dropShortcutWindow;
extern ShortcutWindow *emoteShortcutWindow;
extern ShortcutWindow *itemShortcutWindow;
extern ShortcutWindow *spellShortcutWindow;

#endif  // GUI_WINDOWS_SHORTCUTWINDOW_H
