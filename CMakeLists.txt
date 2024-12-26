cmake_minimum_required(VERSION 3.10)

project(nasal VERSION 11.3)

message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

# -std=c++17 -Wshadow -Wall
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wshadow -Wall")

add_compile_options(-fPIC)
# MSVC needs this command option to really enable utf-8 output
if (MSVC)
    add_compile_options(/utf-8)
endif()

if (APPLE)
    add_compile_options(-mmacosx-version-min=10.15)
endif()

# generate release executables
set(CMAKE_BUILD_TYPE "Release")

# build nasal used object
set(NASAL_OBJECT_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/src/cli/cli.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/builtin.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/coroutine.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/fg_props.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/bits_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/io_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/json_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/math_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/dylib_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/regex_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/subprocess.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/unix_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/repl/repl.cpp
    ${CMAKE_SOURCE_DIR}/src/util/fs.cpp
    ${CMAKE_SOURCE_DIR}/src/util/util.cpp
    ${CMAKE_SOURCE_DIR}/src/ast_dumper.cpp
    ${CMAKE_SOURCE_DIR}/src/ast_visitor.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_ast.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_codegen.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_dbg.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_err.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_gc.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_import.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_lexer.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_opcode.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_parse.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_type.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_vm.cpp
    ${CMAKE_SOURCE_DIR}/src/optimizer.cpp
    ${CMAKE_SOURCE_DIR}/src/symbol_finder.cpp)
add_library(nasal-object STATIC ${NASAL_OBJECT_SOURCE_FILE})
target_include_directories(nasal-object PRIVATE ${CMAKE_SOURCE_DIR}/src)

# build nasal
add_executable(nasal ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(nasal nasal-object)

# link ldl and lpthread
if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(nasal dl)
    target_link_libraries(nasal pthread)
endif()
target_include_directories(nasal PRIVATE ${CMAKE_SOURCE_DIR}/src)

# copy nasal from build dir to the outside dir
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
    ${CMAKE_SOURCE_DIR}/src/util/util.cpp
    ${CMAKE_SOURCE_DIR}/src/util/fs.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_type.cpp
    ${CMAKE_SOURCE_DIR}/src/nasal_gc.cpp)
add_library(module-used-object STATIC ${MODULE_USED_OBJECT_SOURCE_FILE})
target_include_directories(module-used-object PRIVATE ${CMAKE_SOURCE_DIR}/src)

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
if (WIN32)
    target_link_libraries(nasock ws2_32)
endif()
target_link_libraries(nasock module-used-object)

# Add web library, not for MSVC now
if (NOT MSVC)
    add_library(nasal-web SHARED
        src/nasal_web.cpp
        ${NASAL_OBJECT_SOURCE_FILE}
    )
    target_include_directories(nasal-web PRIVATE ${CMAKE_SOURCE_DIR}/src)
    set_target_properties(nasal-web PROPERTIES
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()