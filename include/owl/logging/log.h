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

#ifndef __LOG_H__
#define __LOG_H__

#include <string>

namespace owl {
class Log {
public:
	enum LogLevel {
        Debug   = 1 << 0,
        Info    = 1 << 1,
        Warning = 1 << 2,
        Error   = 1 << 3,
        Fatal   = 1 << 4,
        None    = 1 << 5
    };
    
    enum LogBits {
        TimeStampBit   = 1 << 0,
        LevelBit       = 1 << 1,
        CategoryBit    = 1 << 2,
        MessageBit     = 1 << 3,
        AllBit         = TimeStampBit | LevelBit | CategoryBit | MessageBit
    };
    
    Log(LogLevel level);
    virtual ~Log();
    
    void setTimeFormat(const std::string& format);
    std::string getTimeFormat();
    
    void enableBits(LogBits bits);
    void disableBits(LogBits bits);
    
    virtual void log(LogLevel level, const std::string& category, const std::string& message) = 0;
    
    static std::string LevelToString(LogLevel level);

protected:
    
    LogLevel _level;
    std::string _timeFormat;
    LogBits _logBits;

}; // StreamLog
}  // owl 

#endif
