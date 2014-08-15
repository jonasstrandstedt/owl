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

#ifndef __SOCKETSERVER_H__
#define __SOCKETSERVER_H__

#include <owl/network/socket.h>
#include <owl/network/tcpsocketconnection.h>


namespace owl {

class TCPServer {
public:

	TCPServer(OpenCallback_t  openCallback  = nullptr,
              CloseCallback_t closeCallback = nullptr,
              ReadCallback_t  readCallback  = nullptr);
	~TCPServer();
    
    bool initialize(int port);
    
    template<class T>
	void startAcceptIncoming() {
        _listenerThread = std::thread(listenIncomingConnections<T>, this, _connectionOpenCallback, _connectionCloseCallback, _connectionReadCallback);
    }

	void stopAcceptIncoming();
    bool isOpen();
private:
    
    template<class T>
    static void listenIncomingConnections(TCPServer* server, OpenCallback_t openCallback, CloseCallback_t closeCallback, ReadCallback_t readCallback) {
        int clientSocket;
        bool keepgoing = true;
        while (keepgoing) {
            clientSocket = server->acceptConnection();
            if (clientSocket < 0) {
                // time to stop
                keepgoing = false;
            } else {
            
                // TCPSocketConnections will delete itself when connection is stopped/aborted
                // TCPSocketConnections need to call openCallback when ready to send/receive data
                TCPSocketConnection* sc = new T(clientSocket, openCallback, closeCallback, readCallback);
                sc->start();
            }
        }
    };
    
	int acceptConnection();

	std::thread         _listenerThread;
    
    int                 _serverSocket;
	socklen_t           _clilen;
	struct sockaddr_in  _serv_addr;
    struct sockaddr_in  _cli_addr;
    
    OpenCallback_t _connectionOpenCallback;
    CloseCallback_t _connectionCloseCallback;
    ReadCallback_t _connectionReadCallback;
    
};

}

#endif