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

#ifndef __BINARYFILE_H__
#define __BINARYFILE_H__

#include <owl/data/serializer.h>
 
#include <iostream>
#include <fstream>
#include <string>

namespace owl {

class BinaryFile {
public:

    enum class OpenMode {
        In, Out, InOut
    };

    BinaryFile(const std::string& filename, OpenMode mode);
    ~BinaryFile();
    
    bool is_open() const;
    operator bool() const;
    
    void close();
    
    void write(const char* s);
    void write(const char* d, size_t length);
    template<class T> void write(const T& v);
    
    void read(char* d, size_t length);
    template<class T> void read(T& v);
    
private:
    std::fstream _file;
}; // class BinaryFile

class BinaryFileIn: public BinaryFile {
public:
    BinaryFileIn(const std::string& filename);
    
private:
    void write(const char* s) = delete;
    void write(const char* d, size_t length) = delete;
    template<class T> void write(const T& v) = delete;
    
}; // class BinaryFileIn

class BinaryFileOut: public BinaryFile {
public:
    BinaryFileOut(const std::string& filename);
    
private:
    void read(char* d, size_t length) = delete;
    template<class T> void read(T& v) = delete;
    
}; // class BinaryFileOut

template<class T>
void BinaryFile::write(const T& v) {
    Serializer::serialize(v, _file);
}

template<class T>
void BinaryFile::read(T& v) {
    Serializer::deserialize(v, _file);
}

} // namespace owl

#endif