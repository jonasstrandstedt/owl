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

#include <owl/data/serializer.h>
#include <owl/data/any.h>
#include <owl/data/dictionary.h>

namespace owl {

Serializer::size_type Serializer::bytes_left(std::istream& src) {
    size_type position = src.tellg();
    src.seekg(0, std::ios::end);
    size_type src_size = src.tellg();
    src_size -= position;
    
    // reset position
    src.seekg(position, std::ios::beg);
    
    return src_size;
}

void Serializer::deserialize_check(size_t size, std::istream& src) {
#if !defined(NDEBUG)
    size_type src_size = bytes_left(src);
    
    if(src_size == 0) {
        throw EmptyStream();
    }
    
    if(src_size < size) {
        throw InsufficientData();
    }
#endif
}

template<>
void Serializer::serialize(const std::string& v, std::ostream& out) {
    const size_type length = v.length();
    out.write(reinterpret_cast<const value_type*>(&length), sizeof(size_type));
    out.write(v.c_str(), sizeof(value_type)*length);
}

template<>
void Serializer::deserialize<std::string>(std::string& v, std::istream& src) {
    
    deserialize_check(sizeof(size_type), src);
    size_type length;
    src.read(reinterpret_cast<value_type*>(&length), sizeof(size_type));
    
    deserialize_check(length, src);
    std::vector<char> tmp(length);
    src.read(tmp.data(), sizeof(char)*length);
    
    v = std::string(reinterpret_cast<const char*>(tmp.data()), length);
}

template<>
void Serializer::serialize<Any>(const Any& v, std::ostream& out) {
	Any::serialize(v, out);
}

template<>
void Serializer::deserialize<Any>(Any& v, std::istream& src) {
	Any::deserialize(v, src);
}

template<>
void Serializer::serialize<Dictionary>(const Dictionary& v, std::ostream& out) {
    const size_type length = v.keys().size();
    out.write(reinterpret_cast<const value_type*>(&length), sizeof(size_type));

    for(const auto& e: v) {
    	const std::string& key = e.first;
    	const owl::Any& value = e.second;

    	serialize(key, out);
    	serialize(value, out);
    }
}

template<>
void Serializer::deserialize<Dictionary>(Dictionary& v, std::istream& src) {
    
    deserialize_check(sizeof(size_type), src);
    size_type length;
    src.read(reinterpret_cast<value_type*>(&length), sizeof(size_type));
    
    for(size_t i = 0; i < length; ++i) {
    	std::string key;
    	owl::Any value;

    	deserialize(key, src);
    	deserialize(value, src);

    	v.insert(key, value);
    }
}

} // namespace owl

