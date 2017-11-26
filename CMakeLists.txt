cmake_minimum_required(VERSION 2.8.9)
project(EDPA20172)

enable_language(CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# These options can be turned on/off from the 'make' command.
option(debug "Use debugging flag" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
if (debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    # Coverage can only be set if -g is used.
endif()

#Bring the headers, such as Student.h into the project
include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

set(TEST_TIME_KEEP src/tests/test_time_keep.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(EDPA20172-hash.o ${SOURCES})
add_executable(test_time_keep.o ${SOURCES})
