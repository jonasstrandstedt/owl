/*****************************************************************************************
 *                                                                                       *
 * owl                                                                                   *
 *                                                                                       *
 * Copyright (c) 2014 Jonas Strandstedt                                                  *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#ifndef __SOCKET_H__
#define __SOCKET_H__

#define OWL_SOCKET_BUFFER_SIZE 2048
typedef char SocketData_t;

// C includes
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

// Unix includes
#ifdef __WIN32__

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/sha.h>

#endif

// C++11 includes
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <functional>

namespace owl {
class TCPSocketConnection;
class TCPClient;
typedef std::function<void(TCPSocketConnection*)>                           OpenCallback_t;
typedef std::function<void(TCPSocketConnection*, int, const SocketData_t*)> ReadCallback_t;
typedef std::function<void(TCPSocketConnection*)>                           CloseCallback_t;
typedef std::function<void(int, const SocketData_t*)>                       ClientReadCallback_t;
typedef std::function<void(const TCPClient*)>                               ClientCloseCallback_t;

bool NetworkIsInitialized();
bool NetworkInitialize();
void NetworkDeinitialize();

}

#endif