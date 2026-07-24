cmake_minimum_required(VERSION 3.10)

project(nasal VERSION 11.3)

message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

# -std=c++17 -Wshadow -Wall
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wshadow -Wall")

# MSVC does not need -fPIC
if (NOT MSVC)
    add_compile_options(-fPIC)
endif()

# MSVC needs this command option to really enable utf-8 output
if (MSVC)
    add_compile_options(/utf-8)
endif()

if (APPLE)
    add_compile_options(-mmacosx-version-min=10.15)
endif()

# build nasal used object

set(NASAL_AST_MODULE_FILE
    ${CMAKE_SOURCE_DIR}/src/ast/ast.cpp
    ${CMAKE_SOURCE_DIR}/src/ast/dumper.cpp
    ${CMAKE_SOURCE_DIR}/src/ast/optimizer.cpp
    ${CMAKE_SOURCE_DIR}/src/ast/symbol_finder.cpp
    ${CMAKE_SOURCE_DIR}/src/ast/visitor.cpp)
add_library(nasal-ast STATIC ${NASAL_AST_MODULE_FILE})
target_include_directories(nasal-ast PRIVATE ${CMAKE_SOURCE_DIR}/src)

set(NASAL_OBJECT_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/src/cli/cli.cpp
    ${CMAKE_SOURCE_DIR}/src/code/codegen.cpp
    ${CMAKE_SOURCE_DIR}/src/code/codestream.cpp
    ${CMAKE_SOURCE_DIR}/src/code/opcode.cpp
    ${CMAKE_SOURCE_DIR}/src/dbg/dbg.cpp
    ${CMAKE_SOURCE_DIR}/src/error/error.cpp
    ${CMAKE_SOURCE_DIR}/src/lexer/lexer.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/builtin.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/coroutine.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/fg_props.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/bits_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/io_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/json_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/math_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/dylib_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/regex_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/registry.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/subprocess.cpp
    ${CMAKE_SOURCE_DIR}/src/natives/unix_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/parse/parse.cpp
    ${CMAKE_SOURCE_DIR}/src/parse/linker.cpp
    ${CMAKE_SOURCE_DIR}/src/repl/repl.cpp
    ${CMAKE_SOURCE_DIR}/src/util/colorful.cpp
    ${CMAKE_SOURCE_DIR}/src/util/filestream.cpp
    ${CMAKE_SOURCE_DIR}/src/util/fs.cpp
    ${CMAKE_SOURCE_DIR}/src/util/resource_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/util/util.cpp
    ${CMAKE_SOURCE_DIR}/src/vm/gc_stat.cpp
    ${CMAKE_SOURCE_DIR}/src/vm/gc.cpp
    ${CMAKE_SOURCE_DIR}/src/vm/vm.cpp
    ${CMAKE_SOURCE_DIR}/src/vm/type.cpp)
add_library(nasal-object STATIC ${NASAL_OBJECT_SOURCE_FILE})
target_include_directories(nasal-object PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(nasal-object nasal-ast)

set(NASAL_FORMAT_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/src/tools/format/format.cpp)
add_library(nasal-format-object STATIC ${NASAL_FORMAT_SOURCE_FILE})
target_include_directories(nasal-format-object PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(nasal-format-object nasal-object)

set(NASAL_LINTER_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/src/tools/linter/checker/manager.cpp
    ${CMAKE_SOURCE_DIR}/src/tools/linter/checker/space_infix_ops.cpp)
add_library(nasal-linter-object STATIC ${NASAL_LINTER_SOURCE_FILE})
target_include_directories(nasal-linter-object PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(nasal-linter-object nasal-object)

# build nasal
add_executable(nasal ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(nasal nasal-object)

# build nasal-format
add_executable(nasal-format ${CMAKE_SOURCE_DIR}/src/tools/format/main.cpp)
target_link_libraries(nasal-format nasal-format-object)

# build nasal-linter
add_executable(nasal-linter ${CMAKE_SOURCE_DIR}/src/tools/linter/main.cpp)
target_link_libraries(nasal-linter nasal-linter-object)

# link ldl and lpthread
if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(nasal dl)
    target_link_libraries(nasal pthread)
    target_link_libraries(nasal-format pthread)
    target_link_libraries(nasal-linter pthread)
endif()
target_include_directories(nasal PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(nasal-format PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(nasal-linter PRIVATE ${CMAKE_SOURCE_DIR}/src)

# copy nasal from build dir to the outside dir
if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    add_custom_command(
        TARGET nasal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/build/nasal
                ${CMAKE_SOURCE_DIR}/nasal
    )
    add_custom_command(
        TARGET nasal-format POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/build/nasal-format
                ${CMAKE_SOURCE_DIR}/nasal-format
    )
    add_custom_command(
        TARGET nasal-linter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/build/nasal-linter
                ${CMAKE_SOURCE_DIR}/nasal-linter
    )
endif()

# build module and move shared library files to module dir
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/module)

set(MODULE_USED_OBJECT_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/src/util/util.cpp
    ${CMAKE_SOURCE_DIR}/src/util/fs.cpp
    ${CMAKE_SOURCE_DIR}/src/vm/type.cpp
    ${CMAKE_SOURCE_DIR}/src/vm/gc_stat.cpp
    ${CMAKE_SOURCE_DIR}/src/vm/gc.cpp)
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
        src/web/web.cpp
    )
    target_include_directories(nasal-web PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(nasal-web nasal-object)
    set_target_properties(nasal-web PROPERTIES
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()