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

#ifndef __THREADSAFEMAP_H__
#define __THREADSAFEMAP_H__

#include <typeinfo>
#include <string>
#include <vector>

namespace owl {

class Log;
class LogManager;
class StreamLog;
class Time;
class Timer;
class Any;
class Dictionary;
class Socket;
class TCPClient;
class TCPServer;
class TCPSocket;
class TCPSocketConnection;
class Websocket;
class Lua;
class Filesystem;

class TypeInfo {
public:
    
    // unknown type
    template<class T>
    static std::string name() {
        return std::string(typeid(T).name());
    }
    
}; // TypeInfo

#define TYPEINFO_NAME_DECLARATION(class_name) \
    template<>std::string TypeInfo::name<class_name>(); \
    template<>std::string TypeInfo::name<class_name*>(); \
    template<>std::string TypeInfo::name<const class_name>(); \
    template<>std::string TypeInfo::name<const class_name*>(); \
    template<>std::string TypeInfo::name<std::vector<class_name> >(); \
    template<>std::string TypeInfo::name<std::vector<class_name*> >(); \
    template<>std::string TypeInfo::name<std::vector<class_name>*>(); \
    template<>std::string TypeInfo::name<std::vector<class_name*>*>(); \
    template<>std::string TypeInfo::name<std::vector<const class_name> >(); \
    template<>std::string TypeInfo::name<std::vector<const class_name*> >(); \
    template<>std::string TypeInfo::name<std::vector<const class_name>*>(); \
    template<>std::string TypeInfo::name<std::vector<const class_name*>*>(); \
    template<>std::string TypeInfo::name<const std::vector<class_name> >(); \
    template<>std::string TypeInfo::name<const std::vector<class_name*> >(); \
    template<>std::string TypeInfo::name<const std::vector<class_name>*>(); \
    template<>std::string TypeInfo::name<const std::vector<class_name*>*>(); \
    template<>std::string TypeInfo::name<const std::vector<const class_name> >(); \
    template<>std::string TypeInfo::name<const std::vector<const class_name*> >(); \
    template<>std::string TypeInfo::name<const std::vector<const class_name>*>(); \
    template<>std::string TypeInfo::name<const std::vector<const class_name*>*>();
    


// Fundamental types
TYPEINFO_NAME_DECLARATION(std::nullptr_t)
TYPEINFO_NAME_DECLARATION(char)
TYPEINFO_NAME_DECLARATION(bool)
TYPEINFO_NAME_DECLARATION(short)
TYPEINFO_NAME_DECLARATION(int)
TYPEINFO_NAME_DECLARATION(long)
TYPEINFO_NAME_DECLARATION(long long)
TYPEINFO_NAME_DECLARATION(float)
TYPEINFO_NAME_DECLARATION(double)
TYPEINFO_NAME_DECLARATION(long double)
TYPEINFO_NAME_DECLARATION(unsigned char)
TYPEINFO_NAME_DECLARATION(unsigned short)
TYPEINFO_NAME_DECLARATION(unsigned int)
TYPEINFO_NAME_DECLARATION(unsigned long)
TYPEINFO_NAME_DECLARATION(unsigned long long)
TYPEINFO_NAME_DECLARATION(signed char)
TYPEINFO_NAME_DECLARATION(std::string)

// owl classes
TYPEINFO_NAME_DECLARATION(Log)
TYPEINFO_NAME_DECLARATION(LogManager)
TYPEINFO_NAME_DECLARATION(StreamLog)
TYPEINFO_NAME_DECLARATION(Time)
TYPEINFO_NAME_DECLARATION(Timer)
TYPEINFO_NAME_DECLARATION(Any)
TYPEINFO_NAME_DECLARATION(Dictionary)
TYPEINFO_NAME_DECLARATION(Socket)
TYPEINFO_NAME_DECLARATION(TCPClient)
TYPEINFO_NAME_DECLARATION(TCPServer)
TYPEINFO_NAME_DECLARATION(TCPSocket)
TYPEINFO_NAME_DECLARATION(TCPSocketConnection)
TYPEINFO_NAME_DECLARATION(Websocket)
TYPEINFO_NAME_DECLARATION(Lua)
TYPEINFO_NAME_DECLARATION(Filesystem)
}  // owl

#endif
