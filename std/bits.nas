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
#   bits.fld(s,0,3);
#   if s stores 10100010(162)
#   will get 101(5).
var fld = func(str, startbit, len) {
    return __fld;
}

# get sign-extended data from a special string. for example:
#   bits.sfld(s,0,3);
#   if s stores 10100010(162)
#   will get 101(5) then this will be signed extended to
#   11111101(-3).
var sfld = func(str, startbit, len) {
    return __sfld;
}

# set value into a special string to store it. little-endian, for example:
#   bits.setfld(s,0,8,69);
#   set 01000101(69) to string will get this:
#   10100010(162)
#   so s[0]=162.
var setfld = func(str, startbit, len, val) {
    return __setfld;
}

# get a special string filled by '\0' to use in setfld.
var buf = func(len) {
    return __buf;
}

var bit = func() {
    var res = [var __ = 1];
    var append = func(vec, arg...) {
        return __append;
    }
    for(var i = 1; i<32; i += 1) {
        append(res, __ += __);
    }
    return res;
}();

var test = func(n, b) {
    n /= bit[b];
    return int(n) != int(n/2)*2;
}

# returns number <n> with bit <b> set
var set = func(n, b) {
    return n+!test(n, b)*bit[b];
}


# returns number <n> with bit <b> cleared
var clear = func(n, b) {
    return n-test(n, b)*bit[b];
}


# returns number <n> with bit <b> toggled
var toggle = func(n, b) {
    return test(n, b) ? (n-bit[b]):(n+bit[b]);
}


# returns number <n> with bit <b> set to value <v>
var switch = func(n, b, v) {
    return n-(test(n, b)-!!v)*bit[b];
}


# returns number <n> as bit string, zero-padded to <len> digits:
#   bits.string(6)     ->       "110"
#   bits.string(6, 8)  ->  "00000110"
var string = func(n, len = 1) {
	var s = "";
	while (n) {
		var v = int(n/2);
		s = (v+v!=n)~s;
		n = v;
	}
	for (var i = size(s); i<len; i += 1)
		s = '0'~s;
	return s;
}


# returns bit string <s> as number:  bits.value("110")  ->  6
var value = func(s) {
	var n = 0;
	var len = size(s);
	for (var i = 0; i<len; i += 1)
		n += n+(s[i]!= `0`);
	return n;
}