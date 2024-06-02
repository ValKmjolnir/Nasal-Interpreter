#include "util/util.h"

namespace nasal::util {

bool is_windows() {
#if defined(_WIN32) || defined(_WIN64)
    return true;
#else
    return false;
#endif
}

bool is_linux() {
#if defined __linux__
    return true;
#else
    return false;
#endif
}

bool is_macos() {
#if defined __APPLE__
    return true;
#else
    return false;
#endif
}

bool is_x86() {
#if defined(__i386__) || defined(_M_IX86)
    return true;
#else
    return false;
#endif
}

bool is_amd64() {
#if defined(__amd64__) || defined(_M_X64)
    return true;
#else
    return false;
#endif
}

bool is_x86_64() {
    return is_amd64();
}

bool is_arm() {
#if defined(__arm__) || defined(_M_ARM)
    return true;
#else
    return false;
#endif
}

bool is_aarch64() {
#if defined(__aarch64__) || defined(_M_ARM64)
    return true;
#else
    return false;
#endif
}

bool is_ia64() {
#if defined(__ia64__)
    return true;
#else
    return false;
#endif
}

bool is_powerpc() {
#if defined(__powerpc__)
    return true;
#else
    return false;
#endif
}

bool is_superh() {
#if defined(__sh__)
    return true;
#else
    return false;
#endif
}

const char* get_platform() {
    if (is_windows()) {
        return "windows";
    } else if (is_linux()) {
        return "linux";
    } else if (is_macos()) {
        return "macOS";
    }
    return "unknown";
}

const char* get_arch() {
    if (is_x86()) {
        return "x86";
    } else if (is_x86_64()) {
        return "x86-64";
    } else if (is_amd64()) {
        return "amd64";
    } else if (is_arm()) {
        return "arm";
    } else if (is_aarch64()) {
        return "aarch64";
    } else if (is_ia64()) {
        return "ia64";
    } else if (is_powerpc()) {
        return "powerpc";
    } else if (is_superh()) {
        return "superh";
    }
    return "unknown";
}

}