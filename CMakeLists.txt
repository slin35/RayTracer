cmake_minimum_required(VERSION 3.10)


# set the project name
project(raytrace)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/scr/*.cpp")
file (GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/scr/*.h")

# add the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()