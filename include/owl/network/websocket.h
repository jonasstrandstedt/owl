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

#ifndef __WEBSOCKET_H__
#define __WEBSOCKET_H__


#include <owl/network/socket.h>
#include <owl/network/tcpsocketconnection.h>

namespace owl {

class WebSocket: public TCPSocketConnection {
public:
    
	WebSocket(int socket, OpenCallback_t oc = nullptr, CloseCallback_t cc = nullptr, ReadCallback_t rc = nullptr);
	~WebSocket();
    
protected:
    void handleRead(int length, SocketData_t* data);
    void handleWrite(const std::string& data);
    void handleWrite(int length, const SocketData_t* data);
    
    void handshake(int length, const SocketData_t* data);
    static char* base64Encode(const unsigned char *data, size_t input_length);
    
    bool _isOpen;
    
};
}

#endif