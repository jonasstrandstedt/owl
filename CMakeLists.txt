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
# Meta-Settings
############################################
if(NOT OWL_ROOT_DIR)
    set(OWL_ROOT_DIR ${PROJECT_SOURCE_DIR})
endif(NOT OWL_ROOT_DIR)
set(OWL_EXT_DIR "${OWL_ROOT_DIR}/ext")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${OWL_EXT_DIR})
set(OWL_BINARY_DIR ${OWL_ROOT_DIR}/bin)
set(OWL_LIBRARY_DIR ${OWL_ROOT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OWL_LIBRARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OWL_LIBRARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OWL_BINARY_DIR})
get_directory_property(hasParent PARENT_DIRECTORY)

############################################
# Settings
############################################
if(hasParent)
	option(OWL_BUILD_DEMO "Build demo application" OFF)
	option(OWL_BUILD_TEST "Build test application" OFF)
else()
	option(OWL_BUILD_DEMO "Build demo application" ON)
	option(OWL_BUILD_TEST "Build test application" ON)
endif()


############################################
# Dependencies
############################################

# Threads
find_package(Threads REQUIRED)
set(OWL_DEPENDENCIES ${OWL_DEPENDENCIES} ${CMAKE_THREAD_LIBS_INIT})

# Lua
find_package(Lua REQUIRED)
set(OWL_DEPENDENCIES ${OWL_DEPENDENCIES} ${LUA_LIBRARIES})
set(OWL_INCLUDE_DIRS ${OWL_INCLUDE_DIRS} ${LUA_INCLUDE_DIR})

# SmallSHA1
find_package(SmallSHA1 REQUIRED)
set(OWL_DEPENDENCIES ${OWL_DEPENDENCIES} ${SMALLSHA1_LIBRARIES})
set(OWL_INCLUDE_DIRS ${OWL_INCLUDE_DIRS} ${SMALLSHA1_INCLUDE_DIR})

# LZ4
find_package(LZ4 REQUIRED)
set(OWL_DEPENDENCIES ${OWL_DEPENDENCIES} ${LZ4_LIBRARIES})
set(OWL_INCLUDE_DIRS ${OWL_INCLUDE_DIRS} ${LZ4_INCLUDE_DIR})

# Google Test
find_package(GTest REQUIRED)
set(OWL_DEPENDENCIES ${OWL_DEPENDENCIES} ${GTEST_LIBRARIES})
set(OWL_INCLUDE_DIRS ${OWL_INCLUDE_DIRS} ${GTEST_INCLUDE_DIR})

############################################
# owl source
############################################
add_subdirectory(${OWL_ROOT_DIR}/src)

############################################
# Demo app
############################################
if(OWL_BUILD_DEMO)
	add_subdirectory(${OWL_ROOT_DIR}/demo)
endif()

############################################
# Test app
############################################
if(OWL_BUILD_TEST)
	add_subdirectory(${OWL_ROOT_DIR}/test)
endif()

############################################
# Pass variables to parent
############################################
if(hasParent)
	set(OWL_LIBRARIES ${OWL_LIBRARIES} PARENT_SCOPE)
	set(OWL_INCLUDE_DIRS ${OWL_INCLUDE_DIRS} PARENT_SCOPE)
endif()