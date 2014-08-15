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

#include <owl/network/websocket.h>
#include <owl/logging/logmanager.h>

namespace {
    const char FIN_BIT = (1<<7);
    const char OPT_TEXT_BIT = 0x1;
    const char OPT_BINARY_BIT = 0x2;
    const char OPT_CLOSE_BIT = 0x8;
    //const char OPT_PING_BIT = 0x9;
    //const char OPT_PONG_BIT = 0x8;
    //const char MASK_BIT = (1<<7);
    
    std::string _loggerCat = "WebSocket";
    
    template<class T>
    std::string getBinary(T number, bool separation = false) {
        std::string val;
        for (int i = 0; i<sizeof(T)*8; ++i) {
            if(separation && i > 0 && i < sizeof(T)*8 && i % 4 == 0)
                val = " " + val;
            if ((number & (1 << i)) > 0) {
                val = "1" + val;
            } else {
                val = "0" + val;
            }
        }
        return val;
    }
    
    std::string getValue(const std::string& str,const std::string& key) {
        size_t startpos;
        size_t stoppos;
        size_t diff = key.length() + 2;
        std::string value = "";
        startpos = str.find(key);
        if (startpos != std::string::npos)
        {
            stoppos = str.find_first_of("\r\n", startpos);
            value = str.substr(startpos+diff, stoppos-startpos-diff);
        }
        return value;
    };
    
    static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'};
    static char* decoding_table = nullptr;
    static int mod_table[] = {0, 2, 1};
}

namespace owl {

WebSocket::WebSocket(int socket, OpenCallback_t oc, CloseCallback_t cc, ReadCallback_t rc)
    :TCPSocketConnection(socket, oc, cc, rc)
    ,_isOpen(false) {}

WebSocket::~WebSocket() {}

void WebSocket::handleRead(int length, SocketData_t* data) {
    if ( ! _isOpen) {
        handshake(length, data);
    } else if (_isOpen && data[0] & FIN_BIT && data[0] & OPT_TEXT_BIT) {
		unsigned int i;
		unsigned char mask[4];
		unsigned int packet_length = ((unsigned char) data[1]) & 0x7f;
		int rc;
        
        //LDEBUG("length: " << length);
        //LDEBUG("packet_length: " << packet_length);
        //LDEBUG("data: '" << data << "'");
        //LDEBUG("buff: '" << buffer << "'");
        
		mask[0] = data[2];
		mask[1] = data[3];
		mask[2] = data[4];
		mask[3] = data[5];
        
        
		if (packet_length <= 126) {
			// Unmask the payload.
			for (i = 0; i < packet_length; i++)
				data[6 + i] ^= mask[i % 4];
			rc = asprintf(&data, "%.*s", packet_length, data + 6);
		} else if (packet_length == 127) {
			// Unmask the payload.
			for (i = 0; i < packet_length; i++)
				data[8 + i] ^= mask[i % 4];
			rc = asprintf(&data, "%.*s", packet_length, data + 8);
		}
		callCallback(length, data);
        
	} else if (_isOpen && data[0] & FIN_BIT && data[0] & OPT_BINARY_BIT) {
		unsigned int packet_length = ((unsigned char) data[1]) & 0x7f;
		callCallback(packet_length, data +2);
        
	} else if(data[0] & OPT_CLOSE_BIT){
        //LDEBUG("Closing bit received");
		close();
	} else {
        LERROR("Something unexpected received, closing!");
        LERROR("data: '" << data << "'");
		close();
    }
}

void WebSocket::handleWrite(const std::string& data) {
    std::string _loggerCat = "WebSocket::handleWrite";
    size_t length = data.size();
    if (length <= 125) {
        //LDEBUG("length <= 125");
        for (int i = length-1; i >= 0; --i) {
            _writeBuffer[i+2] = data[i];
        }
        _writeBuffer[0] = 0 | FIN_BIT | OPT_TEXT_BIT;
        _writeBuffer[1] = length;
        _write(length+2, _writeBuffer);
    } else if (length >= 126 && length <= 65535){
        //LDEBUG("length >= 126 && length <= 65535");
        for (int i = length-1; i >= 0; --i) {
            _writeBuffer[i+4] = data[i];
        }
        _writeBuffer[0] = 0 | FIN_BIT | OPT_TEXT_BIT;
        _writeBuffer[1] = 126;
        _writeBuffer[2] = (length >> 8 ) & 255;
        _writeBuffer[3] = (length) & 255;
        _write(length+4, _writeBuffer);
    }
}

void WebSocket::handleWrite(int length, const SocketData_t* data) {
    std::string _loggerCat = "WebSocket::handleWrite";
    for (int i = length-1; i >= 0; --i) {
        _writeBuffer[i+2] = data[i];
    }
    _writeBuffer[0] = 0 | FIN_BIT | OPT_BINARY_BIT;
    _writeBuffer[1] = length;
    _write(length+2, _writeBuffer);
}

void WebSocket::handshake(int length, const char* data) {
	std::string str = std::string(data, length);
	std::string origin = "";
	std::string host = "";
	std::string secKey = "";
    
    origin = getValue(str, "Origin");
    host = getValue(str, "Host");
    secKey = getValue(str, "Sec-WebSocket-Key");
    
	if (origin == "" || secKey == "" || host == "") {
        LERROR("Handshake unsuccessful, terminating connection!");
		close();
		return;
	}
    
	//std::cout << "secKey :  " << secKey << std::endl;
	//std::cout << "origin :  " << origin <<  std::endl;
	//std::cout << "host   :  " << host << std::endl;
    
	std::string magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	secKey.append(magic);
    
	unsigned char hash[20];
	SHA1(reinterpret_cast<const unsigned char*>(secKey.c_str()),secKey.length(),hash);
	char *encoded_data = base64Encode(hash, 20);
	std::string encoded = std::string(encoded_data, 28);
	delete[] encoded_data;
    
	std::string reply = "";
	reply += "HTTP/1.1 101 Web Socket Protocol Handshake\r\n";
	reply += "Upgrade: WebSocket\r\n";
	reply += "Connection: Upgrade\r\n";
	reply += "WebSocket-Origin: " + origin + "\r\n";
	reply += "WebSocket-Location: ws://"+host+"\r\n";
	reply += "Sec-WebSocket-Accept: "+encoded+"\r\n\r\n";
    
    _write(reply.length(), reply.c_str());
    
    // The connection is only assumed to be OK at this point (It should be OK). Need a
    // way to confirm before callback. If the response is not accepted the open callback
    // will be called and soon after the close callback is called, so nothing
    // devastating happens.
    _isOpen = true;
    callOpenCallback();
}

char* WebSocket::base64Encode(const unsigned char *data, size_t input_length) {
    
    size_t output_length = 4 * ((input_length + 2) / 3);
    
    //char *encoded_data = (char*)malloc(output_length);
    char* encoded_data = new char[output_length];
    if (encoded_data == nullptr) return nullptr;
    
    for (int i = 0, j = 0; i < input_length;) {
        
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;
        
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    
    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[output_length - 1 - i] = '=';
    
    return encoded_data;
}

}