cmake_minimum_required(VERSION 3.10)

project(nasal VERSION 10.1)

message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

# -std=c++14 -Wshadow -Wall
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wshadow -Wall")

# generate release executables
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/module)

add_library(fib SHARED ${CMAKE_SOURCE_DIR}/module/fib.cpp)
target_include_directories(fib PRIVATE ${CMAKE_SOURCE_DIR})

add_library(key SHARED ${CMAKE_SOURCE_DIR}/module/keyboard.cpp)
target_include_directories(key PRIVATE ${CMAKE_SOURCE_DIR})

add_library(mat SHARED ${CMAKE_SOURCE_DIR}/module/matrix.cpp)
target_include_directories(mat PRIVATE ${CMAKE_SOURCE_DIR})

add_library(nasock SHARED ${CMAKE_SOURCE_DIR}/module/nasocket.cpp)
target_include_directories(nasock PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(nasal main.cpp)

if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(nasal dl)
endif()

target_include_directories(nasal PRIVATE ${CMAKE_SOURCE_DIR})

if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    add_custom_command(
        TARGET nasal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/build/nasal
                ${CMAKE_SOURCE_DIR}/nasal
    )
endif()