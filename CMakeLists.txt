cmake_minimum_required(VERSION 3.10)

project(nasal VERSION 10.1)

# -std=c++14 -Wshadow -Wall
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wshadow -Wall")

# generate release executables
set(CMAKE_BUILD_TYPE "Release")

add_library(fib SHARED ${CMAKE_SOURCE_DIR}/module/fib.cpp)
target_include_directories(fib PRIVATE ${CMAKE_SOURCE_DIR})

add_library(key SHARED ${CMAKE_SOURCE_DIR}/module/keyboard.cpp)
target_include_directories(key PRIVATE ${CMAKE_SOURCE_DIR})

add_library(mat SHARED ${CMAKE_SOURCE_DIR}/module/matrix.cpp)
target_include_directories(mat PRIVATE ${CMAKE_SOURCE_DIR})

add_library(nasock SHARED ${CMAKE_SOURCE_DIR}/module/nasocket.cpp)
target_include_directories(nasock PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(nasal main.cpp)
target_link_libraries(nasal dl)
target_include_directories(nasal PRIVATE ${CMAKE_SOURCE_DIR})