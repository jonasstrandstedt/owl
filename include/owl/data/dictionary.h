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

#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <owl/data/any.h>
#include <owl/data/typeinfo.h>
#include <owl/logging/logmanager.h>

#include <map>
#include <string>
#include <functional>

namespace owl {

class Dictionary {
    
    const std::string Separator = ".";
    const size_t SeparatorLength = Separator.length();

public:
    
    typedef std::map<std::string, owl::Any>::iterator iterator;
    typedef std::map<std::string, owl::Any>::const_iterator const_iterator;
    
    Dictionary();
    Dictionary(const Dictionary& rhs);
    Dictionary operator=(const Dictionary& rhs);
    
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    
    iterator end() noexcept;
    const_iterator end() const noexcept;
    
    std::vector<std::string> keys() const;

    bool insert(const std::string& key, Any value);
    bool insert(const std::string& key, const char* value);
    
    template<class T>
    bool get(const std::string& key, T& value) const {
        size_t sep = key.find_first_of(Separator);
        std::string k = key.substr(0, sep);
        
        // do I have this key?
        if(_map.count(k)==0) {
            return false;
        }
        
        // is this a single key?
        if(sep == std::string::npos) {
            Any a = _map.at(k);
            if(a.is<T>()) {
                value = a.as<T>();
                return true;
            }
            LWARNINGC("Dictionary","Wrong type, expected '" << TypeInfo::name<T>() << "', got '" << a.typeName() << "'");
            return false;
        }
        
        // Expecting a dictionary
        if( ! _map.at(k).is<Dictionary>()) {
            return false;
        }
        return _map.at(k).as<Dictionary>().get(_rest(key, sep), value);
    };
    
    template<class T>
    T getValidate(const std::string& key, std::function<void(T&)> f = [](T& value) {}) const {
        T value{};
        get(key, value);
        f(value);
        return value;
    }

private:

    // Helper functions
    std::string _rest(const std::string& key, size_t separator) const;

    // Member variables
    std::map<std::string, owl::Any> _map;

};

}

#endif