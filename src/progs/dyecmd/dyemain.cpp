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

#include "logger.h"

#include "graphicsmanager.h"
#include "maingui.h"
#include "sdlshared.h"

#include "fs/virtfs/fs.h"

#include "resources/dye/dyepalette.h"

#include "resources/image/image.h"

#ifdef USE_SDL2
#include "resources/surfaceimagehelper.h"
#endif  // USE_SDL2

#include "resources/loaders/imageloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/cpu.h"
#include "utils/gettext.h"
#include "utils/pnglib.h"
#include "utils/sdlhelper.h"

#include <iostream>

#ifndef USE_SDL2
#include "resources/sdlimagehelper.h"
#endif  // USE_SDL2

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

static void printHelp()
{
    // TRANSLATORS: command line help
    std::cout << _("dyecmd srcfile dyestring dstfile") << std::endl;
    // TRANSLATORS: command line help
    std::cout << _("or") << std::endl;
    // TRANSLATORS: command line help
    std::cout << _("dyecmd srcdyestring dstfile") << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "--help") != 0))
    {
        return mainGui(argc, argv);
    }
    if (argc < 3 || argc > 4)
    {
        printHelp();
        return 1;
    }

    logger = new Logger;
    SDL::initLogger();

    VirtFs::init(argv[0]);
    SDL_Init(SDL_INIT_VIDEO);

    Cpu::detect();
    DyePalette::initFunctions();

    GraphicsManager::createWindow(10, 10, 0, SDL_ANYFORMAT);

#ifdef USE_SDL2
    imageHelper = new SurfaceImageHelper;
#else  // USE_SDL2

    imageHelper = new SDLImageHelper;
#endif  // USE_SDL2

    VirtFs::setWriteDir(".");
    VirtFs::mountDir(".", Append_false);
    VirtFs::mountDir("/", Append_false);
    std::string src = argv[1];
    std::string dst;
    if (argc == 4)
    {
        src.append("|").append(argv[2]);
        dst = argv[3];
    }
    else
    {
        dst = argv[2];
    }

    Image *image = Loader::getImage(src);
    if (image == nullptr)
    {
        printf("Error loading image\n");
        VirtFs::deinit();
        return 1;
    }
    SDL_Surface *const surface = ImageHelper::convertTo32Bit(
        image->getSDLSurface());
    PngLib::writePNG(surface, dst);
    SDL_FreeSurface(surface);
    image->decRef();
    ResourceManager::cleanOrphans(true);
    VirtFs::deinit();
    return 0;
}
