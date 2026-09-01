#pragma once

#include "nasal.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

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

std::string char_to_hex(const char);
std::string rawstr(const std::string&, const usize maxlen = 0);
f64 str_to_num(const char*);

class windows_code_page_manager {
private:
    u32 code_page;

public:
    windows_code_page_manager() {
#ifdef _WIN32
        code_page = GetConsoleOutputCP();
#endif
    }

    void set_utf8_output() {
#ifdef _WIN32
        // store previous code page
        code_page = GetConsoleOutputCP();
        // allow 65001 code page
        SetConsoleOutputCP(CP_UTF8);
#endif
    }

    void restore_code_page() {
#ifdef _WIN32
        // restore previous code page
        SetConsoleOutputCP(code_page);
#endif
    }
};

}