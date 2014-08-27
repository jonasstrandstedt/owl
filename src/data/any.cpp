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

namespace owl {

Any::Any(Any& that): ptr(that.clone()) {}
Any::Any(Any&& that): ptr(that.ptr) {
    that.ptr = nullptr;
}
Any::Any(const Any& that): ptr(that.clone()) {}
Any::Any(const Any&& that): ptr(that.clone()) {}

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
    return !ptr;
}

bool Any::not_null() const {
    return ptr;
}

Any::Base* Any::clone() const {
    if (ptr)
        return ptr->clone();
    else
        return nullptr;
}

std::string Any::typeName() {
    if(ptr)
        return ptr->name();
    return "nullptr";
}

size_t Any::typeHashCode() {
    if(ptr)
        return ptr->hash_code();
    return 0;
}
    
}