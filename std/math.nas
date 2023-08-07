# math.nas
# 2023 by ValKmjolnir
# mostly used math functions and special constants, you know.

# constant e
var e = 2.7182818284590452354;

# constant pi
var pi = 3.14159265358979323846264338327950288;

# maybe useless, just tell you
# how to make inf and nan in IEEE754 double.
# carefully using these two constants,
# may cause critical error in calculation.
var inf = 1/0;

# nan can be created by (inf - inf) or (-inf + inf)
var nan = 0/0;

var abs = func(x) {
    return x>0? x:-x;
}

var sgn = func(x) {
    return x<0? -1:(x>0);
}

# floor will get the integral number of input argument
# which is less than or equal to this argument.
# this is basic native function in old nasal, 
# but we think it should have a copy in math module.
var floor = func(x) {
    return __floor(x);
}

var pow = func(x, y) {
    return __pow(x, y);
}

var sin = func(x) {
    return __sin(x);
}

var cos = func(x) {
    return __cos(x);
}

var tan = func(x) {
    return __tan(x);
}

var exp = func(x) {
    return __exp(x);
}

var lg = func(x) {
    return __lg(x);
}

var ln = func(x) {
    return __ln(x);
}

var _iln10 = 1/ln(10);

# log10 is alias of lg
var log10 = lg;

var sqrt = func(x) {
    return __sqrt(x);
}

var atan2 = func(x, y) {
    return __atan2(x, y);
}

var isnan = func(x) {
    return __isnan(x);
}

var max = func(x, arg...) {
    var res = x;
    foreach(var i;arg) {
        if (i>res) {
            res = i;
        }
    }
    return res;
}

var min = func(x, arg...) {
    var res = x;
    foreach(var i;arg) {
        if (i<res) {
            res = i;
        }
    }
    return res;
}

var avg = func(arg...) {
    var x = 0;
    foreach(var i;arg) {
        x += i;
    }
    return x/size(arg);
}

var mod = func(n, m) {
    var x = n-int(n/m)*m;
    return x<0? x+abs(m):x;
}

var fmod = func(n, m) {
    return n-int(n/m)*m;
}

var clamp = func(value, min, max) {
    return (value<min)? min:((value>max)? max:value);
}

var approx_eq = func(a, b, d = 0.000001) {
    return abs(a-b)<d;
}