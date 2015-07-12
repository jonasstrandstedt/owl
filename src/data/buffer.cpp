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

#include <owl/data/buffer.h>
#include <owl/filesystem/binaryfile.h>

#include <lz4/lz4.h>

#include <algorithm>

namespace owl {

Buffer::Buffer(const Buffer& other) {
    // copy memory
    _stream << other._stream.rdbuf();
}
Buffer::Buffer(Buffer&& other) {
    if(this != &other) {
        // move memory
        _stream = std::move(other._stream);
    }
}
Buffer& Buffer::operator=(const Buffer& rhs) {
    if(this != &rhs) {
        // copy memory
        _stream << rhs._stream.rdbuf();
    }
    return *this;
}
Buffer& Buffer::operator=(Buffer&& rhs) {
    if(this != &rhs) {
        // move memory
        _stream = std::move(rhs._stream);
    }
    return *this;
}

Buffer::Buffer(const std::string& filename)
{
    read(filename);
}

void Buffer::reset() {
    _stream = std::stringstream();
}

bool Buffer::write(const std::string& filename, bool compress) {
    BinaryFile file(filename, BinaryFile::OpenMode::Out);
    if (!file)
        return false;

    file.write(compress);

    const std::string& tmp = _stream.str();
    const char* data = tmp.c_str();

    _stream.seekg(0, std::ios::end);
    size_type size = _stream.tellg();
    _stream.seekg(0, std::ios::beg);
    file.write(size);  

    if(compress) {
        value_type* compressedData = new value_type[size];
        int compressed_size = LZ4_compress(reinterpret_cast<const char*>(data),
                                           reinterpret_cast<char*>(compressedData),
                                           size);
        if (compressed_size <= 0) {
            delete[] compressedData;
            return false;
        }                         // orginal size
        file.write(compressed_size);               // compressed size
        file.write(reinterpret_cast<const char*>(compressedData), compressed_size);
        delete[] compressedData;
    } else {
        file.write(reinterpret_cast<const char*>(data), size);
    }
    return true;
}

bool Buffer::read(const std::string& filename) {
    BinaryFile file(filename, BinaryFile::OpenMode::In);
    if (!file)
        return false;
    
    size_t size;
    bool compressed;

    file.read(compressed);
    file.read(size);
    value_type* data = new value_type[size];
    if(compressed) {
        
        // read compressed size
        int compressed_size;
        file.read(compressed_size);
        
        // allocate and read data
        value_type* compressedData = new value_type[compressed_size];
        file.read(reinterpret_cast<char*>(compressedData),compressed_size);
        
        // decompress
        LZ4_decompress_safe(reinterpret_cast<const char*>(compressedData),
                                           reinterpret_cast<char*>(data),
                                           compressed_size,
                                           size);

        _stream.write(data, size);
        delete[] compressedData;
    } else {

        file.read(reinterpret_cast<char*>(data),size);
        _stream.write(data, size);
    }
    delete[] data;
    return true;
}

void Buffer::serialize(const char* data) {
    serialize(std::string(data));
}

void Buffer::serialize(const char* data, size_t size) {
    _stream.write(data, size);
}

void Buffer::deserialize(char* data, size_t size) {
    _stream.read(data, size);
}

} // namespace owl

