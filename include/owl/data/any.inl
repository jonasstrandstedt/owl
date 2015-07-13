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

template<typename U>
Any::Any(U&& value) : ptr(new Derived<StorageType<U>>(std::forward<U>(value))) {}

template<typename U>
bool Any::is() const {
    typedef StorageType<U> T;
    
    auto derived = dynamic_cast<Derived<T>*> (ptr);
    
    return derived;
}

template<typename U>
StorageType<U>& Any::as() const {
    typedef StorageType<U> T;
    
    auto derived = dynamic_cast<Derived<T>*> (ptr);
    
    if (!derived)
        throw std::bad_cast();
    
    return derived->value;
}

template<typename U>
Any::operator U() const{
    return as<StorageType<U>>();
}

template<typename U>
bool Any::get(U& v) const {
    typedef StorageType<U> T;
    
    auto derived = dynamic_cast<Derived<T>*> (ptr);
    
    if (!derived)
    return false;
    
    v = derived->value;
    return true;
}

template<typename T>
template<typename U>
Any::Derived<T>::Derived(U&& value)
    : value(std::forward<U>(value))
{}

template<typename T>
Any::Base* Any::Derived<T>::clone() const {
    return new Derived<T>(value);
}

template<typename T>
std::string Any::Derived<T>::name() const {
    return TypeInfo::name<T>();
}
