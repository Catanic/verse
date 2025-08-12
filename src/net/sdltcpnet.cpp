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

#if defined __linux__ || defined __linux

#include <sys/socket.h>

#if defined(M_TCPOK) && !defined(ANDROID)
#include <netinet/in.h>
#include <netdb.h>
#include <linux/tcp.h>
#else  // defined(M_TCPOK) && !defined(ANDROID)
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <netinet/tcp.h>
// Use linear timeouts for thin streams
#define TCP_THIN_LINEAR_TIMEOUTS 16
// Fast retrans. after 1 dupack
#define TCP_THIN_DUPACK          17
#endif  // defined(M_TCPOK) && !defined(ANDROID)

#endif  // defined __linux__ || defined __linux

PRAGMACLANG6GCC(GCC diagnostic push)
PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
#include "net/sdltcpnet.h"
PRAGMACLANG6GCC(GCC diagnostic pop)

#include "debug.h"

#if !defined(__native_client__) \
    && (defined(TCP_THIN_LINEAR_TIMEOUTS) \
    || defined(TCP_THIN_DUPACK))
// because actual struct is hidden in SDL_net we reinroducing it here
struct TCPsocketHack final
{
    TCPsocketHack() :
        ready(0),
        channel(0),
        remoteAddress(),
        localAddress(),
        sflag()
    { }

    A_DELETE_COPY(TCPsocketHack)

    int ready;
    int channel;
    IPaddress remoteAddress;
    IPaddress localAddress;
    int sflag;
};
#endif  // !defined(__native_client__)
        // && (defined(TCP_THIN_LINEAR_TIMEOUTS)
        // || defined(TCP_THIN_DUPACK))

void TcpNet::init()
{
    SDLNet_Init();
}

void TcpNet::quit()
{
    SDLNet_Quit();
}

void TcpNet::closeSocket(const TcpNet::Socket socket)
{
    SDLNet_TCP_Close(socket);
}

int TcpNet::send(const TcpNet::Socket sock, const void *const data,
                 const int len)
{
    return SDLNet_TCP_Send(sock, data, len);
}

const char *TcpNet::getError()
{
    return SDL_GetError();
}

int TcpNet::resolveHost(IPaddress *const address, const char *const host,
                        const Uint16 port)
{
    return SDLNet_ResolveHost(address, host, port);
}

TcpNet::Socket TcpNet::open(IPaddress *const ip)
{
    const TcpNet::Socket sock = SDLNet_TCP_Open(ip);
#if !defined(__native_client__) \
    && (defined(TCP_THIN_LINEAR_TIMEOUTS) \
    || defined(TCP_THIN_DUPACK))
    if ((sock != nullptr) && (ip != nullptr))
    {
        const TCPsocketHack *const hack
            = reinterpret_cast<const TCPsocketHack *>(sock);
        // here we using some magic to compare TCPsocket and own padding
        // because actual struct TCPsocket not in headers
        if (hack != nullptr)
        {
            const IPaddress &addr = hack->remoteAddress;
            if (addr.host == ip->host && addr.port == ip->port)
            {
                const int val = 1;
#ifdef TCP_THIN_LINEAR_TIMEOUTS
                if (setsockopt(hack->channel, IPPROTO_TCP,
                    TCP_THIN_LINEAR_TIMEOUTS, &val, sizeof(val)) != 0)
                {
                    logger->log_r("error on set TCP_THIN_LINEAR_TIMEOUTS");
                }
#endif  // TCP_THIN_LINEAR_TIMEOUTS
#ifdef TCP_THIN_DUPACK
                if (setsockopt(hack->channel, IPPROTO_TCP,
                    TCP_THIN_DUPACK, &val, sizeof(val)) != 0)
                {
                    logger->log_r("error on set TCP_THIN_DUPACK");
                }
#endif  // TCP_THIN_DUPACK
            }
        }
    }
#endif  // !defined(__native_client__)
        // && (defined(TCP_THIN_LINEAR_TIMEOUTS)
        // || defined(TCP_THIN_DUPACK))
    return sock;
}

TcpNet::SocketSet TcpNet::allocSocketSet(const int maxsockets)
{
    return SDLNet_AllocSocketSet(maxsockets);
}

int TcpNet::addSocket(const TcpNet::SocketSet set, const TcpNet::Socket sock)
{
    PRAGMACLANG6GCC(GCC diagnostic push)
    PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
    return SDLNet_TCP_AddSocket(set, sock);
    PRAGMACLANG6GCC(GCC diagnostic pop)
}

int TcpNet::socketReady(const TcpNet::Socket sock)
{
    PRAGMACLANG5(GCC diagnostic push)
    PRAGMACLANG5(GCC diagnostic ignored "-Wzero-as-null-pointer-constant")
    PRAGMACLANG6GCC(GCC diagnostic push)
    PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
    return SDLNet_SocketReady(sock);
    PRAGMACLANG6GCC(GCC diagnostic pop)
    PRAGMACLANG5(GCC diagnostic pop)
}

int TcpNet::checkSockets(const TcpNet::SocketSet set, const Uint32 timeout)
{
    return SDLNet_CheckSockets(set, timeout);
}

int TcpNet::recv(const TcpNet::Socket sock, void *const data, const int maxlen)
{
    return SDLNet_TCP_Recv(sock, data, maxlen);
}

int TcpNet::delSocket(const TcpNet::SocketSet set, const TcpNet::Socket sock)
{
    PRAGMACLANG6GCC(GCC diagnostic push)
    PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
    return SDLNet_TCP_DelSocket(set, sock);
    PRAGMACLANG6GCC(GCC diagnostic pop)
}

void TcpNet::freeSocketSet(const TcpNet::SocketSet set)
{
    SDLNet_FreeSocketSet(set);
}

TcpNet::Socket TcpNet::accept(const TcpNet::Socket sock)
{
    return SDLNet_TCP_Accept(sock);
}
