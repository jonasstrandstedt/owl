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

#include <owl/signal/signal.h>

#include <signal.h>
#ifdef __WIN32__

#ifndef SIGHUP
#define SIGHUP 1
#endif
#ifndef SIGQUIT
#define SIGQUIT 3
#endif
#ifndef SIGKILL
#define SIGKILL 9
#endif
#ifndef SIGSTOP
#define SIGSTOP 23
#endif
#ifndef SIGTSTP
#define SIGTSTP 20
#endif

#endif

namespace {
    const std::string _loggerCat = "SignalHandler";
}

namespace owl {

void owl_signal_handler(int signo) {
    owl::SignalHandler::ref().call(signo);
}

SignalHandler::SignalHandler() {
    for (auto &c: _callbacks) {
        c = nullptr;
    }
}

SignalHandler::~SignalHandler() {
}

int SignalHandler::signalnumber (Signal s) {
    switch (s) {
        case Signal::Hangup:
            return SIGHUP;
        case Signal::Abort:
            return SIGABRT;
        case Signal::Quit:
            return SIGQUIT;
        case Signal::IllegalInstruction:
            return SIGILL;
        case Signal::Interrupt:
            return SIGINT;
        case Signal::Kill:
            return SIGKILL;
        case Signal::Terminate:
            return SIGTERM;
        case Signal::Stop:
            return SIGSTOP;
        case Signal::TTYStop:
            return SIGTSTP;
        default:
            return 0;
    }
}

int SignalHandler::numberToSignalPosition (int signo) {
    switch (signo) {
        case SIGHUP:
            return 0;
        case SIGABRT:
            return 1;
        case SIGQUIT:
            return 2;
        case SIGILL:
            return 3;
        case SIGINT:
            return 4;
        case SIGKILL:
            return 5;
        case SIGTERM:
            return 6;
        case SIGSTOP:
            return 7;
        case SIGTSTP:
            return 8;
        default:
            return -1;
    }
}

void SignalHandler::setCallback(int s, SignalCallback callback) {
    
    void (*c)(int) = owl_signal_handler;
    if(callback == nullptr)
        c = SIG_DFL;
    
    _lock.lock();
    for (int i = 0; i < SignalHandler::NumberOfCallbacks; ++i) {
        int no = 1 << i;
        if(s & no) {
            int signr = signalnumber(static_cast<Signal>(no));
            if(signr > 0) {
                _callbacks[i] = callback;
                signal(signr, c);
            }
        }
    }
    _lock.unlock();
}
    
std::string SignalHandler::toString(Signal signal) {
    switch(signal) {
        case Signal::Hangup:
            return "Hangup";
        case Signal::Abort:
            return "Abort";
        case Signal::Quit:
            return "Quit";
        case Signal::IllegalInstruction:
            return "IllegalInstruction";
        case Signal::Interrupt:
            return "Interrupt";
        case Signal::Kill:
            return "Kill";
        case Signal::Terminate:
            return "Terminate";
        case Signal::Stop:
            return "Stop";
        case Signal::TTYStop:
            return "TTYStop";
        default:
            return "Unknown";
            break;
    }
}
    
std::string SignalHandler::toString(int signal) {
    switch(signal) {
        case SIGHUP :
            return toString(Signal::Hangup);
        case SIGABRT  :
            return toString(Signal::Abort);
        case SIGQUIT  :
            return toString(Signal::Quit);
        case SIGILL :
            return toString(Signal::IllegalInstruction);
        case SIGINT :
            return toString(Signal::Interrupt);
        case SIGKILL  :
            return toString(Signal::Kill);
        case SIGTERM  :
            return toString(Signal::Terminate);
        case SIGSTOP  :
            return toString(Signal::Stop);
        case SIGTSTP :
            return toString(Signal::TTYStop);
        default:
            return "Unknown";
            break;
    }
}

void SignalHandler::call(int signo) {
    int callbackpos = numberToSignalPosition(signo);
    if(callbackpos < 0)
        return;
    
    _lock.lock();
    
        if(_callbacks[callbackpos] != nullptr)
            _callbacks[callbackpos](static_cast<Signal>(1<<callbackpos));
    
    _lock.unlock();
}

}