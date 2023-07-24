# bits.nas
# 2023 by ValKmjolnir
# functions that do bitwise calculation.
# carefully use it, all the calculations are based on integer.

# u32 xor
var u32_xor = func(a, b) {
    return __u32xor(a, b);
}

# u32 and
var u32_and = func(a, b) {
    return __u32and(a, b);
}

# u32 or
var u32_or = func(a, b) {
    return __u32or(a, b);
}

# u32 nand
var u32_nand = func(a, b) {
    return __u32nand(a, b);
}

# u32 not
var u32_not = func(a) {
    return __u32not(a);
}

# get bit data from a special string. for example:
# bits.fld(s,0,3);
# if s stores 10100010(162)
# will get 101(5).
var fld = func(str, startbit, len) {
    return __fld;
}

# get sign-extended data from a special string. for example:
# bits.sfld(s,0,3);
# if s stores 10100010(162)
# will get 101(5) then this will be signed extended to
# 11111101(-3).
var sfld = func(str, startbit, len) {
    return __sfld;
}

# set value into a special string to store it. little-endian, for example:
# bits.setfld(s,0,8,69);
# set 01000101(69) to string will get this:
# 10100010(162)
# so s[0]=162.
var setfld = func(str, startbit, len, val) {
    return __setfld;
}

# get a special string filled by '\0' to use in setfld.
var buf = func(len) {
    return __buf;
}