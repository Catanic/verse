/*
 *  The ManaVerse Client
 *  Copyright (C) 2014-2020  The ManaPlus Developers
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

#include "unittests/unittests.h"

#include "client.h"
#include "configmanager.h"
#include "configuration.h"
#include "dirs.h"
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "fs/virtfs/fs.h"

#include "gui/userpalette.h"
#include "gui/theme.h"

#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/xmlutils.h"

#include "render/sdlgraphics.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sdlimagehelper.h"

#include "debug.h"

TEST_CASE("xmlutils readXmlIntVector 1", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();

    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        GraphicsManager::createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    GraphicsManager::createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    userPalette = new UserPalette;

    Dirs::initRootDir();
    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();

    STD_VECTOR<int> arr;

    readXmlIntVector("graphics/gui/browserbox.xml",
        "skinset",
        "widget",
        "option",
        "value",
        arr,
        SkipError_false);

    REQUIRE(5 == arr.size());
    REQUIRE(1 == arr[0]);
    REQUIRE(15 == arr[1]);
    REQUIRE(0 == arr[2]);
    REQUIRE(1 == arr[3]);
    REQUIRE(1 == arr[4]);

    delete2(userPalette)
    delete2(theme)
    delete2(client)
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("xmlutils readXmlStringMap 1", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();

    Dirs::initRootDir();
    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        GraphicsManager::createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    GraphicsManager::createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    userPalette = new UserPalette;

    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();

    std::map<std::string, std::string> arr;

    readXmlStringMap("graphics/sprites/manaplus_emotes.xml",
        "emotes",
        "emote",
        "sprite",
        "name",
        "variant",
        arr,
        SkipError_false);

    REQUIRE(arr.size() == 27);
    REQUIRE(arr["Kitty"] == "0");
    REQUIRE(arr["xD"] == "1");
    REQUIRE(arr["Metal"] == "26");

    delete2(userPalette)
    delete2(theme)
    delete2(client)
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("xmlutils readXmlIntMap 1", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();

    Dirs::initRootDir();
    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        GraphicsManager::createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    GraphicsManager::createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    userPalette = new UserPalette;

    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();

    std::map<int32_t, int32_t> arr;

    readXmlIntMap("testintmap.xml",
        "tests",
        "sub",
        "item",
        "id",
        "val",
        arr,
        SkipError_false);

    REQUIRE(arr.size() == 3);
    REQUIRE(arr[1] == 2);
    REQUIRE(arr[10] == 20);
    REQUIRE(arr[3] == 0);

    delete2(userPalette)
    delete2(theme)
    delete2(client)
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}
