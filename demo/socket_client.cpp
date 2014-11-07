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
#include <owl/network/tcpclient.h>

namespace {
    std::string _loggerCat = "demo_socket_client";
}

int main(int argc, char** argv) {
	owl::DefaultInitialize();

    const size_t port = 22222;

    owl::TCPClient client;
    if( ! client.connectLocalhost(port))
        return 1;
    
    bool open = true;
    
    owl::ClientReadCallback_t cc = [](int length, const SocketData_t* data) {
        LDEBUG("server: " << data);
    };
    
    owl::ClientCloseCallback_t ccc = [&open] (const owl::TCPClient* client) {
        open = false;
    };
    
    client.listen(cc, ccc);
    
    std::string line;
    while (std::getline(std::cin, line) && open) {
        if (line == "q" || line == "quit")
            break;
        else {
            client.write(line);
        }
    }
    client.disconnect();
    
	owl::DefaultDeinitialize();
    
    return 0;
}


