var print=func(elements...)
{
    nasal_call_builtin_std_cout(elements);
    return nil;
};
var append=func(vector,elements...)
{
    nasal_call_builtin_push_back(vector,elements);
    return nil;
}
var setsize=func(vector,size)
{
    nasal_call_builtin_set_size(vector,size);
    return nil;
}
var system=func(str)
{
    nasal_call_builtin_system(str);
    return;
}
var input=func()
{
    return nasal_call_builtin_input();
}
var sleep=func(duration)
{
    nasal_call_builtin_sleep(duration);
    return;
}
var split=func(delimeter,string)
{
    return nasal_call_builtin_split(delimeter,string);
}
var rand=func(seed=nil)
{
    return nasal_call_builtin_rand(seed);
}
var id=func(thing)
{
    return nasal_call_builtin_get_id(thing);
}
var int=func(value)
{
    return nasal_call_builtin_trans_int(value);
}
var num=func(value)
{
    return nasal_call_builtin_trans_num(value);
}
var pop=func(vector)
{
    return nasal_call_builtin_pop_back(vector);
}
var str=func(number)
{
    return nasal_call_builtin_trans_str(number);
}
var size=func(object)
{
    return nasal_call_builtin_size(object);
}

var io=
{
    fin:func(filename)
    {
        return nasal_call_builtin_finput(filename);
    },
    fout:func(filename,str)
    {
        nasal_call_builtin_foutput(filename,str);
        return;
    }
};

var bits=
{
    bitxor:func(a,b)
    {
        return nasal_call_builtin_xor(a,b);
    },
    bitand:func(a,b)
    {
        return nasal_call_builtin_and(a,b);
    },
    bitor:func(a,b)
    {
        return nasal_call_builtin_or(a,b);
    },
    bitnand:func(a,b)
    {
        return nasal_call_builtin_nand(a,b);
    },
    bitnot:func(a)
    {
        return nasal_call_builtin_not(a);
    }
};

var math=
{
    e:2.7182818284590452354,
    pi:3.14159265358979323846,
    sin:func(x)
    {
        return nasal_call_builtin_sin(x);
    },
    cos:func(x)
    {
        return nasal_call_builtin_cos(x);
    },
    tan:func(x)
    {
        return nasal_call_builtin_tan(x);
    },
    exp:func(x)
    {
        return nasal_call_builtin_exp(x);
    }, 
    ln:func(x)
    {
        return nasal_call_builtin_cpp_math_ln(x);
    },
    sqrt:func(x)
    {
        return nasal_call_builtin_cpp_math_sqrt(x);
    },
    atan2:func(x,y)
    {
        return nasal_call_builtin_cpp_atan2(x,y);
    },
};

var time=func(begin_time)
{
    return nasal_call_builtin_time(begin_time);
}