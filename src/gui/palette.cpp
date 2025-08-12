/*
 *  The ManaVerse Client
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/palette.h"

#include "utils/foreach.h"
#include "utils/timer.h"

#ifndef USE_SDL2
#include <cmath>
#endif  // USE_SDL2

#include "debug.h"

// required by MSYS2 / Windows
#ifndef M_PI
#define M_PI    3.1415926535897932384626433832795
#endif

const Color Palette::BLACK = Color(0, 0, 0, 255);
Palette::Palettes Palette::mInstances;

const Color Palette::RAINBOW_COLORS[] =
{
    Color(255, 0, 0, 255),
    Color(255, 153, 0, 255),
    Color(255, 255, 0, 255),
    Color(0, 153, 0, 255),
    Color(0, 204, 204, 255),
    Color(51, 0, 153, 255),
    Color(153, 0, 153, 255)
};

Palette::Palette(const int size) :
    mRainbowTime(tick_time),
    mColors(Colors(size)),
    mCharColors(),
    mGradVector()
{
    mInstances.insert(this);
}

Palette::~Palette()
{
    mInstances.erase(this);
}

const Color& Palette::getCharColor(const signed char c, bool &valid) const
{
    const CharColors::const_iterator it = mCharColors.find(c);
    if (it != mCharColors.end())
    {
        valid = true;
        return mColors[(*it).second].color;
    }

    valid = false;
    return BLACK;
}

void Palette::advanceGradients()
{
    FOR_EACH (Palettes::const_iterator, it, mInstances)
        (*it)->advanceGradient();
}

void Palette::advanceGradient()
{
    const int time = get_elapsed_time(mRainbowTime);
    if (time > 5)
    {
        // For slower systems, advance can be greater than one (advance > 1
        // skips advance-1 steps). Should make gradient look the same
        // independent of the framerate.
        const int advance = time / 5;

        for (size_t i = 0, sz = mGradVector.size(); i < sz; i++)
        {
            ColorElem *const elem A_NONNULLPOINTER = mGradVector[i];
            if (elem == nullptr)
                continue;

            int delay = elem->delay;
            const GradientTypeT &grad = elem->grad;

            if (grad == GradientType::PULSE)
                delay = delay / 20;

            const int numOfColors = (elem->grad == GradientType::SPECTRUM ? 6 :
                grad == GradientType::PULSE ? 127 :
                NUM_ELEMENTS(RAINBOW_COLORS));

            elem->gradientIndex = (elem->gradientIndex + advance)
                % (delay * numOfColors);

            const int gradIndex = elem->gradientIndex;
            const int pos = delay != 0 ? (gradIndex % delay) : gradIndex;
            int colIndex;
            if (delay != 0)
                colIndex = gradIndex / delay;
            else
                colIndex = gradIndex;

            Color &color = elem->color;
            int colVal;

            if (grad == GradientType::PULSE)
            {
                colVal = CAST_S32(255.0 *
                        sin(M_PI * colIndex / numOfColors));

                const Color &col = elem->testColor;

                color.r = ((colVal * col.r) / 255) % (col.r + 1);
                color.g = ((colVal * col.g) / 255) % (col.g + 1);
                color.b = ((colVal * col.b) / 255) % (col.b + 1);
            }
            else if (grad == GradientType::SPECTRUM)
            {
                if ((colIndex % 2) != 0)
                {   // falling curve
                    if (delay != 0)
                    {
                        colVal = CAST_S32(255.0 *
                             (cos(M_PI * pos / delay) + 1) / 2);
                    }
                    else
                    {
                        colVal = CAST_S32(255.0 *
                             (cos(M_PI * pos) + 1) / 2);
                    }
                }
                else
                {   // ascending curve
                    if (delay != 0)
                    {
                        colVal = CAST_S32(255.0 * (cos(M_PI *
                            (delay - pos) / delay) + 1) / 2);
                    }
                    else
                    {
                        colVal = CAST_S32(255.0 * (cos(M_PI *
                            (delay - pos)) + 1) / 2);
                    }
                }

                color.r = (colIndex == 0 || colIndex == 5) ? 255 :
                    (colIndex == 1 || colIndex == 4) ? colVal : 0;
                color.g = (colIndex == 1 || colIndex == 2) ? 255 :
                    (colIndex == 0 || colIndex == 3) ? colVal : 0;
                color.b = (colIndex == 3 || colIndex == 4) ? 255 :
                    (colIndex == 2 || colIndex == 5) ? colVal : 0;
            }
            else if (elem->grad == GradientType::RAINBOW)
            {
                const Color &startCol = RAINBOW_COLORS[colIndex];
                const int destIndex
                    = (colIndex + 1) % NUM_ELEMENTS(RAINBOW_COLORS);
                const Color &destCol = RAINBOW_COLORS[destIndex];
                double startColVal;
                double destColVal;

                if (delay != 0)
                    startColVal = (cos(M_PI * pos / delay) + 1) / 2;
                else
                    startColVal = 0;

                destColVal = 1 - startColVal;

                color.r = CAST_S32(startColVal
                    * startCol.r + destColVal * destCol.r);

                color.g = CAST_S32(startColVal
                    * startCol.g + destColVal * destCol.g);

                color.b = CAST_S32(startColVal
                    * startCol.b + destColVal * destCol.b);
            }
        }

        mRainbowTime = tick_time;
    }
}
