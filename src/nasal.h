#pragma once

#ifndef __nasver__
#define __nasver__ "11.1"
#endif

#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cmath>

// abbreviation of some useful basic type
using i32 = std::int32_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using usize = std::size_t;
using f64 = double;

namespace nasal {

bool is_windows();
bool is_linux();
bool is_macos();
bool is_x86();
bool is_amd64();
bool is_x86_64();
bool is_arm();
bool is_aarch64();
bool is_ia64();
bool is_powerpc();
bool is_superh();
const char* get_platform();
const char* get_arch();

// virtual machine stack depth, both global depth and value stack depth
const u32 STACK_DEPTH = 4096;

f64 hex_to_f64(const char*);
f64 oct_to_f64(const char*);
// we have the same reason not using atof here
// just as andy's interpreter does.
// it is not platform independent, and may have strange output.
// so we write a new function here to convert str to number manually.
// but this also makes 0.1+0.2==0.3,
// not another result that you may get in other languages.
f64 dec_to_f64(const char*);

f64 str_to_num(const char*);
i32 utf8_hdchk(const char);
std::string char_to_hex(const char);
std::string rawstr(const std::string&, const usize maxlen = 0);

namespace fs {

class path {
private:
    std::string file_system_path;

public:
    path(const path&) = default;
    path(const std::string& file_path): file_system_path(file_path) {}
    path& operator/(const path&);
    const char* c_str() const {
        return file_system_path.c_str();
    }
    const std::string& str() const {
        return file_system_path;
    }
};

bool exists(const path&);
bool is_regular(const path&);

}

}