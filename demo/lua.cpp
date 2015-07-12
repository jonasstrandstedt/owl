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

namespace {
    std::string _loggerCat = "demo_lua";
}

int main(int argc, char** argv) {
	owl::DefaultInitialize();

    std::string dir = "";
    if(FileSys.findFile("CMakeLists.txt", dir)) {
        FileSys.registerToken("${BASE_PATH}", dir);
    } else {
        FileSys.registerToken("${BASE_PATH}", FileSys.workingDirectory());
    }
    
    const std::string str = R"(
return {
    port = 22222,
}
)";
    
    owl::Dictionary dict;
    owl::Lua lua;
    lua.loadStringIntoDictionary(str, dict);
    double p;
    if( ! dict.get("port", p))
        return EXIT_FAILURE;
    
    std::string source = R"(
    function luafunc(a)
        owl_LDEBUGC("ScriptPrint", a)
        return "val1", 2.01
    end

    owl.LDEBUG("Lua wd: " .. owl_workingDirectory())
    owl.LDEBUG("Lua BASE_PATH: " .. absPath("${BASE_PATH}"))
    s = owl.FileSys.workingDirectory()
    owl.LDEBUG("MOHAHAHAH" .. s)

    )";
    
    std::string buffer;
    double d;
    
    lua.loadString(source);
    lua.call("luafunc", "s>sd", "Hello", &buffer,&d);
    
    LDEBUG("return1: " << buffer);
    LDEBUG("return2: " << d);
    source = R"(
    function luafunc(a)
    owl_LDEBUGC("ScriptPrint2", a)
    return "valla", 4.01
    end
    
    )";
    lua.loadString(source);
    lua.call("luafunc", "s>sd", "Hello", &buffer,&d);
    
    LDEBUG("return1: " << buffer);
    LDEBUG("return2: " << d);

    
	LDEBUG("Done!");
    
    
	owl::DefaultDeinitialize();
    
    return 0;
}


