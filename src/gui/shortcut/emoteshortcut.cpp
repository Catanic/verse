/*
 *  The ManaVerse Client
 *  Copyright (C) 2009  Aethyra Development Team
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

#include "gui/shortcut/emoteshortcut.h"

#include "configuration.h"
#include "settings.h"

#include "being/localplayer.h"

#include "net/homunculushandler.h"
#include "net/mercenaryhandler.h"
#include "net/pethandler.h"

#include "resources/db/emotedb.h"

#include "debug.h"

EmoteShortcut *emoteShortcut = nullptr;

EmoteShortcut::EmoteShortcut() :
    mEmoteSelected(0)
{
    for (int i = 0; i < SHORTCUT_EMOTES; i++)
        mEmotes[i] = 0;
}

EmoteShortcut::~EmoteShortcut()
{
    save();
}

void EmoteShortcut::load()
{
    for (unsigned char i = 0, j = 0, fsz = CAST_U8(EmoteDB::getLast());
         i <= fsz && j < SHORTCUT_EMOTES;
         i++)
    {
        const EmoteSprite *const sprite = EmoteDB::getSprite(i, true);
        if (sprite != nullptr)
        {
            mEmotes[j] = CAST_U8(i + 1);
            j ++;
        }
    }
}

void EmoteShortcut::save() const
{
    for (int i = 0; i < SHORTCUT_EMOTES; i++)
    {
        const unsigned char emoteId = mEmotes[i] != 0U ? mEmotes[i]
            : CAST_U8(0);
        serverConfig.setValue("emoteshortcut" + toString(i),
            CAST_U32(emoteId));
    }
}

void EmoteShortcut::useEmotePlayer(const size_t index) const
{
    if (index <= CAST_SIZE(SHORTCUT_EMOTES))
    {
        if (mEmotes[index - 1] > 0)
            LocalPlayer::emote(mEmotes[index - 1]);
    }
}

void EmoteShortcut::useEmote(const size_t index) const
{
    if (localPlayer == nullptr)
        return;

    if (index <= CAST_SIZE(SHORTCUT_EMOTES))
    {
        if (mEmotes[index - 1] > 0)
        {
            const uint8_t emote = mEmotes[index - 1];
            switch (settings.emoteType)
            {
                case EmoteType::Player:
                default:
                    LocalPlayer::emote(emote);
                    break;
                case EmoteType::Pet:
                    petHandler->emote(emote);
                    break;
                case EmoteType::Homunculus:
                    homunculusHandler->emote(emote);
                    break;
                case EmoteType::Mercenary:
                    mercenaryHandler->emote(emote);
                    break;
            }
        }
    }
}
