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

#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include <owl/network/socket.h>

namespace owl {
    
class TCPClient {
public:

    static const unsigned int MAX_DATA_SIZE = 256;

    TCPClient();
    ~TCPClient();
    
    bool connect(const std::string& hostname, int port);
    bool connectLocalhost(int port);
    bool connect(int p1,int p2,int p3,int p4, int port);
    void disconnect();
    
    void listen(ClientReadCallback_t callback, ClientCloseCallback_t closeCallback);
    
    void write(int length, const char* data);
    void write(const std::string& data);
    bool read(int& length, char* data[]);
    
private:

    static void listener(TCPClient* client);

    int _socket;
    ClientReadCallback_t _callback;
    ClientCloseCallback_t _closeCallback;
    std::thread _listenerThread;
};
} // owl

#endif