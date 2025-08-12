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

#ifndef RENDER_MOCKGRAPHICS_H
#define RENDER_MOCKGRAPHICS_H

#ifdef UNITTESTS

#include "render/graphics.h"

#include "unittests/render/mockdrawitem.h"

#include "utils/vector.h"

#include "localconsts.h"

class MockGraphics final : public Graphics
{
    public:
        MockGraphics();

        A_DELETE_COPY(MockGraphics)

        ~MockGraphics();

        #include "render/graphicsdef.hpp"
        RENDER_GRAPHICSDEF_HPP

        #include "render/softwaregraphicsdef.hpp"
        RENDER_SOFTWAREGRAPHICSDEF_HPP

        STD_VECTOR<MockDrawItem> mDraws;
};

#endif  // UNITTESTS
#endif  // RENDER_MOCKGRAPHICS_H
