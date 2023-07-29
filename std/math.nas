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

var sqrt = func(x) {
    return __sqrt(x);
}

var atan2 = func(x, y) {
    return __atan2(x, y);
}

var isnan = func(x) {
    return __isnan(x);
}

var max = func(x, y) {
    return x>y? x:y;
}

var min = func(x, y) {
    return x<y? x:y;
}
