# nasal lib math.nas
# 2020/2/6
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

var math=
{
    e:2.7182818284590452354,
    pi:3.14159265358979323846,
    sin:func(x)
    {
        var call_inline_sin=func(x){};
        return call_inline_sin(x);
    },
    cos:func(x)
    {
        var call_inline_cos=func(x){};
        return call_inline_cos(x);
    },
    tan:func(x){return me.sin(x)/me.cos(x);},
    exp:func(x)
    {
        var call_inline_pow=func(num,x){};
        return call_inline_pow(me.e,x);
    },
    ln:func(){},
    sqrt:func(){},
    atan2:func(){},
};