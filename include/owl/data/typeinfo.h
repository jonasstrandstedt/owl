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

namespace owl {

class Any;

class TypeInfo {
public:
    
    // unknown type
    template<class T>
    static std::string name() {
        return demangle(typeid(T).name());
    }
    
    // unknown type
    template<class T>
    static std::string name(const T& val) {
        return demangle(typeid(val).name());
    }

private:
    static std::string demangle(const char* name);
    
}; // TypeInfo

// Specials
template<>std::string TypeInfo::name<Any>(const Any& val);
template<>std::string TypeInfo::name<std::pair<const std::string, Any> >();
template<>std::string TypeInfo::name<std::pair<const std::string, Any> >(const std::pair<const std::string, Any>& val);

}  // owl

#endif
