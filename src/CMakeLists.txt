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

############################################
# Settings
############################################
set(SOURCE_ROOT_DIR ${OWL_ROOT_DIR}/src)
set(HEADER_ROOT_DIR ${OWL_ROOT_DIR}/include)

set(OWL_INCLUDE_DIRS ${OWL_INCLUDE_DIRS} ${HEADER_ROOT_DIR})

############################################
# Macros
############################################

macro(add_group GROUP_NAME GROUP_DIRECTORY)
	file(GLOB SOURCE ${SOURCE_ROOT_DIR}/${GROUP_DIRECTORY}/*.cpp ${SOURCE_ROOT_DIR}/${GROUP_DIRECTORY}/*.c)
	set(OWL_SOURCE ${OWL_SOURCE} ${SOURCE})
	file(GLOB HEADERS ${HEADER_ROOT_DIR}/owl/${GROUP_DIRECTORY}/*.h ${HEADER_ROOT_DIR}/owl/${GROUP_DIRECTORY}/*.inl ${HEADER_ROOT_DIR}/owl/${GROUP_DIRECTORY}/*.hpp)
	set(OWL_HEADER ${OWL_HEADER} ${HEADERS})
	source_group(${GROUP_NAME} FILES ${SOURCE} ${HEADERS})
endmacro(add_group)

############################################
# Source groups
############################################

# Logging
add_group("Logging" "logging")

# Time
add_group("Time" "time")

# Data types
add_group("Data" "data")

# Patterns
add_group("DesignPattern" "designpattern")

# Patterns
add_group("Network" "network")

# Patterns
add_group("Lua" "lua")

# Patterns
add_group("Filesystem" "filesystem")

############################################
# Library
############################################
add_library(owl ${OWL_SOURCE} ${OWL_HEADER})
include_directories(${OWL_INCLUDE_DIRS})
target_link_libraries(owl ${OWL_DEPENDENCIES} ssl crypto)
add_definitions(-std=c++11)

# ssl
add_definitions(-lssl)

# crypto
add_definitions(-lcrypto)


# set(OWL_LIBRARIES owl)
# set(OWL_INCLUDE_DIR ${HEADER_ROOT_DIR})

set(OWL_LIBRARIES owl PARENT_SCOPE)
set(OWL_INCLUDE_DIRS ${OWL_INCLUDE_DIRS} PARENT_SCOPE)
# set(OWL_LIBRARIES ${OWL_LIBRARIES} PARENT_SCOPE)
# set(OWL_INCLUDE_DIR ${OWL_INCLUDE_DIR} PARENT_SCOPE)


# message(STATUS "owl libs: ${OWL_LIBRARIES}")
# message(STATUS "owl include: ${OWL_INCLUDE_DIR}")
