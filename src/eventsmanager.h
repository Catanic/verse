/*
 *  The ManaVerse Client
 *  Copyright (C) 2012-2020  The ManaPlus Developers
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

#ifndef EVENTSMANAGER_H
#define EVENTSMANAGER_H

#include "listeners/configlistener.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_events.h>
PRAGMA48(GCC diagnostic pop)

#include "localconsts.h"

class EventsManager final : public ConfigListener
{
    public:
        EventsManager();

        A_DELETE_COPY(EventsManager)

        ~EventsManager() override final;

        void init();

        void enableEvents();

        void shutdown();

        bool handleEvents() const;

        bool handleCommonEvents(const SDL_Event &event) const;

        void handleGameEvents() const;

#ifdef USE_SDL2
        static void handleSDL2WindowEvent(const SDL_Event &event);
#else  // USE_SDL2

        static void handleActive(const SDL_Event &event);
#endif  // USE_SDL2

        void optionChanged(const std::string &name) override final;

        static void logEvent(const SDL_Event &event);

    protected:
        bool mLogInput;
};

extern EventsManager eventsManager;

#endif  // EVENTSMANAGER_H
