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

#ifndef __ANY_H__
#define __ANY_H__

#include <owl/data/typeinfo.h>

#include <type_traits>
#include <typeinfo>
#include <string>
#include <functional>
#include <iostream>

template <class T>
using StorageType = typename std::decay<T>::type;

namespace owl {

class Any {
public:
    
    // Construct Any from any Any
    Any() : ptr(nullptr) {}
    Any(Any& that);
    Any(Any&& that);
    Any(const Any& that);
    Any(const Any&& that);
    Any(const char* value);
    
    Any& operator=(const Any& a);
    Any& operator=(Any&& a);
    
    ~Any();
    
    bool is_null() const;
    bool not_null() const;
    
    // Template functions
    
    // Construct Any from any type
    template<typename U> Any(U&& value);
    template<typename U> bool is() const;
    template<typename U> StorageType<U>& as() const;
    template<typename U> operator U() const;
    template<typename U> bool get(U& v) const;
    
    std::string typeName() const;

    static void serialize(const Any& v, std::ostream& out);
    static void deserialize(Any& v, std::istream& src);
    
private:

    enum AnyTypeId {
        Any_unsupported                 = 0,
        Any_char                        = 1,
        Any_char16_t                    = 2,
        Any_char32_t                    = 3,
        Any_wchar_t                     = 4,
        Any_short                       = 5,
        Any_int                         = 6,
        Any_long_int                    = 7,
        Any_long_long_int               = 8,
        Any_unsigned_short              = 9,
        Any_unsigned_int                = 10,
        Any_unsigned_long_int           = 11,
        Any_unsigned_long_long_int      = 12,
        Any_float                       = 13,
        Any_double                      = 14,
        Any_long_double                 = 15,
        Any_bool                        = 16,
        Any_std_string                  = 17,
        Any_Dictionary                  = 18
    };

    size_t type() const;

    struct Base {
        virtual ~Base() {}
        virtual Base* clone() const = 0;
        virtual std::string name() const = 0;
    }; // Base
    
    template<typename T>
    struct Derived : Base {
        
        T value;
        
        template<typename U> Derived(U&& value);
        Base* clone() const;
        std::string name() const;
    }; // Derived
    
    Base* clone() const;
    Base* ptr;
    
}; // Any

#include <owl/data/any.inl>

} // namespace owl

#endif
