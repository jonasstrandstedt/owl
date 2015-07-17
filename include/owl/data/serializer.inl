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

template<typename T> 
void Serializer::serialize(const T& v, std::ostream& out) {
    out.write(reinterpret_cast<const value_type*>(&v), sizeof(T));
}

template<class T> 
void Serializer::serialize(const std::vector<T>& v, std::ostream& out) {
    size_type n = v.size();
    out.write(reinterpret_cast<const value_type*>(&n), sizeof(size_type));
    if(std::is_pod<T>::value) {
        out.write(reinterpret_cast<const value_type*>(v.data()), sizeof(T)*n);
    } else {
        for(const auto& e: v) {
            serialize(e, out);
        }
    }
}

template<typename T>
void Serializer::deserialize(T& v, std::istream& src) {
    deserialize_check(sizeof(T), src);
    src.read(reinterpret_cast<value_type*>(&v), sizeof(T));
}

template<class T>
void Serializer::deserialize(std::vector<T>& v, std::istream& src) {
    deserialize_check(sizeof(size_type), src);
    size_type n;
    src.read(reinterpret_cast<value_type*>(&n), sizeof(size_type));
    if(std::is_pod<T>::value) {
        deserialize_check(sizeof(T)*n, src);
        v.resize(n);
        src.read(reinterpret_cast<value_type*>(v.data()), sizeof(T)*n);
    } else {
        v.reserve(n);
        for(size_t i = 0; i < n; ++i) {
            T t;
            deserialize(t, src);
            v.emplace_back(t);
        }
    }
}
