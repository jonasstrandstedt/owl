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


#include <owl/logging/streamlog.h>
#include <owl/time/time.h>

#include <iomanip>
#include <iostream>

namespace {
    int roundUp(int numToRound, int multiple) {
        if(multiple < 2) {
            return numToRound;
        }
        
        int remainder = numToRound % multiple;
        if (remainder == 0) {
            return numToRound;
        }
        
        return numToRound + multiple - remainder; 
    }
}

namespace owl {

StreamLog::StreamLog(std::ostream& stream, LogLevel level): Log(level), _stream(stream) {}

StreamLog::~StreamLog() {}

void StreamLog::log(Log::LogLevel level, const std::string& category, const std::string& message) {
    if (level >= _level) {
        
        if(_logBits & Log::LogBits::TimeStampBit) {
            static const int timeWidth = owl::Time::getTimeFormat(_timeFormat).length() + 2;
            _stream << std::setw(timeWidth) << std::left << owl::Time::getTimeFormat(_timeFormat);
        }
        if(_logBits & Log::LogBits::LevelBit) {
            static const int levelWidth = 12;
            _stream << std::setw(levelWidth) << std::left << "(" + LevelToString(level) + ")";
        }
        if(_logBits & Log::LogBits::CategoryBit) {
            const int catWidth = (category.length() <= 20) ? 22 : roundUp(category.length(), 5)+2;
            _stream << std::setw(catWidth) << std::left << category;
        }
        if (_logBits & Log::LogBits::MessageBit) {
            if(_logBits ^ Log::LogBits::MessageBit)
                _stream << "| ";
            _stream << message;
        }
        
        if(_logBits)
            _stream << std::endl;

    }
}

} // owl