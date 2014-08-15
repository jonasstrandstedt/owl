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

#ifndef __TCPSOCKETCONNECTION_H__
#define __TCPSOCKETCONNECTION_H__


#include <owl/network/socket.h>
#include <atomic>

namespace owl {

class TCPSocketConnection {
public:
    
	TCPSocketConnection(int socket, OpenCallback_t oc = nullptr, CloseCallback_t cc = nullptr, ReadCallback_t rc = nullptr);
	virtual ~TCPSocketConnection();
    
    void start();
    void close();
    
    unsigned int identifier();
    void write(int length, const SocketData_t* data);
    void write(const std::string& data);
    
    void setOpenCallback(OpenCallback_t callback);
	void setCloseCallback(CloseCallback_t callback);
	void setReadCallback(ReadCallback_t callback);
    
protected:
    
    int socket();
    void callOpenCallback();
    void callCallback(int length, const SocketData_t* data);
    void _write(int length, const SocketData_t* data);
    
    virtual void handleRead(int length, SocketData_t* data) = 0;
    virtual void handleWrite(const std::string& data) = 0;
    virtual void handleWrite(int length, const SocketData_t* data) = 0;
    
    SocketData_t _writeBuffer[OWL_SOCKET_BUFFER_SIZE];
private:

    static void socketConnectionListener(TCPSocketConnection* connection);
    static std::atomic<unsigned int> c;

    unsigned int _id;
	std::thread _listenerThread;
	int _socket;
    
    OpenCallback_t _openCallback;
    CloseCallback_t _closeCallback;
    ReadCallback_t _readCallback;
};
}

#endif