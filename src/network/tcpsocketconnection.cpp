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

#include <owl/network/tcpsocketconnection.h>
#include <owl/logging/logmanager.h>

namespace {
    std::string _loggerCat = "TCPSocketConnection";
}

namespace owl {

    
std::atomic<unsigned int> TCPSocketConnection::c(0);

TCPSocketConnection::TCPSocketConnection(int socket, OpenCallback_t oc, CloseCallback_t cc, ReadCallback_t rc):
    _socket(socket),
    _openCallback(oc),
    _closeCallback(cc),
    _readCallback(rc)
{

    // get a unique connection id
    _id = c.fetch_add(1);
    LDEBUGC("SocketConnection", "Connection initialized: " << identifier());
}
TCPSocketConnection::~TCPSocketConnection() {
    LDEBUGC("SocketConnection", "Connection deleted: " << identifier());
}

void TCPSocketConnection::start() {

    // detach thread to make it possible to delete SocketConnection when listener is closed
    _listenerThread = std::thread(socketConnectionListener, this);
    _listenerThread.detach();
}

void TCPSocketConnection::close() {
    //LDEBUGC("SocketConnection", "Connection stopped: " << identifier());
	shutdown(_socket,2);
#ifdef __WIN32__
	::closesocket(_socket);
#else
	::close(_socket);
#endif
    _socket = -1;
}

unsigned int TCPSocketConnection::identifier() {
    return _id;
}

void TCPSocketConnection::write(int length, const SocketData_t* data) {
    memset(_writeBuffer,0,OWL_SOCKET_BUFFER_SIZE);
    handleWrite(length, data);
}

void TCPSocketConnection::_write(int length, const SocketData_t* data) {
#ifdef __WIN32__
	int n = send(_socket, data, length, 0);
#else
	int n = ::write(_socket, data, length);
#endif
	if (n < 0)
        LWARNINGC("SocketConnection::send", "Error when sending msg \"" << data << "\"");
}
    
void TCPSocketConnection::write(const std::string& data) {
    memset(_writeBuffer,0,OWL_SOCKET_BUFFER_SIZE);
    handleWrite(data);
}

void TCPSocketConnection::setOpenCallback(OpenCallback_t callback) {
    _openCallback = callback;
}

void TCPSocketConnection::setReadCallback(ReadCallback_t callback) {
    _readCallback = callback;
}

void TCPSocketConnection::setCloseCallback(CloseCallback_t callback) {
    _closeCallback = callback;
}
  
void TCPSocketConnection::callOpenCallback() {
    if(_openCallback)
        _openCallback(this);
}
void TCPSocketConnection::callCallback(int length, const SocketData_t* data) {
    if (_readCallback != nullptr) {
        _readCallback(this, length, data);
    }
}

void TCPSocketConnection::socketConnectionListener(TCPSocketConnection* connection) {
    //static const std::string _loggerCat = "connectionListener";
    int n;
    SocketData_t buffer[OWL_SOCKET_BUFFER_SIZE];
    bool keepgoing = true;
    while(keepgoing) {
        
        // clean up buffer
        memset(buffer,0,OWL_SOCKET_BUFFER_SIZE);
#ifdef __WIN32__
		n = recv(connection->_socket, buffer, OWL_SOCKET_BUFFER_SIZE-1, 0);
#else
        n = read(connection->_socket,buffer,OWL_SOCKET_BUFFER_SIZE-1);
#endif
        //buffer[n] = '\0';
        if (n<=0) {
            keepgoing = false;
            //LDEBUG("Remote closed: " << connection->identifier());
        } else if(n > 0) {
            connection->handleRead(n, buffer);
        }
        if(connection->_socket == -1)
            keepgoing = false;
    }
    
    // Close the socket
    //LDEBUG("Stopped listening: " << connection->identifier());
    connection->close();
    
    // If provided a callback for notifying when closing
    if(connection->_closeCallback)
        connection->_closeCallback(connection);
    
    delete connection;
}
    
int TCPSocketConnection::socket() {
    return _socket;
}
}