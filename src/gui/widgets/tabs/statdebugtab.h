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

#ifndef GUI_WIDGETS_TABS_STATDEBUGTAB_H
#define GUI_WIDGETS_TABS_STATDEBUGTAB_H

#include "gui/widgets/tabs/debugtab.h"

#include "const/utils/perfstat.h"

#include "listeners/actionlistener.h"

class Button;
class Label;

class StatDebugTab final : public DebugTab,
                           public ActionListener
{
    friend class DebugWindow;

    public:
        explicit StatDebugTab(const Widget2 *const widget);

        A_DELETE_COPY(StatDebugTab)

        void logic() override final;

        void action(const ActionEvent &event) override;

    private:
        Label *mLPSLabel A_NONNULLPOINTER;
        Button *mResetButton A_NONNULLPOINTER;
        Button *mCopyButton A_NONNULLPOINTER;
        Label *mStatLabels[PERFSTAT_LAST_STAT - 1] A_NONNULLPOINTER;
        Label *mWorseStatLabels[PERFSTAT_LAST_STAT - 1] A_NONNULLPOINTER;
        size_t mDrawIndex;
};

#endif  // GUI_WIDGETS_TABS_STATDEBUGTAB_H
