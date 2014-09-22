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
set(OWL_LIBRARIES owl)

############################################
# Macros
############################################

# source group macro
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

# Patterns
add_group("Signal" "signal")

# Special group
file(GLOB MAIN_SOURCE ${SOURCE_ROOT_DIR}/*.cpp ${SOURCE_ROOT_DIR}/*.c)
set(OWL_SOURCE ${OWL_SOURCE} ${MAIN_SOURCE})
file(GLOB MAIN_HEADERS ${HEADER_ROOT_DIR}/owl/*.h ${HEADER_ROOT_DIR}/owl/*.inl ${HEADER_ROOT_DIR}/owl/*.hpp ${HEADER_ROOT_DIR}/owl/owl)
set(OWL_HEADER ${OWL_HEADER} ${MAIN_HEADERS})
source_group("Owl" FILES ${MAIN_SOURCE} ${MAIN_HEADERS})

############################################
# Library
############################################
add_library(${OWL_LIBRARIES} ${OWL_SOURCE} ${OWL_HEADER})
include_directories(${OWL_INCLUDE_DIRS})

if(MSVC)
	add_definitions(-D__WIN32__)
	target_link_libraries(${OWL_LIBRARIES} ${OWL_DEPENDENCIES} Ws2_32)
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
	#target_link_libraries(${OWL_LIBRARIES} ${OWL_DEPENDENCIES} ssl crypto)
	target_link_libraries(${OWL_LIBRARIES} ${OWL_DEPENDENCIES})
endif(MSVC)

message(STATUS "owl libs: ${OWL_LIBRARIES}")
message(STATUS "owl include: ${OWL_INCLUDE_DIRS}")

set(OWL_LIBRARIES owl PARENT_SCOPE)
set(OWL_INCLUDE_DIRS ${OWL_INCLUDE_DIRS} PARENT_SCOPE)
