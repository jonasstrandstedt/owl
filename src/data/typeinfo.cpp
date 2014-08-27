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

#include <owl/data/typeinfo.h>

#include <sstream>
#include <owl/data/any.h>

#define TYPEINFO_NAME_IMPLEMENTATION(class_name) \
    template<>std::string TypeInfo::name<class_name>() { \
        return #class_name; \
    } \
    template<>std::string TypeInfo::name<class_name*>() { \
        return #class_name"*"; \
    } \
    template<>std::string TypeInfo::name<const class_name>() { \
        return "const "#class_name; \
    } \
    template<>std::string TypeInfo::name<const class_name*>() { \
        return "const "#class_name"*"; \
    } \
    template<>std::string TypeInfo::name<std::vector<class_name> >() { \
        return "std::vector<"#class_name">";\
    } \
    template<>std::string TypeInfo::name<std::vector<class_name*> >() { \
        return "std::vector<"#class_name"*>";\
    } \
    template<>std::string TypeInfo::name<std::vector<class_name>*>() { \
        return "std::vector<"#class_name">*";\
    } \
    template<>std::string TypeInfo::name<std::vector<class_name*>*>() { \
        return "std::vector<"#class_name"*>*";\
    } \
    template<>std::string TypeInfo::name<std::vector<const class_name> >() { \
        return "std::vector<const "#class_name">";\
    } \
    template<>std::string TypeInfo::name<std::vector<const class_name*> >() { \
        return "std::vector<const "#class_name"*>";\
    } \
    template<>std::string TypeInfo::name<std::vector<const class_name>*>() { \
        return "std::vector<const "#class_name">*";\
    } \
    template<>std::string TypeInfo::name<std::vector<const class_name*>*>() { \
        return "std::vector<const "#class_name"*>*";\
    } \
    template<>std::string TypeInfo::name<const std::vector<class_name> >() { \
        return "const std::vector<"#class_name">";\
    } \
    template<>std::string TypeInfo::name<const std::vector<class_name*> >() { \
        return "const std::vector<"#class_name"*>";\
    } \
    template<>std::string TypeInfo::name<const std::vector<class_name>*>() { \
        return "const std::vector<"#class_name">*";\
    } \
    template<>std::string TypeInfo::name<const std::vector<class_name*>*>() { \
        return "const std::vector<"#class_name"*>*";\
    } \
    template<>std::string TypeInfo::name<const std::vector<const class_name> >() { \
        return "const std::vector<const "#class_name">";\
    } \
    template<>std::string TypeInfo::name<const std::vector<const class_name*> >() { \
        return "const std::vector<const "#class_name"*>";\
    } \
    template<>std::string TypeInfo::name<const std::vector<const class_name>*>() { \
        return "const std::vector<const "#class_name">*";\
    } \
    template<>std::string TypeInfo::name<const std::vector<const class_name*>*>() { \
        return "const std::vector<const "#class_name"*>*";\
    } \


namespace owl {

// Fundamental types
TYPEINFO_NAME_IMPLEMENTATION(std::nullptr_t)
TYPEINFO_NAME_IMPLEMENTATION(char)
TYPEINFO_NAME_IMPLEMENTATION(bool)
TYPEINFO_NAME_IMPLEMENTATION(short)
TYPEINFO_NAME_IMPLEMENTATION(int)
TYPEINFO_NAME_IMPLEMENTATION(long)
TYPEINFO_NAME_IMPLEMENTATION(long long)
TYPEINFO_NAME_IMPLEMENTATION(float)
TYPEINFO_NAME_IMPLEMENTATION(double)
TYPEINFO_NAME_IMPLEMENTATION(long double)
TYPEINFO_NAME_IMPLEMENTATION(unsigned char)
TYPEINFO_NAME_IMPLEMENTATION(unsigned short)
TYPEINFO_NAME_IMPLEMENTATION(unsigned int)
TYPEINFO_NAME_IMPLEMENTATION(unsigned long)
TYPEINFO_NAME_IMPLEMENTATION(unsigned long long)
TYPEINFO_NAME_IMPLEMENTATION(signed char)
TYPEINFO_NAME_IMPLEMENTATION(std::string)

// owl classes
TYPEINFO_NAME_IMPLEMENTATION(owl::Log)
TYPEINFO_NAME_IMPLEMENTATION(owl::LogManager)
TYPEINFO_NAME_IMPLEMENTATION(owl::StreamLog)
TYPEINFO_NAME_IMPLEMENTATION(owl::Time)
TYPEINFO_NAME_IMPLEMENTATION(owl::Timer)
TYPEINFO_NAME_IMPLEMENTATION(owl::Any)
TYPEINFO_NAME_IMPLEMENTATION(owl::Dictionary)
TYPEINFO_NAME_IMPLEMENTATION(owl::Socket)
TYPEINFO_NAME_IMPLEMENTATION(owl::TCPClient)
TYPEINFO_NAME_IMPLEMENTATION(owl::TCPServer)
TYPEINFO_NAME_IMPLEMENTATION(owl::TCPSocket)
TYPEINFO_NAME_IMPLEMENTATION(owl::TCPSocketConnection)
TYPEINFO_NAME_IMPLEMENTATION(owl::Websocket)
TYPEINFO_NAME_IMPLEMENTATION(owl::Lua)
TYPEINFO_NAME_IMPLEMENTATION(owl::Filesystem)

// specials
template<>std::string TypeInfo::name<Any>(const Any& val) {
    return "owl::Any<" + val.typeName()+ ">";
}
template<>std::string TypeInfo::name<std::pair<const std::string, Any> >() {
    return "std::pair<std::string, owl::Any>";
}
template<>std::string TypeInfo::name<std::pair<const std::string, Any> >(const std::pair<const std::string, Any>& val) {
    return "std::pair<std::string, " + TypeInfo::name(val.second) + ">";
}

}