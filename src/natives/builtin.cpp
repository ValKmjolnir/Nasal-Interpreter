﻿#include "natives/builtin.h"
#include "util/util.h"

#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace nasal {

var builtin_unsafe(context* ctx, gc* ngc) {
    return nas_err(
        "unsafe_redirect",
        "you are using unsafe system api under limited mode!"
    );
}

var builtin_print(context* ctx, gc* ngc) {
    for (auto& i : ctx->localr[1].vec().elems) {
        std::cout << i;
    }
    std::cout << std::flush;
    return nil;
}

var builtin_println(context* ctx, gc* ngc) {
    for (auto& i : ctx->localr[1].vec().elems) {
        std::cout << i;
    }
    std::cout << std::endl;
    return nil;
}

var builtin_exit(context* ctx, gc* ngc) {
    std::exit(ctx->localr[1].num());
    return nil;
}

var builtin_abort(context* ctx, gc* ngc) {
    std::abort();
    return nil;
}

var builtin_append(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var vec = local[1];
    var elem = local[2];
    if (!vec.is_vec()) {
        return nas_err("native::append", "\"vec\" must be vector");
    }
    auto& v = vec.vec().elems;
    for (auto& i : elem.vec().elems) {
        v.push_back(i);
    }
    return nil;
}

var builtin_setsize(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var vec = local[1];
    var size = local[2];
    if (!vec.is_vec()) {
        return nas_err("native::setsize", "\"vec\" must be vector");
    }
    if (!size.is_num() || size.num()<0) {
        return nil;
    }
    vec.vec().elems.resize(static_cast<i64>(size.num()), nil);
    return nil;
}

var builtin_system(context* ctx, gc* ngc) {
    auto str = ctx->localr[1];
    if (!str.is_str()) {
        return var::num(-1);
    }
    return var::num(static_cast<f64>(system(str.str().c_str())));
}

var builtin_input(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var end = local[1];
    var ret = ngc->alloc(vm_type::vm_str);
    if (!end.is_str() || end.str().length()>1 || !end.str().length()) {
        std::cin >> ret.str();
    } else {
        std::getline(std::cin, ret.str(), end.str()[0]);
    }
    return ret;
}

var builtin_split(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var separator = local[1];
    var str = local[2];
    if (!separator.is_str()) {
        return nas_err("native::split", "\"separator\" must be string");
    }
    if (!str.is_str()) {
        return nas_err("native::split", "\"str\" must be string");
    }
    const auto& sep = separator.str();
    const auto& s = str.str();

    // avoid being sweeped
    auto res = ngc->temp = ngc->alloc(vm_type::vm_vec);
    auto& vec = res.vec().elems;

    // empty separator means split every char
    if (!sep.length()) {
        for (auto i : s) {
            vec.push_back(ngc->newstr(i));
        }
        ngc->temp = nil;
        return res;
    }

    usize last = 0;
    usize pos = s.find(sep, 0);
    while (pos!=std::string::npos) {
        if (pos>last) {
            vec.push_back(ngc->newstr(s.substr(last, pos-last)));
        }
        last = pos + sep.length();
        pos = s.find(sep, last);
    }
    if (last!=s.length()) {
        vec.push_back(ngc->newstr(s.substr(last)));
    }
    ngc->temp = nil;
    return res;
}

var builtin_split_with_empty_substr(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var separator = local[1];
    var str = local[2];
    if (!separator.is_str()) {
        return nas_err(
            "native::split_with_empty_substr",
            "\"separator\" must be string"
        );
    }
    if (!str.is_str()) {
        return nas_err(
            "native::split_with_empty_substr",
            "\"str\" must be string"
        );
    }
    const auto& sep = separator.str();
    const auto& s = str.str();

    // avoid being sweeped
    auto res = ngc->temp = ngc->alloc(vm_type::vm_vec);
    auto& vec = res.vec().elems;

    // empty separator means split every char
    if (!sep.length()) {
        for (auto i : s) {
            vec.push_back(ngc->newstr(i));
        }
        ngc->temp = nil;
        return res;
    }

    usize last = 0;
    usize pos = s.find(sep, 0);
    while (pos!=std::string::npos) {
        if (pos>=last) {
            vec.push_back(ngc->newstr(s.substr(last, pos-last)));
        }
        last = pos + sep.length();
        pos = s.find(sep, last);
    }
    if (last<=s.length()) {
        vec.push_back(ngc->newstr(s.substr(last)));
    }
    ngc->temp = nil;
    return res;
}

var builtin_rand(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    if (!val.is_num() && !val.is_nil()) {
        return nas_err("native::rand", "\"seed\" must be nil or number");
    }
    if (val.is_num()) {
        srand(static_cast<u32>(val.num()));
        return nil;
    }
    f64 num = 0;
    for (u32 i = 0; i<5; ++i) {
        num = (num+rand())*(1.0/(RAND_MAX+1.0));
    }
    return var::num(num);
}

var builtin_id(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    std::stringstream ss;
    ss << "0";
    if (val.type>vm_type::vm_num) {
        ss << "x" << std::hex;
        ss << reinterpret_cast<u64>(val.val.gcobj) << std::dec;
    }
    return ngc->newstr(ss.str());
}

var builtin_int(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    if (!val.is_num() && !val.is_str()) {
        return nil;
    }
    return var::num(static_cast<f64>(static_cast<i32>(val.to_num())));
}

var builtin_floor(context* ctx, gc* ngc) {
    auto value = ctx->localr[1];
    return var::num(std::floor(value.num()));
}

var builtin_ceil(context* ctx, gc* ngc) {
    auto value = ctx->localr[1];
    return var::num(std::ceil(value.num()));
}

var builtin_num(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    if (val.is_num()) {
        return val;
    }
    if (!val.is_str()) {
        return nil;
    }
    auto res = val.to_num();
    if (std::isnan(res)) {
        return nil;
    }
    return var::num(res);
}

var builtin_pop(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    if (!val.is_vec()) {
        return nas_err("native::pop", "\"vec\" must be vector");
    }
    auto& vec = val.vec().elems;
    if (vec.size()) {
        auto tmp = vec.back();
        vec.pop_back();
        return tmp;
    }
    return nil;
}

var builtin_str(context* ctx, gc* ngc) {
    return ngc->newstr(ctx->localr[1].to_str());
}

var builtin_size(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];

    usize num = 0;
    switch(val.type) {
        case vm_type::vm_num:  return val;
        case vm_type::vm_str:  num = val.str().length(); break;
        case vm_type::vm_vec:  num = val.vec().size(); break;
        case vm_type::vm_hash: num = val.hash().size(); break;
        case vm_type::vm_map:  num = val.map().mapper.size(); break;
        default: break;
    }
    return var::num(static_cast<f64>(num));
}

var builtin_time(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    if (!val.is_num()) {
        return nas_err("native::time", "\"begin\" must be number");
    }
    auto begin = static_cast<time_t>(val.num());
    return var::num(static_cast<f64>(time(&begin)));
}

var builtin_contains(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var hash = local[1];
    var key = local[2];
    if (!hash.is_hash() || !key.is_str()) {
        return zero;
    }
    return hash.hash().elems.count(key.str())? one:zero;
}

var builtin_delete(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var hash = local[1];
    var key = local[2];
    if (!hash.is_hash()) {
        return nas_err("native::delete", "\"hash\" must be hash");
    }
    if (!key.is_str()) {
        return nil;
    }
    if (hash.hash().elems.count(key.str())) {
        hash.hash().elems.erase(key.str());
    }
    return nil;
}

var builtin_keys(context* ctx, gc* ngc) {
    auto hash = ctx->localr[1];
    if (!hash.is_hash() && !hash.is_map()) {
        return nas_err("native::keys", "\"hash\" must be hash");
    }
    // avoid being sweeped
    auto res = ngc->temp = ngc->alloc(vm_type::vm_vec);
    auto& vec = res.vec().elems;
    if (hash.is_hash()) {
        for (const auto& iter : hash.hash().elems) {
            vec.push_back(ngc->newstr(iter.first));
        }
    } else {
        for (const auto& iter : hash.map().mapper) {
            vec.push_back(ngc->newstr(iter.first));
        }
    }
    ngc->temp = nil;
    return res;
}

var builtin_die(context* ctx, gc* ngc) {
    return nas_err("native::error", ctx->localr[1].to_str());
}

var builtin_find(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var needle = local[1];
    var haystack = local[2];
    usize pos = haystack.to_str().find(needle.to_str());
    if (pos==std::string::npos) {
        return var::num(-1.0);
    }
    return var::num(static_cast<f64>(pos));
}

var builtin_type(context* ctx, gc* ngc) {
    switch(ctx->localr[1].type) {
        case vm_type::vm_none: return ngc->newstr("undefined");
        case vm_type::vm_nil: return ngc->newstr("nil");
        case vm_type::vm_num: return ngc->newstr("num");
        case vm_type::vm_str: return ngc->newstr("str");
        case vm_type::vm_vec: return ngc->newstr("vec");
        case vm_type::vm_hash: return ngc->newstr("hash");
        case vm_type::vm_func: return ngc->newstr("func");
        case vm_type::vm_ghost: return ngc->newstr("ghost");
        case vm_type::vm_co: return ngc->newstr("coroutine");
        case vm_type::vm_map: return ngc->newstr("namespace");
        default: break;
    }
    return nil;
}

var builtin_substr(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var str = local[1];
    var beg = local[2];
    var len = local[3];
    if (!str.is_str()) {
        return nas_err("native::substr", "\"str\" must be string");
    }
    if (!beg.is_num() || beg.num()<0) {
        return nas_err("native::substr", "\"begin\" should be number >= 0");
    }
    if (!len.is_num() || len.num()<0) {
        return nas_err("native::substr", "\"length\" should be number >= 0");
    }
    auto begin = static_cast<usize>(beg.num());
    auto length = static_cast<usize>(len.num());
    if (begin >= str.str().length()) {
        return nas_err(
            "native::susbtr",
            "begin index out of range: " + std::to_string(begin)
        );
    }
    return ngc->newstr(str.str().substr(begin, length));
}

var builtin_streq(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var a = local[1];
    var b = local[2];
    return var::num(static_cast<f64>(
        (!a.is_str() || !b.is_str())? 0:(a.str()==b.str())
    ));
}

var builtin_left(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var str = local[1];
    var len = local[2];

    if (!str.is_str()) {
        return nas_err("native::left", "\"string\" must be string");
    }
    if (!len.is_num()) {
        return nas_err("native::left", "\"length\" must be number");
    }
    if (len.num() < 0) {
        return ngc->newstr("");
    }
    return ngc->newstr(str.str().substr(0, len.num()));
}

var builtin_right(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var str = local[1];
    var len = local[2];

    if (!str.is_str()) {
        return nas_err("native::right", "\"string\" must be string");
    }
    if (!len.is_num()) {
        return nas_err("native::right", "\"length\" must be number");
    }

    i32 length = static_cast<i32>(len.num());
    i32 srclen = static_cast<i32>(str.str().length());
    if (length > srclen) {
        length = srclen;
    }
    if (length < 0) {
        length = 0;
    }

    return ngc->newstr(str.str().substr(srclen - length, srclen));
}

var builtin_cmp(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    var a = local[1];
    var b = local[2];
    if (!a.is_str() || !b.is_str()) {
        return nas_err("native::cmp", "\"a\" and \"b\" must be string");
    }
    return var::num(static_cast<f64>(strcmp(
        a.str().c_str(),
        b.str().c_str()
    )));
}

var builtin_chr(context* ctx, gc* ngc) {
    const char* extend[] = {
        "€", " ", "‚", "ƒ", "„", "…", "†", "‡",
        "ˆ", "‰", "Š", "‹", "Œ", " ", "Ž", " ",
        " ", "‘", "’", "“", "”", "•", "–", "—",
        "˜", "™", "š", "›", "œ", " ", "ž", "Ÿ",
        " ", "¡", "¢", "£", "¤", "¥", "¦", "§",
        "¨", "©", "ª", "«", "¬", " ", "®", "¯",
        "°", "±", "²", "³", "´", "µ", "¶", "·",
        "¸", "¹", "º", "»", "¼", "½", "¾", "¿",
        "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç",
        "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï",
        "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "×",
        "Ø", "Ù", "Ú", "Û", "Ü", "Ý", "Þ", "ß",
        "à", "á", "â", "ã", "ä", "å", "æ", "ç",
        "è", "é", "ê", "ë", "ì", "í", "î", "ï",
        "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "÷",
        "ø", "ù", "ú", "û", "ü", "ý", "þ", "ÿ"
    };
    auto num = static_cast<i32>(ctx->localr[1].num());
    if (0<=num && num<128) {
        return ngc->newstr(static_cast<char>(num));
    } else if (128<=num && num<256) {
        return ngc->newstr(extend[num-128]);
    }
    return ngc->newstr(" ");
}

var builtin_char(context* ctx, gc* ngc) {
    return ngc->newstr(static_cast<unsigned char>(ctx->localr[1].num()));
}

var builtin_values(context* ctx, gc* ngc) {
    auto hash = ctx->localr[1];
    if (!hash.is_hash() && !hash.is_map()) {
        return nas_err("native::values", "\"hash\" must be hash or namespace");
    }
    auto vec = ngc->alloc(vm_type::vm_vec);
    auto& v = vec.vec().elems;
    if (hash.is_hash()) {
        for (auto& i : hash.hash().elems) {
            v.push_back(i.second);
        }
    } else {
        for (auto& i : hash.map().mapper) {
            v.push_back(*i.second);
        }
    }
    return vec;
}

var builtin_sleep(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    if (!val.is_num()) {
        return nil;
    }
#if defined(_WIN32) && !defined(_GLIBCXX_HAS_GTHREADS)
    // mingw-w64 win32 thread model has no std::this_thread
    // also msvc will use this
    Sleep(static_cast<i64>(val.num()*1e3));
#else
    std::this_thread::sleep_for (
        std::chrono::microseconds(static_cast<i64>(val.num()*1e6))
    );
#endif
    return nil;
}

var builtin_platform(context* ctx, gc* ngc) {
    return ngc->newstr(util::get_platform());
}

var builtin_version(context* ctx, gc* ngc) {
    return ngc->newstr(__nasver__);
}

var builtin_arch(context* ctx, gc* ngc) {
    return ngc->newstr(util::get_arch());
}

// md5 related functions
std::string tohex(u32 num) {
    const char str16[] = "0123456789abcdef";
    std::string str = "";
    for (u32 i = 0; i<4; i++, num >>= 8) {
        std::string tmp = "";
        for (u32 j = 0, b = num&0xff; j<2; j++, b >>= 4) {
            tmp.insert(0, 1, str16[b&0xf]);
        }
        str += tmp;
    }
    return str;
}

std::string md5(const std::string& src) {
    std::vector<u32> buff;
    usize num = ((src.length()+8)>>6)+1;
    usize buffsize = num<<4;
    buff.resize(buffsize, 0);
    for (usize i = 0; i<src.length(); i++) {
        buff[i>>2] |= (static_cast<u8>(src[i]))<<((i&0x3)<<3);
    }
    buff[src.length()>>2] |= 0x80<<(((src.length()%4))<<3);
    buff[buffsize-2] = (src.length()<<3)&0xffffffff;
    buff[buffsize-1] = ((src.length()<<3)>>32)&0xffffffff;

    // u32(abs(sin(i+1))*(2pow32))
    const u32 k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    // left shift bits
    const u32 s[] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    // index
    const u32 idx[] = {
        0, 1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, // g=i
        1, 6, 11, 0,  5,  10, 15, 4,  9,  14, 3,  8,  13, 2,  7,  12, // g=(5*i+1)%16;
        5, 8, 11, 14, 1,  4,  7,  10, 13, 0,  3,  6,  9,  12, 15, 2,  // g=(3*i+5)%16;
        0, 7, 14, 5,  12, 3,  10, 1,  8,  15, 6,  13, 4,  11, 2,  9   // g=(7*i)%16;
    };
    
#define shift(x, n)  (((x)<<(n))|((x)>>(32-(n)))) // cycle left shift
#define md5f(x, y, z) (((x)&(y))|((~x)&(z)))    
#define md5g(x, y, z) (((x)&(z))|((y)&(~z)))
#define md5h(x, y, z) ((x)^(y)^(z))
#define md5i(x, y, z) ((y)^((x)|(~z)))
    
    u32 atmp = 0x67452301, btmp = 0xefcdab89;
    u32 ctmp = 0x98badcfe, dtmp = 0x10325476;
    for (u32 i = 0; i<buffsize; i += 16) {
        u32 f, a = atmp, b = btmp, c = ctmp, d = dtmp;
        for (u32 j = 0; j<64; j++) {
            if (j<16)      f = md5f(b, c, d);
            else if (j<32) f = md5g(b, c, d);
            else if (j<48) f = md5h(b, c, d);
            else           f = md5i(b, c, d);
            u32 tmp = d;
            d = c;
            c = b;
            b = b+shift((a+f+k[j]+buff[i+idx[j]]), s[j]);
            a = tmp;
        }
        atmp += a;
        btmp += b;
        ctmp += c;
        dtmp += d;
    }
    return tohex(atmp)+tohex(btmp)+tohex(ctmp)+tohex(dtmp);
}

var builtin_md5(context* ctx, gc* ngc) {
    auto str = ctx->localr[1];
    if (!str.is_str()) {
        return nas_err("native::md5", "\"str\" must be string");
    }
    return ngc->newstr(md5(str.str()));
}

class time_stamp {
private:
    std::chrono::high_resolution_clock::time_point stamp;

public:
    time_stamp() {
        stamp = std::chrono::high_resolution_clock::now();
    }

    void make_stamp() {
        stamp = std::chrono::high_resolution_clock::now();
    }

    auto elapsed_milliseconds() const {
        const auto duration = std::chrono::high_resolution_clock::now() - stamp;
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    auto elapsed_microseconds() const {
        const auto duration = std::chrono::high_resolution_clock::now() - stamp;
        return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    }
};

void time_stamp_destructor(void* ptr) {
    delete static_cast<time_stamp*>(ptr);
}

var builtin_maketimestamp(context* ctx, gc* ngc) {
    auto res = ngc->alloc(vm_type::vm_ghost);
    res.ghost().set(
        "nasal-time-stamp",
        time_stamp_destructor,
        nullptr,
        new time_stamp
    );
    return res;
}

var builtin_time_stamp(context* ctx, gc* ngc) {
    auto object = ctx->localr[1];
    if (!object.object_check("nasal-time-stamp")) {
        return nil;
    }
    auto stamp = object.ghost().get<time_stamp>();
    stamp->make_stamp();
    return nil;
}

var builtin_elapsed_millisecond(context* ctx, gc* ngc) {
    auto object = ctx->localr[1];
    if (!object.object_check("nasal-time-stamp")) {
        return var::num(-1);
    }
    auto stamp = object.ghost().get<time_stamp>();
    return var::num(static_cast<f64>(stamp->elapsed_milliseconds()));
}

var builtin_elapsed_microsecond(context* ctx, gc* ngc) {
    auto object = ctx->localr[1];
    if (!object.object_check("nasal-time-stamp")) {
        return var::num(-1);
    }
    auto stamp = object.ghost().get<time_stamp>();
    return var::num(static_cast<f64>(stamp->elapsed_microseconds()));
}

var builtin_gcextend(context* ctx, gc* ngc) {
    auto type = ctx->localr[1];
    if (!type.is_str()) {
        return nil;
    }
    const auto& s = type.str();
    if (s=="str") {
        ngc->extend(vm_type::vm_str);
    } else if (s=="vec") {
        ngc->extend(vm_type::vm_vec);
    } else if (s=="hash") {
        ngc->extend(vm_type::vm_hash);
    } else if (s=="func") {
        ngc->extend(vm_type::vm_func);
    } else if (s=="upval") {
        ngc->extend(vm_type::vm_upval);
    } else if (s=="ghost") {
        ngc->extend(vm_type::vm_ghost);
    } else if (s=="co") {
        ngc->extend(vm_type::vm_co);
    }
    return nil;
}

var builtin_gcinfo(context* ctx, gc* ngc) {
    var res = ngc->alloc(vm_type::vm_hash);
    auto& map = res.hash().elems;
    // using ms
    map["total"] = var::num(ngc->status.gc_time_ms());
    map["average"] = var::num(ngc->status.avg_time_ms());
    map["mark_count"] = var::num(ngc->status.total_mark_count);
    map["sweep_count"] = var::num(ngc->status.total_sweep_count);
    map["avg_mark"] = var::num(ngc->status.avg_mark_time_ms());
    map["avg_sweep"] = var::num(ngc->status.avg_sweep_time_ms());
    map["max_mark"] = var::num(ngc->status.max_mark_time_ms());
    map["max_sweep"] = var::num(ngc->status.max_sweep_time_ms());
    return res;
}

var builtin_logtime(context* ctx, gc* ngc) {
    time_t t = time(nullptr);
    tm* tm_t = localtime(&t);
    char s[64];
    snprintf(
        s, 64, "%d-%.2d-%.2d %.2d:%.2d:%.2d",
        tm_t->tm_year+1900,
        tm_t->tm_mon+1,
        tm_t->tm_mday,
        tm_t->tm_hour,
        tm_t->tm_min,
        tm_t->tm_sec
    );
    return ngc->newstr(s);
}

var builtin_ghosttype(context* ctx, gc* ngc) {
    auto arg = ctx->localr[1];
    if (!arg.is_ghost()) {
        return nas_err("native::ghosttype", "this is not a ghost object.");
    }

    const auto& name = arg.ghost().get_ghost_name();

    // https://wiki.flightgear.org/Nasal_library#ghosttype()
    // tolds us if no name has been set,
    // return a unique id (the pointer to the instance)
    if (!name.length()) {
        std::stringstream ss;
        ss << "0x" << std::hex;
        ss << arg.ghost().convert<u64>() << std::dec;
        return ngc->newstr(ss.str());
    }
    return ngc->newstr(name);
}

var builtin_set_utf8_output(context* ctx, gc* ngc) {
#ifdef _WIN32
    // allow 65001 code page
    SetConsoleOutputCP(CP_UTF8);
#endif
    // do nothing on other platforms
    return nil;
}

var builtin_terminal_size(context* ctx, gc* ngc) {
    var res = ngc->alloc(vm_type::vm_hash);
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        auto rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        auto cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        res.hash().elems["rows"] = var::num(rows);
        res.hash().elems["cols"] = var::num(cols);
    }
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    res.hash().elems["rows"] = var::num(w.ws_row);
    res.hash().elems["cols"] = var::num(w.ws_col);
#endif
    return res;
}

nasal_builtin_table builtin[] = {
    {"__print", builtin_print},
    {"__println", builtin_println},
    {"__exit", builtin_exit},
    {"__abort", builtin_abort},
    {"__append", builtin_append},
    {"__setsize", builtin_setsize},
    {"__system", builtin_system},
    {"__input", builtin_input},
    {"__split", builtin_split},
    {"__split_with_empty_substr", builtin_split_with_empty_substr},
    {"__rand", builtin_rand},
    {"__id", builtin_id},
    {"__int", builtin_int},
    {"__floor", builtin_floor},
    {"__ceil", builtin_ceil},
    {"__num", builtin_num},
    {"__pop", builtin_pop},
    {"__str", builtin_str},
    {"__size", builtin_size},
    {"__time", builtin_time},
    {"__contains", builtin_contains},
    {"__delete", builtin_delete},
    {"__keys", builtin_keys},
    {"__die", builtin_die},
    {"__find", builtin_find},
    {"__type", builtin_type},
    {"__substr", builtin_substr},
    {"__streq", builtin_streq},
    {"__left", builtin_left},
    {"__right", builtin_right},
    {"__cmp", builtin_cmp},
    {"__chr", builtin_chr},
    {"__char", builtin_char},
    {"__values", builtin_values},
    {"__sleep", builtin_sleep},
    {"__platform", builtin_platform},
    {"__arch", builtin_arch},
    {"__version", builtin_version},
    {"__md5", builtin_md5},
    {"__maketimestamp", builtin_maketimestamp},
    {"__time_stamp", builtin_time_stamp},
    {"__elapsed_millisecond", builtin_elapsed_millisecond},
    {"__elapsed_microsecond", builtin_elapsed_microsecond},
    {"__gcextd", builtin_gcextend},
    {"__gcinfo", builtin_gcinfo},
    {"__logtime", builtin_logtime},
    {"__ghosttype", builtin_ghosttype},
    {"__set_utf8_output", builtin_set_utf8_output},
    {"__terminal_size", builtin_terminal_size},
    {nullptr, nullptr}
};

}
