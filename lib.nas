var import=func(filename)
{
    return __builtin_import(filename);
}
var print=func(elems...)
{
    return __builtin_print(elems);
};
var println=func(elems...)
{
    __builtin_print(elems);
    elems=['\n'];
    return __builtin_print(elems);
}
var append=func(vec,elems...)
{
    return __builtin_append(vec,elems);
}
var setsize=func(vec,size)
{
    return __builtin_setsize(vec,size);
}
var system=func(str)
{
    return __builtin_system(str);
}
var input=func()
{
    return __builtin_input();
}
var sleep=func(duration)
{
    return __builtin_sleep(duration);
}
var split=func(deli,str)
{
    return __builtin_split(deli,str);
}
var rand=func(seed=nil)
{
    return __builtin_rand(seed);
}
var id=func(object)
{
    return __builtin_id(object);
}
var int=func(val)
{
    return __builtin_int(val);
}
var num=func(val)
{
    return __builtin_num(val);
}
var pop=func(vec)
{
    return __builtin_pop(vec);
}
var str=func(num)
{
    return __builtin_str(num);
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
    return __builtin_delete(hash,key);
}
var keys=func(hash)
{
    return __builtin_keys(hash);
}
var time=func(begin_time)
{
    return __builtin_time(begin_time);
}
var die=func(str)
{
    return __builtin_die(str);
}
var typeof=func(object)
{
    return __builtin_type(object);
}
var substr=func(str,begin,len)
{
    return __builtin_substr(str,begin,len);
}
var streq=func(a,b)
{
    return __builtin_streq(a,b);
}
var left=func(str,len)
{
    return __builtin_left(str,len);
}
var right=func(str,len)
{
    return __builtin_right(str,len);
}
var cmp=func(a,b)
{
    return __builtin_cmp(a,b);
}
var chr=func(code)
{
    return __builtin_chr(code);
}

var io=
{
    fin: func(filename){return __builtin_fin(filename);},
    fout:func(filename,str){return __builtin_fout(filename,str);}
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
    atan2: func(x,y){return __builtin_atan2(x,y);},
    inf:   1/0,
    nan:   1/0-1/0,
    isnan: func(x)  {return __builtin_isnan(x);  }
};

var D2R=math.pi/180;