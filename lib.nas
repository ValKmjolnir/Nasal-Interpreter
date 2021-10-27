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
    fout:func(filename,str){return __builtin_fout(filename,str);},
    SEEK_SET:0,
    SEEK_CUR:1,
    SEEK_END:2,
    open:func(filename,mode="r"){return __builtin_open(filename,mode);},
    close:func(filehandle){return __builtin_close(filehandle);},
    read:func(filehandle,buf,len){return __builtin_read(filehandle,buf,len);},
    write:func(filehandle,str){return __builtin_write(filehandle,str);},
    seek:func(filehandle,pos,whence){return __builtin_seek(filehandle,pos,whence);},
    tell:func(filehandle){return __builtin_tell(filehandle);},
    readln:func(filehandle){return __builtin_readln(filehandle);},
    stat:func(filename){return __builtin_stat(filename);}
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
    nan:   0/0,
    isnan: func(x)  {return __builtin_isnan(x);  }
};

var D2R=math.pi/180;
var FPS2KT=0.5925;
var FT2M=0.3048;
var GAL2L=3.7854;
var IN2M=0.0254;
var KG2LB=2.2046;
var KT2FPS=1.6878;
var KT2MPS=0.5144;
var L2GAL=0.2642;
var LB2KG=0.4536;
var M2FT=3.2808;
var M2IN=39.3701;
var M2NM=0.00054;
var MPS2KT=1.9438;
var NM2M=1852;
var R2D=180/math.pi;