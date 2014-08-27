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
#include <utility>
#include <typeinfo>
#include <cassert>
#include <string>

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
    
    Any& operator=(const Any& a);
    Any& operator=(Any&& a);
    
    ~Any();
    
    bool is_null() const;
    bool not_null() const;
    
    // Template functions
    
    // Construct Any from any type
    template<typename U>
    Any(U&& value) : ptr(new Derived<StorageType<U>>(std::forward<U>(value))) {}
    
    template<class U> bool is() const {
        typedef StorageType<U> T;
        
        auto derived = dynamic_cast<Derived<T>*> (ptr);
        
        return derived;
    }
    
    template<class U>
    StorageType<U>& as() const {
        typedef StorageType<U> T;
        
        auto derived = dynamic_cast<Derived<T>*> (ptr);
        
        if (!derived)
            throw std::bad_cast();
        
        return derived->value;
    }
    
    template<class U>
    operator U() {
        return as<StorageType<U>>();
    }
    
    template<class U>
    bool get(U& v) {
        typedef StorageType<U> T;
        
        auto derived = dynamic_cast<Derived<T>*> (ptr);
        
        if (!derived)
            return false;
        
        v = derived->value;
        return true;
    }
    
    std::string typeName();
    size_t typeHashCode();
    
private:
    struct Base {
        virtual ~Base() {}
        virtual Base* clone() const = 0;
        virtual std::string name() = 0;
        virtual size_t hash_code() = 0;
    };
    
    template<typename T>
    struct Derived : Base {
        template<typename U> Derived(U&& value) : value(std::forward<U>(value)) { }
        
        T value;
        
        Base* clone() const { return new Derived<T>(value); }
        std::string name() { return TypeInfo::name<T>(); };
        size_t hash_code() { return typeid(T).hash_code(); };
    };
    
    Base* clone() const;
    
    Base* ptr;
};

}

#endif
