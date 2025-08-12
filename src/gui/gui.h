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

#ifndef GUI_GUI_H
#define GUI_GUI_H

#include "gui/color.h"

#include "enums/events/mousebutton.h"
#include "enums/events/mouseeventtype.h"

#include "enums/resources/cursor.h"

#include <deque>
#include <list>

#include "localconsts.h"

class Event;
class FocusHandler;
class FocusListener;
class Graphics;
class GuiConfigListener;
class ImageSet;
class KeyEvent;
class KeyListener;
class MouseEvent;
class MouseInput;
class Font;
class SDLInput;
class Widget;
class Window;

#ifdef USE_SDL2
#define MouseStateType uint32_t
#else  // USE_SDL2
#define MouseStateType uint8_t
#endif  // USE_SDL2

/**
 * \defgroup GUI Core GUI related classes (widgets)
 */

/**
 * \defgroup Interface User interface related classes (windows, dialogs)
 */

/**
 * Main GUI class.
 *
 * \ingroup GUI
 */
class Gui final
{
    public:
        /**
         * Constructor.
         */
        Gui();

        A_DELETE_COPY(Gui)

        /**
         * Destructor.
         */
        ~Gui();

        void postInit(Graphics *const graphics) A_NONNULL(2);

        /**
         * Performs logic of the GUI. Overridden to track mouse pointer
         * activity.
         */
        void logic();

        void slowLogic();

        void clearFonts();

        /**
         * Draws the whole Gui by calling draw functions down in the
         * Gui hierarchy. It also draws the mouse pointer.
         */
        void draw();

        /**
         * Called when the application window has been resized.
         */
        void videoResized() const;

        FocusHandler *getFocusHandler() const noexcept2 A_WARN_UNUSED
        { return mFocusHandler; }

        /**
         * Return game font.
         */
        Font *getFont() const RETURNS_NONNULL A_WARN_UNUSED
        { return mGuiFont; }

        /**
         * Return help font.
         */
        Font *getHelpFont() const RETURNS_NONNULL A_WARN_UNUSED
        { return mHelpFont; }

        /**
         * Return secure font.
         */
        Font *getSecureFont() const RETURNS_NONNULL A_WARN_UNUSED
        { return mSecureFont; }

        /**
         * Return npc font.
         */
        Font *getNpcFont() const RETURNS_NONNULL A_WARN_UNUSED
        { return mNpcFont; }

        /**
         * Return the Font used for "Info Particles", i.e. ones showing, what
         * you picked up, etc.
         */
        Font *getInfoParticleFont() const RETURNS_NONNULL A_WARN_UNUSED
        { return mInfoParticleFont; }

        /**
         * Sets whether a custom cursor should be rendered.
         */
        void setUseCustomCursor(const bool customCursor);

        /**
         * Sets which cursor should be used.
         */
        void setCursorType(const CursorT index)
        { mCursorType = index; }

        void setDoubleClick(const bool b)
        { mDoubleClick = b; }

        void updateFonts();

        bool handleInput();

        bool handleKeyInput();

        void resetClickCount();

        MouseEvent *createMouseEvent(Window *const widget) A_WARN_UNUSED;

        static void getAbsolutePosition(Widget *restrict widget,
                                        int &restrict x,
                                        int &restrict y);

        void addGlobalFocusListener(FocusListener* focusListener);

        void removeGlobalFocusListener(FocusListener* focusListener);

        void distributeGlobalFocusGainedEvent(const Event &focusEvent);

        void removeDragged(const Widget *const widget);

        int getLastMouseX() const
        { return mLastMouseX; }

        int getLastMouseY() const
        { return mLastMouseY; }

        static MouseStateType getMouseState(int &x, int &y);

        /**
         * Sets the top widget. The top widget is the root widget
         * of the GUI. If you want a GUI to be able to contain more
         * than one widget the top widget should be a container.
         *
         * @param top The top widget.
         * @see Container
         */
        void setTop(Widget *const top);

        /**
         * Gets the top widget. The top widget is the root widget
         * of the GUI.
         *
         * @return The top widget. NULL if no top widget has been set.
         */
        Widget* getTop() const noexcept2 A_WARN_UNUSED
        { return mTop; }

        /**
         * Sets the graphics object to use for drawing.
         *
         * @param graphics The graphics object to use for drawing.
         * @see getGraphics, AllegroGraphics, HGEGraphics,
         *      OpenLayerGraphics, OpenGLGraphics, SDLGraphics
         */
        void setGraphics(Graphics *const graphics) A_NONNULL(2);

        /**
         * Gets the graphics object used for drawing.
         *
         *  @return The graphics object used for drawing. NULL if no
         *          graphics object has been set.
         * @see setGraphics, AllegroGraphics, HGEGraphics,
         *      OpenLayerGraphics, OpenGLGraphics, SDLGraphics
         */
        Graphics* getGraphics() const RETURNS_NONNULL A_WARN_UNUSED;

        /**
         * Sets the input object to use for input handling.
         *
         * @param input The input object to use for input handling.
         * @see getInput, AllegroInput, HGEInput, OpenLayerInput,
         *      SDLInput
         */
        void setInput(SDLInput *const input) A_NONNULL(2);

        /**
         * Gets the input object being used for input handling.
         *
         *  @return The input object used for handling input. NULL if no
         *          input object has been set.
         * @see setInput, AllegroInput, HGEInput, OpenLayerInput,
         *      SDLInput
         */
        SDLInput* getInput() const A_WARN_UNUSED;

        /**
         * Adds a global key listener to the Gui. A global key listener
         * will receive all key events generated from the GUI and global
         * key listeners will receive the events before key listeners
         * of widgets.
         *
         * @param keyListener The key listener to add.
         * @see removeGlobalKeyListener
         */
        void addGlobalKeyListener(KeyListener *const keyListener);

        /**
         * Removes global key listener from the Gui.
         *
         * @param keyListener The key listener to remove.
         * @throws Exception if the key listener hasn't been added.
         * @see addGlobalKeyListener
         */
        void removeGlobalKeyListener(KeyListener *const keyListener);

        bool isLongPress() const
        { return getMousePressLength() > 250; }

        int getMousePressLength() const;

    protected:
        void handleMouseMoved(const MouseInput &mouseInput);

        void handleMouseReleased(const MouseInput &mouseInput);

        void handleMousePressed(const MouseInput &mouseInput);

        void handleMouseInput();

        void distributeMouseEvent(Widget *const source,
                                  const MouseEventTypeT type,
                                  const MouseButtonT button,
                                  const int x, const int y,
                                  const bool force,
                                  const bool toSourceOnly);

        /**
         *
         * Handles mouse wheel moved down input.
         *
         * @param mouseInput The mouse input to handle.
         */
        void handleMouseWheelMovedDown(const MouseInput& mouseInput);

        /**
         * Handles mouse wheel moved up input.
         *
         * @param mouseInput The mouse input to handle.
         */
        void handleMouseWheelMovedUp(const MouseInput& mouseInput);

        /**
         * Gets the widget at a certain position.
         *
         * @return The widget at a certain position.
         */
        Widget* getWidgetAt(const int x, const int y) const A_WARN_UNUSED;

        /**
         * Gets the source of the mouse event.
         *
         * @return The source widget of the mouse event.
         */
        Widget* getMouseEventSource(const int x,
                                    const int y) const A_WARN_UNUSED;

        /**
         * Gets the source of the key event.
         *
         * @return The source widget of the key event.
         */
        Widget* getKeyEventSource() const A_WARN_UNUSED;

        /**
         * Distributes a key event.
         *
         * @param event The key event to distribute.

         */
        void distributeKeyEvent(KeyEvent &event) const;

        /**
         * Distributes a key event to the global key listeners.
         *
         * @param event The key event to distribute.
         *
         */
        void distributeKeyEventToGlobalKeyListeners(KeyEvent& event);

        /**
         * Handles modal mouse input focus. Modal mouse input focus needs
         * to be checked at each logic iteration as it might be necessary to
         * distribute mouse entered or mouse exited events.
         *
         */
        void handleModalMouseInputFocus();

        /**
         * Handles modal focus. Modal focus needs to be checked at
         * each logic iteration as it might be necessary to distribute
         * mouse entered or mouse exited events.
         *
         */
        void handleModalFocus();

        /**
         * Handles modal focus gained. If modal focus has been gained it might
         * be necessary to distribute mouse entered or mouse exited events.
         *
         */
        void handleModalFocusGained();

        /**
         * Handles modal mouse input focus gained. If modal focus has been
         * gained it might be necessary to distribute mouse entered or mouse
         * exited events.
         *
         */
        void handleModalFocusReleased();

    private:
        /**
         * Holds the top widget.
         */
        Widget* mTop;

        /**
         * Holds the graphics implementation used.
         */
        Graphics* mGraphics A_NONNULLPOINTER;

        /**
         * Holds the input implementation used.
         */
        SDLInput* mInput A_NONNULLPOINTER;

        /**
         * Holds the focus handler for the Gui.
         */
        FocusHandler* mFocusHandler A_NONNULLPOINTER;

        /**
         * Typedef.
         */
        typedef std::list<KeyListener*> KeyListenerList;

        /**
         * Typedef.
         */
        typedef KeyListenerList::iterator KeyListenerListIterator;

        /**
         * Holds the global key listeners of the Gui.
         */
        KeyListenerList mKeyListeners;

        /**
         * Holds the last mouse button pressed.
         */
        MouseButtonT mLastMousePressButton;

        /**
         * Holds the last mouse press time stamp.
         */
        unsigned int mLastMousePressTimeStamp;

        /**
         * Holds the last mouse x coordinate.
         */
        int mLastMouseX;

        /**
         * Holds the last mouse y coordinate.
         */
        int mLastMouseY;

        /**
         * Holds the current click count. Used to keep track
         * of clicks for a the last pressed button.
         */
        int mClickCount;

        /**
         * Holds the last button used when a drag of a widget
         * was initiated. Used to be able to release a drag
         * when the same button is released.
         */
        MouseButtonT mLastMouseDragButton;

        /**
         * Holds a stack with all the widgets with the mouse.
         * Used to properly distribute mouse events.
         */
        std::deque<Widget*> mWidgetWithMouseQueue;

        GuiConfigListener *mConfigListener;
        /** The global GUI font */
        Font *mGuiFont A_NONNULLPOINTER;
        /** Font for Info Particles */
        Font *mInfoParticleFont A_NONNULLPOINTER;
        /** Font for Help Window */
        Font *mHelpFont A_NONNULLPOINTER;
        /** Font for secure labels */
        Font *mSecureFont A_NONNULLPOINTER;
        /** Font for npc text */
        Font *mNpcFont A_NONNULLPOINTER;
        /** Mouse cursor images */
        ImageSet *mMouseCursors;
        float mMouseCursorAlpha;
        int mMouseInactivityTimer;
        CursorT mCursorType;
#ifdef ANDROID
        uint16_t mLastMouseRealX;
        uint16_t mLastMouseRealY;
#endif  // ANDROID

        typedef std::list<FocusListener*> FocusListenerList;
        typedef FocusListenerList::iterator FocusListenerIterator;
        FocusListenerList mFocusListeners;
        Color mForegroundColor;
        Color mForegroundColor2;
        time_t mTime;
        time_t mTime10;
        bool mCustomCursor;                 /**< Show custom cursor */
        bool mDoubleClick;
};

extern Gui *gui;                            /**< The GUI system */

/**
 * Bolded text font
 */
extern Font *boldFont A_NONNULLPOINTER;

#endif  // GUI_GUI_H
