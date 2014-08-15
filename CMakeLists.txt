#########################################################################################
#                                                                                       #
# OWL                                                                                   #
#                                                                                       #
# Copyright (c) 2014 Jonas Strandstedt                                                  #
#                                                                                       #
# Permission is hereby granted, free of charge, to any person obtaining a copy of this  #
# software and associated documentation files (the "Software"), to deal in the Software #
# without restriction, including without limitation the rights to use, copy, modify,    #
# merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    #
# permit persons to whom the Software is furnished to do so, subject to the following   #
# conditions:                                                                           #
#                                                                                       #
# The above copyright notice and this permission notice shall be included in all copies #
# or substantial portions of the Software.                                              #
#                                                                                       #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   #
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         #
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    #
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  #
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  #
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         #
#########################################################################################

cmake_minimum_required(VERSION 2.8)
project(owl)

############################################
# Settings
############################################
if(NOT OWL_ROOT_DIR)
    set(OWL_ROOT_DIR ${PROJECT_SOURCE_DIR})
endif(NOT OWL_ROOT_DIR)

############################################
# Dependencies
############################################

# Threads
find_package(Threads REQUIRED)
set(OWL_DEPENDENCIES ${OWL_DEPENDENCIES} ${CMAKE_THREAD_LIBS_INIT})

############################################
# owl source
############################################
add_subdirectory(${OWL_ROOT_DIR}/src)


############################################
# demo app
############################################
add_executable(owl_demo ${OWL_ROOT_DIR}/demo/demo.cpp)
add_definitions(-std=c++11)# ssl
add_definitions(-lssl)

# crypto
add_definitions(-lcrypto)
include_directories(${OWL_ROOT_DIR}/include)
target_link_libraries(owl_demo owl)