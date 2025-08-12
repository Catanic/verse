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

#ifndef RESOURCES_SDLMUSIC_H
#define RESOURCES_SDLMUSIC_H

#include "resources/resource.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_mixer.h>
PRAGMA48(GCC diagnostic pop)

#include "localconsts.h"

/**
 * Defines a class for loading and storing music.
 */
class SDLMusic final : public Resource
{
    public:
        SDLMusic() :
            Resource(),
            mName(),
            mMusic(nullptr),
            mRw(nullptr)
        { }

        SDLMusic(Mix_Music *const music,
                 SDL_RWops *const rw,
                 const std::string &name);

        A_DELETE_COPY(SDLMusic)

        /**
         * Destructor.
         */
        ~SDLMusic() override final;

        /**
         * Plays the music.
         *
         * @param loops     Number of times to repeat the playback (-1 means
         *                  forever).
         * @param fadeIn    Duration in milliseconds to fade in the music.
         *
         * @return <code>true</code> if the playback started properly
         *         <code>false</code> otherwise.
         */
        bool play(const int loops,
                  const int fadeIn) const;

        int calcMemoryLocal() const override final;

        std::string getCounterName() const override final
        { return mName; }

    protected:
        std::string mName;
        Mix_Music *mMusic;
        SDL_RWops *mRw;
};

#endif  // RESOURCES_SDLMUSIC_H
