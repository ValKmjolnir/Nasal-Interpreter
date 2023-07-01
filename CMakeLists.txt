cmake_minimum_required(VERSION 3.10)

project(nasal VERSION 10.1)

message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

# -std=c++17 -Wshadow -Wall
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wshadow -Wall")

# generate release executables
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/module)

add_library(fib SHARED ${CMAKE_SOURCE_DIR}/module/fib.cpp)
target_include_directories(fib PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(key SHARED ${CMAKE_SOURCE_DIR}/module/keyboard.cpp)
target_include_directories(key PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(mat SHARED ${CMAKE_SOURCE_DIR}/module/matrix.cpp)
target_include_directories(mat PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(nasock SHARED ${CMAKE_SOURCE_DIR}/module/nasocket.cpp)
target_include_directories(nasock PRIVATE ${CMAKE_SOURCE_DIR}/src)

set(NASAL_OBJECT_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/src/ast_dumper.cpp
    ${CMAKE_SOURCE_DIR}/src/ast_visitor.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_ast.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_builtin.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_codegen.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_dbg.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_err.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_gc.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_import.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_lexer.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_misc.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_opcode.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_parse.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_new_vm.cpp
    ${CMAKE_SOURCE_DIR}/src/optimizer.cpp
    ${CMAKE_SOURCE_DIR}/src/symbol_finder.cpp)

add_executable(nasal ${CMAKE_SOURCE_DIR}/src/nasal_new_main.cpp)

add_library(nasal-object STATIC ${NASAL_OBJECT_SOURCE_FILE})
target_include_directories(nasal-object PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(nasal nasal-object)
if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(nasal dl)
endif()
target_include_directories(nasal PRIVATE ${CMAKE_SOURCE_DIR}/src)

if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    add_custom_command(
        TARGET nasal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/build/nasal
                ${CMAKE_SOURCE_DIR}/nasal
    )
endif()