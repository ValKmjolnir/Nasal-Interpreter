# nasal lib math.nas
# 2020/2/4
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
    }
};
var __e=2.7182818284590452354;
var __pi=3.14159265358979323846;
var __ln_2=0.69314718055994530942;
var __ln_10=2.30258509299404568402;

var abs=func(number)
{
    if(number>0)
        return number;
    else
        return -1*number;
}

var sin=func(number)
{
    return number;
}

var cos=func(number)
{
    return number;
}

var tan=func(number)
{
    return sin(number)/cos(number);
}

var cot=func(number)
{
    return cos(number)/sin(number);
}

var exp=func(number)
{
    var int_num=int(number);
    var f_num=number-int_num;
    var __res_exp=exp(int_num);
    var pw=1;
    for(var i=1;i<6;i+=1)
    {
        __res_exp+=pw/i;
        pw*=f_num;
    }
    return __res_exp;
}

var ln=func(number)
{
    return number;
}

var sqrt=func(number)
{
    var temp = number/8 + 0.5 + 2*number/(4+number);
    var cnt = 10;
    while(cnt!=0)
    {
        cnt-=1;
        temp = (temp+number/temp)/2;
    }
    return temp;
}

var atan2=func(x,y)
{
    return y/x;
}

