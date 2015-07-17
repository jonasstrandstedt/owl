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

#include <owl/data/any.h>
#include <owl/data/dictionary.h>

namespace owl {

Any::Any(Any& that): ptr(that.clone()) {}

Any::Any(Any&& that): ptr(that.ptr) {
    that.ptr = nullptr;
}

Any::Any(const Any& that): ptr(that.clone()) {}

Any::Any(const Any&& that): ptr(that.clone()) {}

Any::Any(const char* value)
    : ptr(new Derived<StorageType<std::string> >(std::string(value)))
{}

Any& Any::operator=(const Any& a) {
    if (ptr == a.ptr)
        return *this;
    
    auto old_ptr = ptr;
    
    ptr = a.clone();
    
    if (old_ptr)
        delete old_ptr;
    
    return *this;
}

Any& Any::operator=(Any&& a) {
    if (ptr == a.ptr)
        return *this;
    
    std::swap(ptr, a.ptr);
    
    return *this;
}

Any::~Any() {
    if (ptr)
        delete ptr;
}

bool Any::is_null() const {
    return ptr == nullptr;
}

bool Any::not_null() const {
    return ptr != nullptr;
}

Any::Base* Any::clone() const {
    if (ptr)
        return ptr->clone();
    else
        return nullptr;
}

std::string Any::typeName() const {
    if(ptr)
        return ptr->name();
    return "nullptr";
}

size_t Any::type() const {
    if(is<char>())                          return Any_char;
    else if(is<char16_t>())                 return Any_char16_t;
    else if(is<char32_t>())                 return Any_char32_t;
    else if(is<wchar_t>())                  return Any_wchar_t;
    else if(is<short>())                    return Any_short;
    else if(is<int>())                      return Any_int;
    else if(is<long int>())                 return Any_long_int;
    else if(is<long long int>())            return Any_long_long_int;
    else if(is<unsigned short>())           return Any_unsigned_short;
    else if(is<unsigned int>())             return Any_unsigned_int;
    else if(is<unsigned long int>())        return Any_unsigned_long_int;
    else if(is<unsigned long long int>())   return Any_unsigned_long_long_int;
    else if(is<float>())                    return Any_float;
    else if(is<double>())                   return Any_double;
    else if(is<long double>())              return Any_long_double;
    else if(is<bool>())                     return Any_bool;
    else if(is<std::string>())              return Any_std_string;
    else if(is<Dictionary>())               return Any_Dictionary;
    else if(is<std::vector<char> >())                    return Any_vector_char;
    else if(is<std::vector<char16_t>>())                 return Any_vector_char16_t;
    else if(is<std::vector<char32_t>>())                 return Any_vector_char32_t;
    else if(is<std::vector<wchar_t>>())                  return Any_vector_wchar_t;
    else if(is<std::vector<short>>())                    return Any_vector_short;
    else if(is<std::vector<int>>())                      return Any_vector_int;
    else if(is<std::vector<long int>>())                 return Any_vector_long_int;
    else if(is<std::vector<long long int>>())            return Any_vector_long_long_int;
    else if(is<std::vector<unsigned short>>())           return Any_vector_unsigned_short;
    else if(is<std::vector<unsigned int>>())             return Any_vector_unsigned_int;
    else if(is<std::vector<unsigned long int>>())        return Any_vector_unsigned_long_int;
    else if(is<std::vector<unsigned long long int>>())   return Any_vector_unsigned_long_long_int;
    else if(is<std::vector<float>>())                    return Any_vector_float;
    else if(is<std::vector<double>>())                   return Any_vector_double;
    else if(is<std::vector<long double>>())              return Any_vector_long_double;
    else if(is<std::vector<std::string>>())              return Any_vector_std_string;
    else if(is<std::vector<Dictionary>>())               return Any_vector_Dictionary;
    
    return Any_unsupported;
} 

void Any::serialize(const Any& v, std::ostream& out) {
    size_t type = v.type();
    Serializer::serialize(type, out);

    if(type == Any_unsupported) {
        return;
    }

    if(type == Any_char)                         serialize<char>(v, out);
    else if(type == Any_char16_t)                serialize<char16_t>(v, out);
    else if(type == Any_char32_t)                serialize<char32_t>(v, out);
    else if(type == Any_wchar_t)                 serialize<wchar_t>(v, out);
    else if(type == Any_short)                   serialize<short>(v, out);
    else if(type == Any_int)                     serialize<int>(v, out);
    else if(type == Any_long_int)                serialize<long int>(v, out);
    else if(type == Any_long_long_int)           serialize<long long int>(v, out);
    else if(type == Any_unsigned_short)          serialize<unsigned short>(v, out);
    else if(type == Any_unsigned_int)            serialize<unsigned int>(v, out);
    else if(type == Any_unsigned_long_int)       serialize<unsigned long int>(v, out);
    else if(type == Any_unsigned_long_long_int)  serialize<unsigned long long int>(v, out);
    else if(type == Any_float)                   serialize<float>(v, out);
    else if(type == Any_double)                  serialize<double>(v, out);
    else if(type == Any_long_double)             serialize<long double>(v, out);
    else if(type == Any_bool)                    serialize<bool>(v, out);
    else if(type == Any_std_string)              serialize<std::string>(v, out);
    else if(type == Any_Dictionary)              serialize<Dictionary>(v, out);
    else if(type == Any_vector_char)                    serialize<std::vector<char>>(v, out);
    else if(type == Any_vector_char16_t)                serialize<std::vector<char16_t>>(v, out);
    else if(type == Any_vector_char32_t)                serialize<std::vector<char32_t>>(v, out);
    else if(type == Any_vector_wchar_t)                 serialize<std::vector<wchar_t>>(v, out);
    else if(type == Any_vector_short)                   serialize<std::vector<short>>(v, out);
    else if(type == Any_vector_int)                     serialize<std::vector<int>>(v, out);
    else if(type == Any_vector_long_int)                serialize<std::vector<long int>>(v, out);
    else if(type == Any_vector_long_long_int)           serialize<std::vector<long long int>>(v, out);
    else if(type == Any_vector_unsigned_short)          serialize<std::vector<unsigned short>>(v, out);
    else if(type == Any_vector_unsigned_int)            serialize<std::vector<unsigned int>>(v, out);
    else if(type == Any_vector_unsigned_long_int)       serialize<std::vector<unsigned long int>>(v, out);
    else if(type == Any_vector_unsigned_long_long_int)  serialize<std::vector<unsigned long long int>>(v, out);
    else if(type == Any_vector_float)                   serialize<std::vector<float>>(v, out);
    else if(type == Any_vector_double)                  serialize<std::vector<double>>(v, out);
    else if(type == Any_vector_long_double)             serialize<std::vector<long double>>(v, out);
    else if(type == Any_vector_std_string)              serialize<std::vector<std::string>>(v, out);
    else if(type == Any_vector_Dictionary)              serialize<std::vector<Dictionary>>(v, out);
    
}

void Any::deserialize(Any& v, std::istream& src) {
    size_t type;
    Serializer::deserialize(type, src);
    if(type == Any_char)                         deserialize<char>(v, src);
    else if(type == Any_char16_t)                deserialize<char16_t>(v, src);
    else if(type == Any_char32_t)                deserialize<char32_t>(v, src);
    else if(type == Any_wchar_t)                 deserialize<wchar_t>(v, src);
    else if(type == Any_short)                   deserialize<short>(v, src);
    else if(type == Any_int)                     deserialize<int>(v, src);
    else if(type == Any_long_int)                deserialize<long int>(v, src);
    else if(type == Any_long_long_int)           deserialize<long long int>(v, src);
    else if(type == Any_unsigned_short)          deserialize<unsigned short>(v, src);
    else if(type == Any_unsigned_int)            deserialize<unsigned int>(v, src);
    else if(type == Any_unsigned_long_int)       deserialize<unsigned long int>(v, src);
    else if(type == Any_unsigned_long_long_int)  deserialize<unsigned long long int>(v, src);
    else if(type == Any_float)                   deserialize<float>(v, src);
    else if(type == Any_double)                  deserialize<double>(v, src);
    else if(type == Any_long_double)             deserialize<long double>(v, src);
    else if(type == Any_bool)                    deserialize<bool>(v, src);
    else if(type == Any_std_string)              deserialize<std::string>(v, src);
    else if(type == Any_Dictionary)              deserialize<Dictionary>(v, src);
    else if(type == Any_vector_char)                    deserialize<std::vector<char>>(v, src);
    else if(type == Any_vector_char16_t)                deserialize<std::vector<char16_t>>(v, src);
    else if(type == Any_vector_char32_t)                deserialize<std::vector<char32_t>>(v, src);
    else if(type == Any_vector_wchar_t)                 deserialize<std::vector<wchar_t>>(v, src);
    else if(type == Any_vector_short)                   deserialize<std::vector<short>>(v, src);
    else if(type == Any_vector_int)                     deserialize<std::vector<int>>(v, src);
    else if(type == Any_vector_long_int)                deserialize<std::vector<long int>>(v, src);
    else if(type == Any_vector_long_long_int)           deserialize<std::vector<long long int>>(v, src);
    else if(type == Any_vector_unsigned_short)          deserialize<std::vector<unsigned short>>(v, src);
    else if(type == Any_vector_unsigned_int)            deserialize<std::vector<unsigned int>>(v, src);
    else if(type == Any_vector_unsigned_long_int)       deserialize<std::vector<unsigned long int>>(v, src);
    else if(type == Any_vector_unsigned_long_long_int)  deserialize<std::vector<unsigned long long int>>(v, src);
    else if(type == Any_vector_float)                   deserialize<std::vector<float>>(v, src);
    else if(type == Any_vector_double)                  deserialize<std::vector<double>>(v, src);
    else if(type == Any_vector_long_double)             deserialize<std::vector<long double>>(v, src);
    else if(type == Any_vector_std_string)              deserialize<std::vector<std::string>>(v, src);
    else if(type == Any_vector_Dictionary)              deserialize<std::vector<Dictionary>>(v, src);
    else {
        // Unsupported type
        v = Any();
    }
}
    
} // namespace owl