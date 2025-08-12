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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "options.h"

#include "enums/emotetype.h"

#include "enums/input/keyboardfocus.h"

#include "enums/resources/map/maptype.h"

#include "utils/vector.h"

#include "localconsts.h"

class Settings final
{
    public:
        Settings() :
            updateHost(),
            login(),
            oldUpdates(),
            updatesDir(),
            configDir(),
            localDataDir(),
            tempDir(),
            serverName(),
            screenshotDir(),
            onlineListUrl(),
            serverConfigDir(),
            usersDir(),
            usersIdDir(),
            npcsDir(),
            supportUrl(),
            logFileName(),
            rootDir(),
            windowCaption(),
            gmCommandSymbol("@"),
            gmCharCommandSymbol("#"),
            linkCommandSymbol("="),
            userAgent(),
            updateMirrors(),
            options(),
            guiAlpha(1.0F),
            textureSize(1024),
            moveType(0U),
            crazyMoveType(0U),
            moveToTargetType(0U),
            followMode(0U),
            attackWeaponType(0U),
            attackType(0U),
            quickDropCounter(0U),
            pickUpType(0U),
            magicAttackType(0U),
            pvpAttackType(0U),
            imitationMode(0U),
            cameraMode(0U),
            crazyMoveState(0U),
            targetingType(0U),
            overweightPercent(50U),
            fixedInventorySize(100U),
            playerNameOffset(64),
            playerBadgeAtRightOffset(32),
            mapDrawType(MapType::NORMAL),
            emoteType(EmoteType::Player),
            persistentIp(true),
            limitFps(false),
            inputFocused(KeyboardFocus::Focused),
            mouseFocused(true),
            disableGameModifiers(false),
            awayMode(false),
            pseudoAwayMode(false),
            fixDeadAnimation(true),
            disableLoggingInGame(false),
            enableRemoteCommands(true),
            uselonglivesprites(false),
            unknownSkillsAutoTab(true),
            enableNewMailSystem(true),
            showButtonIcons(false),
            exitcode(0)
        { }

        A_DELETE_COPY(Settings)

        void init();

        std::string updateHost;
        std::string login;
        std::string oldUpdates;
        std::string updatesDir;
        std::string configDir;
        std::string localDataDir;
        std::string tempDir;
        std::string serverName;
        std::string screenshotDir;
        std::string onlineListUrl;
        std::string serverConfigDir;
        std::string usersDir;
        std::string usersIdDir;
        std::string npcsDir;
        std::string supportUrl;
        std::string logFileName;
        std::string rootDir;
        std::string windowCaption;
        std::string gmCommandSymbol;
        std::string gmCharCommandSymbol;
        std::string linkCommandSymbol;
        std::string userAgent;
        STD_VECTOR<std::string> updateMirrors;
        Options options;
        float guiAlpha;
        unsigned int textureSize;
        unsigned int moveType;
        unsigned int crazyMoveType;
        unsigned int moveToTargetType;
        unsigned int followMode;
        unsigned int attackWeaponType;
        unsigned int attackType;
        unsigned int quickDropCounter;
        unsigned int pickUpType;
        unsigned int magicAttackType;
        unsigned int pvpAttackType;
        unsigned int imitationMode;
        unsigned int cameraMode;
        unsigned int crazyMoveState;
        unsigned int targetingType;
        unsigned int overweightPercent;
        unsigned int fixedInventorySize;
        int playerNameOffset;
        int playerBadgeAtRightOffset;
        MapTypeT mapDrawType;
        EmoteTypeT emoteType;
        bool persistentIp;
        bool limitFps;
        KeyboardFocusT inputFocused;
        bool mouseFocused;
        bool disableGameModifiers;
        bool awayMode;
        bool pseudoAwayMode;
        bool fixDeadAnimation;
        bool disableLoggingInGame;
        bool enableRemoteCommands;
        bool uselonglivesprites;
        bool unknownSkillsAutoTab;
        bool enableNewMailSystem;
        bool showButtonIcons;
        int exitcode;
};

extern Settings settings;

#endif  // SETTINGS_H
