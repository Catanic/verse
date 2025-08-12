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

#include "gui/widgets/scrollarea.h"

#include "settings.h"

#include "gui/gui.h"
#include "gui/skin.h"

#include "utils/delete2.h"
#include "utils/performance.h"
#include "utils/stringutils.h"

#include "render/graphics.h"

#include "render/vertexes/imagecollection.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "debug.h"

int ScrollArea::instances = 0;
float ScrollArea::mAlpha = 1.0;
bool ScrollArea::mShowButtons = true;
int ScrollArea::mMarkerSize = 0;
int ScrollArea::mScrollbarSize = 12;
ImageRect ScrollArea::background;
ImageRect ScrollArea::vMarker;
ImageRect ScrollArea::vMarkerHi;
ImageRect ScrollArea::vBackground;
ImageRect ScrollArea::hBackground;
Image *ScrollArea::buttons[4][2];

static std::string const buttonFiles[2] =
{
    "scrollbuttons.xml",
    "scrollbuttons_pressed.xml"
};

ScrollArea::ScrollArea(Widget2 *const widget2,
                       Widget *const widget,
                       const Opaque opaque,
                       const std::string &skin) :
    BasicContainer(widget2),
    MouseListener(),
    WidgetListener(),
    mVertexes(new ImageCollection),
    mVertexes2(new ImageCollection),
    mHPolicy(SHOW_AUTO),
    mVPolicy(SHOW_AUTO),
    mVScroll(0),
    mHScroll(0),
    mScrollbarWidth(12),
    mUpButtonScrollAmount(10),
    mDownButtonScrollAmount(10),
    mLeftButtonScrollAmount(10),
    mRightButtonScrollAmount(10),
    mHorizontalMarkerDragOffset(0),
    mVerticalMarkerDragOffset(0),
    mX(0),
    mY(0),
    mClickX(0),
    mClickY(0),
    mXOffset(0),
    mYOffset(0),
    mDrawWidth(0),
    mDrawHeight(0),
    mVBarVisible(false),
    mHBarVisible(false),
    mUpButtonPressed(false),
    mDownButtonPressed(false),
    mLeftButtonPressed(false),
    mRightButtonPressed(false),
    mIsVerticalMarkerDragged(false),
    mIsHorizontalMarkerDragged(false),
    mOpaque(Opaque_true),
    mHasMouse(false)
{
    setContent(widget);
    addMouseListener(this);
    mOpaque = opaque;
    init(skin);
}

ScrollArea::~ScrollArea()
{
    if (gui != nullptr)
        gui->removeDragged(this);

    // Garbage collection
    delete getContent();

    instances--;
    if (instances == 0)
    {
        Theme::unloadRect(background, 0, 8);
        Theme::unloadRect(vMarker, 0, 8);
        Theme::unloadRect(vMarkerHi, 0, 8);
        Theme::unloadRect(vBackground, 0, 8);
        Theme::unloadRect(hBackground, 0, 8);
        for (int i = 0; i < 2; i ++)
        {
            for (int f = UP; f < BUTTONS_DIR; f ++)
            {
                if (buttons[f][i] != nullptr)
                    buttons[f][i]->decRef();
            }
        }
    }

    delete2(mVertexes)
    delete2(mVertexes2)

    setContent(nullptr);
}

void ScrollArea::init(std::string skinName)
{
    setOpaque(mOpaque);

    setUpButtonScrollAmount(2);
    setDownButtonScrollAmount(2);
    setLeftButtonScrollAmount(2);
    setRightButtonScrollAmount(2);

    if (instances == 0)
    {
        for (int f = 0; f < 9; f ++)
        {
            background.grid[f] = nullptr;
            vMarker.grid[f] = nullptr;
            vMarkerHi.grid[f] = nullptr;
            vBackground.grid[f] = nullptr;
            hBackground.grid[f] = nullptr;
        }

        // +++ here probably need move background from static
        if (skinName.empty())
            skinName = "scroll_background.xml";
        if (theme != nullptr)
        {
            theme->loadRect(background,
                skinName,
                "scroll_background.xml",
                0,
                8);
            theme->loadRect(vMarker,
                "scroll.xml",
                "",
                0,
                8);
            theme->loadRect(vMarkerHi,
                "scroll_highlighted.xml",
                "scroll.xml",
                0,
                8);
            theme->loadRect(vBackground,
                "scroll_vbackground.xml",
                "",
                0,
                8);
            theme->loadRect(hBackground,
                "scroll_hbackground.xml",
                "",
                0,
                8);
        }

        for (int i = 0; i < 2; i ++)
        {
            Skin *skin = nullptr;
            if (theme != nullptr)
            {
                skin = theme->load(buttonFiles[i],
                    "scrollbuttons.xml",
                    true,
                    Theme::getThemePath());
            }
            if (skin != nullptr)
            {
                const ImageRect &rect = skin->getBorder();
                for (int f = UP; f < BUTTONS_DIR; f ++)
                {
                    if (rect.grid[f] != nullptr)
                        rect.grid[f]->incRef();
                    buttons[f][i] = rect.grid[f];
                }
                if (i == 0)
                {
                    mShowButtons = (skin->getOption("showbuttons", 1) == 1);
                    mMarkerSize = skin->getOption("markersize", 0);
                    mScrollbarSize = skin->getOption("scrollbarsize", 12);
                }
            }
            else
            {
                for (int f = UP; f < BUTTONS_DIR; f ++)
                    buttons[f][i] = nullptr;
            }
            if (theme != nullptr)
                theme->unload(skin);
        }
    }
    mScrollbarWidth = mScrollbarSize;
    instances++;
}

void ScrollArea::logic()
{
    BLOCK_START("ScrollArea::logic")
    if (!isVisible())
    {
        BLOCK_END("ScrollArea::logic")
        return;
    }

    checkPolicies();

    setVerticalScrollAmount(mVScroll);
    setHorizontalScrollAmount(mHScroll);

    Widget *const content = getContent();
    if (content != nullptr)
    {
        unsigned int frameSize = content->getFrameSize();
        content->setPosition(-mHScroll + frameSize, -mVScroll + frameSize);
        content->logic();

        // When no scrollbar in a certain direction,
        // adapt content size to match the content dimension exactly.
        frameSize = 2 * content->getFrameSize();
        if (mHPolicy == ScrollArea::SHOW_NEVER)
        {
            content->setWidth((mVBarVisible ? (mDimension.width
                - mScrollbarWidth) : mDimension.width) - frameSize);
        }
        if (mVPolicy == ScrollArea::SHOW_NEVER)
        {
            content->setHeight((mHBarVisible ? (mDimension.height
                - mScrollbarWidth) : mDimension.height) - frameSize);
        }
    }

    if (mUpButtonPressed)
        setVerticalScrollAmount(mVScroll - mUpButtonScrollAmount);
    else if (mDownButtonPressed)
        setVerticalScrollAmount(mVScroll + mDownButtonScrollAmount);
    else if (mLeftButtonPressed)
        setHorizontalScrollAmount(mHScroll - mLeftButtonScrollAmount);
    else if (mRightButtonPressed)
        setHorizontalScrollAmount(mHScroll + mRightButtonScrollAmount);
    BLOCK_END("ScrollArea::logic")
}

void ScrollArea::updateAlpha()
{
    const float alpha = std::max(settings.guiAlpha,
        theme->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int a = 0; a < 9; a++)
        {
            if (background.grid[a] != nullptr)
                background.grid[a]->setAlpha(mAlpha);
            if (hBackground.grid[a] != nullptr)
                hBackground.grid[a]->setAlpha(mAlpha);
            if (vBackground.grid[a] != nullptr)
                vBackground.grid[a]->setAlpha(mAlpha);
            if (vMarker.grid[a] != nullptr)
                vMarker.grid[a]->setAlpha(mAlpha);
            if (vMarkerHi.grid[a] != nullptr)
                vMarkerHi.grid[a]->setAlpha(mAlpha);
        }
    }
}

void ScrollArea::draw(Graphics *const graphics)
{
    BLOCK_START("ScrollArea::draw")
    if (mVBarVisible || mHBarVisible)
    {
        if (mOpaque == Opaque_false)
            updateCalcFlag(graphics);
        // need add caching or remove calc calls.
//        if (mRedraw)
        {
            mVertexes->clear();
            if (mVBarVisible)
            {
                if (mShowButtons)
                {
                    calcButton(graphics, UP);
                    calcButton(graphics, DOWN);
                }
                calcVBar(graphics);
                calcVMarker(graphics);
            }

            if (mHBarVisible)
            {
                if (mShowButtons)
                {
                    calcButton(graphics, LEFT);
                    calcButton(graphics, RIGHT);
                }
                calcHBar(graphics);
                calcHMarker(graphics);
            }
            graphics->finalize(mVertexes);
        }
        graphics->drawTileCollection(mVertexes);
    }

    updateAlpha();

    if (mRedraw)
    {
        const bool redraw = graphics->getRedraw();
        graphics->setRedraw(true);
        drawChildren(graphics);
        graphics->setRedraw(redraw);
    }
    else
    {
        drawChildren(graphics);
    }
    mRedraw = false;
    BLOCK_END("ScrollArea::draw")
}

void ScrollArea::safeDraw(Graphics *const graphics)
{
    BLOCK_START("ScrollArea::draw")
    if (mVBarVisible)
    {
        if (mShowButtons)
        {
            drawButton(graphics, UP);
            drawButton(graphics, DOWN);
        }
        drawVBar(graphics);
        drawVMarker(graphics);
    }

    if (mHBarVisible)
    {
        if (mShowButtons)
        {
            drawButton(graphics, LEFT);
            drawButton(graphics, RIGHT);
        }
        drawHBar(graphics);
        drawHMarker(graphics);
    }

    updateAlpha();

    safeDrawChildren(graphics);
    mRedraw = false;
    BLOCK_END("ScrollArea::draw")
}

void ScrollArea::updateCalcFlag(const Graphics *const graphics)
{
    if (!mRedraw)
    {
        // because we don't know where parent windows was moved,
        // need recalc vertexes
        const ClipRect &rect = graphics->getTopClip();
        if (rect.xOffset != mXOffset || rect.yOffset != mYOffset)
        {
            mRedraw = true;
            mXOffset = rect.xOffset;
            mYOffset = rect.yOffset;
        }
        else if (rect.width != mDrawWidth || rect.height != mDrawHeight)
        {
            mRedraw = true;
            mDrawWidth = rect.width;
            mDrawHeight = rect.height;
        }
        else if (graphics->getRedraw())
        {
            mRedraw = true;
        }
    }
}

void ScrollArea::drawFrame(Graphics *const graphics)
{
    BLOCK_START("ScrollArea::drawFrame")
    if (mOpaque == Opaque_true)
    {
        updateCalcFlag(graphics);

        if (mRedraw)
        {
            const int bs = mFrameSize * 2;
            const int w = mDimension.width + bs;
            const int h = mDimension.height + bs;
            mVertexes2->clear();
            graphics->calcWindow(mVertexes2,
                0, 0,
                w, h,
                background);
            graphics->finalize(mVertexes2);
        }
        graphics->drawTileCollection(mVertexes2);
    }
    BLOCK_END("ScrollArea::drawFrame")
}

void ScrollArea::safeDrawFrame(Graphics *const graphics)
{
    BLOCK_START("ScrollArea::drawFrame")
    if (mOpaque == Opaque_true)
    {
        const int bs = mFrameSize * 2;
        const int w = mDimension.width + bs;
        const int h = mDimension.height + bs;

        updateCalcFlag(graphics);
        graphics->drawImageRect(0, 0,
            w, h,
            background);
    }
    BLOCK_END("ScrollArea::drawFrame")
}

void ScrollArea::setOpaque(Opaque opaque)
{
    mOpaque = opaque;
    setFrameSize(mOpaque == Opaque_true ? 2 : 0);
}

Image *ScrollArea::getImageByState(Rect &dim, const BUTTON_DIR dir)
{
    int state = 0;

    switch (dir)
    {
        case UP:
            state = mUpButtonPressed ? 1 : 0;
            dim = getUpButtonDimension();
            break;
        case DOWN:
            state = mDownButtonPressed ? 1 : 0;
            dim = getDownButtonDimension();
            break;
        case LEFT:
            state = mLeftButtonPressed ? 1 : 0;
            dim = getLeftButtonDimension();
            break;
        case RIGHT:
            state = mRightButtonPressed ? 1 : 0;
            dim = getRightButtonDimension();
            break;
        case BUTTONS_DIR:
        default:
            logger->log("ScrollArea::drawButton unknown dir: "
                        + toString(CAST_U32(dir)));
            return nullptr;
    }
    return buttons[CAST_SIZE(dir)][state];
}

void ScrollArea::drawButton(Graphics *const graphics,
                            const BUTTON_DIR dir)
{
    Rect dim;
    const Image *const image = getImageByState(dim, dir);

    if (image != nullptr)
        graphics->drawImage(image, dim.x, dim.y);
}

void ScrollArea::calcButton(Graphics *const graphics,
                            const BUTTON_DIR dir)
{
    Rect dim;
    const Image *const image = getImageByState(dim, dir);

    if (image != nullptr)
    {
        static_cast<Graphics*>(graphics)->calcTileCollection(
            mVertexes, image, dim.x, dim.y);
    }
}

void ScrollArea::drawVBar(Graphics *const graphics) const
{
    const Rect &dim = getVerticalBarDimension();

    if (vBackground.grid[4] != nullptr)
    {
        graphics->drawPattern(vBackground.grid[4],
            dim.x, dim.y, dim.width, dim.height);
    }
    if (vBackground.grid[1] != nullptr)
    {
        graphics->drawPattern(vBackground.grid[1],
            dim.x, dim.y,
            dim.width, vBackground.grid[1]->getHeight());
    }
    if (vBackground.grid[7] != nullptr)
    {
        graphics->drawPattern(vBackground.grid[7],
            dim.x, dim.height - vBackground.grid[7]->getHeight() + dim.y,
            dim.width, vBackground.grid[7]->getHeight());
    }
}

void ScrollArea::calcVBar(const Graphics *const graphics)
{
    const Rect &dim = getVerticalBarDimension();

    if (vBackground.grid[4] != nullptr)
    {
        graphics->calcPattern(mVertexes,
            vBackground.grid[4],
            dim.x, dim.y,
            dim.width, dim.height);
    }
    if (vBackground.grid[1] != nullptr)
    {
        graphics->calcPattern(mVertexes,
            vBackground.grid[1],
            dim.x, dim.y,
            dim.width, vBackground.grid[1]->getHeight());
    }
    if (vBackground.grid[7] != nullptr)
    {
        graphics->calcPattern(mVertexes,
            vBackground.grid[7],
            dim.x, dim.height - vBackground.grid[7]->getHeight() + dim.y,
            dim.width, vBackground.grid[7]->getHeight());
    }
}

void ScrollArea::drawHBar(Graphics *const graphics) const
{
    const Rect &dim = getHorizontalBarDimension();

    if (hBackground.grid[4] != nullptr)
    {
        graphics->drawPattern(hBackground.grid[4],
            dim.x, dim.y,
            dim.width, dim.height);
    }

    if (hBackground.grid[3] != nullptr)
    {
        graphics->drawPattern(hBackground.grid[3],
            dim.x, dim.y,
            hBackground.grid[3]->getWidth(), dim.height);
    }

    if (hBackground.grid[5] != nullptr)
    {
        graphics->drawPattern(hBackground.grid[5],
            dim.x + dim.width - hBackground.grid[5]->getWidth(),
            dim.y,
            hBackground.grid[5]->getWidth(),
            dim.height);
    }
}

void ScrollArea::calcHBar(const Graphics *const graphics)
{
    const Rect &dim = getHorizontalBarDimension();

    if (hBackground.grid[4] != nullptr)
    {
        graphics->calcPattern(mVertexes,
            hBackground.grid[4],
            dim.x, dim.y,
            dim.width, dim.height);
    }

    if (hBackground.grid[3] != nullptr)
    {
        graphics->calcPattern(mVertexes,
            hBackground.grid[3],
            dim.x, dim.y,
            hBackground.grid[3]->getWidth(), dim.height);
    }

    if (hBackground.grid[5] != nullptr)
    {
        graphics->calcPattern(mVertexes,
            hBackground.grid[5],
            dim.x + dim.width - hBackground.grid[5]->getWidth(),
            dim.y,
            hBackground.grid[5]->getWidth(),
            dim.height);
    }
}

void ScrollArea::drawVMarker(Graphics *const graphics)
{
    const Rect &dim = getVerticalMarkerDimension();

    if ((mHasMouse) && (mX > (mDimension.width - mScrollbarWidth)))
    {
        graphics->drawImageRect(dim.x, dim.y,
            dim.width, dim.height,
            vMarkerHi);
    }
    else
    {
        graphics->drawImageRect(dim.x, dim.y,
            dim.width, dim.height,
            vMarker);
    }
}

void ScrollArea::calcVMarker(Graphics *const graphics)
{
    const Rect &dim = getVerticalMarkerDimension();

    if ((mHasMouse) && (mX > (mDimension.width - mScrollbarWidth)))
    {
        graphics->calcWindow(mVertexes,
            dim.x, dim.y,
            dim.width, dim.height,
            vMarkerHi);
    }
    else
    {
        graphics->calcWindow(mVertexes,
            dim.x, dim.y,
            dim.width, dim.height,
            vMarker);
    }
}

void ScrollArea::drawHMarker(Graphics *const graphics)
{
    const Rect dim = getHorizontalMarkerDimension();

    if ((mHasMouse) && (mY > (mDimension.height - mScrollbarWidth)))
    {
        graphics->drawImageRect(dim.x, dim.y,
            dim.width, dim.height,
            vMarkerHi);
    }
    else
    {
        graphics->drawImageRect(
            dim.x, dim.y,
            dim.width, dim.height,
            vMarker);
    }
}

void ScrollArea::calcHMarker(Graphics *const graphics)
{
    const Rect dim = getHorizontalMarkerDimension();

    if ((mHasMouse) && (mY > (mDimension.height - mScrollbarWidth)))
    {
        graphics->calcWindow(mVertexes,
            dim.x, dim.y,
            dim.width, dim.height,
            vMarkerHi);
    }
    else
    {
        graphics->calcWindow(mVertexes,
            dim.x, dim.y,
            dim.width, dim.height,
            vMarker);
    }
}

void ScrollArea::mouseMoved(MouseEvent& event)
{
    mX = event.getX();
    mY = event.getY();
}

void ScrollArea::mouseEntered(MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void ScrollArea::mouseExited(MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void ScrollArea::widgetResized(const Event &event A_UNUSED)
{
    mRedraw = true;
    Widget *const content = getContent();
    if (content != nullptr)
    {
        const unsigned int frameSize = 2 * mFrameSize;
        content->setSize(mDimension.width - frameSize,
            mDimension.height - frameSize);
    }
}

void ScrollArea::widgetMoved(const Event& event A_UNUSED)
{
    mRedraw = true;
}

void ScrollArea::mousePressed(MouseEvent& event)
{
    const int x = event.getX();
    const int y = event.getY();

    if (getUpButtonDimension().isPointInRect(x, y))
    {
        setVerticalScrollAmount(mVScroll
                                - mUpButtonScrollAmount);
        mUpButtonPressed = true;
        event.consume();
    }
    else if (getDownButtonDimension().isPointInRect(x, y))
    {
        setVerticalScrollAmount(mVScroll
                                + mDownButtonScrollAmount);
        mDownButtonPressed = true;
        event.consume();
    }
    else if (getLeftButtonDimension().isPointInRect(x, y))
    {
        setHorizontalScrollAmount(mHScroll
                                  - mLeftButtonScrollAmount);
        mLeftButtonPressed = true;
        event.consume();
    }
    else if (getRightButtonDimension().isPointInRect(x, y))
    {
        setHorizontalScrollAmount(mHScroll
                                  + mRightButtonScrollAmount);
        mRightButtonPressed = true;
        event.consume();
    }
    else if (getVerticalMarkerDimension().isPointInRect(x, y))
    {
        mIsHorizontalMarkerDragged = false;
        mIsVerticalMarkerDragged = true;

        mVerticalMarkerDragOffset = y - getVerticalMarkerDimension().y;
        event.consume();
    }
    else if (getVerticalBarDimension().isPointInRect(x, y))
    {
        if (y < getVerticalMarkerDimension().y)
        {
            setVerticalScrollAmount(mVScroll
                - CAST_S32(getChildrenArea().height * 0.1));
        }
        else
        {
            setVerticalScrollAmount(mVScroll
                + CAST_S32(getChildrenArea().height * 0.1));
        }
        event.consume();
    }
    else if (getHorizontalMarkerDimension().isPointInRect(x, y))
    {
        mIsHorizontalMarkerDragged = true;
        mIsVerticalMarkerDragged = false;
        mHorizontalMarkerDragOffset = x - getHorizontalMarkerDimension().x;
        event.consume();
    }
    else if (getHorizontalBarDimension().isPointInRect(x, y))
    {
        if (x < getHorizontalMarkerDimension().x)
        {
            setHorizontalScrollAmount(mHScroll
                - CAST_S32(getChildrenArea().width * 0.1));
        }
        else
        {
            setHorizontalScrollAmount(mHScroll
                + CAST_S32(getChildrenArea().width * 0.1));
        }
        event.consume();
    }

    if (event.getButton() == MouseButton::LEFT &&
        !event.isConsumed())
    {
        mClickX = event.getX();
        mClickY = event.getY();
    }
}

void ScrollArea::mouseReleased(MouseEvent& event)
{
    if (event.getButton() == MouseButton::LEFT &&
        mClickX != 0 &&
        mClickY != 0)
    {
        if (!event.isConsumed())
        {
#ifdef ANDROID
            int dx = mClickX - event.getX();
            int dy = mClickY - event.getY();
#else  // ANDROID

            int dx = event.getX() - mClickX;
            int dy = event.getY() - mClickY;
#endif  // ANDROID

            if ((dx < 20 && dx > 0) || (dx > -20 && dx < 0))
                dx = 0;

            if ((dy < 20 && dy > 0) || (dy > -20 && dy < 0))
                dy = 0;

            if (abs(dx) > abs(dy))
            {
                int s = mHScroll + dx;
                if (s < 0)
                {
                    s = 0;
                }
                else
                {
                    const int maxH = getHorizontalMaxScroll();
                    if (s > maxH)
                        s = maxH;
                }

                setHorizontalScrollAmount(s);
            }
            else if (dy != 0)
            {
                int s = mVScroll + dy;
                if (s < 0)
                {
                    s = 0;
                }
                else
                {
                    const int maxV = getVerticalMaxScroll();
                    if (s > maxV)
                        s = maxV;
                }

                setVerticalScrollAmount(s);
            }
            mClickX = 0;
            mClickY = 0;
            if (mMouseConsume && ((dx != 0) || (dy != 0)))
                event.consume();
        }
    }
    mUpButtonPressed = false;
    mDownButtonPressed = false;
    mLeftButtonPressed = false;
    mRightButtonPressed = false;
    mIsHorizontalMarkerDragged = false;
    mIsVerticalMarkerDragged = false;
    if (mMouseConsume)
        event.consume();
    mRedraw = true;
}

void ScrollArea::mouseDragged(MouseEvent &event)
{
    if (mIsVerticalMarkerDragged)
    {
        const Rect barDim = getVerticalBarDimension();
        const int length = getVerticalMarkerDimension().height;

        if ((barDim.height - length) > 0)
        {
            const int pos = event.getY() - barDim.y
                - mVerticalMarkerDragOffset;
            setVerticalScrollAmount((getVerticalMaxScroll() * pos)
                                      / (barDim.height - length));
        }
        else
        {
            setVerticalScrollAmount(0);
        }
    }

    if (mIsHorizontalMarkerDragged)
    {
        const Rect barDim = getHorizontalBarDimension();
        const int length = getHorizontalMarkerDimension().width;

        if ((barDim.width - length) > 0)
        {
            const int pos = event.getX() - barDim.x
                - mHorizontalMarkerDragOffset;
            setHorizontalScrollAmount((getHorizontalMaxScroll() * pos)
                                      / (barDim.width - length));
        }
        else
        {
            setHorizontalScrollAmount(0);
        }
    }

    event.consume();
    mRedraw = true;
}

Rect ScrollArea::getVerticalBarDimension() const
{
    if (!mVBarVisible)
        return Rect(0, 0, 0, 0);

    const int height = mShowButtons ? mScrollbarWidth : 0;
    if (mHBarVisible)
    {
        return Rect(mDimension.width - mScrollbarWidth,
            height,
            mScrollbarWidth,
            mDimension.height - 2 * height - mScrollbarWidth);
    }

    return Rect(mDimension.width - mScrollbarWidth,
        height,
        mScrollbarWidth,
        mDimension.height - 2 * height);
}

Rect ScrollArea::getHorizontalBarDimension() const
{
    if (!mHBarVisible)
        return Rect(0, 0, 0, 0);

    const int width = mShowButtons ? mScrollbarWidth : 0;
    if (mVBarVisible)
    {
        return Rect(width,
            mDimension.height - mScrollbarWidth,
            mDimension.width - 2 * width - mScrollbarWidth,
            mScrollbarWidth);
    }

    return Rect(width,
                      mDimension.height - mScrollbarWidth,
                      mDimension.width - 2 * width,
                      mScrollbarWidth);
}

Rect ScrollArea::getVerticalMarkerDimension()
{
    if (!mVBarVisible)
        return Rect(0, 0, 0, 0);

    int length;
    int pos;
    int height;
    const int h2 = mShowButtons
        ? mScrollbarWidth : mMarkerSize / 2;
    const Widget *content;
    if (!mWidgets.empty())
        content = *mWidgets.begin();
    else
        content = nullptr;

    if (mHBarVisible)
        height = mDimension.height - 2 * h2 - mScrollbarWidth;
    else
        height = mDimension.height - 2 * h2;

    const int maxV = getVerticalMaxScroll();
    if ((mMarkerSize != 0) && (maxV != 0))
    {
        pos = (mVScroll * height / maxV - mMarkerSize / 2);
        length = mMarkerSize;
    }
    else
    {
        if (content != nullptr)
        {
            const int h3 = content->getHeight();
            if (h3 != 0)
                length = (height * getChildrenArea().height) / h3;
            else
                length = height;
        }
        else
        {
            length = height;
        }

        if (length < mScrollbarWidth)
            length = mScrollbarWidth;

        if (length > height)
            length = height;

        if (maxV != 0)
            pos = ((height - length) * mVScroll) / maxV;
        else
            pos = 0;
    }

    return Rect(mDimension.width - mScrollbarWidth, h2 + pos,
        mScrollbarWidth, length);
}

Rect ScrollArea::getHorizontalMarkerDimension()
{
    if (!mHBarVisible)
        return Rect(0, 0, 0, 0);

    int length;
    int pos;
    int width;
    const int w2 = mShowButtons
        ? mScrollbarWidth : mMarkerSize / 2;
    const Widget *content;
    if (!mWidgets.empty())
        content = *mWidgets.begin();
    else
        content = nullptr;

    if (mVBarVisible)
        width = mDimension.width - 2 * w2 - mScrollbarWidth;
    else
        width = mDimension.width - w2 - mScrollbarWidth;

    const int maxH = getHorizontalMaxScroll();
    if (mMarkerSize != 0 && maxH != 0)
    {
        pos = (mHScroll * width / maxH - mMarkerSize / 2);
        length = mMarkerSize;
    }
    else
    {
        if (content != nullptr)
        {
            const int w3 = content->getWidth();
            if (w3 != 0)
                length = (width * getChildrenArea().width) / w3;
            else
                length = width;
        }
        else
        {
            length = width;
        }

        if (length < mScrollbarWidth)
            length = mScrollbarWidth;

        if (length > width)
            length = width;

        if (maxH != 0)
        {
            pos = ((width - length) * mHScroll) / maxH;
        }
        else
        {
            pos = 0;
        }
    }

    return Rect(w2 + pos, mDimension.height - mScrollbarWidth,
        length, mScrollbarWidth);
}

Rect ScrollArea::getUpButtonDimension() const
{
    if (!mVBarVisible || !mShowButtons)
        return Rect(0, 0, 0, 0);

    return Rect(mDimension.width - mScrollbarWidth, 0,
        mScrollbarWidth, mScrollbarWidth);
}

Rect ScrollArea::getDownButtonDimension() const
{
    if (!mVBarVisible || !mShowButtons)
        return Rect(0, 0, 0, 0);

    if (mHBarVisible)
    {
        return Rect(mDimension.width - mScrollbarWidth,
            mDimension.height - mScrollbarWidth * 2,
            mScrollbarWidth,
            mScrollbarWidth);
    }

    return Rect(mDimension.width - mScrollbarWidth,
        mDimension.height - mScrollbarWidth,
        mScrollbarWidth,
        mScrollbarWidth);
}

Rect ScrollArea::getLeftButtonDimension() const
{
    if (!mHBarVisible || !mShowButtons)
        return Rect(0, 0, 0, 0);

    return Rect(0, mDimension.height - mScrollbarWidth,
        mScrollbarWidth, mScrollbarWidth);
}

Rect ScrollArea::getRightButtonDimension() const
{
    if (!mHBarVisible || !mShowButtons)
        return Rect(0, 0, 0, 0);

    if (mVBarVisible)
    {
        return Rect(mDimension.width - mScrollbarWidth*2,
            mDimension.height - mScrollbarWidth,
            mScrollbarWidth,
            mScrollbarWidth);
    }

    return Rect(mDimension.width - mScrollbarWidth,
        mDimension.height - mScrollbarWidth,
        mScrollbarWidth,
        mScrollbarWidth);
}

void ScrollArea::setContent(Widget* widget)
{
    if (widget != nullptr)
    {
        clear();
        add(widget);
        widget->setPosition(0, 0);
    }
    else
    {
        clear();
    }

    checkPolicies();
}

Widget* ScrollArea::getContent()
{
    if (!mWidgets.empty())
        return *mWidgets.begin();

    return nullptr;
}

void ScrollArea::setHorizontalScrollPolicy(const ScrollPolicy hPolicy)
{
    mHPolicy = hPolicy;
    checkPolicies();
}

void ScrollArea::setVerticalScrollPolicy(const ScrollPolicy vPolicy)
{
    mVPolicy = vPolicy;
    checkPolicies();
}

void ScrollArea::setScrollPolicy(const ScrollPolicy hPolicy,
                                 const ScrollPolicy vPolicy)
{
    mHPolicy = hPolicy;
    mVPolicy = vPolicy;
    checkPolicies();
}

void ScrollArea::setVerticalScrollAmount(const int vScroll)
{
    const int max = getVerticalMaxScroll();

    mVScroll = vScroll;

    if (vScroll > max)
        mVScroll = max;

    if (vScroll < 0)
        mVScroll = 0;
}

void ScrollArea::setHorizontalScrollAmount(int hScroll)
{
    const int max = getHorizontalMaxScroll();

    mHScroll = hScroll;

    if (hScroll > max)
        mHScroll = max;
    else if (hScroll < 0)
        mHScroll = 0;
}

void ScrollArea::setScrollAmount(const int hScroll, const int vScroll)
{
    setHorizontalScrollAmount(hScroll);
    setVerticalScrollAmount(vScroll);
}

int ScrollArea::getHorizontalMaxScroll()
{
    checkPolicies();

    const Widget *const content = getContent();
    if (content == nullptr)
        return 0;

    const int value = content->getWidth() - getChildrenArea().width +
        2 * content->getFrameSize();

    if (value < 0)
        return 0;

    return value;
}

int ScrollArea::getVerticalMaxScroll()
{
    checkPolicies();

    const Widget *const content = getContent();
    if (content == nullptr)
        return 0;

    int value;

    value = content->getHeight() - getChildrenArea().height +
        2 * content->getFrameSize();

    if (value < 0)
        return 0;

    return value;
}

void ScrollArea::setScrollbarWidth(const int width)
{
    if (width > 0)
        mScrollbarWidth = width;
}

void ScrollArea::showWidgetPart(Widget *const widget, const Rect &area)
{
    const Widget *const content = getContent();
    if (widget != content || (content == nullptr))
        return;

    BasicContainer::showWidgetPart(widget, area);

    setHorizontalScrollAmount(content->getFrameSize()
        - content->getX());
    setVerticalScrollAmount(content->getFrameSize()
        - content->getY());
}

Rect ScrollArea::getChildrenArea()
{
    const Rect area = Rect(0, 0,
        mVBarVisible ? (getWidth() - mScrollbarWidth) : getWidth(),
        mHBarVisible ? (getHeight() - mScrollbarWidth) : getHeight());

    if (area.width < 0 || area.height < 0)
        return Rect();

    return area;
}

Widget *ScrollArea::getWidgetAt(int x, int y)
{
    if (getChildrenArea().isPointInRect(x, y))
        return getContent();

    return nullptr;
}

void ScrollArea::setWidth(int width)
{
    Widget::setWidth(width);
    checkPolicies();
}

void ScrollArea::setHeight(int height)
{
    Widget::setHeight(height);
    checkPolicies();
}

void ScrollArea::setDimension(const Rect& dimension)
{
    Widget::setDimension(dimension);
    checkPolicies();
}

void ScrollArea::mouseWheelMovedUp(MouseEvent& event)
{
    if (event.isConsumed())
        return;

    setVerticalScrollAmount(mVScroll
        - getChildrenArea().height / 8);

    event.consume();
}

void ScrollArea::mouseWheelMovedDown(MouseEvent& event)
{
    if (event.isConsumed())
        return;

    setVerticalScrollAmount(mVScroll
        + getChildrenArea().height / 8);

    event.consume();
}

void ScrollArea::checkPolicies()
{
    const int w = getWidth();
    const int h = getHeight();

    mHBarVisible = false;
    mVBarVisible = false;

    const Widget *const content = getContent();
    if (content == nullptr)
    {
        mHBarVisible = (mHPolicy == SHOW_ALWAYS);
        mVBarVisible = (mVPolicy == SHOW_ALWAYS);
        return;
    }

    if (mHPolicy == SHOW_AUTO &&
        mVPolicy == SHOW_AUTO)
    {
        if (content->getWidth() <= w
            && content->getHeight() <= h)
        {
            mHBarVisible = false;
            mVBarVisible = false;
        }

        if (content->getWidth() > w)
        {
            mHBarVisible = true;
        }

        if ((content->getHeight() > h)
            || (mHBarVisible && content->getHeight()
            > h - mScrollbarWidth))
        {
            mVBarVisible = true;
        }

        if (mVBarVisible && content->getWidth() > w - mScrollbarWidth)
            mHBarVisible = true;

        return;
    }

    switch (mHPolicy)
    {
        case SHOW_NEVER:
            mHBarVisible = false;
            break;

        case SHOW_ALWAYS:
            mHBarVisible = true;
            break;

        case SHOW_AUTO:
            if (mVPolicy == SHOW_NEVER)
            {
                mHBarVisible = (content->getWidth() > w);
            }
            else  // (mVPolicy == SHOW_ALWAYS)
            {
                mHBarVisible = (content->getWidth()
                    > w - mScrollbarWidth);
            }
            break;

        default:
            break;
    }

    switch (mVPolicy)
    {
        case SHOW_NEVER:
            mVBarVisible = false;
            break;

        case SHOW_ALWAYS:
            mVBarVisible = true;
            break;

        case SHOW_AUTO:
            if (mHPolicy == SHOW_NEVER)
            {
                mVBarVisible = (content->getHeight() > h);
            }
            else  // (mHPolicy == SHOW_ALWAYS)
            {
                mVBarVisible = (content->getHeight()
                    > h - mScrollbarWidth);
            }
            break;
        default:
            break;
    }
}

bool ScrollArea::isSelectable() const noexcept2
{
    if (mVBarVisible || mHBarVisible)
        return true;
    return Widget::isSelectable();
}
