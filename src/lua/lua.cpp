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

#include <owl/lua/lua.h>


#include <cmath>

namespace {
    
    std::string _loggerCat = "Lua";
    std::string _loggerCatScript = "LuaScript";
    
    static int owl_LDEBUG (lua_State* L) {
        const char* msg = luaL_checkstring(L, 1);
        LDEBUGC(_loggerCatScript, msg);
        return 0;
    }
    
    static int owl_LDEBUGC (lua_State* L) {
        const char* cat = luaL_checkstring(L, 1);
        const char* msg = luaL_checkstring(L, 2);
        LDEBUGC(cat,msg);
        return 0;
    }
    
    static int owl_LINFO (lua_State* L) {
        const char* msg = luaL_checkstring(L, 1);
        LINFOC(_loggerCatScript, msg);
        return 0;
    }
    
    static int owl_LINFOC (lua_State* L) {
        const char* cat = luaL_checkstring(L, 1);
        const char* msg = luaL_checkstring(L, 2);
        LINFOC(cat,msg);
        return 0;
    }
    
    static int owl_LWARNING (lua_State* L) {
        const char* msg = luaL_checkstring(L, 1);
        LWARNINGC(_loggerCatScript, msg);
        return 0;
    }
    
    static int owl_LWARNINGC (lua_State* L) {
        const char* cat = luaL_checkstring(L, 1);
        const char* msg = luaL_checkstring(L, 2);
        LWARNINGC(cat,msg);
        return 0;
    }
    
    static int owl_LERROR (lua_State* L) {
        const char* msg = luaL_checkstring(L, 1);
        LERRORC(_loggerCatScript, msg);
        return 0;
    }
    
    static int owl_LERRORC (lua_State* L) {
        const char* cat = luaL_checkstring(L, 1);
        const char* msg = luaL_checkstring(L, 2);
        LERRORC(cat,msg);
        return 0;
    }
}

namespace owl {

Lua::Lua() {
    
    // create new state and load standard lua libraries
    _state = luaL_newstate();
    luaL_openlibs(_state);
    
    LINFOF("Opened Lua state: %s.%s.%s", LUA_VERSION_MAJOR, LUA_VERSION_MINOR, LUA_VERSION_RELEASE);
    
    pushOwlFunctions();
}

Lua::~Lua() {
    lua_close(_state);
}

void Lua::pushFunction(const std::string& name, lua_CFunction f) {
    
    lua_pushcfunction(_state, f);
    lua_setglobal(_state, name.c_str());
}

void Lua::loadFile(const std::string& filename) {


    int status = luaL_loadfile(_state, filename.c_str());
    if (status != LUA_OK) {
        LERROR("Error loading script: '" << lua_tostring(_state, -1) << "'");
        return;
    }
    
    //LDEBUG("Executing script");
    if (lua_pcall(_state, 0, LUA_MULTRET, 0)) {
        LERROR("Error executing script: " << lua_tostring(_state, -1));
        return;
    }
}

void Lua::loadString(const std::string& source) {
    
    int status = luaL_loadstring(_state, source.c_str());
    if (status != LUA_OK) {
        LERROR("Error loading source: '" << lua_tostring(_state, -1) << "'");
        return;
    }
    
    //LDEBUG("Executing script");
    if (lua_pcall(_state, 0, LUA_MULTRET, 0)) {
        LERROR("Error executing script: " << lua_tostring(_state, -1));
        return;
    }
}

// Originally from: http://www.lua.org/pil/25.3.html
void Lua::call(const std::string& func, const std::string sig, ...) {
    va_list vl;
    int narg, nres;  // number of arguments and results
    
    va_start(vl, sig);
    lua_getglobal(_state, func.c_str());  // get function
    
    // push arguments
    narg = 0;
    bool foundBreak = false;
    for(int i = 0; i < sig.size() && foundBreak == false; ++i) {
        switch (sig.at(i)) {
                
            case 'b':  // bool argument
                lua_pushboolean(_state, static_cast<bool>(va_arg(vl, int)));
                break;
            case 'd':  // double argument
                lua_pushnumber(_state, va_arg(vl, double));
                break;
            case 'i':  // int argument
                lua_pushinteger(_state, va_arg(vl, int));
                break;
            case 's':  // string argument
                lua_pushstring(_state, va_arg(vl, char *));
                break;
            case '>':
                --narg;
                foundBreak = true;
                break;
            default:
                LERRORF("invalid option (%c)", sig.at(i));
        }
        ++narg;
        luaL_checkstack(_state, 1, "too many arguments");
    };
    
    // do the call
    nres =  sig.size() - narg - 1;  // number of expected results
    if (lua_pcall(_state, narg, nres, 0) != 0) {  // do the call
        LERRORF("error running function `%s': %s", func.c_str(), lua_tostring(_state, -1));
        va_end(vl);
        return;
    }
    
    // retrieve results
    nres = -nres;  // stack index of first result
    for(int i = narg+1; i < sig.size(); ++i) {
        //while (*sig) {  // get results
        //LDEBUG("i: " << i);
        //LDEBUG("nres: " << nres);
        
        //int t = lua_type(_state, nres);
        //const char* tn = lua_typename(_state, t);
        //LDEBUG("name: " << tn);
        
        switch (sig.at(i)) {
            case 'b':  // double result
                if (!lua_isboolean(_state, nres))
                    LERROR("Wrong result type: " << nres);
                else
                    *va_arg(vl, bool*) = lua_toboolean(_state, nres);
                break;
                
            case 'd':  // double result
                if (!lua_isnumber(_state, nres))
                    LERROR("Wrong result type: " << nres);
                else
                    *va_arg(vl, double*) = lua_tonumber(_state, nres);
                break;
                
            case 'i':  // int result
                if (!lua_isnumber(_state, nres))
                    LERROR("Wrong result type: " << nres);
                else
                    *va_arg(vl, int *) = lua_tointeger(_state, nres);
                break;
                
            case 's':  // string result
                if (!lua_isstring(_state, nres))
                    LERROR("Wrong result type: " << nres);
                else
                    *va_arg(vl, std::string*) = std::string(lua_tostring(_state, nres));
                break;
                
            default:
                LERRORF("invalid option (%c)", sig.at(i));
        }
        nres++;
    }
    va_end(vl);
}

void Lua::pushOwlFunctions() {
    
    // Register the owl logging functions
    pushFunction("owl_LDEBUG", owl_LDEBUG);
    pushFunction("owl_LDEBUGC", owl_LDEBUGC);
    pushFunction("owl_LINFO", owl_LINFO);
    pushFunction("owl_LINFOC", owl_LINFOC);
    pushFunction("owl_LWARNING", owl_LWARNING);
    pushFunction("owl_LWARNINGC", owl_LWARNINGC);
    pushFunction("owl_LERROR", owl_LERROR);
    pushFunction("owl_LERRORC", owl_LERRORC);
}

}