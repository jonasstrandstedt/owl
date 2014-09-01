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
#include <owl/filesystem/filesystem.h>

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
    
    static int owl_absPath (lua_State* L) {
        const char* path = luaL_checkstring(L, 1);
        std::string abspath = absPath(path);
        lua_pushstring(L, abspath.c_str());
        return 1;
    }
    
    static int owl_absolutePath (lua_State* L) {
        const char* path = luaL_checkstring(L, 1);
        std::string abspath = FileSys.absolutePath(path);
        lua_pushstring(L, abspath.c_str());
        return 1;
    }
    
    static int owl_registerToken (lua_State* L) {
        const char* token = luaL_checkstring(L, 1);
        const char* path = luaL_checkstring(L, 2);
        bool success = FileSys.registerToken(token, path);
        lua_pushboolean(L, success);
        return 1;
    }
    
    static int owl_tokenRegistered (lua_State* L) {
        const char* token = luaL_checkstring(L, 1);
        bool success = FileSys.tokenRegistered(token);
        lua_pushboolean(L, success);
        return 1;
    }
    
    static int owl_workingDirectory (lua_State* L) {
        std::string wd = FileSys.workingDirectory();
        lua_pushstring(L, wd.c_str());
        return 1;
    }
    
    static int owl_fileExists (lua_State* L) {
        const char* file = luaL_checkstring(L, 1);
        bool success = FileSys.fileExists(file);
        lua_pushboolean(L, success);
        return 1;
    }
    
    static int owl_directoryExists (lua_State* L) {
        const char* dir = luaL_checkstring(L, 1);
        bool success = FileSys.directoryExists(dir);
        lua_pushboolean(L, success);
        return 1;
    }
    
    void populateDictionary(lua_State* state, owl::Dictionary& dict) {
        static const int KEY = -2;
        static const int VAL = -1;
        enum class TableType {
            Undefined = 1,  // 001
            Map = 3,        // 010
            Array = 5       // 101
        };
        TableType type = TableType::Undefined;
        lua_pushnil(state);
        while (lua_next(state, KEY) != 0) {
            // get the key name
            std::string key;
            const int keyType = lua_type(state, KEY);
            switch (keyType) {
                case LUA_TNUMBER:
                    if (type == TableType::Map) {
                        LERROR("Dictionary can only contain a pure map or a pure array");
                        return;
                    }
                    type = TableType::Array;
                    key = std::to_string(lua_tointeger(state, KEY));
                    break;
                case LUA_TSTRING:
                    if (type == TableType::Array) {
                        LERROR("Dictionary can only contain a pure map or a pure array");
                        return;
                    }
                    type = TableType::Map;
                    key = lua_tostring(state, KEY);
                    break;
                default:
                    LERRORC("luaTableToString", "Missing type: " << keyType);
                    break;
            }
            // get the value
            switch (lua_type(state, VAL)) {
                case LUA_TNUMBER: {
                    double value = lua_tonumber(state, VAL);
                    dict.insert(key, value);
                } break;
                case LUA_TBOOLEAN: {
                    bool value = (lua_toboolean(state, VAL) == 1);
                    dict.insert(key, value);
                } break;
                case LUA_TSTRING: {
                    std::string value = lua_tostring(state, VAL);
                    dict.insert(key, value);
                } break;
                case LUA_TTABLE: {
                    owl::Dictionary d;
                    populateDictionary(state, d);
                    dict.insert(key, d);
                } break;
                default:
                    LERROR("Unknown type: " << std::to_string(lua_type(state, VAL)));
                    return;
            }
            // get back up one level
            lua_pop(state, 1);
        }
    }
}

namespace owl {

Lua::Lua() {
    
    // create new state and load standard lua libraries
    _state = luaL_newstate();
    luaL_openlibs(_state);
    
    LINFOF("Opened Lua state: %s.%s.%s", LUA_VERSION_MAJOR, LUA_VERSION_MINOR, LUA_VERSION_RELEASE);
    
    pushOwlObject();
    pushOwlFunctions();
}

Lua::~Lua() {
    lua_close(_state);
}

void Lua::pushFunction(const std::string& name, lua_CFunction f) {
    lua_pushcfunction(_state, f);
    lua_setglobal(_state, name.c_str());
}

bool Lua::loadFile(const std::string& filename) {


    int status = luaL_loadfile(_state, filename.c_str());
    if (status != LUA_OK) {
        LERROR("Error loading script: '" << lua_tostring(_state, -1) << "'");
        return false;
    }
    
    if (lua_pcall(_state, 0, LUA_MULTRET, 0)) {
        LERROR("Error executing script: " << lua_tostring(_state, -1));
        return false;
    }
    
    return true;
}

bool Lua::loadString(const std::string& source) {
    
    int status = luaL_loadstring(_state, source.c_str());
    if (status != LUA_OK) {
        LERROR("Error loading source: '" << lua_tostring(_state, -1) << "'");
        return false;
    }
    
    if (lua_pcall(_state, 0, LUA_MULTRET, 0)) {
        LERROR("Error executing script: " << lua_tostring(_state, -1));
        return false;
    }
    
    return true;
}

// Originally from: http://www.lua.org/pil/25.3.html
bool Lua::call(const std::string& func, const std::string sig, ...) {
    va_list vl;
    int narg, nres;  // number of arguments and results
    
    va_start(vl, sig);
    lua_getglobal(_state, func.c_str());  // get function
    
    // push arguments
    narg = 0;
    bool foundBreak = false;
    for(size_t i = 0; i < sig.size() && foundBreak == false; ++i) {
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
                LERRORF("invalid option (%c), option ignored", sig.at(i));
        }
        ++narg;
        luaL_checkstack(_state, 1, "too many arguments");
    };
    
    // do the call
    nres =  sig.size() - narg - 1;  // number of expected results
    if (lua_pcall(_state, narg, nres, 0) != 0) {  // do the call
        LERRORF("error running function `%s': %s", func.c_str(), lua_tostring(_state, -1));
        va_end(vl);
        return false;
    }
    
    // retrieve results
    nres = -nres;  // stack index of first result
    for(size_t i = narg+1; i < sig.size(); ++i) {
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
                LERRORF("invalid option (%c), option ignored", sig.at(i));
        }
        nres++;
    }
    va_end(vl);
    return true;
}

bool Lua::loadStringIntoDictionary(const std::string& str, Dictionary& d) {
    if( ! loadString(str))
        return false;
    
    return doPopulateDictionary(d);
}

bool Lua::loadFileIntoDictionary(const std::string& filename, Dictionary& d) {
    if( ! loadFile(filename))
        return false;
    
    return doPopulateDictionary(d);
}
    
void Lua::pushOwlObject() {
    const std::string s = R"(
    owl = {}
    owl.__index = owl
    owl.FileSys = {}

    )";
    loadString(s);
}
void Lua::pushOwlFunctions() {
    
    // Register the owl logging functions
    pushOwlFunction("owl_LDEBUG", owl_LDEBUG);
    pushOwlFunction("owl_LDEBUGC", owl_LDEBUGC);
    pushOwlFunction("owl_LINFO", owl_LINFO);
    pushOwlFunction("owl_LINFOC", owl_LINFOC);
    pushOwlFunction("owl_LWARNING", owl_LWARNING);
    pushOwlFunction("owl_LWARNINGC", owl_LWARNINGC);
    pushOwlFunction("owl_LERROR", owl_LERROR);
    pushOwlFunction("owl_LERRORC", owl_LERRORC);
    
    // Register Filesystem functions
    pushFunction("absPath", owl_absPath);
    pushOwlFunction("owl_absolutePath", owl_absolutePath, "FileSys.");
    pushOwlFunction("owl_registerToken", owl_registerToken, "FileSys.");
    pushOwlFunction("owl_tokenRegistered", owl_tokenRegistered, "FileSys.");
    pushOwlFunction("owl_workingDirectory", owl_workingDirectory, "FileSys.");
    pushOwlFunction("owl_fileExists", owl_fileExists, "FileSys.");
    pushOwlFunction("owl_directoryExists", owl_directoryExists, "FileSys.");
}

void Lua::pushOwlFunction(const std::string& name, lua_CFunction f, const std::string& group) {
    
    const std::string s = "\n\
    function owl." + group + name.substr(4, name.length()-4) +"(...) \n\
        return " + name + "(...)\n\
    end";
    // LDEBUG(s);
    
    pushFunction(name, f);
    loadString(s);
    
}

bool Lua::doPopulateDictionary(Dictionary& d) {
    if (lua_isnil(_state, -1)) {
        LERROR("Error in Lua string: Did not return anything.");
        return false;
    }
    if (!lua_istable(_state, -1)) {
        LERROR("Error in Lua string: Did not return a table.");
        return false;
    }
    
    populateDictionary(_state, d);
    
    // Clean up after ourselves by cleaning the stack
    lua_settop(_state, 0);
    return true;
}

}