cmake_minimum_required(VERSION 3.16..3.24)
project(ChessEngine VERSION 0.1.0 DESCRIPTION "Chess Engine" LANGUAGES CXX)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

add_library(cxx_setup INTERFACE)
target_compile_options(cxx_setup INTERFACE -Wall -Wextra -Wpedantic)

# Conditional compilation options based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring for debug...")
    target_compile_options(cxx_setup INTERFACE -g -O0) # Debug symbols
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring for release...")
    target_compile_options(cxx_setup INTERFACE -g -O2 -ffast-math -DNDEBUG) # Enable optimizations
endif()

target_compile_definitions(cxx_setup INTERFACE cxx_std_17)
target_include_directories(cxx_setup INTERFACE ${PROJECT_SOURCE_DIR})

include(CTest)

add_subdirectory(${PROJECT_NAME})
add_subdirectory(executables)
add_subdirectory(external)
