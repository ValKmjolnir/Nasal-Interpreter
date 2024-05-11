#include "natives/bits_lib.h"

namespace nasal {

var builtin_u32xor(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    return var::num(static_cast<f64>(
        static_cast<u32>(local[1].num()) ^
        static_cast<u32>(local[2].num())
    ));
}

var builtin_u32and(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    return var::num(static_cast<f64>(
        static_cast<u32>(local[1].num()) &
        static_cast<u32>(local[2].num())
    ));
}

var builtin_u32or(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    return var::num(static_cast<f64>(
        static_cast<u32>(local[1].num()) |
        static_cast<u32>(local[2].num())
    ));
}

var builtin_u32nand(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    return var::num(static_cast<f64>(~(
        static_cast<u32>(local[1].num()) &
        static_cast<u32>(local[2].num())
    )));
}

var builtin_u32not(context* ctx, gc* ngc) {
    return var::num(static_cast<f64>(
        ~static_cast<u32>(ctx->localr[1].num())
    ));
}

var builtin_fld(context* ctx, gc* ngc) {
    // bits.fld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5)
    auto local = ctx->localr;
    auto str = local[1];
    auto startbit = local[2];
    auto length = local[3];
    if (!str.is_str() || str.val.gcobj->immutable) {
        return nas_err("bits::fld", "\"str\" must be mutable string");
    }
    if (!startbit.is_num() || !length.is_num()) {
        return nas_err("bits::fld", "\"startbit\", \"len\" must be number");
    }
    u32 bit = static_cast<u32>(startbit.num());
    u32 len = static_cast<u32>(length.num());
    if (bit+len>8*str.str().length()) {
        return nas_err("bits::fld", "bitfield out of bounds");
    }
    u32 res = 0;
    auto& s = str.str();
    for(u32 i = bit; i<bit+len; ++i) {
        if (s[i>>3]&(1<<(7-(i&7)))) {
            res |= 1<<(bit+len-i-1);
        }
    }
    return var::num(static_cast<f64>(res));
}

var builtin_sfld(context* ctx, gc* ngc) {
    // bits.sfld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5) then this will be signed extended to
    // 11111101(-3)
    auto local = ctx->localr;
    auto str = local[1];
    auto startbit = local[2];
    auto length = local[3];
    if (!str.is_str() || str.val.gcobj->immutable) {
        return nas_err("bits::sfld", "\"str\" must be mutable string");
    }
    if (!startbit.is_num() || !length.is_num()) {
        return nas_err("bits::sfld", "\"startbit\",\"len\" must be number");
    }
    u32 bit = static_cast<u32>(startbit.num());
    u32 len = static_cast<u32>(length.num());
    if (bit+len>8*str.str().length()) {
        return nas_err("bits::sfld", "bitfield out of bounds");
    }
    u32 res = 0;
    auto& s = str.str();
    for(u32 i = bit; i<bit+len; ++i) {
        if (s[i>>3]&(1<<(7-(i&7)))) {
            res |= 1<<(bit+len-i-1);
        }
    }
    if (res&(1<<(len-1))) {
        res |= ~((1<<len)-1);
    }
    return var::num(static_cast<f64>(static_cast<i32>(res)));
}

var builtin_setfld(context* ctx, gc* ngc) {
    // bits.setfld(s,0,8,69);
    // set 01000101(69) to string will get this:
    // 10100010(162)
    // so s[0]=162
    auto local = ctx->localr;
    auto str = local[1];
    auto startbit = local[2];
    auto length = local[3];
    auto value = local[4];
    if (!str.is_str() || str.val.gcobj->immutable) {
        return nas_err("bits::setfld", "\"str\" must be mutable string");
    }
    if (!startbit.is_num() || !length.is_num() || !value.is_num()) {
        return nas_err("bits::setfld",
            "\"startbit\", \"len\", \"val\" must be number"
        );
    }
    u32 bit = static_cast<u32>(startbit.num());
    u32 len = static_cast<u32>(length.num());
    u64 val = static_cast<u64>(value.num());
    if (bit+len>8*str.str().length()) {
        return nas_err("bits::setfld", "bitfield out of bounds");
    }
    auto& s = str.str();
    for(u32 i = bit; i<bit+len; ++i) {
        if (val&(1<<(i-bit))) {
            s[i>>3] |= (1<<(7-(i&7)));
        } else {
            s[i>>3] &= ~(1<<(7-(i&7)));
        }
    }
    return nil;
}

var builtin_buf(context* ctx, gc* ngc) {
    var length = ctx->localr[1];
    if (!length.is_num() || length.num()<=0) {
        return nas_err("bits::buf", "\"len\" must be number greater than 0");
    }
    var str = ngc->alloc(vm_type::vm_str);
    auto& s = str.str();
    s.resize(length.num(), '\0');
    return str;
}

nasal_builtin_table bits_native[] = {
    {"__u32xor", builtin_u32xor},
    {"__u32and", builtin_u32and},
    {"__u32or", builtin_u32or},
    {"__u32nand", builtin_u32nand},
    {"__u32not", builtin_u32not},
    {"__fld", builtin_fld},
    {"__sfld", builtin_sfld},
    {"__setfld", builtin_setfld},
    {"__buf", builtin_buf},
    {nullptr, nullptr}
};

}
