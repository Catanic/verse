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

#include "gui/widgets/window.h"

#include "client.h"
#include "configuration.h"
#ifndef DYECMD
#include "dragdrop.h"
#else  // DYECMD
#include "resources/image/image.h"
#endif  // DYECMD
#include "soundmanager.h"

#include "const/sound.h"

#include "gui/focushandler.h"
#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/skin.h"
#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layout.h"

#include "render/renderers.h"

#include "render/vertexes/imagecollection.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/performance.h"

#include "debug.h"

const int resizeMask = 8 + 4 + 2 + 1;

int Window::windowInstances = 0;
int Window::mouseResize = 0;

Window::Window(const std::string &caption,
               const Modal modal,
               Window *const parent,
               std::string skin) :
    BasicContainer2(nullptr),
    MouseListener(),
    WidgetListener(),
    mCaption(caption),
    mAlignment(Graphics::CENTER),
    mPadding(2),
    mTitleBarHeight(16),
    mMovable(Move_true),
    mDragOffsetX(0),
    mDragOffsetY(0),
    mMoved(false),
    mSkin(nullptr),
    mDefaultX(0),
    mDefaultY(0),
    mDefaultWidth(0),
    mDefaultHeight(0),
    mCaptionOffsetX(7),
    mCaptionOffsetY(5),
    mShowTitle(true),
    mLastRedraw(true),
    mGrip(nullptr),
    mParentWindow(parent),
    mLayout(nullptr),
    mCloseRect(),
    mStickyRect(),
    mGripRect(),
    mTextChunk(),
    mWindowName("window"),
    mMinWinWidth(100),
    mMinWinHeight(40),
    mMaxWinWidth(mainGraphics->mWidth),
    mMaxWinHeight(mainGraphics->mHeight),
    mVertexes(new ImageCollection),
    mCaptionAlign(Graphics::LEFT),
    mTitlePadding(4),
    mGripPadding(2),
    mResizeHandles(-1),
    mOldResizeHandles(-1),
    mClosePadding(0),
    mStickySpacing(0),
    mStickyPadding(0),
    mCaptionFont(getFont()),
    mModal(modal),
    mCloseWindowButton(false),
    mDefaultVisible(false),
    mSaveVisible(false),
    mStickyButton(false),
    mSticky(false),
    mStickyButtonLock(false),
    mPlayVisibleSound(false),
    mInit(false),
    mTextChanged(true),
    mAllowClose(false)
{
    logger->log("Window::Window(\"%s\")", caption.c_str());

    mWindow = this;

    windowInstances++;

//    mFrameSize = 1;
    addMouseListener(this);

    setFrameSize(0);
    setPadding(3);
    setTitleBarHeight(20);

    if (skin.empty())
    {
        reportAlways("Default skin was used for window: %s",
            caption.c_str())
        skin = "window.xml";
    }

    int childPalette = 1;
    // Loads the skin
    if (theme != nullptr)
    {
        mSkin = theme->load(skin,
            "window.xml",
            true,
            Theme::getThemePath());
        if (mSkin != nullptr)
        {
            setPadding(mSkin->getPadding());
            if (getOptionBool("titlebarBold", false))
                mCaptionFont = boldFont;
            mTitlePadding = mSkin->getTitlePadding();
            mGripPadding = getOption("resizePadding", 0);
            mCaptionOffsetX = getOption("captionoffsetx", 0);
            if (mCaptionOffsetX == 0)
                mCaptionOffsetX = 7;
            mCaptionOffsetY = getOption("captionoffsety", 0);
            if (mCaptionOffsetY == 0)
                mCaptionOffsetY = 5;
            mCaptionAlign = static_cast<Graphics::Alignment>(
                getOption("captionalign", 0));
            if (mCaptionAlign < Graphics::LEFT
                || mCaptionAlign > Graphics::RIGHT)
            {
                mCaptionAlign = Graphics::LEFT;
            }
            setTitleBarHeight(CAST_U32(
                getOption("titlebarHeight", 0)));
            if (mTitleBarHeight == 0U)
                mTitleBarHeight = mCaptionFont->getHeight() + mPadding;

            mTitleBarHeight += getOption("titlebarHeightRelative", 0);
            setPalette(getOption("palette", 0));
            childPalette = getOption("childPalette", 0);
            mShowTitle = getOptionBool("showTitle", true);
            mClosePadding = getOption("closePadding", 0);
            mStickySpacing = getOption("stickySpacing", 0);
            mStickyPadding = getOption("stickyPadding", 0);
        }
    }

    // Add this window to the window container
    if (windowContainer != nullptr)
        windowContainer->add(this);

    if (mModal == Modal_true)
    {
        gui->setCursorType(Cursor::CURSOR_POINTER);
        requestModalFocus();
    }

    // Windows are invisible by default
    setVisible(Visible_false, false);

    addWidgetListener(this);
    mForegroundColor = getThemeColor(ThemeColorId::WINDOW, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::WINDOW_OUTLINE, 255U);
    setPalette(childPalette);
}

void Window::postInit()
{
    if (mInit)
    {
        reportAlways("error: Window created with calling postInit() "
            "more than once: %s",
            mWindowName.c_str())
    }
    mInit = true;
}

Window::~Window()
{
    logger->log("Window::~Window(\"%s\")", getCaption().c_str());

    if (gui != nullptr)
        gui->removeDragged(this);

#ifndef DYECMD
    if (setupWindow != nullptr)
        setupWindow->unregisterWindowForReset(this);
#endif  // DYECMD

    client->windowRemoved(this);

    saveWindowState();

    delete2(mLayout)

    while (!mWidgets.empty())
        delete mWidgets.front();

    mWidgets.clear();

    removeWidgetListener(this);
    delete2(mVertexes)

    windowInstances--;

    if (mSkin != nullptr)
    {
        if (theme != nullptr)
            theme->unload(mSkin);
        mSkin = nullptr;
    }
    if (mGrip != nullptr)
    {
        mGrip->decRef();
        mGrip = nullptr;
    }
    if (!mInit)
    {
        reportAlways("error: Window created without calling postInit(): %s",
            mWindowName.c_str())
    }
}

void Window::setWindowContainer(WindowContainer *const wc)
{
    windowContainer = wc;
}

void Window::draw(Graphics *const graphics)
{
    if (mSkin == nullptr)
        return;

    BLOCK_START("Window::draw")
    bool update = false;

    if (mResizeHandles != mOldResizeHandles)
    {
        mRedraw = true;
        mOldResizeHandles = mResizeHandles;
    }
    if (mRedraw)
    {
        mLastRedraw = true;
        mRedraw = false;
        update = true;
        mVertexes->clear();
        graphics->calcWindow(mVertexes,
            0, 0,
            mDimension.width,
            mDimension.height,
            mSkin->getBorder());

        // Draw Close Button
        if (mCloseWindowButton)
        {
            const Image *const button = mSkin->getCloseImage(
                mResizeHandles == CLOSE);
            if (button != nullptr)
            {
                graphics->calcTileCollection(mVertexes,
                    button,
                    mCloseRect.x,
                    mCloseRect.y);
            }
        }
        // Draw Sticky Button
        if (mStickyButton)
        {
            const Image *const button = mSkin->getStickyImage(mSticky);
            if (button != nullptr)
            {
                graphics->calcTileCollection(mVertexes,
                    button,
                    mStickyRect.x,
                    mStickyRect.y);
            }
        }

        if (mGrip != nullptr)
        {
            graphics->calcTileCollection(mVertexes,
                mGrip,
                mGripRect.x,
                mGripRect.y);
        }
        graphics->finalize(mVertexes);
    }
    else
    {
        mLastRedraw = false;
    }
    graphics->drawTileCollection(mVertexes);

    // Draw title
    if (mShowTitle)
    {
        int x;
        switch (mCaptionAlign)
        {
            case Graphics::LEFT:
            default:
                x = mCaptionOffsetX;
                break;
            case Graphics::CENTER:
                x = mCaptionOffsetX - mCaptionFont->getWidth(mCaption) / 2;
                break;
            case Graphics::RIGHT:
                x = mCaptionOffsetX - mCaptionFont->getWidth(mCaption);
                break;
        }
        if (mTextChanged)
        {
            mTextChunk.textFont = mCaptionFont;
            mTextChunk.deleteImage();
            mTextChunk.text = mCaption;
            mTextChunk.color = mForegroundColor;
            mTextChunk.color2 = mForegroundColor2;
            mCaptionFont->generate(mTextChunk);
            mTextChanged = false;
        }

        const Image *const image = mTextChunk.img;
        if (image != nullptr)
            graphics->drawImage(image, x, mCaptionOffsetY);
    }

    if (update)
    {
        graphics->setRedraw(update);
        drawChildren(graphics);
        graphics->setRedraw(false);
    }
    else
    {
        drawChildren(graphics);
    }
    BLOCK_END("Window::draw")
}

void Window::safeDraw(Graphics *const graphics)
{
    if (mSkin == nullptr)
        return;

    BLOCK_START("Window::safeDraw")

    graphics->drawImageRect(0, 0,
        mDimension.width,
        mDimension.height,
        mSkin->getBorder());

    // Draw Close Button
    if (mCloseWindowButton)
    {
        const Image *const button = mSkin->getCloseImage(
            mResizeHandles == CLOSE);
        if (button != nullptr)
            graphics->drawImage(button, mCloseRect.x, mCloseRect.y);
    }
    // Draw Sticky Button
    if (mStickyButton)
    {
        const Image *const button = mSkin->getStickyImage(mSticky);
        if (button != nullptr)
            graphics->drawImage(button, mStickyRect.x, mStickyRect.y);
    }

    if (mGrip != nullptr)
        graphics->drawImage(mGrip, mGripRect.x, mGripRect.y);

    // Draw title
    if (mShowTitle)
    {
        int x;
        switch (mCaptionAlign)
        {
            case Graphics::LEFT:
            default:
                x = mCaptionOffsetX;
                break;
            case Graphics::CENTER:
                x = mCaptionOffsetX - mCaptionFont->getWidth(mCaption) / 2;
                break;
            case Graphics::RIGHT:
                x = mCaptionOffsetX - mCaptionFont->getWidth(mCaption);
                break;
        }
        if (mTextChanged)
        {
            mTextChunk.textFont = mCaptionFont;
            mTextChunk.deleteImage();
            mTextChunk.text = mCaption;
            mTextChunk.color = mForegroundColor;
            mTextChunk.color2 = mForegroundColor2;
            mCaptionFont->generate(mTextChunk);
            mTextChanged = false;
        }

        const Image *const image = mTextChunk.img;
        if (image != nullptr)
            graphics->drawImage(image, x, mCaptionOffsetY);
    }

    safeDrawChildren(graphics);

    BLOCK_END("Window::safeDraw")
}

void Window::setContentSize(int width, int height)
{
    width = width + 2 * mPadding;
    height = height + mPadding + mTitleBarHeight;

    if (mMinWinWidth > width)
        width = mMinWinWidth;
    else if (mMaxWinWidth < width)
        width = mMaxWinWidth;
    if (mMinWinHeight > height)
        height = mMinWinHeight;
    else if (mMaxWinHeight < height)
        height = mMaxWinHeight;

    setSize(width, height);
}

void Window::setLocationRelativeTo(const Widget *const widget)
{
    if (widget == nullptr)
        return;

    int wx;
    int wy;
    int x;
    int y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(mDimension.x + (wx + (widget->getWidth()
        - mDimension.width) / 2 - x),
        mDimension.y + (wy + (widget->getHeight()
        - mDimension.height) / 2 - y));
}

void Window::setLocationHorisontallyRelativeTo(const Widget *const widget)
{
    if (widget == nullptr)
        return;

    int wx;
    int wy;
    int x;
    int y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(mDimension.x + (wx + (widget->getWidth()
        - mDimension.width) / 2 - x), 0);
}

void Window::setLocationRelativeTo(const ImagePosition::Type &position,
                                   int offsetX, int offsetY)
{
    if (position == ImagePosition::UPPER_LEFT)
    {
    }
    else if (position == ImagePosition::UPPER_CENTER)
    {
        offsetX += (mainGraphics->mWidth - mDimension.width) / 2;
    }
    else if (position == ImagePosition::UPPER_RIGHT)
    {
        offsetX += mainGraphics->mWidth - mDimension.width;
    }
    else if (position == ImagePosition::LEFT)
    {
        offsetY += (mainGraphics->mHeight - mDimension.height) / 2;
    }
    else if (position == ImagePosition::CENTER)
    {
        offsetX += (mainGraphics->mWidth - mDimension.width) / 2;
        offsetY += (mainGraphics->mHeight - mDimension.height) / 2;
    }
    else if (position == ImagePosition::RIGHT)
    {
        offsetX += mainGraphics->mWidth - mDimension.width;
        offsetY += (mainGraphics->mHeight - mDimension.height) / 2;
    }
    else if (position == ImagePosition::LOWER_LEFT)
    {
        offsetY += mainGraphics->mHeight - mDimension.height;
    }
    else if (position == ImagePosition::LOWER_CENTER)
    {
        offsetX += (mainGraphics->mWidth - mDimension.width) / 2;
        offsetY += mainGraphics->mHeight - mDimension.height;
    }
    else if (position == ImagePosition::LOWER_RIGHT)
    {
        offsetX += mainGraphics->mWidth - mDimension.width;
        offsetY += mainGraphics->mHeight - mDimension.height;
    }

    setPosition(offsetX, offsetY);
}

void Window::setMinWidth(const int width)
{
    if (mSkin != nullptr)
    {
        mMinWinWidth = width > mSkin->getMinWidth()
            ? width : mSkin->getMinWidth();
    }
    else
    {
        mMinWinWidth = width;
    }
}

void Window::setMinHeight(const int height)
{
    if (mSkin != nullptr)
    {
        mMinWinHeight = height > mSkin->getMinHeight()
            ? height : mSkin->getMinHeight();
    }
    else
    {
        mMinWinHeight = height;
    }
}

void Window::setMaxWidth(const int width)
{
    mMaxWinWidth = width;
}

void Window::setMaxHeight(const int height)
{
    mMaxWinHeight = height;
}

void Window::setResizable(const bool r)
{
    if ((mGrip != nullptr) == r)
        return;

    if (mGrip != nullptr)
        mGrip->decRef();
    if (r)
    {
        mGrip = Theme::getImageFromThemeXml("resize.xml", "");
        if (mGrip != nullptr)
        {
            mGripRect.x = mDimension.width - mGrip->getWidth() - mGripPadding;
            mGripRect.y = mDimension.height - mGrip->getHeight()
                - mGripPadding;
        }
        else
        {
            mGripRect.x = 0;
            mGripRect.y = 0;
        }
    }
    else
    {
        mGrip = nullptr;
    }
}

void Window::widgetResized(const Event &event A_UNUSED)
{
    if (mGrip != nullptr)
    {
        mGripRect.x = mDimension.width - mGrip->getWidth() - mGripPadding;
        mGripRect.y = mDimension.height - mGrip->getHeight() - mGripPadding;
    }

    if (mLayout != nullptr)
    {
        const Rect area = getChildrenArea();
        int w = area.width;
        int h = area.height;
        mLayout->reflow(w, h);
    }
    if (mSkin != nullptr)
    {
        const bool showClose = mCloseWindowButton
            && (mSkin->getCloseImage(false) != nullptr);
        if (showClose)
        {
            const Image *const button = mSkin->getCloseImage(false);
            if (button != nullptr)
            {
                const int buttonWidth = button->getWidth();
                mCloseRect.x = mDimension.width - buttonWidth - mClosePadding;
                mCloseRect.y = mClosePadding;
                mCloseRect.width = buttonWidth;
                mCloseRect.height = button->getHeight();
            }
        }
        if (mStickyButton)
        {
            const Image *const button = mSkin->getStickyImage(mSticky);
            if (button != nullptr)
            {
                const int buttonWidth = button->getWidth();
                int x = mDimension.width - buttonWidth
                    - mStickySpacing - mClosePadding;

                if (showClose)
                    x -= mSkin->getCloseImage(false)->getWidth();

                mStickyRect.x = x;
                mStickyRect.y = mStickyPadding;
                mStickyRect.width = buttonWidth;
                mStickyRect.height = button->getHeight();
            }
        }
    }
    else
    {
        mCloseRect.x = 0;
        mCloseRect.y = 0;
        mCloseRect.width = 0;
        mCloseRect.height = 0;
        mStickyRect.x = 0;
        mStickyRect.y = 0;
        mStickyRect.width = 0;
        mStickyRect.height = 0;
    }

    mRedraw = true;
}

void Window::widgetMoved(const Event& event A_UNUSED)
{
    mRedraw = true;
}

void Window::widgetHidden(const Event &event A_UNUSED)
{
    if (isBatchDrawRenders(openGLMode))
        mVertexes->clear();

    mTextChunk.deleteImage();

    mTextChanged = true;
    mRedraw = true;

    if (gui != nullptr)
        gui->setCursorType(Cursor::CURSOR_POINTER);

    if (mFocusHandler == nullptr)
        return;

    for (WidgetListConstIterator it = mWidgets.begin();
         it != mWidgets.end(); ++ it)
    {
        if (mFocusHandler->isFocused(*it))
            mFocusHandler->focusNone();
    }
}

void Window::setCloseButton(const bool flag)
{
    mCloseWindowButton = flag;
    if (flag)
        mAllowClose = true;
}

bool Window::isResizable() const
{
    return mGrip != nullptr;
}

void Window::setStickyButton(const bool flag)
{
    mStickyButton = flag;
}

void Window::setSticky(const bool sticky)
{
    mSticky = sticky;
    mRedraw = true;
}

void Window::setStickyButtonLock(const bool lock)
{
    mStickyButtonLock = lock;
    mStickyButton = lock;
}

void Window::setVisible(Visible visible)
{
    setVisible(visible, false);
}

void Window::setVisible(const Visible visible, const bool forceSticky)
{
    if (visible == mVisible)
        return;

    // Check if the window is off screen...
    if (visible == Visible_true)
        ensureOnScreen();
    else
        mResizeHandles = 0;

    if (mStickyButtonLock)
    {
        BasicContainer2::setVisible(visible);
    }
    else
    {
        BasicContainer2::setVisible(fromBool((!forceSticky && mSticky) ||
            visible == Visible_true, Visible));
    }
    if (visible == Visible_true)
    {
        if (mPlayVisibleSound)
            soundManager.playGuiSound(SOUND_SHOW_WINDOW);
        if (gui != nullptr)
        {
            MouseEvent *const event = reinterpret_cast<MouseEvent*>(
                gui->createMouseEvent(this));
            if (event != nullptr)
            {
                const int x = event->getX();
                const int y = event->getY();
                if (x >= 0 && x <= mDimension.width
                    && y >= 0 && y <= mDimension.height)
                {
                    mouseMoved(*event);
                }
                delete event;
            }
        }
    }
    else
    {
        if (mPlayVisibleSound)
            soundManager.playGuiSound(SOUND_HIDE_WINDOW);
    }
}

void Window::scheduleDelete()
{
    windowContainer->scheduleDelete(this);
}

void Window::mousePressed(MouseEvent &event)
{
    if (event.isConsumed())
        return;

    if (event.getSource() == this)
    {
        if (getParent() != nullptr)
            getParent()->moveToTop(this);

        mDragOffsetX = event.getX();
        mDragOffsetY = event.getY();
        mMoved = event.getY() <= CAST_S32(mTitleBarHeight);
    }

    const MouseButtonT button = event.getButton();
    if (button == MouseButton::LEFT)
    {
        const int x = event.getX();
        const int y = event.getY();

        // Handle close button
        if (mCloseWindowButton &&
            mSkin != nullptr &&
            mCloseRect.isPointInRect(x, y))
        {
            mouseResize = 0;
            mMoved = false;
            event.consume();
            close();
            return;
        }

        // Handle sticky button
        if (mStickyButton &&
            mSkin != nullptr &&
            mStickyRect.isPointInRect(x, y))
        {
            setSticky(!isSticky());
            mouseResize = 0;
            mMoved = false;
            event.consume();
            return;
        }

        // Handle window resizing
        mouseResize = getResizeHandles(event) & resizeMask;
        if (mouseResize != 0)
            event.consume();
        if (canMove())
            mMoved = (mouseResize == 0);
        else
            mMoved = false;
    }
#ifndef DYECMD
    else if (button == MouseButton::RIGHT)
    {
        if (popupMenu != nullptr)
        {
            event.consume();
            popupMenu->showWindowPopup(this);
        }
    }
#endif  // DYECMD
}

void Window::close()
{
    setVisible(Visible_false);
}

void Window::mouseReleased(MouseEvent &event A_UNUSED)
{
    if ((mGrip != nullptr) && (mouseResize != 0))
    {
        mouseResize = 0;
        if (gui != nullptr)
            gui->setCursorType(Cursor::CURSOR_POINTER);
    }

    mMoved = false;
}

void Window::mouseEntered(MouseEvent &event)
{
    updateResizeHandler(event);
}

void Window::mouseExited(MouseEvent &event A_UNUSED)
{
    if ((mGrip != nullptr) && (mouseResize == 0) && (gui != nullptr))
        gui->setCursorType(Cursor::CURSOR_POINTER);
}

void Window::updateResizeHandler(MouseEvent &event)
{
    if (gui == nullptr)
        return;

#ifndef DYECMD
    if (!dragDrop.isEmpty())
        return;
#endif  // DYECMD

    mResizeHandles = getResizeHandles(event);

    // Changes the custom mouse cursor based on it's current position.
    switch (mResizeHandles & resizeMask)
    {
        case BOTTOM | RIGHT:
        case TOP | LEFT:
            gui->setCursorType(Cursor::CURSOR_RESIZE_DOWN_RIGHT);
            break;
        case TOP | RIGHT:
        case BOTTOM | LEFT:
            gui->setCursorType(Cursor::CURSOR_RESIZE_DOWN_LEFT);
            break;
        case BOTTOM:
        case TOP:
            gui->setCursorType(Cursor::CURSOR_RESIZE_DOWN);
            break;
        case RIGHT:
        case LEFT:
            gui->setCursorType(Cursor::CURSOR_RESIZE_ACROSS);
            break;
        default:
            gui->setCursorType(Cursor::CURSOR_POINTER);
            break;
    }
}

void Window::mouseMoved(MouseEvent &event)
{
    updateResizeHandler(event);
    if ((popupManager != nullptr) && !event.isConsumed())
    {
        PopupManager::hideBeingPopup();
        PopupManager::hideTextPopup();
    }
}

bool Window::canMove() const
{
    return !mStickyButtonLock || !mSticky;
}

void Window::mouseDragged(MouseEvent &event)
{
    if (canMove())
    {
        if (!event.isConsumed() && event.getSource() == this)
        {
            if (isMovable() && mMoved)
            {
                setPosition(event.getX() - mDragOffsetX + getX(),
                    event.getY() - mDragOffsetY + getY());
            }

            event.consume();
        }
    }
    else
    {
        if (!event.isConsumed() && event.getSource() == this)
            event.consume();
        return;
    }

    // Keep guichan window inside screen when it may be moved
    if (isMovable() && mMoved)
    {
        setPosition(std::min(mainGraphics->mWidth - mDimension.width,
            std::max(0, mDimension.x)),
            std::min(mainGraphics->mHeight - mDimension.height,
            std::max(0, mDimension.y)));
    }

    if ((mouseResize != 0) && !mMoved)
    {
        const int dy = event.getY() - mDragOffsetY;
        Rect newDim = getDimension();

        if ((mouseResize & (TOP | BOTTOM)) != 0)
        {
            const int newHeight = newDim.height
                + ((mouseResize & TOP) != 0 ? -dy : dy);
            newDim.height = std::min(mMaxWinHeight,
                std::max(mMinWinHeight, newHeight));

            if ((mouseResize & TOP) != 0)
                newDim.y -= newDim.height - getHeight();
        }

        if ((mouseResize & (LEFT | RIGHT)) != 0)
        {
            const int dx = event.getX() - mDragOffsetX;
            const int newWidth = newDim.width
                + ((mouseResize & LEFT) != 0 ? -dx : dx);
            newDim.width = std::min(mMaxWinWidth,
                std::max(mMinWinWidth, newWidth));

            if ((mouseResize & LEFT) != 0)
                newDim.x -= newDim.width - mDimension.width;
        }

        // Keep guichan window inside screen (supports resizing any side)
        if (newDim.x < 0)
        {
            newDim.width += newDim.x;
            newDim.x = 0;
        }
        if (newDim.y < 0)
        {
            newDim.height += newDim.y;
            newDim.y = 0;
        }
        if (newDim.x + newDim.width > mainGraphics->mWidth)
            newDim.width = mainGraphics->mWidth - newDim.x;
        if (newDim.y + newDim.height > mainGraphics->mHeight)
            newDim.height = mainGraphics->mHeight - newDim.y;

        // Update mouse offset when dragging bottom or right border
        if ((mouseResize & BOTTOM) != 0)
            mDragOffsetY += newDim.height - mDimension.height;

        if ((mouseResize & RIGHT) != 0)
            mDragOffsetX += newDim.width - mDimension.width;

        // Set the new window and content dimensions
        setDimension(newDim);
    }
}

void Window::setModal(const Modal modal)
{
    if (mModal != modal)
    {
        mModal = modal;
        if (mModal == Modal_true)
        {
            if (gui != nullptr)
                gui->setCursorType(Cursor::CURSOR_POINTER);
            requestModalFocus();
        }
        else
        {
            releaseModalFocus();
        }
    }
}

void Window::loadWindowState()
{
    const std::string &name = mWindowName;
    if (name.empty())
        return;

    setPosition(config.getValueInt(name + "WinX", mDefaultX),
        config.getValueInt(name + "WinY", mDefaultY));

    if (mSaveVisible)
    {
        setVisible(fromBool(config.getValueBool(name
            + "Visible", mDefaultVisible), Visible));
    }

    if (mStickyButton)
    {
        setSticky(config.getValueBool(name
            + "Sticky", isSticky()));
    }

    if (mGrip != nullptr)
    {
        int width = config.getValueInt(name + "WinWidth", mDefaultWidth);
        int height = config.getValueInt(name + "WinHeight", mDefaultHeight);

        if (getMinWidth() > width)
            width = getMinWidth();
        else if (getMaxWidth() < width)
            width = getMaxWidth();
        if (getMinHeight() > height)
            height = getMinHeight();
        else if (getMaxHeight() < height)
            height = getMaxHeight();

        setSize(width, height);
    }
    else
    {
        setSize(mDefaultWidth, mDefaultHeight);
    }

    // Check if the window is off screen...
    ensureOnScreen();

    if (viewport != nullptr)
    {
        int width = mDimension.width;
        int height = mDimension.height;

        if (mDimension.x + width > viewport->getWidth())
            width = viewport->getWidth() - mDimension.x;
        if (mDimension.y + height > viewport->getHeight())
            height = viewport->getHeight() - mDimension.y;
        if (width < 0)
            width = 0;
        if (height < 0)
            height = 0;
        setSize(width, height);
    }
}

void Window::saveWindowState()
{
    // Saving X, Y and Width and Height for resizables in the config
    if (!mWindowName.empty() && mWindowName != "window")
    {
        config.setValue(mWindowName + "WinX", mDimension.x);
        config.setValue(mWindowName + "WinY", mDimension.y);

        if (mSaveVisible)
            config.setValue(mWindowName + "Visible", isWindowVisible());

        if (mStickyButton)
            config.setValue(mWindowName + "Sticky", isSticky());

        if (mGrip != nullptr)
        {
            if (getMinWidth() > mDimension.width)
                setWidth(getMinWidth());
            else if (getMaxWidth() < mDimension.width)
                setWidth(getMaxWidth());
            if (getMinHeight() > mDimension.height)
                setHeight(getMinHeight());
            else if (getMaxHeight() < mDimension.height)
                setHeight(getMaxHeight());

            config.setValue(mWindowName + "WinWidth", mDimension.width);
            config.setValue(mWindowName + "WinHeight", mDimension.height);
        }
    }
}

void Window::setDefaultSize(const int defaultX, const int defaultY,
                            int defaultWidth, int defaultHeight)
{
    if (mMinWinWidth > defaultWidth)
        defaultWidth = mMinWinWidth;
    else if (mMaxWinWidth < defaultWidth)
        defaultWidth = mMaxWinWidth;
    if (mMinWinHeight > defaultHeight)
        defaultHeight = mMinWinHeight;
    else if (mMaxWinHeight < defaultHeight)
        defaultHeight = mMaxWinHeight;

    mDefaultX = defaultX;
    mDefaultY = defaultY;
    mDefaultWidth = defaultWidth;
    mDefaultHeight = defaultHeight;
}

void Window::setDefaultSize()
{
    mDefaultX = mDimension.x;
    mDefaultY = mDimension.y;
    mDefaultWidth = mDimension.width;
    mDefaultHeight = mDimension.height;
}

void Window::setDefaultSize(const int defaultWidth, const int defaultHeight,
                            const ImagePosition::Type &position,
                            const int offsetX, const int offsetY)
{
    int x = 0;
    int y = 0;

    if (position == ImagePosition::UPPER_LEFT)
    {
    }
    else if (position == ImagePosition::UPPER_CENTER)
    {
        x = (mainGraphics->mWidth - defaultWidth) / 2;
    }
    else if (position == ImagePosition::UPPER_RIGHT)
    {
        x = mainGraphics->mWidth - defaultWidth;
    }
    else if (position == ImagePosition::LEFT)
    {
        y = (mainGraphics->mHeight - defaultHeight) / 2;
    }
    else if (position == ImagePosition::CENTER)
    {
        x = (mainGraphics->mWidth - defaultWidth) / 2;
        y = (mainGraphics->mHeight - defaultHeight) / 2;
    }
    else if (position == ImagePosition::RIGHT)
    {
        x = mainGraphics->mWidth - defaultWidth;
        y = (mainGraphics->mHeight - defaultHeight) / 2;
    }
    else if (position == ImagePosition::LOWER_LEFT)
    {
        y = mainGraphics->mHeight - defaultHeight;
    }
    else if (position == ImagePosition::LOWER_CENTER)
    {
        x = (mainGraphics->mWidth - defaultWidth) / 2;
        y = mainGraphics->mHeight - defaultHeight;
    }
    else if (position == ImagePosition::LOWER_RIGHT)
    {
        x = mainGraphics->mWidth - defaultWidth;
        y = mainGraphics->mHeight - defaultHeight;
    }

    mDefaultX = x - offsetX;
    mDefaultY = y - offsetY;
    mDefaultWidth = defaultWidth;
    mDefaultHeight = defaultHeight;
}

void Window::resetToDefaultSize()
{
    setPosition(mDefaultX, mDefaultY);
    setSize(mDefaultWidth, mDefaultHeight);
    saveWindowState();
}

void Window::adjustPositionAfterResize(const int oldScreenWidth,
                                       const int oldScreenHeight)
{
    // If window was aligned to the right or bottom, keep it there
    const int rightMargin = oldScreenWidth - (mDimension.x + mDimension.width);
    const int bottomMargin = oldScreenHeight
        - (mDimension.y + mDimension.height);
    if (mDimension.x > 0 && mDimension.x > rightMargin)
        mDimension.x = mainGraphics->mWidth - rightMargin - mDimension.width;
    if (mDimension.y > 0 && mDimension.y > bottomMargin)
    {
        mDimension.y = mainGraphics->mHeight
            - bottomMargin - mDimension.height;
    }

    ensureOnScreen();
    adjustSizeToScreen();
}

void Window::adjustSizeToScreen()
{
    if (mGrip == nullptr)
        return;

    const int screenWidth = mainGraphics->mWidth;
    const int screenHeight = mainGraphics->mHeight;
    const int oldWidth = mDimension.width;
    const int oldHeight = mDimension.height;
    if (oldWidth + mDimension.x > screenWidth)
        mDimension.x = 0;
    if (oldHeight + mDimension.y > screenHeight)
        mDimension.x = 0;
    if (mDimension.width > screenWidth)
        mDimension.width = screenWidth;
    if (mDimension.height > screenHeight)
        mDimension.height = screenHeight;
    if (oldWidth != mDimension.width || oldHeight != mDimension.height)
        widgetResized(Event(this));
}

int Window::getResizeHandles(const MouseEvent &event)
{
    if (event.getX() < 0 || event.getY() < 0)
        return 0;

    int resizeHandles = 0;
    const unsigned y = event.getY();
    const unsigned x = event.getX();
    if (mCloseRect.isPointInRect(x, y))
        return CLOSE;

    if (!mStickyButtonLock || !mSticky)
    {
        if ((mGrip != nullptr) &&
            (y > mTitleBarHeight ||
            (CAST_S32(y) < mPadding &&
            CAST_S32(mTitleBarHeight) > mPadding)))
        {
            if (!getWindowArea().isPointInRect(x, y)
                && event.getSource() == this)
            {
                resizeHandles |= (x > mDimension.width - resizeBorderWidth)
                    ? RIGHT : (x < resizeBorderWidth) ? LEFT : 0;
                resizeHandles |= (y > mDimension.height - resizeBorderWidth)
                    ? BOTTOM : (y < resizeBorderWidth) ? TOP : 0;
            }
            if (x >= CAST_U32(mGripRect.x)
                && y >= CAST_U32(mGripRect.y))
            {
                mDragOffsetX = x;
                mDragOffsetY = y;
                resizeHandles |= BOTTOM | RIGHT;
            }
        }
    }

    return resizeHandles;
}

bool Window::isResizeAllowed(const MouseEvent &event) const
{
    const int y = event.getY();

    if ((mGrip != nullptr) &&
        (y > CAST_S32(mTitleBarHeight) ||
        y < mPadding))
    {
        const int x = event.getX();

        if (!getWindowArea().isPointInRect(x, y) && event.getSource() == this)
            return true;

        if (x >= mGripRect.x && y >= mGripRect.y)
            return true;
    }

    return false;
}

Layout &Window::getLayout()
{
    if (mLayout == nullptr)
        mLayout = new Layout;
    return *mLayout;
}

void Window::clearLayout()
{
    clear();

    // Recreate layout instance when one is present
    if (mLayout != nullptr)
    {
        delete mLayout;
        mLayout = new Layout;
    }
}

LayoutCell &Window::place(const int x, const int y, Widget *const wg,
                          const int w, const int h)
{
    add(wg);
    return getLayout().place(wg, x, y, w, h);
}

ContainerPlacer Window::getPlacer(const int x, const int y)
{
    return ContainerPlacer(this, &getLayout().at(x, y));
}

void Window::reflowLayout(int w, int h)
{
    if (mLayout == nullptr)
        return;

    mLayout->reflow(w, h);
    delete2(mLayout)
    setContentSize(w, h);
}

void Window::redraw()
{
    if (mLayout != nullptr)
    {
        const Rect area = getChildrenArea();
        int w = area.width;
        int h = area.height;
        mLayout->reflow(w, h);
    }
}

void Window::center()
{
    setLocationRelativeTo(getParent());
}

void Window::centerHorisontally()
{
    setLocationHorisontallyRelativeTo(getParent());
}

void Window::ensureOnScreen()
{
    // Skip when a window hasn't got any size initialized yet
    if (mDimension.width == 0 && mDimension.height == 0)
        return;

    // Check the left and bottom screen boundaries
    if (mDimension.x + mDimension.width > mainGraphics->mWidth)
        mDimension.x = mainGraphics->mWidth - mDimension.width;
    if (mDimension.y + mDimension.height > mainGraphics->mHeight)
        mDimension.y = mainGraphics->mHeight - mDimension.height;

    // But never allow the windows to disappear in to the right and top
    if (mDimension.x < 0)
        mDimension.x = 0;
    if (mDimension.y < 0)
        mDimension.y = 0;
}

Rect Window::getWindowArea() const
{
    return Rect(mPadding,
        mPadding,
        mDimension.width - mPadding * 2,
        mDimension.height - mPadding * 2);
}

int Window::getOption(const std::string &name, const int def) const
{
    if (mSkin != nullptr)
    {
        const int val = mSkin->getOption(name);
        if (val != 0)
            return val;
        return def;
    }
    return def;
}

bool Window::getOptionBool(const std::string &name, const bool def) const
{
    if (mSkin != nullptr)
        return mSkin->getOption(name, static_cast<int>(def)) != 0;
    return def;
}

Rect Window::getChildrenArea()
{
    return Rect(mPadding,
        mTitleBarHeight,
        mDimension.width - mPadding * 2,
        mDimension.height - mPadding - mTitleBarHeight);
}

void Window::resizeToContent()
{
    int w = 0;
    int h = 0;
    for (WidgetListConstIterator it = mWidgets.begin();
          it != mWidgets.end(); ++ it)
    {
        const Widget *const widget = *it;
        const int x = widget->getX();
        const int y = widget->getY();
        const int width = widget->getWidth();
        const int height = widget->getHeight();
        if (x + width > w)
            w = x + width;

        if (y + height > h)
            h = y + height;
    }

    setSize(w + 2 * mPadding,
        h + mPadding + mTitleBarHeight);
}

#ifdef USE_PROFILER
void Window::logic()
{
    BLOCK_START("Window::logic")
    logicChildren();
    BLOCK_END("Window::logic")
}
#endif  // USE_PROFILER
