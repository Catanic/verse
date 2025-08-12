/*
 *  The ManaVerse Client
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

#include "utils/process.h"

#include <unistd.h>

#include "localconsts.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifdef USE_SDL2
#ifdef ANDROID
#include <SDL_system.h>
#endif  // ANDROID
#endif  // USE_SDL2
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

#if defined(_WIN32) || defined(__linux__) || \
    defined(__linux) || defined(__APPLE__)
const int timeOut = 10;
#endif  // defined(_WIN32) || defined(__linux__) ||
        // defined(__linux) || defined(__APPLE__)

#ifdef _WIN32

#include "utils/stringutils.h"

#include <windows.h>

int execFileWait(const std::string &pathName, const std::string &name A_UNUSED,
                 const std::string &arg1, const std::string &arg2,
                 const int waitTime A_UNUSED)
{
//    if (!waitTime)
//        waitTime = timeOut;

    STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));
    siStartupInfo.cb = sizeof(siStartupInfo);
    DWORD ret = -1;
    std::string args(std::string(pathName).append(" ").append(arg1));
    if (!arg2.empty())
        args.append(" ").append(arg2);

    if (CreateProcess(pathName.c_str(), const_cast<char*>(args.c_str()),
        nullptr, nullptr, false, CREATE_DEFAULT_ERROR_MODE, nullptr, nullptr,
        &siStartupInfo, &piProcessInfo) != false)
    {
        if (!WaitForSingleObject(piProcessInfo.hProcess, timeOut * 1000))
        {
            if (GetExitCodeProcess(piProcessInfo.hProcess, &ret))
            {
                CloseHandle(piProcessInfo.hProcess);
                CloseHandle(piProcessInfo.hThread);
                return ret;
            }
        }
        TerminateProcess(piProcessInfo.hProcess, -1);
    }

    CloseHandle(piProcessInfo.hProcess);
    CloseHandle(piProcessInfo.hThread);
    return -1;
}

bool execFile(const std::string &pathName, const std::string &name A_UNUSED,
              const std::string &arg1, const std::string &arg2)
{
    STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));
    siStartupInfo.cb = sizeof(siStartupInfo);
    std::string args(std::string(pathName).append(" ").append(arg1));
    if (!arg2.empty())
        args.append(" ").append(arg2);

    bool res = CreateProcess(pathName.c_str(), const_cast<char*>(
        args.c_str()), nullptr, nullptr, false,
        CREATE_DEFAULT_ERROR_MODE, nullptr, nullptr, &siStartupInfo,
        &piProcessInfo);

    CloseHandle(piProcessInfo.hProcess);
    CloseHandle(piProcessInfo.hThread);
    return res;
}


#elif defined __linux__ || defined __linux || defined __APPLE__

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <csignal>

int execFileWait(const std::string &pathName, const std::string &name,
                 const std::string &arg1, const std::string &arg2,
                 int waitTime)
{
    pid_t mon_pid;
    int status;

    if (waitTime == 0)
        waitTime = timeOut;

    if ((mon_pid = fork()) == -1)
    {   // fork error
        return -1;
    }
    else if (mon_pid == 0)
    {   // monitoring child
        pid_t pid;
        if ((pid = fork()) == -1)
        {   // fork error
            return -1;
        }
        else if (pid == 0)
        {   // work child
            if (arg2.empty())
            {
                execl(pathName.c_str(), name.c_str(),
                   arg1.c_str(), static_cast<char *>(nullptr));
            }
            else
            {
                execl(pathName.c_str(), name.c_str(), arg1.c_str(),
                    arg2.c_str(), static_cast<char *>(nullptr));
            }
            _exit(-1);
        }

        // monitoring process
        pid_t sleep_pid;
        if ((sleep_pid = fork()) == -1)
        {   // fork error
            return -1;
        }
        else if (sleep_pid == 0)
        {   // sleep pid
            sleep(waitTime);
            execl("/bin/true", "/bin/true", static_cast<char *>(nullptr));
            _exit(-1);
        }

        // monitoring process
        const pid_t exited_pid = wait(&status);
        int ret = -1;
        if (exited_pid == pid)
        {
            kill(sleep_pid, SIGKILL);
            if (WIFEXITED(status))
                ret = WEXITSTATUS(status);
        }
        else
        {
            kill(pid, SIGKILL);
            ret = -1;
        }
        wait(nullptr);
        execl("/bin/true", "/bin/true", static_cast<char *>(nullptr));
        _exit(ret);
    }

    // monitoring parent
    waitpid(mon_pid, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return -1;
}

bool execFile(const std::string &pathName, const std::string &name,
              const std::string &arg1, const std::string &arg2)
{
    struct stat statbuf;
    // file not exists
    if (stat(pathName.c_str(), &statbuf) != 0)
        return false;

    pid_t pid;
    if ((pid = fork()) == -1)
    {   // fork error
        return false;
    }
    else if (pid == 0)
    {   // work child
        if (arg2.empty())
        {
            execl(pathName.c_str(), name.c_str(),
                arg1.c_str(), static_cast<char *>(nullptr));
        }
        else
        {
            execl(pathName.c_str(), name.c_str(), arg1.c_str(),
                arg2.c_str(), static_cast<char *>(nullptr));
        }
        _exit(-1);
        PRAGMACLANG6(GCC diagnostic push)
        PRAGMACLANG6(GCC diagnostic ignored "-Wunreachable-code-return")
        return false;
        PRAGMACLANG6(GCC diagnostic pop)
    }
    return true;
}

#else  // OTHER

int execFileWait(const std::string &pathName A_UNUSED,
                 const std::string &name A_UNUSED,
                 const std::string &arg1 A_UNUSED,
                 const std::string &arg2 A_UNUSED,
                 int waitTime A_UNUSED)
{
    return -1;
}

bool execFile(const std::string &pathName A_UNUSED,
              const std::string &name A_UNUSED,
              const std::string &arg1 A_UNUSED,
              const std::string &arg2 A_UNUSED)
{
    return false;
}

#endif  // _WIN32

#if defined _WIN64
bool openBrowser(std::string url)
{
    return reinterpret_cast<int64_t>(ShellExecute(nullptr, "open",
        replaceAll(url, " ", "").c_str(),
        nullptr, nullptr, SW_SHOWNORMAL)) > 32;
}
#elif defined _WIN32
bool openBrowser(std::string url)
{
    return reinterpret_cast<int32_t>(ShellExecute(nullptr, "open",
        replaceAll(url, " ", "").c_str(),
        nullptr, nullptr, SW_SHOWNORMAL)) > 32;
}
#elif defined ANDROID
#include "utils/stringutils.h"
#ifndef USE_SDL2
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_screenkeyboard.h>
PRAGMA48(GCC diagnostic pop)
#endif  // USE_SDL2

bool openBrowser(std::string url)
{
#ifdef USE_SDL2
    SDL_OpenBrowser(replaceAll(url, " ", "").c_str());
#else  // USE_SDL2

    SDL_ANDROID_OpenBrowser(replaceAll(url, " ", "").c_str());
#endif  // USE_SDL2

    return true;
}
#elif defined __APPLE__
bool openBrowser(std::string url)
{
    return execFile("/usr/bin/open", "/usr/bin/open", url, "");
}
#elif defined __OpenBSD__ || defined __FreeBSD__ || defined __DragonFly__
bool openBrowser(std::string url)
{
    return execFile("/usr/local/bin/xdg-open",
        "/usr/local/bin/xdg-open", url, "");
}
#elif defined __linux__ || defined __linux
bool openBrowser(std::string url)
{
    return execFile("/usr/bin/xdg-open", "/usr/bin/xdg-open", url, "");
}
#elif defined __native_client__

#include "utils/naclmessages.h"

bool openBrowser(std::string url)
{
    naclPostMessage("open-browser", url);
    return true;
}
#else  // OTHER
bool openBrowser(std::string url A_UNUSED)
{
    return false;
}

#endif  // _WIN32

#ifdef _WIN32
void setPriority(const bool big)
{
    HANDLE hCurrentProcess = GetCurrentProcess();
    if (big)
        SetPriorityClass(hCurrentProcess, ABOVE_NORMAL_PRIORITY_CLASS);
    else
        SetPriorityClass(hCurrentProcess, BELOW_NORMAL_PRIORITY_CLASS);
}
#else  // _WIN32

void setPriority(const bool big A_UNUSED)
{
}
#endif  // _WIN32
