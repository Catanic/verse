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

#include "gui/widgets/slider.h"

#include "settings.h"

#include "enums/gui/slidergrid.h"

#include "gui/gui.h"

#include "utils/delete2.h"
#include "utils/performance.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "render/graphics.h"

#include "render/vertexes/imagecollection.h"

#include "debug.h"

ImageRect Slider::buttons[2];
float Slider::mAlpha = 1.0;
int Slider::mInstances = 0;

static std::string const data[2] =
{
    "slider.xml",
    "slider_highlighted.xml"
};

Slider::Slider(Widget2 *const widget,
               const double scaleEnd,
               const double stepLength) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    mValue(0),
    mStepLength(stepLength),
    mScaleStart(0),
    mScaleEnd(scaleEnd),
    mOrientation(Orientation::HORIZONTAL),
    mVertexes(new ImageCollection),
    mMarkerLength(10),
    mHasMouse(false)
{
    init();
}

Slider::Slider(Widget2 *const widget,
               const double scaleStart,
               const double scaleEnd,
               const double stepLength) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    mValue(scaleStart),
    mStepLength(stepLength),
    mScaleStart(scaleStart),
    mScaleEnd(scaleEnd),
    mOrientation(Orientation::HORIZONTAL),
    mVertexes(new ImageCollection),
    mMarkerLength(10),
    mHasMouse(false)
{
    init();
}

Slider::~Slider()
{
    if (gui != nullptr)
        gui->removeDragged(this);

    delete2(mVertexes)
    mInstances--;
    if (mInstances == 0)
    {
        for (int mode = 0; mode < 2; mode ++)
            Theme::unloadRect(buttons[mode], 0, 8);
    }
}

void Slider::init()
{
    mAllowLogic = false;
    setFocusable(true);
    setFrameSize(1);

    addMouseListener(this);
    addKeyListener(this);

    setFrameSize(0);

    // Load resources
    if (mInstances == 0)
    {
        if (theme != nullptr)
        {
            for (int mode = 0; mode < 2; mode ++)
                theme->loadRect(buttons[mode], data[mode], "slider.xml", 0, 8);
        }
        updateAlpha();
    }

    mInstances++;

    if (buttons[0].grid[SliderGrid::HGRIP] != nullptr)
        setMarkerLength(buttons[0].grid[SliderGrid::HGRIP]->getWidth());
}

void Slider::updateAlpha()
{
    const float alpha = std::max(settings.guiAlpha,
        theme->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int f = 0; f < 2; f ++)
        {
            for (int d = 0; d < SliderGrid::SLIDER_MAX; d ++)
            {
                if (buttons[f].grid[d] != nullptr)
                    buttons[f].grid[d]->setAlpha(mAlpha);
            }
        }
    }
}

void Slider::draw(Graphics *const graphics)
{
    BLOCK_START("Slider::draw")
    if ((buttons[0].grid[SliderGrid::HSTART] == nullptr) ||
        (buttons[1].grid[SliderGrid::HSTART] == nullptr) ||
        (buttons[0].grid[SliderGrid::HEND] == nullptr))
    {
        BLOCK_END("Slider::draw")
        return;
    }

    int w = getWidth();
    const int h = getHeight();
    const int y = mHasMouse ?
        (h - buttons[1].grid[SliderGrid::HSTART]->getHeight()) / 2 :
        (h - buttons[0].grid[SliderGrid::HSTART]->getHeight()) / 2;

    updateAlpha();

    if (mRedraw || graphics->getRedraw())
    {
        int x = 0;
        mRedraw = false;
        mVertexes->clear();
        if (!mHasMouse)
        {
            graphics->calcTileCollection(mVertexes,
                buttons[0].grid[SliderGrid::HSTART],
                x, y);

            const int width = buttons[0].grid[SliderGrid::HSTART]->getWidth();
            w -= width + buttons[0].grid[SliderGrid::HEND]->getWidth();
            x += width;

            if (buttons[0].grid[SliderGrid::HMID] != nullptr)
            {
                const Image *const hMid = buttons[0].grid[SliderGrid::HMID];
                graphics->calcPattern(mVertexes,
                    hMid,
                    x, y,
                    w, hMid->getHeight());
            }

            x += w;
            graphics->calcTileCollection(mVertexes,
                buttons[0].grid[SliderGrid::HEND],
                x, y);

            const Image *const img = buttons[0].grid[SliderGrid::HGRIP];
            if (img != nullptr)
            {
                graphics->calcTileCollection(mVertexes,
                    img,
                    getMarkerPosition(),
                    (mDimension.height - img->getHeight()) / 2);
            }
        }
        else
        {
            graphics->calcTileCollection(mVertexes,
                buttons[1].grid[SliderGrid::HSTART],
                x, y);

            const int width = buttons[1].grid[SliderGrid::HSTART]->getWidth();
            w -= width;
            if (buttons[1].grid[SliderGrid::HEND] != nullptr)
                w -= buttons[1].grid[SliderGrid::HEND]->getWidth();
            x += width;

            if (buttons[1].grid[SliderGrid::HMID] != nullptr)
            {
                const Image *const hMid = buttons[1].grid[SliderGrid::HMID];
                graphics->calcPattern(mVertexes,
                    hMid,
                    x, y,
                    w, hMid->getHeight());
            }

            x += w;
            if (buttons[1].grid[SliderGrid::HEND] != nullptr)
            {
                graphics->calcTileCollection(mVertexes,
                    buttons[1].grid[SliderGrid::HEND], x, y);
            }

            const Image *const img = buttons[1].grid[SliderGrid::HGRIP];
            if (img != nullptr)
            {
                graphics->calcTileCollection(mVertexes,
                    img,
                    getMarkerPosition(),
                    (mDimension.height - img->getHeight()) / 2);
            }
        }
        graphics->finalize(mVertexes);
    }
    graphics->drawTileCollection(mVertexes);

    BLOCK_END("Slider::draw")
}

void Slider::safeDraw(Graphics *const graphics)
{
    BLOCK_START("Slider::draw")
    if ((buttons[0].grid[SliderGrid::HSTART] == nullptr) ||
        (buttons[1].grid[SliderGrid::HSTART] == nullptr) ||
        (buttons[0].grid[SliderGrid::HEND] == nullptr))
    {
        BLOCK_END("Slider::draw")
        return;
    }

    int w = getWidth();
    const int h = getHeight();
    int x = 0;
    const int y = mHasMouse ?
        (h - buttons[1].grid[SliderGrid::HSTART]->getHeight()) / 2 :
        (h - buttons[0].grid[SliderGrid::HSTART]->getHeight()) / 2;

    updateAlpha();

    if (!mHasMouse)
    {
        graphics->drawImage(buttons[0].grid[SliderGrid::HSTART], x, y);
        const int width = buttons[0].grid[SliderGrid::HSTART]->getWidth();
        w -= width + buttons[0].grid[SliderGrid::HEND]->getWidth();
        x += width;

        if (buttons[0].grid[SliderGrid::HMID] != nullptr)
        {
            const Image *const hMid = buttons[0].grid[SliderGrid::HMID];
            graphics->drawPattern(hMid, x, y, w, hMid->getHeight());
        }

        x += w;
        graphics->drawImage(buttons[0].grid[SliderGrid::HEND], x, y);

        const Image *const img = buttons[0].grid[SliderGrid::HGRIP];
        if (img != nullptr)
        {
            graphics->drawImage(img, getMarkerPosition(),
                (mDimension.height - img->getHeight()) / 2);
        }
    }
    else
    {
        graphics->drawImage(buttons[1].grid[SliderGrid::HSTART], x, y);

        const int width = buttons[1].grid[SliderGrid::HSTART]->getWidth();
        w -= width;
        if (buttons[1].grid[SliderGrid::HEND] != nullptr)
            w -= buttons[1].grid[SliderGrid::HEND]->getWidth();
        x += width;

        if (buttons[1].grid[SliderGrid::HMID] != nullptr)
        {
            const Image *const hMid = buttons[1].grid[SliderGrid::HMID];
            graphics->drawPattern(hMid, x, y, w, hMid->getHeight());
        }

        x += w;
        if (buttons[1].grid[SliderGrid::HEND] != nullptr)
            graphics->drawImage(buttons[1].grid[SliderGrid::HEND], x, y);

        const Image *const img = buttons[1].grid[SliderGrid::HGRIP];
        if (img != nullptr)
        {
            graphics->drawImage(img, getMarkerPosition(),
                (mDimension.height - img->getHeight()) / 2);
        }
    }

    BLOCK_END("Slider::draw")
}

void Slider::mouseEntered(MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
    mRedraw = true;
}

void Slider::mouseExited(MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
    mRedraw = true;
}

void Slider::mousePressed(MouseEvent &event)
{
    const int x = event.getX();
    const int y = event.getY();
    const int width = mDimension.width;
    const int height = mDimension.height;

    if (event.getButton() == MouseButton::LEFT
        && x >= 0 && x <= width && y >= 0 && y <= height)
    {
        event.consume();
        if (mOrientation == Orientation::HORIZONTAL)
            setValue(markerPositionToValue(x - mMarkerLength / 2));
        else
            setValue(markerPositionToValue(height - y - mMarkerLength / 2));
        distributeActionEvent();
    }
}

void Slider::mouseDragged(MouseEvent &event)
{
    if (mOrientation == Orientation::HORIZONTAL)
    {
        setValue(markerPositionToValue(event.getX() - mMarkerLength / 2));
    }
    else
    {
        setValue(markerPositionToValue(
            mDimension.height - event.getY() - mMarkerLength / 2));
    }

    distributeActionEvent();

    event.consume();
}

void Slider::mouseWheelMovedUp(MouseEvent &event)
{
    setValue(mValue + mStepLength);
    distributeActionEvent();
    event.consume();
}

void Slider::mouseWheelMovedDown(MouseEvent &event)
{
    setValue(mValue - mStepLength);
    distributeActionEvent();
    event.consume();
}

void Slider::keyPressed(KeyEvent& event)
{
    const InputActionT action = event.getActionId();

    if (mOrientation == Orientation::HORIZONTAL)
    {
        if (action == InputAction::GUI_RIGHT)
        {
            setValue(mValue + mStepLength);
            distributeActionEvent();
            event.consume();
        }
        else if (action == InputAction::GUI_LEFT)
        {
            setValue(mValue - mStepLength);
            distributeActionEvent();
            event.consume();
        }
    }
    else
    {
        if (action == InputAction::GUI_UP)
        {
            setValue(mValue + mStepLength);
            distributeActionEvent();
            event.consume();
        }
        else if (action == InputAction::GUI_DOWN)
        {
            setValue(mValue - mStepLength);
            distributeActionEvent();
            event.consume();
        }
    }
}

void Slider::setScale(const double scaleStart, const double scaleEnd)
{
    mScaleStart = scaleStart;
    mScaleEnd = scaleEnd;
}

void Slider::setValue(const double value)
{
    mRedraw = true;
    if (value > mScaleEnd)
        mValue = mScaleEnd;
    else if (value < mScaleStart)
        mValue = mScaleStart;
    else
        mValue = value;
    mValue = CAST_S32((mValue - mScaleStart) / mStepLength)
        * mStepLength + mScaleStart;
}

double Slider::markerPositionToValue(const int v) const
{
    int w;
    if (mOrientation == Orientation::HORIZONTAL)
        w = mDimension.width;
    else
        w = mDimension.height;

    const double pos = v / (static_cast<double>(w) - mMarkerLength);
    return (1.0 - pos) * mScaleStart + pos * mScaleEnd;
}

int Slider::valueToMarkerPosition(const double value) const
{
    int v;
    if (mOrientation == Orientation::HORIZONTAL)
        v = mDimension.width;
    else
        v = mDimension.height;

    const int w = CAST_S32((v - mMarkerLength)
            * (value  - mScaleStart)
            / (mScaleEnd - mScaleStart));

    if (w < 0)
        return 0;

    if (w > v - mMarkerLength)
        return v - mMarkerLength;

    return w;
}
