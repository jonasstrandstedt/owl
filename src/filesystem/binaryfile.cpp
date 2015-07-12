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

#include <owl/filesystem/binaryfile.h>

namespace owl {

BinaryFile::BinaryFile(const std::string& filename, OpenMode mode)
{
    std::iostream::openmode m;
    switch(mode) {
        case OpenMode::In:
            m = std::iostream::in;
            break;
        case OpenMode::Out:
            m = std::iostream::out;
            break;
        case OpenMode::InOut:
            m = std::iostream::in | std::iostream::out;
            break;
    }
    
    _file.open(filename, std::iostream::binary | m);
}

BinaryFileIn::BinaryFileIn(const std::string& filename)
    : BinaryFile(filename, BinaryFile::OpenMode::In)
{}

BinaryFileOut::BinaryFileOut(const std::string& filename)
    : BinaryFile(filename, BinaryFile::OpenMode::Out)
{}

BinaryFile::~BinaryFile() {
    close();
}

    
bool BinaryFile::is_open() const {
    return _file.is_open();
}

BinaryFile::operator bool() const {
    return is_open();
}

void BinaryFile::close() {
    _file.close();
}

void BinaryFile::write(const char* s) {
    write(std::string(s));
}
    
void BinaryFile::write(const char* d, size_t length) {
    _file.write(d, length);
}

void BinaryFile::read(char* d, size_t length) {
    _file.read(d, length);
}

} // namespace owl