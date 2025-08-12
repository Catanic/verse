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

#ifndef GUI_WIDGETS_EMOTEPAGE_H
#define GUI_WIDGETS_EMOTEPAGE_H

#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

#include "gui/widgets/widget.h"

#include "localconsts.h"

class ImageCollection;

class EmotePage final : public Widget,
                        public MouseListener,
                        public WidgetListener
{
    public:
        explicit EmotePage(const Widget2 *const widget);

        A_DELETE_COPY(EmotePage)

        ~EmotePage() override final;

        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void mousePressed(MouseEvent &event) override final;

        int getIndexFromGrid(const int x, const int y) const;

        void widgetResized(const Event &event A_UNUSED) override final;

        void widgetMoved(const Event &event A_UNUSED) override final;

        void resetAction();

        int getSelectedIndex() const
        { return mSelectedIndex; }

    private:
        ImageSet *mEmotes;
        ImageCollection *mVertexes;
        int mSelectedIndex;
};

#endif  // GUI_WIDGETS_EMOTEPAGE_H
