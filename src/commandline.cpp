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

#include "commandline.h"

#include "main.h"
#include "settings.h"

#include "fs/paths.h"

#include "utils/cast.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <getopt.h>
#include <iostream>
#include <unistd.h>

#include "debug.h"

static void printHelp()
{
    std::cout <<
        // TRANSLATORS: command line help
        _("manaplus [options] [manaplus-file]") <<
        std::endl << std::endl <<
        // TRANSLATORS: command line help
        _("[manaplus-file] : The manaplus file is an XML file (.manaplus)") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("                  used to set custom parameters") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("                  to the manaverse client.") <<
        std::endl << std::endl <<
        // TRANSLATORS: command line help
        _("Options:") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -l --log-file <arg>       : Log file to use") << std::endl <<
        // TRANSLATORS: command line help
        _("  -a --chat-log-dir <arg>   : Chat log dir to use") << std::endl <<
        // TRANSLATORS: command line help
        _("  -v --version              : Display the version") << std::endl <<
        // TRANSLATORS: command line help
        _("  -h --help                 : Display this help") << std::endl <<
        // TRANSLATORS: command line help
        _("  -C --config-dir <arg>     : Configuration directory to use") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -U --username <arg>       : Login with this username") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -P --password <arg>       : Login with this password") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -c --character <arg>      : Login with this character") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -s --server <arg>         : Login server name or IP") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -y --server-type <arg>    : Login server type") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -p --port <arg>           : Login server port") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -H --update-host <arg>    : Use this update host") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -D --default              : Choose default character server and "
            "character") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -u --skip-update          : Skip the update downloads") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -d --data <arg>           : Directory to load game data from") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -L --localdata-dir <arg>  : Directory to use as local "
            "data directory") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("     --screenshot-dir <arg> : Directory to store screenshots") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("     --safemode             : Start game in safe mode") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("     --renderer <arg>       : Set renderer type") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -T --tests                : Start testing drivers and "
            "auto configuring") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -V --validate             : Start validating client data") <<
        std::endl <<
        // TRANSLATORS: command line help
        _("  -S --uniqueSession        : Disables back to login "
            "(Launcher/Vault/Steam only option!)") <<
        std::endl
#ifdef USE_OPENGL
        <<
        // TRANSLATORS: command line help
        _("  -O --no-opengl            : Disable OpenGL for this session") <<
        std::endl
#endif  // USE_OPENGL
        ;
}

static void printVersion()
{
    std::cout <<
        strprintf("ManaVerse client %s", FULL_VERSION) <<
        std::endl;
}

void parseOptions(const int argc, char *const argv[])
{
    const char *const optstring = "hvud:U:P:Dc:p:y:l:L:C:s:t:T:a:r:e:V:S";

    const option long_options[] =
    {
        { "config-dir",     required_argument, nullptr, 'C' },
        { "data",           required_argument, nullptr, 'd' },
        { "default",        no_argument,       nullptr, 'D' },
        { "password",       required_argument, nullptr, 'P' },
        { "character",      required_argument, nullptr, 'c' },
        { "help",           no_argument,       nullptr, 'h' },
        { "localdata-dir",  required_argument, nullptr, 'L' },
        { "update-host",    required_argument, nullptr, 'H' },
        { "port",           required_argument, nullptr, 'p' },
        { "server",         required_argument, nullptr, 's' },
        { "skip-update",    no_argument,       nullptr, 'u' },
        { "username",       required_argument, nullptr, 'U' },
        { "no-opengl",      no_argument,       nullptr, 'O' },
        { "chat-log-dir",   required_argument, nullptr, 'a' },
        { "version",        no_argument,       nullptr, 'v' },
        { "log-file",       required_argument, nullptr, 'l' },
        { "screenshot-dir", required_argument, nullptr, 'i' },
        { "safemode",       no_argument,       nullptr, 'm' },
        { "tests",          no_argument,       nullptr, 'T' },
        { "test",           required_argument, nullptr, 't' },
        { "renderer",       required_argument, nullptr, 'r' },
        { "server-type",    required_argument, nullptr, 'y' },
        { "enable-ipc",     no_argument,       nullptr, 'I' },
        { "hide-cursor",    no_argument,       nullptr, 'q' },
        { "error",          no_argument,       nullptr, 'e' },
        { "validate",       no_argument,       nullptr, 'V' },
        { "unique-session", no_argument,       nullptr, 'S' },
        { nullptr,          0,                 nullptr, 0 }
    };

    Options &options = settings.options;

    while (optind < argc)
    {
        const int result = getopt_long(argc,
            argv,
            optstring,
            long_options,
            nullptr);

        if (result == -1)
            break;

        switch (result)
        {
            case 'C':
                options.configDir = optarg;
                break;
            case 'd':
                options.dataPath = optarg;
                break;
            case 'D':
                options.chooseDefault = true;
                break;
            case '?':  // Unknown option
            case ':':  // Missing argument
            case 'h':
                options.printHelp = true;
                break;
            case 'H':
                if (checkPath(optarg))
                    options.updateHost = optarg;
                else
                    options.updateHost.clear();
                break;
            case 'c':
                options.character = optarg;
                break;
            case 'P':
                options.password = optarg;
                break;
            case 's':
                options.serverName = optarg;
                break;
            case 'p':
                options.serverPort = CAST_U16(atoi(optarg));
                break;
            case 'u':
                options.skipUpdate = true;
                break;
            case 'U':
                options.username = optarg;
                break;
            case 'v':
                options.printVersion = true;
                break;
            case 'L':
                options.localDataDir = optarg;
                break;
            case 'O':
                options.noOpenGL = true;
                break;
            case 'l':
                options.logFileName = std::string(optarg);
                break;
            case 'a':
                options.chatLogDir = std::string(optarg);
                break;
            case 'i':
                options.screenshotDir = optarg;
                break;
            case 'm':
                options.safeMode = true;
                break;
            case 'T':
                options.testMode = true;
                options.test.clear();
                break;
            case 't':
                options.testMode = true;
                options.test = std::string(optarg);
                break;
            case 'r':
                options.renderer = CAST_S32(atoi(optarg));
                break;
            case 'y':
                options.serverType = optarg;
                break;
            case 'I':
                options.ipc = true;
                break;
            case 'q':
                options.hideCursor = true;
                break;
            case 'e':
                options.error = true;
                break;
            case 'V':
                options.validate = true;
                break;
            case 'S':
                options.uniqueSession = true;
                break;
            default:
                break;
        }
    }

    // when there are still options left use the last
    // one as branding file
    if (optind < argc)
    {
        options.brandingPath = argv[optind];
    }

    if (settings.options.printHelp)
    {
        printHelp();
        _exit(0);
    }
    else if (settings.options.printVersion)
    {
        printVersion();
        _exit(0);
    }
}
