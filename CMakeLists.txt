cmake_minimum_required(VERSION 3.10)

project(nasal VERSION 10.1)

message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

# -std=c++17 -Wshadow -Wall
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wshadow -Wall")

add_compile_options(-fPIC)

# generate release executables
set(CMAKE_BUILD_TYPE "Release")

# build nasal used object
set(NASAL_OBJECT_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/src/ast_dumper.cpp
    ${CMAKE_SOURCE_DIR}/src/ast_visitor.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_ast.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_builtin.cpp
    ${CMAKE_SOURCE_DIR}/src/coroutine.cpp
    ${CMAKE_SOURCE_DIR}/src/fg_props.cpp
    ${CMAKE_SOURCE_DIR}/src/bits_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/math_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_codegen.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_dbg.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_err.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_gc.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_import.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_lexer.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_misc.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_opcode.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_parse.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_vm.cpp
    ${CMAKE_SOURCE_DIR}/src/optimizer.cpp
    ${CMAKE_SOURCE_DIR}/src/symbol_finder.cpp)
add_library(nasal-object STATIC ${NASAL_OBJECT_SOURCE_FILE})
target_include_directories(nasal-object PRIVATE ${CMAKE_SOURCE_DIR}/src)

# build nasal
add_executable(nasal ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(nasal nasal-object)
if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(nasal dl)
    target_link_libraries(nasal pthread)
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

# build module
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/module)

set(MODULE_USED_OBJECT_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/src/nasal_misc.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_gc.cpp)
add_library(module-used-object STATIC ${MODULE_USED_OBJECT_SOURCE_FILE})

add_library(fib SHARED ${CMAKE_SOURCE_DIR}/module/fib.cpp)
target_include_directories(fib PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(fib module-used-object)

add_library(key SHARED ${CMAKE_SOURCE_DIR}/module/keyboard.cpp)
target_include_directories(key PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(key module-used-object)

add_library(mat SHARED ${CMAKE_SOURCE_DIR}/module/matrix.cpp)
target_include_directories(mat PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(mat module-used-object)

add_library(nasock SHARED ${CMAKE_SOURCE_DIR}/module/nasocket.cpp)
target_include_directories(nasock PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(nasock module-used-object)