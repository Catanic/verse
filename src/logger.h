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

#ifndef LOGGER_H
#define LOGGER_H

#include "localconsts.h"

#include "utils/vector.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_mutex.h>
PRAGMA48(GCC diagnostic pop)

#include <string>

#ifdef ENABLEDEBUGLOG
#define DEBUGLOG(str) \
    if (logger && !mIgnore) \
        logger->dlog(str)
#define DEBUGLOG2(str, pos, comment) \
    if (logger && !mIgnore) \
        logger->dlog2(str, pos, comment)
#define DEBUGLOGSTR(str) \
    if (logger) \
        logger->dlog(str)
#define IGNOREDEBUGLOG mIgnore = Net::isIgnorePacket(mId)
#else  // ENABLEDEBUGLOG
#define DEBUGLOG(str) {}
#define DEBUGLOG2(str, comment) {}
#define DEBUGLOGSTR(str)
#define IGNOREDEBUGLOG {}
#endif  // ENABLEDEBUGLOG

#define UNIMPLEMENTEDPACKET \
    logger->unimplemented(msg.getId())

#define UNIMPLEMENTEDPACKETFIELD(field) \
    logger->unimplemented(msg.getId(), field)

/**
 * The Log Class : Useful to write debug or info messages
 */
class Logger final
{
    public:
        /**
         * Constructor.
         */
        Logger();

        A_DELETE_COPY(Logger)

        /**
         * Destructor, closes log file.
         */
        ~Logger();

        /**
         * Sets the file to log to and opens it
         */
        void setLogFile(const std::string &logFilename);

        /**
         * Sets whether the log should be written to standard output.
         */
        void setLogToStandardOut(const bool value)
        { mLogToStandardOut = value; }

        /**
         * Enters a message in the log. The message will be timestamped.
         */
        void log(const char *const log_text, ...)
            A_NONNULL(2)
            A_FORMAT_PRINTF(true, 1, 2);

        /**
         * Enters a message in the log. The message will be timestamped.
         */
        void assertLog(const char *const log_text, ...)
            A_NONNULL(2)
            A_FORMAT_PRINTF(true, 1, 2);

        /**
         * Enters a message in the log (thread safe).
         */
        void log_r(const char *const log_text, ...)
            A_NONNULL(2)
            A_FORMAT_PRINTF(true, 1, 2);

        /**
         * Enters a message in the log. The message will be timestamped.
         */
        void log1(const char *const log_text) A_NONNULL(2);

        /**
         * Enters a message in the log. The message will be timestamped.
         */
        void log(const std::string &str);

        void flush();

#ifdef ENABLEDEBUGLOG
        /**
         * Enters debug message in the log. The message will be timestamped.
         */
        void dlog(const std::string &str);

        void dlog2(const std::string &str,
                   const int pos,
                   const char* const comment);
#endif  // ENABLEDEBUGLOG

        void closeFile();

        void setDebugLog(const bool n)
        { mDebugLog = n; }

        void setReportUnimplemented(const bool n)
        { mReportUnimplemented = n; }

        /**
         * Log an error and quit. The error will pop-up on Windows and Mac, and
         * will be printed to standard error everywhere else.
         */
        void error(const std::string &error_text)
#ifndef BAD_CILKPLUS
        __attribute__ ((noreturn))
#endif  // BAD_CILKPLUS
        ;

        /**
         * Log an error and quit. The error will pop-up on Windows and Mac, and
         * will be printed to standard error everywhere else.
         */
        void safeError(const std::string &error_text)
#ifndef BAD_CILKPLUS
        __attribute__ ((noreturn))
#endif  // BAD_CILKPLUS
        ;

        void unimplemented(const int id);

        void unimplemented(const int id,
                           const int id2);

        void unimplemented(const uint32_t id,
                           const uint32_t id2,
                           const uint32_t id3) const;

        FILE *getFile() const;

    private:
        FILE *mLogFile;
        STD_VECTOR<std::string> mDelayedLog;
        SDL_mutex *mMutex;
        volatile bool mThreadLocked;
        bool mLogToStandardOut;
        bool mDebugLog;
        bool mReportUnimplemented;
};

extern Logger *logger;

#endif  // LOGGER_H
