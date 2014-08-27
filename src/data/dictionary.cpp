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

#include <owl/data/dictionary.h>

namespace {
    const std::string _loggerCat = "Dictionary";
}

namespace owl {

Dictionary::Dictionary() {}
Dictionary::Dictionary(const Dictionary& rhs): _map(rhs._map.begin(),rhs._map.end()) {}

Dictionary::iterator Dictionary::begin() noexcept {
    return _map.begin();
}

Dictionary::const_iterator Dictionary::begin() const noexcept {
    return _map.begin();
}

Dictionary::iterator Dictionary::end() noexcept {
    return _map.end();
}

Dictionary::const_iterator Dictionary::end() const noexcept {
    return _map.end();
}

std::vector<std::string> Dictionary::keys() const {
    std::vector<std::string> keys;
    for(auto m: _map) {
        keys.push_back(m.first);
    }
    return keys;
}

bool Dictionary::insert(const std::string& key, Any value){
    
    size_t sep = key.find_first_of(Separator);
    std::string k = key.substr(0, sep);
    
    // is this a single key?
    if(sep == std::string::npos) {
        _map[k] = value;
        return true;
    }
    
    // do I have this key?
    if(_map.count(k)==0) {
        _map[k] = Dictionary();
    } else if( ! _map[k].is<Dictionary>()) {
        return false;
    }
    
    return _map[k].as<Dictionary>().insert(_rest(key, sep), value);
}

bool Dictionary::insert(const std::string& key, const char* value){
    return insert(key, std::string(value));
}

std::string Dictionary::_rest(const std::string& key, size_t separator) const {
    return key.substr(separator+SeparatorLength,key.length() - separator - SeparatorLength);
}

}