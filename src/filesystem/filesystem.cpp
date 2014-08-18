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

#include <owl/filesystem/filesystem.h>

#include <owl/logging/logmanager.h>

#include <dirent.h>
#include <sys/param.h>
#include <sys/stat.h>

namespace {
    const std::string _loggerCat = "Filesystem";
    const std::string TokenOpen = "${";
    const std::string TokenClose = "}";
    const std::string Separator = "/";
}

namespace owl {
    
Filesystem::Filesystem() {
    
}

Filesystem::~Filesystem() {
    
}

bool Filesystem::registerToken(const std::string& token, const std::string& path) {
    // check length
    if(token.length() < TokenOpen.length() + TokenClose.length() + 1)
        return false;
    
    // check beginning
    if(token.substr(0, TokenOpen.length()) != TokenOpen)
        return false;
    
    // check end
    if(token.substr(token.length()-TokenClose.length(), TokenClose.length()) != TokenClose)
        return false;
    
    _tokens[token] = path;
#ifdef OWL_FILESYSTEM_CACHE
    _cache.clear();
#endif
    return true;
}
    
bool Filesystem::tokenRegistered(const std::string& token) const {
    auto it = _tokens.find(token);
    return it != _tokens.end();
}

std::string Filesystem::absolutePath(const std::string& path) {
    
#ifdef OWL_FILESYSTEM_CACHE
    auto it = _cache.find(path);
    if (it != _cache.end()) {
        return (*it).second;
    }
#endif
    
    std::string p = _resolveTokens(path);
#ifdef OWL_FILESYSTEM_CACHE
    p = _absolutePath(p, false);
    _cache[path] = p;
    return p;
#else
    return _absolutePath(p, false)
#endif
}

std::string Filesystem::workingDirectory() const {
    return _absolutePath(".");
}

std::vector<std::string> Filesystem::listFiles(const std::string& path) const {
    return _list(DT_REG, path);
}

std::vector<std::string> Filesystem::listDirectories(const std::string& path) const {
    return _list(DT_DIR, path);
}

bool Filesystem::fileExists(const std::string& path) const {
    return _exists(S_IFREG, _absolutePath(path, false));
}

bool Filesystem::directoryExists(const std::string& path) const {
    return _exists(S_IFDIR, _absolutePath(path, false));
}

    
bool Filesystem::findFile(const std::string& file, std::string& directory) {
    
    std::string currentDirectory = workingDirectory();
    do {
        auto f = currentDirectory + Separator + file;
        if(fileExists(f)) {
            directory = currentDirectory;
            return true;
        }
        currentDirectory = currentDirectory.substr(0,currentDirectory.find_last_of(Separator));
    } while(currentDirectory.size() > 1);

    return false;
}

bool Filesystem::_hasTokens(const std::string& path) {
    size_t hasOpen = path.find_first_of(TokenOpen);
    size_t hasClose = path.find_first_of(TokenClose, hasOpen);
    return hasOpen != std::string::npos && hasClose != std::string::npos;
}

std::string Filesystem::_resolveTokens(const std::string& path) {
    
    std::string p = path;
    
    // limited loop to prevent
    for (int i = 0; i < 2 && _hasTokens(p); ++i) {
        size_t p1 = p.find_first_of(TokenOpen);
        size_t p2 = p.find_first_of(TokenClose, p1);
        std::string token = p.substr(p1,p2-p1 + TokenClose.size());
        auto it = _tokens.find(token);
        if (it != _tokens.end()) {
            p.replace(p1,p2-p1 + TokenClose.size(), (*it).second);
        }
    }
    return p;
}

std::string Filesystem::_absolutePath(const std::string& path, bool verbose) const {

    std::string result = path;
    
    char* b = nullptr;
    char errorBuffer[MAX_PATH_SIZE];
    memset(errorBuffer, 0, MAX_PATH_SIZE);
    
    b = realpath(path.c_str(), errorBuffer);
    if (b == NULL && verbose) {
        LERROR("Error resolving the real path. Problem part: '" << errorBuffer << "'");
    } else if(b != NULL) {
        result = {b};
    }
    return result;
}

std::vector<std::string> Filesystem::_list(int type, const std::string& path) const {
    std::vector<std::string> list;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            std::string name{ent->d_name};
            if (ent->d_type == type && name != "." && name != "..") {
                list.push_back(name);
            }
        }
        closedir (dir);
    } else {
        LERROR("Could not open directory: '" << path << "'");
    }
    return list;
}

bool Filesystem::_exists(int type, const std::string& path) const {
    struct stat buffer;
    const int statResult = stat(path.c_str(), &buffer);
    if (statResult != 0)
        return false;
    
    const int is = (((buffer.st_mode) & S_IFMT) == type);
    //const int isFile = S_ISREG(buffer.st_mode);
    if (is == 0)
        return false;
    
    return true;
}

}