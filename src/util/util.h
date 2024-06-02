#pragma once

#include "nasal.h"

#include <cstring>
#include <sstream>

namespace nasal::util {

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

u32 utf8_hdchk(const char);

std::string char_to_hex(const char);
std::string rawstr(const std::string&, const usize maxlen = 0);

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

}