# The Mana Verse Client

![ManaVerse logo](data/icons/manaverse.png)

### Downloads and Build status
<!-- can't link to job pages directly (https://gitlab.com/gitlab-org/gitlab/-/issues/19845#note_435733246)
     but the browse link has a job page reference and a download button. -->
  <!-- direct link: */-/jobs/artifacts/master/download?job=mxe_gcc5 -->
  [![gitlab windows build](https://git.themanaworld.org/mana/docker-windows-builder/badges/master/pipeline.svg?key_text=Windows+Build&key_width=100px)](https://git.themanaworld.org/mana/docker-windows-builder/-/jobs/artifacts/master/browse?job=mxe_gcc5)
  [![gitlab appimg build](https://git.themanaworld.org/mana/appimg-builder/badges/master/pipeline.svg?key_text=AppImg+Build&key_width=100px)](https://git.themanaworld.org/mana/appimg-builder/-/jobs/artifacts/master/browse?job=x86_64-AppImage)
  [![gitlab .deb build](https://git.themanaworld.org/mana/appimg-builder/badges/master/pipeline.svg?key_text=.deb+Build&key_width=100px)](https://git.themanaworld.org/mana/appimg-builder/-/jobs/artifacts/master/browse?job=deb_pkg)

  [![gitlab build status](https://git.themanaworld.org/mana/verse/badges/master/pipeline.svg?key_text=CI%20on%20master&key_width=100px)](https://git.themanaworld.org/mana/verse/-/pipelines)
<!--
Cicle CI: [![CircleCI](https://circleci.com/gh/ManaPlus/ManaPlus/tree/master.svg?style=svg)](https://circleci.com/gh/ManaPlus/ManaPlus/tree/master)

Semaphore CI: [![Build Status](https://semaphoreci.com/api/v1/4144/manaplus/branches/master/badge.svg)](https://semaphoreci.com/4144/manaplus)

Travis CI: [![travis build status](https://travis-ci.org/ManaPlus/ManaPlus.svg?branch=master)](https://travis-ci.org/ManaPlus/ManaPlus/builds/)

Azure CI: [![Build Status macosx](https://dev.azure.com/manaplus/ManaPlus/_apis/build/status/ManaPlus%20macosx)](https://dev.azure.com/manaplus/ManaPlus/_build/latest?definitionId=2)
          [![Build Status windows](https://dev.azure.com/manaplus/ManaPlus/_apis/build/status/ManaPlus%20main)](https://dev.azure.com/manaplus/ManaPlus/_build/latest?definitionId=1)
-->

  Click on an image above to download last good build or ![Build yourself](INSTALL)

<!-- no pipelines rn :(

 - Coverage: ![coverage report](https://git.themanaworld.org/mana/verse/badges/master/coverage.svg)

 - Doxygen: ![doxygen](https://img.shields.io/badge/Docs-Doxygen-blue.svg)

 - Coverity: ![coverity](https://scan.coverity.com/projects/318/badge.svg)
-->

### Development team:
 - See [AUTHORS](AUTHORS) file for a list

### Dependencies:

 - [SDL](http://libsdl.org/), SDL_image, SDL_mixer, SDL_ttf, SDL_net (Media framework), SDL_gfx
 - [libxml2](http://xmlsoft.org/) or [pugixml](http://pugixml.org/) or [tinyxml2](http://www.grinninglizard.com/tinyxml2/) (XML parsing and writing)
 - [libcurl](http://curl.haxx.se/) (HTTP downloads)
 - [libpng](http://www.libpng.org/) (save screenshots)
 - [zlib](http://zlib.net/) (Archives)

z16sra-codex/fix-cmake-configuration-errors
Make sure the development headers for these libraries are installed. On
Debian or Ubuntu the required packages can be installed with:

    sudo apt install build-essential cmake pkg-config \
        libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev \
        libsdl2-net-dev libsdl2-ttf-dev libsdl2-gfx-dev \
        libcurl4-openssl-dev

For Arch Linux and derivatives use:

z16sra-codex/fix-cmake-configuration-errors
    sudo pacman -S base-devel cmake sdl2 sdl2_image \
        sdl2_mixer sdl2_net sdl2_ttf sdl2_gfx curl

#### Optional dependencies:

 - [gettext](https://www.gnu.org/software/gettext/) (translations)

#### Embedded libs:

  - [catch](https://github.com/philsquared/Catch) and [doctest](https://github.com/onqtam/doctest) for unit testing.
  - [nvwa](https://github.com/adah1972/nvwa) for debug memory leaks.
  - [SaferCPlusPlus](https://github.com/duneroadrunner/SaferCPlusPlus) for debug issues in std::vector usage.

### Additional run time deps:
 - xsel (for clipboard access, used only in SDL 1.2)
 - xdg-open (from xdg-utils, for open url in browser)

### Images and fonts sources:
 - data/icons
 - data/fonts/src


## 0. Index

1. Account
2. Commands
3. Skills
4. Support
5. License

## 1. Account

To create an account you can usually press the "Register" button after
choosing your server. When this doesn't work, visit the website of the server
you'd like to register on, since they may be using an online registration
form instead.

## 2. Commands

### KEYBOARD:

Use arrow keys to move around. Other keys:

- Ctrl               attack
- F1                 toggle the online help
- F2                 toggle profile window
- F3                 toggle inventory window
- F4                 toggle equipment window
- F5                 toggle skills window
- F6                 toggle minimap
- F7                 toggle chat window
- F8                 toggle shortcut window
- F9                 show setup window
- F10                toggle debug window
- Alt + 0-9,-, etc   show emotions.
- S                  sit down / stand up.
- F                  toggle debug pathfinding feature (change map mode)
- P                  take screenshot
- R                  turns on anti-trade function.
- A                  target nearest monster
- H                  hide all non-sticky windows
- Z                  pick up item
- Enter              focus chat window / send message

### MOUSE:

Left click to execute default action: walk, pick up an item, attack a monster
and talk to NPCs (be sure to click on their feet). Right click to show up a
context menu. Holding [Left Shift] prevents from walking when attacking.

### /Commands:

Whispers:
- /closeall          close all whispers.
- /ignoreall         add all whispers to ignore list.
- /msg NICK text
- /whisper NICK text
- /w NICK text       send whisper message to nick.
- /query NICK
- /q NICK            open new whisper tab for nick.

### Actions:
- /help              show small help about chat commands. /target NICK - select nick as target. Can be monster or player nick.
- /outfit N          wear outfit number N.
- /outfit next       wear next outfit.
- /outfit prev       wear previous outfit.
- /emote N           use emotion number N.
- /away
- /away MSG          set away mode.
- /follow NICK       start follow mode.
- /imitation NICK    start imitation mode.
- /heal NICK         heal somebody by nickname
- /move X Y          move to X,Y position in short distance.
- /navigate x y      move to position x,y in current map in any distance.
- /mail NICK MSG     send offline message to NICK. Working only in servers with mail support.
- /disconnect        quick disconnect from server.
- /attack            attack target.

### Trade:
- /trade NICK        start trade with nick.
- /priceload         load shop price from disk.
- /pricesave         save shop price to disk.

### Player relations:
- /ignore NICK       add nick to ignore list.
- /unignore NICK     Remove nick from ignore list.
- /friend NICK
- /befriend NICK     add nick to friends list.
- /disregard NICK    add nick to disregarded list.
- /neutral NICK      add nick to neutral relation list.
- /erase NICK        add nick to erased list.
- /clear             clear current chat tab.
- /createparty NAME  create party with selected name.
- /me text           send text to chat as /me command in irc.

### Debug:
- /who               print online players number to chat.
- /all               show visible beings list in debug tab.
- /where             Show map and coordiantes of your avatar
- /cacheinfo         show text cache info.
- /dirs              show client directories in debug window.

### Other:
- /help              Displays the list of commands
- /announce          (GM) broadcasts a global msg

## 4. Support

Please report any issues you are having with this client.
You can report them at:

<!-- Can't easily get a badge for *open* issues from GitLab itself: https://stackoverflow.com/questions/48829255/gitlab-count-total-number-of-issues -->
- ![GitLab Issues](https://img.shields.io/gitlab/issues/open/mana%2Fplus?gitlab_url=https%3A%2F%2Fgit.themanaworld.org) (if you have a TMW GitLab account)
- [![forum](https://img.shields.io/badge/forum-forums.themanaworld.org-yellow.svg)](https://forums.themanaworld.org/viewforum.php?f=56)
- [![IRC](https://img.shields.io/badge/IRC-Libera%2Fthemanaworld--irc-yellow)](https://web.libera.chat/?channels=themanaworld-irc)
- ![Discord](https://shields.io/discord/775612630836445206.svg?label=Discord)
- In game: to game masters, developers or #world channel.

If you have feedback about a specific game that uses the ManaVerse client, be sure
to instead contact the developers of that game.

## 5. Licenses

All code in this repository is under ![GPL 2+](https://img.shields.io/badge/license-GPL2+-blue.svg) license. Art can be under ![CC-BY-SA](https://img.shields.io/badge/license-CC_BY_SA-blue.svg) or ![GPL 2+](https://img.shields.io/badge/license-GPL2+-blue.svg).

GitHub or other non-free hosting providers may use strange Terms of Service that may violate these licenses.
If any of their terms violate these licenses then these providers should remove ManaVerse repositories from their platforms.

ManaVerse is a fork of ManaPlus, the main repository of the latter is hosted on [gitlab.com](https://gitlab.com/manaplus/manaplus/)
