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
#include <owl/data/serializer.h>

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
    else if (is<std::string>())             return Any_std_string;
    else if (is<Dictionary>())              return Any_Dictionary;
    return Any_unsupported;
} 

void Any::serialize(const Any& v, std::ostream& out) {
    size_t type = v.type();
    Serializer::serialize(type, out);

    if(type == Any_char) {
        char value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_char16_t) {
        char16_t value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_char32_t) {
        char32_t value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_wchar_t) {
        wchar_t value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_short) {
        short value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_int) {
        int value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_long_int) {
        long int value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_long_long_int) {
        long long int value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_unsigned_short) {
        unsigned short value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_unsigned_int) {
        unsigned int value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_unsigned_long_int) {
        unsigned long int value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_unsigned_long_long_int) {
        unsigned long long int value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_float) {
        float value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_double) {
        double value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_long_double) {
        long double value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_bool) {
        bool value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_std_string) {
        std::string value = v;
        Serializer::serialize(value, out);
    } else if(type == Any_Dictionary) {
        Dictionary value = v;
        Serializer::serialize(value, out);
    }
}

void Any::deserialize(Any& v, std::istream& src) {
    size_t type;
    Serializer::deserialize(type, src);
    if(type == Any_char) {
        char value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_char16_t) {
        char16_t value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_char32_t) {
        char32_t value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_wchar_t) {
        wchar_t value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_short) {
        short value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_int) {
        int value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_long_int) {
        long int value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_long_long_int) {
        long long int value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_unsigned_short) {
        unsigned short value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_unsigned_int) {
        unsigned int value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_unsigned_long_int) {
        unsigned long int value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_unsigned_long_long_int) {
        unsigned long long int value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_float) {
        float value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_double) {
        double value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_long_double) {
        long double value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_bool) {
        bool value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_std_string) {
        std::string value;
        Serializer::deserialize(value, src);
        v = value;
    } else if(type == Any_Dictionary) {
        Dictionary value;
        Serializer::deserialize(value, src);
        v = value;
    } else {
        // Unsupported type
        v = Any();
    }
}
    
} // namespace owl