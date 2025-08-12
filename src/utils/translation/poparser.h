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

#ifndef UTILS_TRANSLATION_POPARSER_H
#define UTILS_TRANSLATION_POPARSER_H

#include "localconsts.h"

#include <sstream>

class PoDict;

class PoParser final
{
    public:
        PoParser();

        A_DELETE_COPY(PoParser)

        PoDict *load(const std::string &restrict lang,
                     const std::string &restrict fileName,
                     PoDict *restrict const dict);

        static bool checkLang(const std::string &lang);

        static PoDict *getEmptyDict();

    private:
        void setLang(const std::string &lang)
        { mLang = lang; }

        void openFile(const std::string &name);

        bool readLine();

        bool readMsgId();

        bool readMsgStr();

        bool checkLine() const;

        static std::string getFileName(const std::string &lang);

        PoDict *getDict() const RETURNS_NONNULL A_WARN_UNUSED;

        static void convertStr(std::string &str);

        // current lang
        std::string mLang;

        // po file object
        std::istringstream mFile;

        // current line from po file
        std::string mLine;

        std::string mMsgId;

        std::string mMsgStr;

        PoDict *mDict;

        bool mReadingId;

        bool mReadingStr;

        bool mSkipId;
};

#endif  // UTILS_TRANSLATION_POPARSER_H
