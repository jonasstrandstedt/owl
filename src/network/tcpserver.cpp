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

#include <owl/network/tcpserver.h>
#include <owl/logging/logmanager.h>
#include <owl/network/tcpsocketconnection.h>

namespace {
    std::string _loggerCat = "TCPServer";
}

namespace owl {

TCPServer::TCPServer(OpenCallback_t openCallback, CloseCallback_t closeCallback, ReadCallback_t readCallback):
    _serverSocket(-1), _connectionOpenCallback(openCallback), _connectionCloseCallback(closeCallback), _connectionReadCallback(readCallback) {
    _clilen = sizeof(_cli_addr);
}

TCPServer::~TCPServer() {
    
}

bool TCPServer::initialize(int port) {

    if (isOpen()) {
        LWARNING("Already initialized, aborting initialize.");
        return false;
    }

    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0)
		return false;
    
    // set all values in structs to zero
    memset(&_serv_addr, 0, sizeof(_serv_addr));
    
	_serv_addr.sin_family           = AF_INET;
	_serv_addr.sin_addr.s_addr      = INADDR_ANY;
	_serv_addr.sin_port             = htons(port);
	if (bind(_serverSocket, (struct sockaddr *) &_serv_addr,sizeof(_serv_addr)) < 0) {
		LERROR("Could not bind port: " << port);
		return false;
	}
    
    // start listen
	listen(_serverSocket,5);
    
    LINFO("Successfully initialized socket on port " << port);
    
    return true;
}

void TCPServer::stopAcceptIncoming() {
    ::shutdown(_serverSocket,SHUT_RDWR);
	::close(_serverSocket);
    _listenerThread.join();
    _serverSocket = -1;
}

int TCPServer::acceptConnection() {
    return accept(_serverSocket, (struct sockaddr *) &_cli_addr, &_clilen);
}

bool TCPServer::isOpen() {
    return _serverSocket >= 0;
}

}