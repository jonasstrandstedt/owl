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

#include <owl/logging/logmanager.h>
#include <owl/logging/streamlog.h>

#include <owl/network/tcpclient.h>
#include <owl/network/tcpserver.h>
#include <owl/network/tcpsocket.h>
#include <owl/network/websocket.h>

#include <owl/data/any.h>
#include <owl/data/threadsafevector.hpp>
#include <owl/data/threadsafemap.hpp>
#include <owl/time/timer.h>

#include <owl/lua/lua.h>

#include <limits>

#define relative "../.."

namespace {
    std::string _loggerCat = "Demo";
}


owl::ThreadSafeVector<owl::TCPSocketConnection*> _connections;
//owl::ThreadSafeMap<owl::TCPSocketConnection*> _connectionsM;

owl::OpenCallback_t o = [](owl::TCPSocketConnection* connection) {
    LDEBUGCF("incoming","[%i] Open", connection->identifier());
    _connections.push_back(connection);
    
    auto v = _connections.getVector();
    LDEBUGC("incoming", "v.size(): " << v->size());
};
owl::CloseCallback_t c = [](owl::TCPSocketConnection* connection) {
    LDEBUGCF("incoming","[%i] Close", connection->identifier());
    _connections.remove(connection);
    auto v = _connections.getVector();
    LDEBUGC("incoming", "v.size(): " << v->size());
};
owl::ReadCallback_t r = [](owl::TCPSocketConnection* connection, int length, const SocketData_t* data) {
    
    // print
    LDEBUGCF("incoming","[%i] Msg: '%s'", connection->identifier(), data);
    
    // respond
    connection->write("hihi"); // websockets text message, tcp message
    const char* d = "data";
    connection->write(4, d);   // websockets data message, tcp message
};

int main(int argc, char** argv) {

    int port = 22222;

    owl::LogManager::initialize();
    owl::LogManager::ref().addLogger(new owl::StreamLog(std::cout));
    
    std::string source = R"(
function luafunc(a)
    owl_LDEBUGC("ScriptPrint", a)
    return "val1", 2.01
end
owl_LDEBUG("Print from Lua script")

)";
    
    owl::Lua lua;
    std::string buffer;
    double d;
    
    lua.loadString(source);
    lua.call("luafunc", "s>sd", "Hello", &buffer,&d);
    
    LDEBUG("return1: " << buffer);
    LDEBUG("return2: " << d);
    
    if(argc == 1) {
        owl::TCPServer server(o, c, r);
        
        if ( ! server.initialize(port))
            return 1;
        
        server.startAcceptIncoming<owl::WebSocket>();
        
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line == "q" || line == "quit")
                break;
        }
        server.stopAcceptIncoming();
    } else {
        owl::TCPClient client;
        if( ! client.connectLocalhost(port))
            return 1;
        
        owl::ClientReadCallback_t cc = [](int length, const SocketData_t* data) {
            LDEBUG("server: " << data);
        };
        
        client.listen(cc);
        
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line == "q" || line == "quit")
                break;
            else {
                client.write(line);
            }
        }
        client.disconnect();
    }
    
    
    LDEBUG("Done!");
    owl::LogManager::deinitialize();
    return 0;
}


