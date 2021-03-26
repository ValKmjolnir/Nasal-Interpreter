var import=func(filename)
{
    __builtin_import(filename);
    return nil;
}
var print=func(elements...)
{
    __builtin_std_cout(elements);
    return nil;
};
var println=func(elements...)
{
    __builtin_std_cout(elements);
    print('\n');
    return nil;
}
var append=func(vector,elements...)
{
    __builtin_push_back(vector,elements);
    return nil;
}
var setsize=func(vector,size)
{
    __builtin_set_size(vector,size);
    return nil;
}
var system=func(str)
{
    __builtin_system(str);
    return;
}
var input=func()
{
    return __builtin_input();
}
var sleep=func(duration)
{
    __builtin_sleep(duration);
    return;
}
var split=func(delimeter,string)
{
    return __builtin_split(delimeter,string);
}
var rand=func(seed=nil)
{
    return __builtin_rand(seed);
}
var id=func(thing)
{
    return __builtin_get_id(thing);
}
var int=func(value)
{
    return __builtin_int(value);
}
var num=func(value)
{
    return __builtin_num(value);
}
var pop=func(vector)
{
    return __builtin_pop_back(vector);
}
var str=func(number)
{
    return __builtin_str(number);
}
var size=func(object)
{
    return __builtin_size(object);
}
var contains=func(hash,key)
{
    return __builtin_contains(hash,key);
}
var delete=func(hash,key)
{
    __builtin_delete(hash,key);
    return;
}
var keys=func(hash)
{
    return __builtin_get_keys(hash);
}
var time=func(begin_time)
{
    return __builtin_time(begin_time);
}
var die=func(str)
{
    __builtin_die(str);
    return nil;
}
var typeof=func(object)
{
    return __builtin_type(object);
}
var substr=func(str,begin,length)
{
    return __builtin_substr(str,begin,length);
}
var streq=func(a,b)
{
    return __builtin_streq(a,b);
}
var left=func(string,length)
{
    return __builtin_left(string,length);
}
var right=func(string,length)
{
    return __builtin_right(string,length);
}
var cmp=func(a,b)
{
    return __builtin_cmp(a,b);
}
var chr=func(code) #//Unlike in FG, this chr does not support Extended ASCII
{
    return __builtin_chr(code);
}

var io=
{
    fin:func(filename)
    {
        return __builtin_fin(filename);
    },
    fout:func(filename,str)
    {
        __builtin_fout(filename,str);
        return;
    }
};

var bits=
{
    bitxor:  func(a,b){return __builtin_xor(a,b); },
    bitand:  func(a,b){return __builtin_and(a,b); },
    bitor:   func(a,b){return __builtin_or(a,b);  },
    bitnand: func(a,b){return __builtin_nand(a,b);},
    bitnot:  func(a)  {return __builtin_not(a);   }
};

var math=
{
    e:     2.7182818284590452354,
    pi:    3.14159265358979323846264338327950288,
    sin:   func(x)  {return __builtin_sin(x);    },
    cos:   func(x)  {return __builtin_cos(x);    },
    tan:   func(x)  {return __builtin_tan(x);    },
    exp:   func(x)  {return __builtin_exp(x);    }, 
    ln:    func(x)  {return __builtin_ln(x);     },
    sqrt:  func(x)  {return __builtin_sqrt(x);   },
    atan2: func(x,y){return __builtin_atan2(x,y);}
};
