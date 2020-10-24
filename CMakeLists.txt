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

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(${CMAKE_PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()


set(GLM_INCLUDE_DIR "$ENV{GLM_INCLUDE_DIR}")
if (GLM_INCLUDE_DIR)
    include_directories(${GLM_INCLUDE_DIR})
    message(STATUS "GLM environment variable found")
else()
    message(STATUS "GLM environment variable `GLM_INCLUDE_DIR` not found, GLM must be installed with the system")
endif()