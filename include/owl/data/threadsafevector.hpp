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

#ifndef __THREADSAFEVECTOR_H__
#define __THREADSAFEVECTOR_H__

#include <owl/logging/logmanager.h>

#include <vector>
#include <mutex>
#include <algorithm>

namespace owl {

/*****************************************************************************************
 *                                                                                       *
 * This vector wrapper is NOT entierly thread safe. IT only makes it a bit easier  to    *
 * use in threaded environments. When accessing/changing contents it is safer to         *
 * manually lock beforehand.                                                             *
 *                                                                                       *
 ****************************************************************************************/
template<class T>
class ThreadSafeVector {
public:
    
    /********************************************
     * Member functions                         *
     ********************************************/
    template <typename... Args>
    ThreadSafeVector(Args... args) {
        _vector = new std::vector<T>(std::forward<Args>(args)...);
    }
    
    ~ThreadSafeVector() {
        delete _vector;
    }
    
    /********************************************
     * Locks                                    *
     ********************************************/
    void lock() {
        _lock.lock();
    }
    
    bool try_lock() {
        return _lock.try_lock();
    }
    
    void unlock() {
        _lock.unlock();
    }
    
    /********************************************
     * Iterators                                *
     ********************************************/
    typename std::vector<T>::iterator begin() noexcept {
        return _vector->begin();
    }
    
    typename std::vector<T>::const_iterator begin() const noexcept {
        return _vector->begin();
    }
    
    typename std::vector<T>::iterator end() noexcept {
        return _vector->end();
    }
    
    typename std::vector<T>::const_iterator end() const noexcept {
        return _vector->end();
    }
    
    /********************************************
     * Capacity                                 *
     ********************************************/
    size_t size() const {
        lock();
        size_t s = _vector->size();
        unlock();
        return s;
    }
    
    size_t max_size() const {
        lock();
        size_t s = _vector->max_size();
        unlock();
        return s;
    }
    
    void resize(size_t n) {
        lock();
        _vector->resize(n);
        unlock();
    }
    
    size_t capacity() const {
        lock();
        size_t s = _vector->capacity();
        unlock();
        return s;
    }
    
    bool empty() const {
        lock();
        bool e = _vector->empty();
        unlock();
        return e;
    }
    
    void reserve(size_t n) {
        lock();
        _vector->reserve(n);
        unlock();
    }
    
    void shrink_to_fit() {
        lock();
        _vector->shrink_to_fit();
        unlock();
    }
    
    /********************************************
     * Modifiers                                *
     ********************************************/
    void push_back(const T& val) {
        lock();
        _vector->push_back(val);
        unlock();
    }
    
    void pop_back() {
        lock();
        _vector->pop_back();
        unlock();
    }
    
    typename std::vector<T>::iterator insert(typename std::vector<T>::const_iterator position, const T& val) {
        lock();
        auto it = _vector->insert(position, val);
        unlock();
        return it;
    }
    
    template <class InputIterator>
    typename std::vector<T>::iterator insert(typename std::vector<T>::const_iterator position, InputIterator first, InputIterator last) {
        lock();
        auto it = _vector->insert(position, first, last);
        unlock();
        return it;
    }
    
    typename std::vector<T>::iterator insert (typename std::vector<T>::const_iterator position, T&& val) {
        lock();
        auto it = _vector->insert(position, val);
        unlock();
        return it;
    }
    
    typename std::vector<T>::iterator insert (typename std::vector<T>::const_iterator position, std::initializer_list<T> il) {
        lock();
        auto it = _vector->insert(position, il);
        unlock();
        return it;
    }
    
    void remove(const T& val) {
        auto cmp = [&val](const T& v) {
            return val == v;
        };
        lock();
        auto p = std::remove_if(_vector->begin(), _vector->end(), cmp);
        _vector->erase(p, _vector->end());
        unlock();
    }
    
    void remove_all() {
        lock();
        _vector->erase(_vector->begin(), _vector->end());
        unlock();
    }
    
    /********************************************
     * Element access (not thread safe!)        *
     ********************************************/
    T& at(size_t n) {
        return _vector->at(n);
    }
    
    const T& at(size_t n) const {
        return _vector->at(n);
    }
    
    T& operator[](size_t n) {
        return _vector->operator[](n);
    }
    
    const T& operator[](size_t n) const {
        return _vector->operator[](n);
    }
    
    /********************************************
     * Algorithms                               *
     ********************************************/
    T* find(const T& val) {
        lock();
        auto e = std::find(_vector->begin(), _vector->end(), val);
        if(e == _vector->end())
            e = nullptr;
        unlock();
        return e;
    }
    
    void sort(std::function<bool(const T& v1, const T& v2)> cmp) {
        lock();
        std::sort(_vector->begin(), _vector->end(), cmp);
        unlock();
    }
    
    /********************************************
     * Vector access                            *
     ********************************************/
    std::vector<T>* getVector() {
        return _vector;
    }

private:
    std::mutex _lock;
    std::vector<T>* _vector;
    
}; // ThreadSafeVector
}  // owl

#endif
