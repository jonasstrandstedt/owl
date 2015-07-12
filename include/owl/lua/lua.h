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

#ifndef __LUA_H__
#define __LUA_H__

#include <owl/logging/logmanager.h>
#include <owl/data/any.h>
#include <owl/data/dictionary.h>
 
#include <cstdarg>

extern "C"{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

namespace owl {

class Lua {
public:
    Lua();
    ~Lua();
    
    void pushFunction(const std::string& name, lua_CFunction f);
    bool loadFile(const std::string& filename);
    bool loadString(const std::string& source);
    bool call(const std::string& func, const std::string sig = "", ...);
    
    bool loadStringIntoDictionary(const std::string& str, Dictionary& d);
    bool loadFileIntoDictionary(const std::string& filename, Dictionary& d);
    
private:
    void pushOwlObject();
    void pushOwlFunctions();
    void pushOwlFunction(const std::string& name, lua_CFunction f, const std::string& group = "");
    
    bool doPopulateDictionary(Dictionary& d);

    lua_State* _state;
};

} // owl

#endif