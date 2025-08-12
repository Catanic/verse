/*
 *  The ManaVerse Client
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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GUI_WIDGETS_BASICCONTAINER_H
#define GUI_WIDGETS_BASICCONTAINER_H

#include "gui/widgets/widget.h"

#include "listeners/widgetdeathlistener.h"

/**
  * A base class for containers. The class implements the most
  * common things for a container. If you are implementing a
  * container, consider inheriting from this class.
  *
  * @see Container
  */
class BasicContainer notfinal : public Widget,
                                public WidgetDeathListener
{
    public:
        explicit BasicContainer(const Widget2 *restrict const widget) :
            Widget(widget),
            WidgetDeathListener(),
            mWidgets(),
            mLogicWidgets()
        { }

        A_DELETE_COPY(BasicContainer)

        /**
          * Destructor
          */
        ~BasicContainer() override;

        /**
          * Shows a certain part of a widget in the basic container.
          * Used when widgets want a specific part to be visible in
          * its parent. An example is a TextArea that wants a specific
          * part of its text to be visible when a TextArea is a child
          * of a ScrollArea.
          *
          * @param widget The widget whom wants a specific part of
          *               itself to be visible.
          * @param area The rectangle to be visible.
          */
        void showWidgetPart(Widget *restrict const widget,
                            const Rect &restrict area) restrict2 override;

        // Inherited from Widget

        void moveToTop(Widget *const widget) restrict2 override;

        void moveToBottom(Widget *const widget) restrict2 override;

        Rect getChildrenArea() restrict2 override A_WARN_UNUSED;

        void focusNext() restrict2 override;

        void focusPrevious() restrict2 override;

        void logic() restrict2 override;

        void setFocusHandler(FocusHandler *restrict const focusHandler)
                             restrict2 override;

        void setInternalFocusHandler(FocusHandler *const restrict focusHandler)
                                     restrict2;

        Widget *getWidgetAt(int x, int y) restrict2 override A_WARN_UNUSED;

        // Inherited from WidgetDeathListener

        void death(const Event &restrict event) restrict2 override;

        Widget *findFirstWidget(const std::set<Widget*> &restrict list)
                                restrict2;

        /**
          * Adds a widget to the basic container.
          *
          * @param widget The widget to add.
          * @see remove, clear
          */
        void add(Widget *const widget) restrict2;

        /**
          * Removes a widget from the basic container.
          *
          * @param widget The widget to remove.
          * @see add, clear
          */
        virtual void remove(Widget *const restrict widget) restrict2;

        /**
          * Clears the basic container from all widgets.
          *
          * @see remove, clear
          */
        virtual void clear() restrict2;

    protected:
        /**
          * Draws the children widgets of the basic container.
          *
          * @param graphics A graphics object to draw with.
          */
        virtual void drawChildren(Graphics *const restrict graphics)
                                  restrict2 A_NONNULL(2);

        virtual void safeDrawChildren(Graphics *const restrict graphics)
                                      restrict2 A_NONNULL(2);

        /**
          * Calls logic for the children widgets of the basic
          * container.
          */
        virtual void logicChildren() restrict2;

        /**
          * Typedef.
          */
        typedef STD_VECTOR<Widget *> WidgetList;

          /**
          * Typedef.
          */
        typedef WidgetList::iterator WidgetListIterator;

          /**
          * Typedef.
          */
        typedef WidgetList::const_iterator WidgetListConstIterator;

        /**
          * Typedef.
          */
        typedef WidgetList::reverse_iterator WidgetListReverseIterator;

        /**
          * Typedef.
          */
        typedef WidgetList::const_reverse_iterator WidgetListCReverseIterator;

        /**
          * Holds all widgets of the basic container.
          */
        WidgetList mWidgets;

        WidgetList mLogicWidgets;
};

#endif  // GUI_WIDGETS_BASICCONTAINER_H
