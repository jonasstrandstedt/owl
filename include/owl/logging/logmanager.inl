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

#ifndef __LOGMANAGER_INL__
#define __LOGMANAGER_INL__

#define OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE 200

//****************************************************************************************
// Category
//****************************************************************************************

#define LDEBUGC(__category__, __message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Debug, __category__, ss.str());\
    } while(false)

#define LINFOC(__category__, __message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Info, __category__, ss.str());\
    } while(false)

#define LWARNINGC(__category__, __message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Warning, __category__, ss.str());\
    } while(false)

#define LERRORC(__category__, __message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Error, __category__, ss.str());\
    } while(false)

#define LFATALC(__category__, __message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Fatal, __category__, ss.str());\
    } while(false)



//****************************************************************************************
// _loggerCat
//****************************************************************************************

#define LDEBUG(__message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Debug, _loggerCat, ss.str());\
    } while(false)

#define LINFO(__message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Info, _loggerCat, ss.str());\
    } while(false)

#define LWARNING(__message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Warning, _loggerCat, ss.str());\
    } while(false)

#define LERROR(__message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Error, _loggerCat, ss.str());\
    } while(false)

#define LFATAL(__message__) \
    do {\
            std::stringstream ss;\
            ss << __message__;\
            owl::LogManager::ref().log(owl::Log::LogLevel::Fatal, _loggerCat, ss.str());\
    } while(false)



//****************************************************************************************
// Category Format
//****************************************************************************************

#define LDEBUGCF(__category__, __format__ ,...) \
    do {\
        if(owl::LogManager::isInitialized()) {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Debug, __category__, buffer);\
        }\
    } while(false)

#define LINFOCF(__category__, __format__ ,...) \
    do {\
        if(owl::LogManager::isInitialized()) {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Info, __category__, buffer);\
        }\
    } while(false)

#define LWARNINGCF(__category__, __format__ ,...) \
    do {\
        if(owl::LogManager::isInitialized()) {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Warning, __category__, buffer);\
        }\
    } while(false)

#define LERRORCF(__category__, __format__ ,...) \
    do {\
        if(owl::LogManager::isInitialized()) {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Error, __category__, buffer);\
        }\
    } while(false)

#define LFATALCF(__category__, __format__ ,...) \
    do {\
        if(owl::LogManager::isInitialized()) {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Fatal, __category__, buffer);\
        }\
    } while(false)



//****************************************************************************************
// _loggerCat Format
//****************************************************************************************

#define LDEBUGF(__format__ ,...) \
    do {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Debug, _loggerCat, buffer);\
    } while(false)

#define LINFOF(__format__ ,...) \
    do {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Info, _loggerCat, buffer);\
    } while(false)

#define LWARNINGF(__format__ ,...) \
    do {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Warning, _loggerCat, buffer);\
    } while(false)

#define LERRORF(__format__ ,...) \
    do {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Error, _loggerCat, buffer);\
    } while(false)

#define LFATALF(__format__ ,...) \
    do {\
            char buffer [OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE];\
            memset(buffer,0,OWL_LOGMANAGER_SPRINTF_BUFFER_SIZE);\
            int n;\
            n = sprintf (buffer, __format__, __VA_ARGS__);\
            if(n >= 0)\
            owl::LogManager::ref().log(owl::Log::LogLevel::Fatal, _loggerCat, buffer);\
    } while(false)

#endif