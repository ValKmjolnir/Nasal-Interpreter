#include "bits_lib.h"

namespace nasal {

var builtin_u32xor(var* local, gc& ngc) {
    return var::num(static_cast<f64>(
        static_cast<u32>(local[1].num()) ^
        static_cast<u32>(local[2].num())
    ));
}

var builtin_u32and(var* local, gc& ngc) {
    return var::num(static_cast<f64>(
        static_cast<u32>(local[1].num()) &
        static_cast<u32>(local[2].num())
    ));
}

var builtin_u32or(var* local, gc& ngc) {
    return var::num(static_cast<f64>(
        static_cast<u32>(local[1].num()) |
        static_cast<u32>(local[2].num())
    ));
}

var builtin_u32nand(var* local, gc& ngc) {
    return var::num(static_cast<f64>(~(
        static_cast<u32>(local[1].num()) &
        static_cast<u32>(local[2].num())
    )));
}

var builtin_u32not(var* local, gc& ngc) {
    return var::num(static_cast<f64>(~static_cast<u32>(local[1].num())));
}

var builtin_fld(var* local, gc& ngc) {
    // bits.fld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5)
    var str = local[1];
    var startbit = local[2];
    var length = local[3];
    if (str.type!=vm_str || str.val.gcobj->unmut) {
        return nas_err("fld", "\"str\" must be mutable string");
    }
    if (startbit.type!=vm_num || length.type!=vm_num) {
        return nas_err("fld", "\"startbit\",\"len\" must be number");
    }
    u32 bit = static_cast<u32>(startbit.num());
    u32 len = static_cast<u32>(length.num());
    if (bit+len>8*str.str().length()) {
        return nas_err("fld", "bitfield out of bounds");
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

var builtin_sfld(var* local, gc& ngc) {
    // bits.sfld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5) then this will be signed extended to
    // 11111101(-3)
    var str = local[1];
    var startbit = local[2];
    var length = local[3];
    if (str.type!=vm_str || str.val.gcobj->unmut) {
        return nas_err("sfld", "\"str\" must be mutable string");
    }
    if (startbit.type!=vm_num || length.type!=vm_num) {
        return nas_err("sfld", "\"startbit\",\"len\" must be number");
    }
    u32 bit = static_cast<u32>(startbit.num());
    u32 len = static_cast<u32>(length.num());
    if (bit+len>8*str.str().length()) {
        return nas_err("sfld", "bitfield out of bounds");
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

var builtin_setfld(var* local, gc& ngc) {
    // bits.setfld(s,0,8,69);
    // set 01000101(69) to string will get this:
    // 10100010(162)
    // so s[0]=162
    var str = local[1];
    var startbit = local[2];
    var length = local[3];
    var value = local[4];
    if (str.type!=vm_str || str.val.gcobj->unmut) {
        return nas_err("setfld", "\"str\" must be mutable string");
    }
    if (startbit.type!=vm_num || length.type!=vm_num || value.type!=vm_num) {
        return nas_err("setfld", "\"startbit\",\"len\",\"val\" must be number");
    }
    u32 bit = static_cast<u32>(startbit.num());
    u32 len = static_cast<u32>(length.num());
    u64 val = static_cast<u64>(value.num());
    if (bit+len>8*str.str().length()) {
        return nas_err("setfld", "bitfield out of bounds");
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

var builtin_buf(var* local, gc& ngc) {
    var length = local[1];
    if (length.type!=vm_num || length.num()<=0) {
        return nas_err("buf", "\"len\" must be number greater than 0");
    }
    var str = ngc.alloc(vm_str);
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
