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

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>
#include <owl/data/serializer.h>
#include <iostream>
#include <sstream>

namespace owl {

class Buffer {
public:

    typedef Serializer::value_type value_type;
    typedef Serializer::size_type size_type;

    Buffer() = default;
    ~Buffer() = default;

    Buffer(const Buffer& other);
    Buffer(Buffer&& other);
    Buffer& operator=(const Buffer& rhs);
    Buffer& operator=(Buffer&& rhs);

    Buffer(const std::string& filename);

    void reset();

    bool write(const std::string& filename, bool compress = false);
    bool read(const std::string& filename);
    
    template<typename T>
    void serialize(const T& v);
    void serialize(const char* data);
    void serialize(const char* data, size_t size);

    template<typename T>
    void deserialize(T& v);
    void deserialize(char* data, size_t size);

private:

    Buffer(size_t) = delete;

    std::stringstream _stream;

}; // class Buffer

template<typename T>
void Buffer::serialize(const T& v) {
    Serializer::serialize(v, _stream);
}

template<typename T>
void Buffer::deserialize(T& v) {
    Serializer::deserialize(v, _stream);
}

} // namespace owl 

#endif