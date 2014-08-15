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

#include <owl/network/tcpclient.h>
#include <owl/logging/logmanager.h>

namespace {
    std::string _loggerCat = "TCPClient";
}

namespace owl {
    TCPClient::TCPClient() {
        
    }
    
    TCPClient::~TCPClient() {
        
    }
    
    bool TCPClient::connect(const std::string& hostname, int port) {
        if (port < 0) {
            return false;
        }

        struct sockaddr_in serv_addr;
        struct hostent *server;
        
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_socket < 0){
            LERROR("Could not open socket");
            return false;
        }
        server = gethostbyname(hostname.c_str());
        if (server == NULL) {
            LERROR("No such host: " << hostname);
            return false;
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
              (char *)&serv_addr.sin_addr.s_addr,
              server->h_length);
        serv_addr.sin_port = htons(port);
        if (::connect(_socket,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
            LERROR("Could not connect");
            return false;
        }
        
        return true;
    }
    
    bool TCPClient::connectLocalhost(int port) {
        return connect("127.0.0.1", port);
    }
    
    bool TCPClient::connect(int p1, int p2, int p3, int p4, int port) {
        if (p1 < 0 || p2 < 0 || p3 < 0 || p4 < 0)
            return false;
        if (p1 > 255 || p2 > 255 || p3 > 255 || p4 > 255)
            return false;
        if (port < 0)
            return false;
        
        const size_t bufferSize = 30;
        char buffer[bufferSize];
        snprintf(buffer, bufferSize, "%i.%i.%i.%i", p1, p2, p3, p4);
        
        return connect(buffer, port);
    }
    
    void TCPClient::disconnect() {
        ::close(_socket);
    }
    
    void TCPClient::listen(ClientReadCallback_t callback) {
        _callback = callback;
        _listenerThread = std::thread(listener, this);
        _listenerThread.detach();
    }
    
    void TCPClient::write(int length, const char* data) {
        int n = ::write(_socket,data,length);
        if (n < 0)
            LERROR("Could not write to socket");
    }
    
    void TCPClient::write(const std::string& data) {
        write(data.size(), data.c_str());
    }
    
    bool TCPClient::read(int& length, char* data[]) {
        int n;
        SocketData_t buffer[MAX_DATA_SIZE];
        n = ::read(_socket, buffer, MAX_DATA_SIZE-1);
        if (n<=0) {
            return false;
        }
        
        length = n;
        bcopy(buffer, *data,length);
        
        return true;
    }
    
    void TCPClient::listener(TCPClient* client) {
        int length;
        char* buffer = new char[TCPClient::MAX_DATA_SIZE];
        while (client->read(length, &buffer)) {
            client->_callback(length, buffer);
        }
        delete[] buffer;
        LDEBUGC("TCPClient::listener", "Done..");
    }
}