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

// many owl stuff
#include <owl/owl>

// network
#include <owl/network/tcpserver.h>
#include <owl/network/tcpsocket.h>
#include <owl/network/websocket.h>

namespace {
    std::string _loggerCat = "demo_socket_server";
}

owl::ThreadSafeVector<owl::TCPSocketConnection*> _connections;

owl::OpenCallback_t o = [](owl::TCPSocketConnection* connection) {
    LDEBUGF("[%i] Open", connection->identifier());
    _connections.push_back(connection);
};
owl::CloseCallback_t c = [](owl::TCPSocketConnection* connection) {
    LDEBUGF("[%i] Close", connection->identifier());
    _connections.remove(connection);
    auto v = _connections.getVector();
    LDEBUG("v.size(): " << v->size());
};
owl::ReadCallback_t r = [](owl::TCPSocketConnection* connection, int length, const SocketData_t* data) {
    
    // print
    LDEBUGF("[%i] Msg: '%s'", connection->identifier(), data);
    
    // respond
    connection->write("Ping"); // websockets text message, tcp message
    const char* d = "Pong";
    connection->write(4, d);   // websockets data message, tcp message
};

int main(int argc, char** argv) {
	owl::DefaultInitialize();

    
    const size_t port = 22222;
    const size_t websocket_port = 33333;
    
    owl::TCPServer server(o, c, r);
    owl::TCPServer websocket_server(o, c, r);
    
    if ( ! server.initialize(port))
        return 1;
    if ( ! websocket_server.initialize(websocket_port))
        return 1;
    
    server.startAcceptIncoming<owl::TCPSocket>();
    websocket_server.startAcceptIncoming<owl::WebSocket>();
    std::string line;
    std::getline(std::cin, line);
    
    LDEBUG("Stopping");
    server.stopAcceptIncoming();
    websocket_server.stopAcceptIncoming();
    for(auto c: _connections) {
        c->close();
    }

	owl::DefaultDeinitialize();
    
    return 0;
}


