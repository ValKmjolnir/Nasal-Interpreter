#include "util/util.h"

#include <cmath>

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

u32 utf8_hdchk(const char head) {
    // RFC-2279 but now we use RFC-3629 so nbytes is less than 4
    const auto c = static_cast<u8>(head);
    if ((c>>5)==0x06) { // 110x xxxx (10xx xxxx)^1
        return 1;
    }
    if ((c>>4)==0x0e) { // 1110 xxxx (10xx xxxx)^2
        return 2;
    }
    if ((c>>3)==0x1e) { // 1111 0xxx (10xx xxxx)^3
        return 3;
    }
    return 0;
}

std::string char_to_hex(const char c) {
    const char hextbl[] = "0123456789abcdef";
    return {hextbl[(c&0xf0)>>4], hextbl[c&0x0f]};
}

std::string rawstr(const std::string& str, const usize maxlen) {
    std::string ret("");
    for (auto i : str) {
        // windows doesn't output unicode normally, so we output the hex
        if (util::is_windows() && i<=0) {
            ret += "\\x" + char_to_hex(i);
            continue;
        }
        switch(i) {
            case '\0':  ret += "\\0";  break;
            case '\a':  ret += "\\a";  break;
            case '\b':  ret += "\\b";  break;
            case '\t':  ret += "\\t";  break;
            case '\n':  ret += "\\n";  break;
            case '\v':  ret += "\\v";  break;
            case '\f':  ret += "\\f";  break;
            case '\r':  ret += "\\r";  break;
            case '\033':ret += "\\e";  break;
            case '\"':  ret += "\\\""; break;
            case '\'':  ret += "\\\'"; break;
            case '\\':  ret += "\\\\"; break;
            default:    ret += i;      break;
        }
    }
    if (maxlen && ret.length()>maxlen) {
        ret = ret.substr(0, maxlen)+"...";
    }
    return ret;
}

f64 hex_to_f64(const char* str) {
    f64 ret = 0;
    for (; *str; ++str) {
        if ('0'<=*str && *str<='9') {
            ret = ret*16+(*str-'0');
        } else if ('a'<=*str && *str<='f') {
            ret = ret*16+(*str-'a'+10);
        } else if ('A'<=*str && *str<='F') {
            ret = ret*16+(*str-'A'+10);
        } else {
            return nan("");
        }
    }
    return ret;
}

f64 oct_to_f64(const char* str) {
    f64 ret = 0;
    while('0'<=*str && *str<'8') {
        ret = ret*8+(*str++-'0');
    }
    if (*str) {
        return nan("");
    }
    return ret;
}

// we have the same reason not using atof here
// just as andy's interpreter does.
// it is not platform independent, and may have strange output.
// so we write a new function here to convert str to number manually.
// but this also makes 0.1+0.2==0.3,
// not another result that you may get in other languages.
f64 dec_to_f64(const char* str) {
    f64 ret = 0, num_pow = 0;
    bool negative = false;
    while('0'<=*str && *str<='9') {
        ret = ret*10+(*str++-'0');
    }
    if (!*str) {
        return ret;
    }
    if (*str=='.') {
        if (!*++str) {
            return nan("");
        }
        num_pow = 0.1;
        while('0'<=*str && *str<='9') {
            ret += num_pow*(*str++-'0');
            num_pow *= 0.1;
        }
        if (!*str) {
            return ret;
        }
    }
    if (*str!='e' && *str!='E') {
        return nan("");
    }
    if (!*++str) {
        return nan("");
    }
    if (*str=='-' || *str=='+') {
        negative = (*str++=='-');
    }
    if (!*str) {
        return nan("");
    }
    num_pow = 0;
    while('0'<=*str && *str<='9') {
        num_pow = num_pow*10+(*str++-'0');
    }
    if (*str) {
        return nan("");
    }
    return negative?
        ret*std::pow(10, 1-num_pow)*0.1:
        ret*std::pow(10, num_pow-1)*10;
}

f64 str_to_num(const char* str) {
    bool negative = false;
    f64 res = 0;
    if (*str=='-' || *str=='+') {
        negative = (*str++=='-');
    }
    if (!*str) {
        return nan("");
    }
    if (str[0]=='0' && str[1]=='x') {
        res = hex_to_f64(str+2);
    } else if (str[0]=='0' && str[1]=='o') {
        res = oct_to_f64(str+2);
    } else {
        res = dec_to_f64(str);
    }
    return negative? -res:res;
}

}