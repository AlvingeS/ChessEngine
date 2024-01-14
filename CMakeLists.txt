cmake_minimum_required(VERSION 3.16..3.24)
project(our_project VERSION 0.1.0
                 DESCRIPTION "Template project"
                 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

add_library(cxx_setup INTERFACE)
target_compile_options(cxx_setup INTERFACE -Wall -Wextra -Wpedantic)
target_compile_definitions(cxx_setup INTERFACE cxx_std_17)
target_include_directories(cxx_setup INTERFACE ${PROJECT_SOURCE_DIR})

include(CTest)

add_subdirectory(${PROJECT_NAME})
add_subdirectory(executables)
add_subdirectory(external)