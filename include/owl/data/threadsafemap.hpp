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

#ifndef __THREADSAFEMAP_H__
#define __THREADSAFEMAP_H__

#include <owl/logging/logmanager.h>

#include <map>
#include <mutex>
#include <algorithm>

namespace owl {

/*****************************************************************************************
 *                                                                                       *
 * This map wrapper is NOT entierly thread safe. IT only makes it a bit easier  to use   *
 * in threaded environments. When accessing/changing contents it is safer to manually    *
 * lock beforehand.                                                                      *
 *                                                                                       *
 ****************************************************************************************/
template<class Key, class T>
class ThreadSafeMap {
public:
    
    /********************************************
     * Member functions                         *
     ********************************************/
    template <typename... Args>
    ThreadSafeMap(Args... args) {
        _map = new std::map<Key, T>(std::forward<Args>(args)...);
    }
    
    ~ThreadSafeMap() {
        delete _map;
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
    typename std::map<Key, T>::iterator begin() noexcept {
        return _map->begin();
    }
    
    typename std::map<Key, T>::const_iterator begin() const noexcept {
        return _map->begin();
    }
    
    typename std::map<Key, T>::iterator end() noexcept {
        return _map->end();
    }
    
    typename std::map<Key, T>::const_iterator end() const noexcept {
        return _map->end();
    }
    
    /********************************************
     * Capacity                                 *
     ********************************************/
    size_t size() const {
        lock();
        size_t s = _map->size();
        unlock();
        return s;
    }
    
    size_t max_size() const {
        lock();
        size_t s = _map->max_size();
        unlock();
        return s;
    }
    
    bool empty() const {
        lock();
        bool e = _map->empty();
        unlock();
        return e;
    }
    
    /********************************************
     * Modifiers                                *
     ********************************************/
    std::pair<typename std::map<Key, T>::iterator,bool> insert(const T& val) {
        lock();
        auto it = _map->insert(val);
        unlock();
        return it;
    }
    
    template <class InputIterator>
    typename std::map<Key, T>::iterator insert(typename std::map<Key, T>::const_iterator position, InputIterator first, InputIterator last) {
        lock();
        auto it = _map->insert(position, first, last);
        unlock();
        return it;
    }
    
    typename std::map<Key, T>::iterator insert (typename std::map<Key, T>::const_iterator position, T&& val) {
        lock();
        auto it = _map->insert(position, val);
        unlock();
        return it;
    }
    
    typename std::map<Key, T>::iterator insert (typename std::map<Key, T>::const_iterator position, std::initializer_list<T> il) {
        lock();
        auto it = _map->insert(position, il);
        unlock();
        return it;
    }
    
    typename std::map<Key, T>::iterator erase(typename std::map<Key, T>::const_iterator position) {
        lock();
        auto it = _map->erase(position);
        unlock();
        return it;
    }
    
    size_t erase(const Key& k) {
        lock();
        auto it = _map->erase(k);
        unlock();
        return it;
    }
    
    typename std::map<Key, T>::iterator erase(typename std::map<Key, T>::const_iterator begin, typename std::map<Key, T>::const_iterator end) {
        lock();
        auto it = _map->erase(begin, end);
        unlock();
        return it;
    }
    
    /********************************************
     * Element access (not thread safe!)        *
     ********************************************/
    T& at(const Key& k) {
        lock();
        auto ret = _map->at(k);
        unlock();
        return ret;
    }
    
    const T& at(const Key& k) const {
        return _map->at(k);
    }
    
    T& operator[](const Key& k) {
        lock();
        auto ret = _map->operator[](k);
        unlock();
        return ret;
    }
    
    const T& operator[](const Key& k) const {
        return _map->operator[](k);
    }
    
    /********************************************
     * Operations                               *
     ********************************************/
    typename std::map<Key, T>::iterator find(const Key& k) {
        lock();
        auto it = _map->find(k);
        unlock();
        return it;
    }
    
    typename std::map<Key, T>::const_iterator find(const Key& k) const {
        lock();
        auto it = _map->find(k);
        unlock();
        return it;
    }
    
    /********************************************
     * Vector access                            *
     ********************************************/
    std::map<Key, T>* getMap() {
        return _map;
    }

private:
    std::mutex _lock;
    std::map<Key, T>* _map;
    
}; // ThreadSafeVector
}  // owl

#endif
