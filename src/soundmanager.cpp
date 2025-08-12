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

#include "soundmanager.h"

#include "configuration.h"

#ifndef DYECMD
#include "being/localplayer.h"
#endif  // DYECMD

#include "fs/virtfs/fs.h"

#include "resources/sdlmusic.h"
#include "resources/soundeffect.h"

#include "resources/loaders/musicloader.h"
#include "resources/loaders/soundloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#ifdef DYECMD
#include "utils/cast.h"
#endif
#include "utils/checkutils.h"
#include "utils/performance.h"
#include "utils/sdlmusichelper.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

SoundManager soundManager;

/**
 * This will be set to true, when a music can be freed after a fade out
 * Currently used by fadeOutCallBack()
 */
static bool sFadingOutEnded = false;

/**
 * Callback used at end of fadeout.
 * It is called by Mix_MusicFadeFinished().
 */
static void fadeOutCallBack()
{
    sFadingOutEnded = true;
}

SoundManager::SoundManager() :
    ConfigListener(),
    mNextMusicFile(),
    mInstalled(false),
    mSfxVolume(100),
    mMusicVolume(60),
    mCurrentMusicFile(),
    mMusic(nullptr),
    mGuiChannel(-1),
    mPlayBattle(false),
    mPlayGui(false),
    mPlayMusic(false),
    mFadeoutMusic(true),
    mCacheSounds(true)
{
    // This set up our callback function used to
    // handle fade outs endings.
    sFadingOutEnded = false;
    Mix_HookMusicFinished(&fadeOutCallBack);
}

SoundManager::~SoundManager()
{
}

void SoundManager::shutdown()
{
    config.removeListeners(this);

    // Unlink the callback function.
    Mix_HookMusicFinished(nullptr);

    CHECKLISTENERS
}

void SoundManager::optionChanged(const std::string &value)
{
    if (value == "playBattleSound")
        mPlayBattle = config.getBoolValue("playBattleSound");
    else if (value == "playGuiSound")
        mPlayGui = config.getBoolValue("playGuiSound");
    else if (value == "playMusic")
        mPlayMusic = config.getBoolValue("playMusic");
    else if (value == "sfxVolume")
        setSfxVolume(config.getIntValue("sfxVolume"));
    else if (value == "musicVolume")
        setMusicVolume(config.getIntValue("musicVolume"));
    else if (value == "fadeoutmusic")
        mFadeoutMusic = (config.getIntValue("fadeoutmusic") != 0);
    else if (value == "uselonglivesounds")
        mCacheSounds = (config.getIntValue("uselonglivesounds") != 0);
    else if (value == "parallelAudioChannels")
        setChannels(config.getIntValue("parallelAudioChannels"));
}

void SoundManager::init()
{
    // Don't initialize sound engine twice
    if (mInstalled)
        return;

    logger->log1("SoundManager::init() Initializing sound...");

    mPlayBattle = config.getBoolValue("playBattleSound");
    mPlayGui = config.getBoolValue("playGuiSound");
    mPlayMusic = config.getBoolValue("playMusic");
    mFadeoutMusic = config.getBoolValue("fadeoutmusic");
    mMusicVolume = config.getIntValue("musicVolume");
    mSfxVolume = config.getIntValue("sfxVolume");
    mCacheSounds = (config.getIntValue("uselonglivesounds") != 0);

    config.addListener("playBattleSound", this);
    config.addListener("playGuiSound", this);
    config.addListener("playMusic", this);
    config.addListener("sfxVolume", this);
    config.addListener("musicVolume", this);
    config.addListener("fadeoutmusic", this);
    config.addListener("uselonglivesounds", this);
    config.addListener("parallelAudioChannels", this);

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) == -1)
    {
        logger->log1("SoundManager::init() Failed to "
            "initialize audio subsystem");
        return;
    }

    const size_t audioBuffer = 4096;
    int channels = config.getIntValue("audioChannels");
    switch (channels)
    {
        case 3:
            channels = 4;
            break;
        case 4:
            channels = 6;
            break;
        default:
            break;
    }

    const int res = SDL::MixOpenAudio(config.getIntValue("audioFrequency"),
        MIX_DEFAULT_FORMAT, channels, audioBuffer);

    if (res < 0)
    {
        logger->log("SoundManager::init Could not initialize audio: %s",
                    SDL_GetError());
        if (SDL::MixOpenAudio(22010, MIX_DEFAULT_FORMAT, 2, audioBuffer) < 0)
            return;
        logger->log("Fallback to stereo audio");
    }

    Mix_AllocateChannels(config.getIntValue("parallelAudioChannels"));
    Mix_VolumeMusic(mMusicVolume);
    Mix_Volume(-1, mSfxVolume);

    info();

    mInstalled = true;

    if (!mCurrentMusicFile.empty() && mPlayMusic)
        playMusic(mCurrentMusicFile, SkipError_true);
}

void SoundManager::testAudio()
{
    mPlayBattle = config.getBoolValue("playBattleSound");
    mPlayGui = config.getBoolValue("playGuiSound");
    mPlayMusic = config.getBoolValue("playMusic");
    mFadeoutMusic = config.getBoolValue("fadeoutmusic");
    mMusicVolume = config.getIntValue("musicVolume");
    mSfxVolume = config.getIntValue("sfxVolume");
    mCacheSounds = (config.getIntValue("uselonglivesounds") != 0);

    const size_t audioBuffer = 4096;
    int channels = config.getIntValue("audioChannels");
    switch (channels)
    {
        case 3:
            channels = 4;
            break;
        case 4:
            channels = 6;
            break;
        default:
            break;
    }

    SDL_AudioSpec desired;
    SDL_AudioSpec actual;

    desired.freq = config.getIntValue("audioFrequency");
    desired.format = MIX_DEFAULT_FORMAT;
    desired.channels = CAST_U8(channels);
    desired.samples = audioBuffer;
    desired.callback = nullptr;
    desired.userdata = nullptr;

    if (SDL_OpenAudio(&desired, &actual) < 0)
    {
        logger->log("SoundManager::testAudio error: %s",
            SDL_GetError());
        return;
    }
    if (desired.freq != actual.freq)
    {
        logger->log("SoundManager::testAudio frequence: %d -> %d",
            actual.freq, desired.freq);
    }
    if (desired.format != actual.format)
    {
        logger->log("SoundManager::testAudio format: %d -> %d",
            actual.format, desired.format);
    }
    if (desired.channels != actual.channels)
    {
        logger->log("SoundManager::testAudio channels: %d -> %d",
            actual.channels, desired.channels);
    }
    if (desired.samples != actual.samples)
    {
        logger->log("SoundManager::testAudio samples: %d -> %d",
            actual.samples, desired.samples);
    }
    SDL_CloseAudio();
}

void SoundManager::info()
{
    SDL_version compiledVersion;
    const char *format = "Unknown";
    int rate = 0;
    uint16_t audioFormat = 0;
    int channels = 0;

    MIX_VERSION(&compiledVersion)
    const SDL_version *const linkedVersion = Mix_Linked_Version();

#ifdef USE_SDL2
    const char *driver = SDL_GetCurrentAudioDriver();
#else  // USE_SDL2
    char driver[40] = "Unknown";
    SDL_AudioDriverName(driver, 40);
#endif  // USE_SDL2

    Mix_QuerySpec(&rate, &audioFormat, &channels);
    switch (audioFormat)
    {
        case AUDIO_U8:
            format = "U8";
            break;
        case AUDIO_S8:
            format = "S8"; break;
        case AUDIO_U16LSB:
            format = "U16LSB";
            break;
        case AUDIO_S16LSB:
            format = "S16LSB";
            break;
        case AUDIO_U16MSB:
            format = "U16MSB";
            break;
        case AUDIO_S16MSB:
            format = "S16MSB";
            break;
        default: break;
    }

    logger->log("SoundManager::info() SDL_mixer: %i.%i.%i (compiled)",
            compiledVersion.major,
            compiledVersion.minor,
            compiledVersion.patch);
    if (linkedVersion != nullptr)
    {
        logger->log("SoundManager::info() SDL_mixer: %i.%i.%i (linked)",
            linkedVersion->major,
            linkedVersion->minor,
            linkedVersion->patch);
    }
    else
    {
        logger->log1("SoundManager::info() SDL_mixer: unknown");
    }
    logger->log("SoundManager::info() Driver: %s", driver);
    logger->log("SoundManager::info() Format: %s", format);
    logger->log("SoundManager::info() Rate: %i", rate);
    logger->log("SoundManager::info() Channels: %i", channels);
}

void SoundManager::setMusicVolume(const int volume)
{
    mMusicVolume = volume;

    if (mInstalled)
        Mix_VolumeMusic(mMusicVolume);
}

void SoundManager::setSfxVolume(const int volume)
{
    mSfxVolume = volume;

    if (mInstalled)
        Mix_Volume(-1, mSfxVolume);
}

static SDLMusic *loadMusic(const std::string &fileName,
                           const SkipError skipError)
{
    const std::string path = pathJoin(paths.getStringValue("music"),
        fileName);
    if (!VirtFs::exists(path))
    {
        if (skipError == SkipError_false)
            reportAlways("Music file not found: %s", fileName.c_str())
        return nullptr;
    }
    return Loader::getMusic(path);
}

void SoundManager::playMusic(const std::string &fileName,
                             const SkipError skipError)
{
    if (!mInstalled || !mPlayMusic)
        return;

    if (mCurrentMusicFile == fileName)
        return;

    mCurrentMusicFile = fileName;

    haltMusic();

    if (!fileName.empty())
    {
        mMusic = loadMusic(fileName,
            skipError);
        if (mMusic != nullptr)
            mMusic->play(-1, 0);
    }
}

void SoundManager::stopMusic()
{
    haltMusic();
}

/*
void SoundManager::fadeInMusic(const std::string &fileName, const int ms)
{
    mCurrentMusicFile = fileName;

    if (!mInstalled || !mPlayMusic)
        return;

    haltMusic();

    if (!fileName.empty())
    {
        mMusic = loadMusic(fileName);
        if (mMusic)
            mMusic->play(-1, ms);
    }
}
*/

void SoundManager::fadeOutMusic(const int ms)
{
    if (!mPlayMusic)
        return;

    mCurrentMusicFile.clear();

    if (!mInstalled)
        return;

    logger->log("SoundManager::fadeOutMusic() Fading-out (%i ms)", ms);

    if ((mMusic != nullptr) && mFadeoutMusic)
    {
        Mix_FadeOutMusic(ms);
        // Note: The fadeOutCallBack handler will take care about freeing
        // the music file at fade out ending.
    }
    else
    {
        sFadingOutEnded = true;
        if (!mFadeoutMusic)
            haltMusic();
    }
}

void SoundManager::fadeOutAndPlayMusic(const std::string &fileName,
                                       const int ms)
{
    if (!mPlayMusic)
        return;

    mNextMusicFile = fileName;
    fadeOutMusic(ms);
}

void SoundManager::logic()
{
    BLOCK_START("SoundManager::logic")
    if (sFadingOutEnded)
    {
        if (mMusic != nullptr)
        {
            mMusic->decRef();
            mMusic = nullptr;
        }
        sFadingOutEnded = false;

        if (!mNextMusicFile.empty())
        {
            playMusic(mNextMusicFile,
                SkipError_false);
            mNextMusicFile.clear();
        }
    }
    BLOCK_END("SoundManager::logic")
}

#ifdef DYECMD
void SoundManager::playSfx(const std::string &path A_UNUSED,
                           const int x A_UNUSED,
                           const int y A_UNUSED) const
{
}
#else  // DYECMD
void SoundManager::playSfx(const std::string &path,
                           const int x, const int y) const
{
    if (!mInstalled || path.empty() || !mPlayBattle)
        return;

    std::string tmpPath = pathJoin(paths.getStringValue("sfx"), path);
    SoundEffect *const sample = Loader::getSoundEffect(tmpPath);
    if (sample != nullptr)
    {
        logger->log("SoundManager::playSfx() Playing: %s", path.c_str());
        int vol = 120;
        if ((localPlayer != nullptr) && (x > 0 || y > 0))
        {
            int dx = localPlayer->getTileX() - x;
            int dy = localPlayer->getTileY() - y;
            if (dx < 0)
                dx = -dx;
            if (dy < 0)
                dy = -dy;
            const int dist = dx > dy ? dx : dy;
            if (dist * 8 > vol)
                return;

            vol -= dist * 8;
        }
        sample->play(0, vol, -1);
        if (!mCacheSounds)
            sample->decRef();
    }
}
#endif  // DYECMD

void SoundManager::playGuiSound(const std::string &name)
{
    const std::string sound = config.getStringValue(name);
    if (sound == "(no sound)")
        return;
    playGuiSfx(pathJoin(branding.getStringValue("systemsounds"),
        std::string(sound).append(".ogg")));
}

void SoundManager::playGuiSfx(const std::string &path)
{
    if (!mInstalled ||
        !mPlayGui ||
        path.empty())
    {
        return;
    }

    std::string tmpPath;
    if (path.compare(0, 4, "sfx/") == 0)
        tmpPath = path;
    else
        tmpPath = pathJoin(paths.getValue("sfx", "sfx"), path);
    SoundEffect *const sample = Loader::getSoundEffect(tmpPath);
    if (sample != nullptr)
    {
        logger->log("SoundManager::playGuiSfx() Playing: %s", path.c_str());
        const int ret = static_cast<int>(sample->play(0, 120, mGuiChannel));
        if (ret != -1)
            mGuiChannel = ret;
        if (!mCacheSounds)
            sample->decRef();
    }
}

void SoundManager::close()
{
    if (!mInstalled)
        return;

    if (mMusic != nullptr)
    {
        Mix_HaltMusic();
        ResourceManager::decRefDelete(mMusic);
        mMusic = nullptr;
        mCurrentMusicFile.clear();
    }

    logger->log1("SoundManager::close() Shutting down sound...");
    Mix_CloseAudio();

    mInstalled = false;
}

void SoundManager::haltMusic()
{
    if (mMusic == nullptr)
        return;

    Mix_HaltMusic();
    mMusic->decRef();
    mMusic = nullptr;
    mCurrentMusicFile.clear();
}

void SoundManager::changeAudio()
{
    if (mInstalled)
        close();
    else
        init();
}

void SoundManager::volumeOff() const
{
    if (mInstalled)
    {
        Mix_VolumeMusic(0);
        Mix_Volume(-1, 0);
    }
}

void SoundManager::volumeRestore() const
{
    if (mInstalled)
    {
        Mix_VolumeMusic(mMusicVolume);
        Mix_Volume(-1, mSfxVolume);
    }
}

void SoundManager::setChannels(const int channels) const
{
    if (mInstalled)
        Mix_AllocateChannels(channels);
}
