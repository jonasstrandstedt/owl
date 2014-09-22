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

#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <owl/designpattern/singleton.hpp>

#include <functional>
#include <mutex>
#include <array>

namespace owl {

class SignalHandler: public Singleton<SignalHandler> {
public:
/*
    enum class Signal {
        Hangup, Abort, Quit, IllegalInstruction, Interrupt, Kill, Terminate, Stop, TTYStop
    };
  */
    
    static const int NumberOfCallbacks = 9;
    
    enum Signal {
        Hangup              = 1 << 0,
        Abort               = 1 << 1,
        Quit                = 1 << 2,
        IllegalInstruction  = 1 << 3,
        Interrupt           = 1 << 4,
        Kill                = 1 << 5,
        Terminate           = 1 << 6,
        Stop                = 1 << 7,
        TTYStop             = 1 << 8,
        All                 = Hangup | Abort | Quit | IllegalInstruction | Interrupt | Kill | Terminate | Stop | TTYStop
    };

    typedef std::function<void(Signal)> SignalCallback;

    SignalHandler();
    ~SignalHandler();
    
    void setCallback(int signals, SignalCallback callback);
    
    static std::string toString(Signal signal);
    static std::string toString(int signal);
    
    
private:
    friend void owl_signal_handler(int signo);
    void call(int signo);

    std::mutex _lock;
    
    std::array<SignalCallback, NumberOfCallbacks> _callbacks;
    
    int signalnumber (Signal s);
    int numberToSignalPosition (int signo);
    
}; // StreamLog
}  // owl 

#endif
