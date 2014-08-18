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

#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <owl/designpattern/singleton.hpp>

#include <string>
#include <vector>
#include <map>

#define OWL_FILESYSTEM_CACHE

namespace owl {

class Filesystem: public Singleton<Filesystem> {
public:

    static const size_t MAX_PATH_SIZE = 4096;

    Filesystem();
    ~Filesystem();
    
    void registerToken(const std::string& token, const std::string& path);
    bool tokenRegistered(const std::string& token) const;
    std::string absolutePath(const std::string& path);
    
    std::string workingDirectory() const;
    std::vector<std::string> listFiles(const std::string& path) const;
    std::vector<std::string> listDirectories(const std::string& path) const;
    
    
    bool fileExists(const std::string& path) const;
    bool directoryExists(const std::string& path) const;
    
    bool findFile(const std::string& file, std::string& directory);
    
private:
    bool _hasTokens(const std::string& path);
    std::string _resolveTokens(const std::string& path);
    std::string _absolutePath(const std::string& path, bool verbose = true) const;
    std::vector<std::string> _list(int type, const std::string& path) const;
    bool _exists(int type, const std::string& path) const;
    
    std::map<std::string,std::string> _tokens;
    
#ifdef OWL_FILESYSTEM_CACHE
    std::map<std::string,std::string> _cache;
#endif
};

} // owl

#define FileSys owl::Filesystem::ref()
#define absPath(s) owl::Filesystem::ref().absolutePath(s)

#endif