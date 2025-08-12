/*
 *  The ManaVerse Client
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

#include "net/download.h"

#include "configuration.h"
#include "logger.h"
#include "settings.h"

#include "fs/files.h"

#include "utils/cast.h"
#include "utils/performance.h"
#include "utils/sdlhelper.h"

#include <zlib.h>

#include <fstream>
#include <sstream>

#include "debug.h"

const char *DOWNLOAD_ERROR_MESSAGE_THREAD
    = "Could not create download thread!";

extern volatile bool isTerminate;

enum
{
    OPTIONS_NONE = 0,
    OPTIONS_MEMORY = 1
};

namespace Net
{

std::string Download::mUploadResponse;

Download::Download(void *const ptr,
                   const std::string &url,
                   const DownloadUpdate updateFunction,
                   const bool ignoreError,
                   const bool isUpload,
                   const bool isXml) :
    mPtr(ptr),
    mUrl(url),
    mOptions(),
    mFileName(),
    mUrlQueue(),
    mWriteFunction(nullptr),
    mAdler(0),
    mUpdateFunction(updateFunction),
    mThread(nullptr),
    mCurl(nullptr),
    mHeaders(nullptr),
#if LIBCURL_VERSION_NUM < 0x073800
    mFormPost(nullptr),
#else
    mMime(nullptr),
#endif  // LIBCURL_VERSION_NUM < 0x073800
    mError(static_cast<char*>(calloc(CURL_ERROR_SIZE + 1, 1))),
    mIgnoreError(ignoreError),
    mUpload(isUpload),
    mIsXml(isXml)
{
    if (mError != nullptr)
        mError[0] = 0;

    mOptions.cancel = 0;
    mOptions.memoryWrite = 0;
    mOptions.checkAdler = 1U;
    if (!mUpload)
    {
        const std::string serverName = settings.serverName;
        if (!serverName.empty())
        {
            if (mUrl.find('?') == std::string::npos)
                mUrl.append("?host=");
            else
                mUrl.append("&host=");
            mUrl.append(serverName);
        }
    }
    mUrlQueue.push(url);
}

Download::~Download()
{
#if LIBCURL_VERSION_NUM < 0x073800
    if (mFormPost != nullptr)
    {
        curl_formfree(mFormPost);
        mFormPost = nullptr;
    }
#else
    if (mMime != nullptr)
    {
        curl_mime_free(mMime);
        mMime = nullptr;
    }
#endif  // LIBCURL_VERSION_NUM < 0x073800


    if (mHeaders != nullptr)
    {
        curl_slist_free_all(mHeaders);
        mHeaders = nullptr;
    }

    SDL::WaitThread(mThread);
    mThread = nullptr;
    free(mError);
}

/**
 * Calculates the Alder-32 checksum for the given file.
 */
unsigned long Download::fadler32(FILE *const file)
{
    if (file == nullptr)
        return 0;

    // Obtain file size
    fseek(file, 0, SEEK_END);
    const long fileSize = ftell(file);
    if (fileSize < 0)
    {   // file size error
        return 0;
    }
    rewind(file);

    // Calculate Adler-32 checksum
    char *const buffer = new char[CAST_SIZE(fileSize)];
    const uInt read = static_cast<uInt>(fread(buffer, 1, fileSize, file));
    unsigned long adler = adler32(0L, nullptr, 0);
    adler = adler32(static_cast<uInt>(adler),
        reinterpret_cast<Bytef*>(buffer), read);
    delete [] buffer;
    return adler;
}

unsigned long Download::adlerBuffer(const char *const buffer, int size)
{
    FUNC_BLOCK("Download::adlerBuffer", 1)
    unsigned long adler = adler32(0L, nullptr, 0);
    return adler32(static_cast<uInt>(adler),
        reinterpret_cast<const Bytef*>(buffer), size);
}

void Download::addHeader(const std::string &header)
{
    mHeaders = curl_slist_append(mHeaders, header.c_str());
}

void Download::noCache()
{
    addHeader("pragma: no-cache");
    addHeader("Cache-Control: no-cache");
}

void Download::setFile(const std::string &filename, const int64_t adler32)
{
    mOptions.memoryWrite = 0;
    mFileName = filename;

    if (adler32 > -1)
    {
        mAdler = static_cast<unsigned long>(adler32);
        mOptions.checkAdler = 1U;
    }
    else
    {
        mOptions.checkAdler = 0;
    }
}

void Download::setWriteFunction(WriteFunction write)
{
    mOptions.memoryWrite = 1U;
    mWriteFunction = write;
}

bool Download::start()
{
    if (mUpload)
        logger->log("Starting upload: %s", mUrl.c_str());
    else
        logger->log("Starting download: %s", mUrl.c_str());

    mThread = SDL::createThread(&downloadThread, "download", this);
    if (mThread == nullptr)
    {
        logger->log1(DOWNLOAD_ERROR_MESSAGE_THREAD);
        if (mError != nullptr)
            strcpy(mError, DOWNLOAD_ERROR_MESSAGE_THREAD);
        mUpdateFunction(mPtr, DownloadStatus::ThreadError, 0, 0);
        if (!mIgnoreError)
            return false;
    }

    return true;
}

void Download::cancel()
{
    logger->log("Canceling download: %s", mUrl.c_str());

    mOptions.cancel = 1U;
    SDL::WaitThread(mThread);
    mThread = nullptr;
}

const char *Download::getError() const
{
    return mError;
}

int Download::downloadProgress(void *clientp, double dltotal, double dlnow,
                               double ultotal A_UNUSED, double ulnow A_UNUSED)
{
    Download *const d = reinterpret_cast<Download *>(clientp);

    if (d == nullptr)
        return -5;

    if (d->mUpload)
        return 0;

    if (d->mOptions.cancel != 0U)
    {
        return d->mUpdateFunction(d->mPtr, DownloadStatus::Cancelled,
                                  CAST_SIZE(dltotal),
                                  CAST_SIZE(dlnow));
    }

    return d->mUpdateFunction(d->mPtr, DownloadStatus::Idle,
                              CAST_SIZE(dltotal),
                              CAST_SIZE(dlnow));
}

int Download::downloadThread(void *ptr)
{
    int attempts = 0;
    bool complete = false;
    Download *const d = reinterpret_cast<Download*>(ptr);
    CURLcode res;

    if (d == nullptr)
        return 0;

    std::string outFilename;
    if (d->mUpload)
    {
        outFilename = d->mFileName;
        prepareForm(d, outFilename);
    }
    else
    {
        if (d->mOptions.memoryWrite == 0U)
            outFilename = d->mFileName + ".part";
        else
            outFilename.clear();
    }

    while (!d->mUrlQueue.empty())
    {
        attempts = 0;
        complete = false;
        d->mUrl = d->mUrlQueue.front();
        d->mUrlQueue.pop();

        logger->log_r("selected url: %s", d->mUrl.c_str());
        while (attempts < 3 &&
               !complete &&
               (d->mOptions.cancel == 0U) &&
               isTerminate == false)
        {
            d->mUpdateFunction(d->mPtr, DownloadStatus::Starting, 0, 0);

            if ((d->mOptions.cancel != 0U) || isTerminate == true)
            {
                return 0;
            }
            d->mCurl = curl_easy_init();

            if (d->mCurl != nullptr &&
                d->mOptions.cancel == 0U &&
                isTerminate == false)
            {
                FILE *file = nullptr;

                if (d->mUpload)
                {
                    logger->log_r("Uploading: %s", d->mUrl.c_str());
                    curl_easy_setopt(d->mCurl, CURLOPT_URL, d->mUrl.c_str());
#if LIBCURL_VERSION_NUM < 0x073800
                    curl_easy_setopt(d->mCurl, CURLOPT_HTTPPOST, d->mFormPost);
#else
                    curl_easy_setopt(d->mCurl, CURLOPT_MIMEPOST, d->mMime);
#endif  // LIBCURL_VERSION_NUM < 0x073800
                    curl_easy_setopt(d->mCurl, CURLOPT_WRITEFUNCTION,
                        &Download::writeFunction);
                    mUploadResponse.clear();
                }
                else
                {
                    logger->log_r("Downloading: %s", d->mUrl.c_str());
                    curl_easy_setopt(d->mCurl, CURLOPT_FOLLOWLOCATION, 1);
                    curl_easy_setopt(d->mCurl, CURLOPT_HTTPHEADER,
                        d->mHeaders);
                    // ignore SSL verificytion, windows' OpenSSL is very limited
                    if (strstr(d->mUrl.c_str(), "https://") != NULL)
                    {
                        curl_easy_setopt(d->mCurl, CURLOPT_SSL_VERIFYPEER, 0L);
                    }
                    if (d->mOptions.memoryWrite != 0U)
                    {
                        curl_easy_setopt(d->mCurl, CURLOPT_FAILONERROR, 1);
                        curl_easy_setopt(d->mCurl, CURLOPT_WRITEFUNCTION,
                                         d->mWriteFunction);
                        curl_easy_setopt(d->mCurl, CURLOPT_WRITEDATA, d->mPtr);
                    }
                    else
                    {
                        file = fopen(outFilename.c_str(), "w+b");
                        if (file != nullptr)
                        {
                            curl_easy_setopt(d->mCurl, CURLOPT_WRITEDATA,
                                file);
                        }
                    }
                    curl_easy_setopt(d->mCurl,
                        CURLOPT_USERAGENT,
                        settings.userAgent.c_str());

                    curl_easy_setopt(d->mCurl, CURLOPT_ERRORBUFFER, d->mError);
                    curl_easy_setopt(d->mCurl, CURLOPT_URL, d->mUrl.c_str());
                    curl_easy_setopt(d->mCurl, CURLOPT_NOPROGRESS, 0);
#if LIBCURL_VERSION_NUM < 0x072000
                    curl_easy_setopt(d->mCurl, CURLOPT_PROGRESSFUNCTION,
                                            &downloadProgress);
#else
                    curl_easy_setopt(d->mCurl, CURLOPT_XFERINFOFUNCTION,
                                            &downloadProgress);

#endif  // LIBCURL_VERSION_NUM < 0x072000
                    curl_easy_setopt(d->mCurl, CURLOPT_PROGRESSDATA, ptr);
#if LIBCURL_VERSION_NUM >= 0x070a00
                    curl_easy_setopt(d->mCurl, CURLOPT_NOSIGNAL, 1);
#endif  // LIBCURL_VERSION_NUM >= 0x070a00
                    curl_easy_setopt(d->mCurl, CURLOPT_CONNECTTIMEOUT, 30);
                    curl_easy_setopt(d->mCurl, CURLOPT_TIMEOUT, 1800);
                    addHeaders(d->mCurl);
                    addProxy(d->mCurl);
                    secureCurl(d->mCurl);
                }
                addCommonFlags(d->mCurl);

                if ((res = curl_easy_perform(d->mCurl)) != 0 &&
                    (d->mOptions.cancel == 0U) &&
                    isTerminate == false)
                {
                    PRAGMA45(GCC diagnostic push)
                    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
                    switch (res)
                    {
                        case CURLE_ABORTED_BY_CALLBACK:
                            d->mOptions.cancel = 1U;
                            break;
                        case CURLE_COULDNT_CONNECT:
                        default:
                            break;
                    }
                    PRAGMA45(GCC diagnostic pop)

                    if (res != 0U)
                    {
                        if (d->mError != nullptr)
                        {
                            logger->log_r("curl error %d: %s host: %s",
                                res, d->mError, d->mUrl.c_str());
                        }
                        attempts++;
                        continue;
                    }

                    if ((d->mOptions.cancel != 0U) || isTerminate == true)
                        break;

//                    d->mUpdateFunction(d->mPtr, DownloadStatus::Error, 0, 0);

                    if (file != nullptr)
                    {
                        fclose(file);
                        file = nullptr;
                    }
                    if (!d->mUpload && (d->mOptions.memoryWrite == 0U))
                        ::remove(outFilename.c_str());
                    attempts++;
                    continue;
                }

                curl_easy_cleanup(d->mCurl);
                d->mCurl = nullptr;

                if (d->mUpload)
                {
                    if (file != nullptr)
                    {
                        fclose(file);
                        file = nullptr;
                    }
                    // need check first if we read data from server
                    complete = true;
                }
                else
                {
                    if (d->mOptions.memoryWrite == 0U)
                    {
                        // Don't check resources.xml checksum
                        if (d->mOptions.checkAdler != 0U)
                        {
                            const unsigned long adler = fadler32(file);

                            if (d->mAdler != adler)
                            {
                                if (file != nullptr)
                                {
                                    fclose(file);
                                    file = nullptr;
                                }

                                // Remove the corrupted file
                                ::remove(d->mFileName.c_str());
                                logger->log_r("Checksum for file %s failed:"
                                    " (%lx/%lx)",
                                    d->mFileName.c_str(),
                                    adler, d->mAdler);
                                attempts++;
                                continue;
                            }
                        }

                        if (file != nullptr)
                        {
                            fclose(file);
                            file = nullptr;
                        }

                        // Any existing file with this name is deleted first,
                        // otherwise the rename will fail on Windows.
                        if ((d->mOptions.cancel == 0U) && isTerminate == false)
                        {
                            if (d->mIsXml)
                            {
                                if (!XML::Document::validateXml(outFilename))
                                {
                                    logger->log_r("Xml file validation error");
                                    attempts++;
                                    continue;
                                }
                            }

                            ::remove(d->mFileName.c_str());
                            Files::renameFile(outFilename, d->mFileName);

                            // Check if we can open it and no errors were
                            // encountered during renaming
                            file = fopen(d->mFileName.c_str(), "rb");
                            if (file != nullptr)
                            {
                                fclose(file);
                                file = nullptr;
                                complete = true;
                            }
                        }
                    }
                    else
                    {
                        // It's stored in memory, we're done
                        complete = true;
                    }
                }
            }

            if (d->mCurl != nullptr)
            {
                curl_easy_cleanup(d->mCurl);
                d->mCurl = nullptr;
            }

            if ((d->mOptions.cancel != 0U) || isTerminate == true)
            {
                return 0;
            }
            attempts++;
        }

        if ((complete && attempts < 3) || (d->mOptions.cancel != 0U))
            break;
    }

    if ((d->mOptions.cancel != 0U) || isTerminate == true)
    {
        // Nothing to do...
    }
    else if (!complete || attempts >= 3)
    {
        d->mUpdateFunction(d->mPtr, DownloadStatus::Error, 0, 0);
    }
    else
    {
        d->mUpdateFunction(d->mPtr, DownloadStatus::Complete, 0, 0);
    }

    return 0;
}

void Download::addProxy(CURL *const curl)
{
    const int mode = config.getIntValue("downloadProxyType");
    if (mode == 0)
        return;

    if (mode > 1)
    {
        curl_easy_setopt(curl, CURLOPT_PROXY,
            config.getStringValue("downloadProxy").c_str());
    }

    switch (mode)
    {
        case 1:  // direct connection
        default:
            curl_easy_setopt(curl, CURLOPT_PROXY, "");
            break;
        case 2:  // HTTP
#if LIBCURL_VERSION_NUM >= 0x070300
            curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL,
                config.getIntValue("downloadProxyTunnel"));
#endif  // LIBCURL_VERSION_NUM >= 0x070300
            break;
        case 3:  // HTTP 1.0
#if LIBCURL_VERSION_NUM >= 0x071304
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP_1_0);
#endif  // LIBCURL_VERSION_NUM >= 0x071304
#if LIBCURL_VERSION_NUM >= 0x070300
            curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL,
                config.getIntValue("downloadProxyTunnel"));
#endif  // LIBCURL_VERSION_NUM >= 0x070300
            break;
        case 4:  // SOCKS4
#if LIBCURL_VERSION_NUM >= 0x070a00
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
#endif  // LIBCURL_VERSION_NUM >= 0x070a00
            break;
        case 5:  // SOCKS4A
#if LIBCURL_VERSION_NUM >= 0x071200
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4A);
#elif LIBCURL_VERSION_NUM >= 0x071000
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
#endif  // LIBCURL_VERSION_NUM >= 0x071200

            break;
        case 6:  // SOCKS5
#if LIBCURL_VERSION_NUM >= 0x071200
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
#endif  // LIBCURL_VERSION_NUM >= 0x071200

            break;
        case 7:  // SOCKS5 hostname
#if LIBCURL_VERSION_NUM >= 0x071200
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE,
                CURLPROXY_SOCKS5_HOSTNAME);
#endif  // LIBCURL_VERSION_NUM >= 0x071200

            break;
    }
}

#if LIBCURL_VERSION_NUM >= 0x070a08
void Download::secureCurl(CURL *const curl)
#else  // LIBCURL_VERSION_NUM >= 0x070f01
void Download::secureCurl(CURL *const curl A_UNUSED)
#endif  // LIBCURL_VERSION_NUM >= 0x070f01
{
#if LIBCURL_VERSION_NUM >= 0x071304 && LIBCURL_VERSION_NUM <= 0x075500
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS,
        CURLPROTO_HTTP | CURLPROTO_HTTPS);
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS,
        CURLPROTO_HTTP | CURLPROTO_HTTPS);
#endif  // LIBCURL_VERSION_NUM >= 0x071304 && LIBCURL_VERSION_NUM <= 0x075500
#if LIBCURL_VERSION_NUM >= 0x075500
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR,
        "http,https");
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS_STR,
        "http,https");
#endif  // LIBCURL_VERSION_NUM >= 0x075500
#if LIBCURL_VERSION_NUM >= 0x071500
    curl_easy_setopt(curl, CURLOPT_WILDCARDMATCH, 0);
#endif  // LIBCURL_VERSION_NUM >= 0x071500
#if LIBCURL_VERSION_NUM >= 0x070f01
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3);
#endif  // LIBCURL_VERSION_NUM >= 0x070f01
#if LIBCURL_VERSION_NUM >= 0x070a08
    curl_easy_setopt(curl, CURLOPT_MAXFILESIZE, 536870912);
#endif  // LIBCURL_VERSION_NUM >= 0x070a08

#if LIBCURL_VERSION_NUM >= 0x073100
    curl_easy_setopt(curl, CURLOPT_TCP_FASTOPEN, 1L);
#endif  // LIBCURL_VERSION_NUM >= 0x073100

    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 60L);
}

#if LIBCURL_VERSION_NUM >= 0x071507
void Download::addHeaders(CURL *const curl)
{
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
}
#else  // LIBCURL_VERSION_NUM >= 0x071507

void Download::addHeaders(CURL *const curl A_UNUSED)
{
}
#endif  // LIBCURL_VERSION_NUM >= 0x071507

void Download::addCommonFlags(CURL *const curl)
{
    curl_easy_setopt(curl, CURLOPT_STDERR, logger->getFile());
#if LIBCURL_VERSION_NUM >= 0x072D00
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "http");
#endif  // LIBCURL_VERSION_NUM >= 0x072D00
}

void Download::prepareForm(Download *const d, const std::string &fileName)
{
    std::ifstream file;
    file.open(fileName.c_str(), std::ios::in);
    if (!file.is_open())
        return;

    char *line = new char[10001];
    std::ostringstream str;
    while (file.getline(line, 10000))
        str << line << "\n";

    delete [] line;

#if LIBCURL_VERSION_NUM < 0x073800
    curl_httppost *lastPtr = nullptr;
    curl_formadd(&d->mFormPost, &lastPtr,
        CURLFORM_COPYNAME, "f:1",
        CURLFORM_COPYCONTENTS, str.str().c_str(),
        CURLFORM_END);
#else
    curl_mimepart *part = curl_mime_addpart(d->mMime);
    curl_mime_init(d->mCurl);
    curl_mime_name(part, "f:1");
    curl_mime_data(part, str.str().c_str(), str.str().length());
#endif
}

size_t Download::writeFunction(void *ptr,
                               size_t size,
                               size_t nmemb,
                               void *stream A_UNUSED)
{
    const size_t totalMem = size * nmemb;
    char *buf = new char[totalMem + 1];
    memcpy(buf, ptr, totalMem);
    buf[totalMem] = 0;
    mUploadResponse.append(buf);
    delete [] buf;
    return totalMem;
}

}  // namespace Net
