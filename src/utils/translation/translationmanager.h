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

#ifndef UTILS_TRANSLATION_TRANSLATIONMANAGER_H
#define UTILS_TRANSLATION_TRANSLATIONMANAGER_H

#include "localconsts.h"

#include "utils/langs.h"
#include "utils/stringvector.h"

class PoDict;

class TranslationManager final
{
    public:
        A_DELETE_COPY(TranslationManager)

        static PoDict *loadLang(const LangVect &lang,
                                const std::string &subName,
                                PoDict *const dict);

        static void init();

        static void close();

        static void loadCurrentLang();

        static void loadDictionaryLang();

#ifdef ENABLE_CUSTOMNLS
        static void loadGettextLang();
#endif  // ENABLE_CUSTOMNLS

        static bool translateFile(const std::string &fileName,
                                  PoDict *const dict,
                                  StringVect &lines);
        static PoDict *reverseLang(const PoDict *const dict);
};

#endif  // UTILS_TRANSLATION_TRANSLATIONMANAGER_H
