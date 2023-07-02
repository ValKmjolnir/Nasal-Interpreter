#pragma once

#ifndef __nasver
#define __nasver "11.0"
#endif

#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <vector>

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

using i32 = std::int32_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using usize = std::size_t;
using f64 = double;

const u32 STACK_DEPTH=4096;

f64 hex2f(const char*);
f64 oct2f(const char*);

// we have the same reason not using atof here
// just as andy's interpreter does.
// it is not platform independent, and may have strange output.
// so we write a new function here to convert str to number manually.
// but this also makes 0.1+0.2==0.3,
// not another result that you may get in other languages.
f64 dec2f(const char*);

f64 str2num(const char*);
i32 utf8_hdchk(const char);
std::string chrhex(const char);
std::string rawstr(const std::string&, const usize maxlen=0);

#include "nasal_new_gc.h"