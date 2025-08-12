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

#include "input/touch/multitouchmanager.h"

#ifdef USE_SDL2
#include "render/graphics.h"

#include "gui/sdlinput.h"
#endif  // USE_SDL2
#include "debug.h"

MultiTouchManager multiTouchManager;

MultiTouchManager::MultiTouchManager() :
    mEvents()
{
}

MultiTouchManager::~MultiTouchManager()
{
}

void MultiTouchManager::init()
{
}

#ifdef USE_SDL2
void MultiTouchManager::updateFinger(const SDL_Event &event, const bool active)
{
    const SDL_TouchFingerEvent &touch = event.tfinger;
    MultiTouchEventsMap &device = mEvents[touch.touchId];
    MultiTouchEvent &finger = device[touch.fingerId];
    finger.active = active;
    finger.x = touch.x;
    finger.y = touch.y;
}

void MultiTouchManager::handleFingerDown(const SDL_Event &event)
{
    updateFinger(event, true);
    const SDL_TouchFingerEvent &touch = event.tfinger;
    checkDevice(touch.touchId, touch.fingerId);
}

void MultiTouchManager::handleFingerUp(const SDL_Event &event)
{
    updateFinger(event, false);
}

void MultiTouchManager::checkDevice(const long touchId,
                                    const long fingerId)
{
    if (fingerId != 1 || !guiInput)
        return;

    MultiTouchEventsMap &device = mEvents[touchId];
    MultiTouchEvent &finger0 = device[0];
    if (finger0.active)
    {
        MultiTouchEvent &finger1 = device[1];
        if (finger1.active)
        {
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            guiInput->simulateMouseClick(finger0.x * w, finger0.y * h,
                MouseButton::RIGHT);
        }
    }
}
#endif  // USE_SDL2
