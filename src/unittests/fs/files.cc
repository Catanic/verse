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

#include "unittests/unittests.h"


#include "fs/files.h"

#include "fs/virtfs/fs.h"
#include "fs/virtfs/tools.h"

#include "utils/stringutils.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

TEST_CASE("Files renameFile", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    const int sz = 1234567;
    char *buf = new char[sz];
    for (int f = 0; f < sz; f ++)
        buf[f] = f;

    const std::string name1 = "file1.test";
    const std::string name2 = "file2.test";
    FILE *file = fopen(name1.c_str(), "w+b");
    fwrite(buf, 1, sz, file);
    fclose(file);

    REQUIRE(0 == Files::renameFile(name1, name2));
    char *buf2 = new char[sz];
    FILE *file2 = fopen(name2.c_str(), "rb");
    REQUIRE_FALSE(nullptr == file2);
    fread(buf2, 1, sz, file2);
    fclose(file2);
    ::remove(name1.c_str());
    ::remove(name2.c_str());

    for (int f = 0; f < sz; f ++)
        REQUIRE(buf[f] == buf2[f]);

    delete [] buf;
    delete [] buf2;
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("Files existsLocal", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    REQUIRE(Files::existsLocal(VirtFs::getPath("help/about.txt")) == true);
    REQUIRE_FALSE(Files::existsLocal(VirtFs::getPath("help/about1.txt")));
    REQUIRE_FALSE(Files::existsLocal(VirtFs::getPath("help1/about.txt")));
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("Files loadTextFileString", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    REQUIRE(VirtFs::loadTextFileString("test/simplefile.txt") ==
        "this is test \nfile.");
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("Files loadTextFile", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    StringVect lines;
    VirtFs::loadTextFile("test/simplefile.txt", lines);
    REQUIRE(lines.size() == 2);
    REQUIRE(lines[0] == "this is test ");
    REQUIRE(lines[1] == "file.");
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("Files saveTextFile", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    const std::string dir = VirtFs::getPath("test");
    REQUIRE(!dir.empty());
    Files::saveTextFile(dir, "tempfile.txt", "test line\ntext line2");
    std::string data = VirtFs::loadTextFileString("test/tempfile.txt");
    ::remove((dir + "/tempfile.txt").c_str());
#ifdef _WIN32
    REQUIRE(data == "test line\r\ntext line2\r\n");
#else  // _WIN32

    REQUIRE(data == "test line\ntext line2\n");
#endif  // _WIN32

    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("Files copyFile1", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    const std::string dir = VirtFs::getPath("test");
    REQUIRE(!dir.empty());
    SECTION("copy")
    {
        REQUIRE(Files::copyFile(pathJoin(dir, "test.txt"),
            pathJoin(dir, "tempfile.txt")) == 0);
        std::string data = VirtFs::loadTextFileString("test/tempfile.txt");
        ::remove((dir + "/tempfile.txt").c_str());
        REQUIRE(data == "test line 1\ntest line 2");
    }

    SECTION("errors")
    {
        REQUIRE(Files::copyFile(pathJoin(dir, "test_not_exists.txt"),
            pathJoin(dir, "tempfile.txt")) == -1);
        REQUIRE(Files::copyFile(pathJoin(dir, "test.txt"),
            "/nonexist/root/dir123") == -1);
    }

    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("Files loadTextFileLocal", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    const std::string dir = VirtFs::getPath("test");
    REQUIRE(!dir.empty());
    Files::saveTextFile(dir, "tempfile.txt", "test line\ntext line2");
    StringVect lines;
    REQUIRE(Files::loadTextFileLocal(pathJoin(dir, "tempfile.txt"),
        lines));
    ::remove((dir + "/tempfile.txt").c_str());
    REQUIRE(lines.size() == 2);
    REQUIRE(lines[0] == "test line");
    REQUIRE(lines[1] == "text line2");

    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("Files getFilesInDir", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    StringVect list;
    VirtFs::getFilesInDir("test",
        list,
        ".gpl");
    REQUIRE(list.size() == 1);
    REQUIRE(list[0] == pathJoin("test", "palette.gpl"));

    list.clear();
    VirtFs::getFilesInDir("perserver/default",
        list,
        ".xml");
    REQUIRE(list.size() == 6);
    REQUIRE(list[0] == pathJoin("perserver", "default", "charcreation.xml"));
    REQUIRE(list[1] == pathJoin("perserver", "default", "deadmessages.xml"));
    REQUIRE(list[2] ==
        pathJoin("perserver", "default", "defaultcommands.xml"));
    REQUIRE(list[3] == pathJoin("perserver", "default", "features.xml"));
    REQUIRE(list[4] == pathJoin("perserver", "default", "groups.xml"));
    REQUIRE(list[5] == pathJoin("perserver", "default", "weapons.xml"));
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}
