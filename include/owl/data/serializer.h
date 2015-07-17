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

#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include <string>
#include <vector>
#include <iostream>
#include <exception>

namespace owl {

class Dictionary;
class Any;

class Serializer {
public:

    struct SerializerException : std::exception {
        const char* what() const noexcept {return "SerializerException";}
    };
    struct EmptyStream : SerializerException {
        const char* what() const noexcept {return "EmptyStream";}
    };
    struct InsufficientData : SerializerException {
        const char* what() const noexcept {return "InsufficientData";}
    };

    typedef char value_type;
    typedef size_t size_type;

    template<typename T> 
    static void serialize(const T& v, std::ostream& out);
    template<class T> 
    static void serialize(const std::vector<T>& v, std::ostream& out) ;
    
    template<typename T> 
    static void deserialize(T& v, std::istream& src);
    template<class T>
    static void deserialize(std::vector<T>& v, std::istream& src);
    
    static size_type bytes_left(std::istream& src);
    
private:
    static void deserialize_check(size_t size, std::istream& src);
    
}; // class Serializer

// Specializations
template<>
void Serializer::serialize<std::string>(const std::string& v, std::ostream& out);
template<>
void Serializer::deserialize<std::string>(std::string& v, std::istream& src);
template<>
void Serializer::serialize<Dictionary>(const Dictionary& v, std::ostream& out);
template<>
void Serializer::deserialize<Dictionary>(Dictionary& v, std::istream& src);
template<>
void Serializer::serialize<Any>(const Any& v, std::ostream& out);
template<>
void Serializer::deserialize<Any>(Any& v, std::istream& src);

#include <owl/data/serializer.inl>

} // namespace owl


#endif
