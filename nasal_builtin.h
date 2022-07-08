#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__

#if defined __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

// declaration of builtin functions
// to add new builtin function, declare it here and write the definition below
#define nas_native(name) nasal_ref name(nasal_ref*,nasal_gc&)
nas_native(builtin_print);
nas_native(builtin_println);
nas_native(builtin_abort);
nas_native(builtin_append);
nas_native(builtin_setsize);
nas_native(builtin_system);
nas_native(builtin_input);
nas_native(builtin_fin);
nas_native(builtin_fout);
nas_native(builtin_split);
nas_native(builtin_rand);
nas_native(builtin_id);
nas_native(builtin_int);
nas_native(builtin_floor);
nas_native(builtin_num);
nas_native(builtin_pop);
nas_native(builtin_str);
nas_native(builtin_size);
nas_native(builtin_i32xor);
nas_native(builtin_i32and);
nas_native(builtin_i32or);
nas_native(builtin_i32nand);
nas_native(builtin_i32not);
nas_native(builtin_u32xor);
nas_native(builtin_u32and);
nas_native(builtin_u32or);
nas_native(builtin_u32nand);
nas_native(builtin_u32not);
nas_native(builtin_pow);
nas_native(builtin_sin);
nas_native(builtin_cos);
nas_native(builtin_tan);
nas_native(builtin_exp);
nas_native(builtin_lg);
nas_native(builtin_ln);
nas_native(builtin_sqrt);
nas_native(builtin_atan2);
nas_native(builtin_isnan);
nas_native(builtin_time);
nas_native(builtin_contains);
nas_native(builtin_delete);
nas_native(builtin_keys);
nas_native(builtin_die);
nas_native(builtin_find);
nas_native(builtin_type);
nas_native(builtin_substr);
nas_native(builtin_streq);
nas_native(builtin_left);
nas_native(builtin_right);
nas_native(builtin_cmp);
nas_native(builtin_chr);
nas_native(builtin_values);
nas_native(builtin_open);
nas_native(builtin_close);
nas_native(builtin_read);
nas_native(builtin_write);
nas_native(builtin_seek);
nas_native(builtin_tell);
nas_native(builtin_readln);
nas_native(builtin_stat);
nas_native(builtin_eof);
nas_native(builtin_fld);
nas_native(builtin_sfld);
nas_native(builtin_setfld);
nas_native(builtin_buf);
nas_native(builtin_sleep);
nas_native(builtin_pipe);
nas_native(builtin_fork);
nas_native(builtin_waitpid);
nas_native(builtin_opendir);
nas_native(builtin_readdir);
nas_native(builtin_closedir);
nas_native(builtin_chdir);
nas_native(builtin_environ);
nas_native(builtin_getcwd);
nas_native(builtin_getenv);
nas_native(builtin_dlopen);
nas_native(builtin_dlsym);
nas_native(builtin_dlclose);
nas_native(builtin_dlcall);
nas_native(builtin_platform);
nas_native(builtin_gc);
nas_native(builtin_md5);
nas_native(builtin_cocreate);
nas_native(builtin_coresume);
nas_native(builtin_coyield);
nas_native(builtin_costatus);
nas_native(builtin_corun);
nas_native(builtin_millisec);
nas_native(builtin_sysargv);
nas_native(builtin_logtime);

nasal_ref builtin_err(const char* func_name,const std::string& info)
{
    std::cerr<<"[vm] "<<func_name<<": "<<info<<".\n";
    return {vm_none};
}

// register builtin function's name and it's address here in this table below
// this table must end with {nullptr,nullptr}
struct
{
    const char* name;
    nasal_ref (*func)(nasal_ref*,nasal_gc&);
} builtin[]=
{
    {"__builtin_print",   builtin_print   },
    {"__builtin_println", builtin_println },
    {"__builtin_abort",   builtin_abort   },
    {"__builtin_append",  builtin_append  },
    {"__builtin_setsize", builtin_setsize },
    {"__builtin_system",  builtin_system  },
    {"__builtin_input",   builtin_input   },
    {"__builtin_fin",     builtin_fin     },
    {"__builtin_fout",    builtin_fout    },
    {"__builtin_split",   builtin_split   },
    {"__builtin_rand",    builtin_rand    },
    {"__builtin_id",      builtin_id      },
    {"__builtin_int",     builtin_int     },
    {"__builtin_floor",   builtin_floor   },
    {"__builtin_num",     builtin_num     },
    {"__builtin_pop",     builtin_pop     },
    {"__builtin_str",     builtin_str     },
    {"__builtin_size",    builtin_size    },
    {"__builtin_i32xor",  builtin_i32xor  },
    {"__builtin_i32and",  builtin_i32and  },
    {"__builtin_i32or",   builtin_i32or   },
    {"__builtin_i32nand", builtin_i32nand },
    {"__builtin_i32not",  builtin_i32not  },
    {"__builtin_u32xor",  builtin_u32xor  },
    {"__builtin_u32and",  builtin_u32and  },
    {"__builtin_u32or",   builtin_u32or   },
    {"__builtin_u32nand", builtin_u32nand },
    {"__builtin_u32not",  builtin_u32not  },
    {"__builtin_pow",     builtin_pow     },
    {"__builtin_sin",     builtin_sin     },
    {"__builtin_cos",     builtin_cos     },
    {"__builtin_tan",     builtin_tan     },
    {"__builtin_exp",     builtin_exp     },
    {"__builtin_lg",      builtin_lg      },
    {"__builtin_ln",      builtin_ln      },
    {"__builtin_sqrt",    builtin_sqrt    },
    {"__builtin_atan2",   builtin_atan2   },
    {"__builtin_isnan",   builtin_isnan   },
    {"__builtin_time",    builtin_time    },
    {"__builtin_contains",builtin_contains},
    {"__builtin_delete",  builtin_delete  },
    {"__builtin_keys",    builtin_keys    },
    {"__builtin_die",     builtin_die     },
    {"__builtin_find",    builtin_find    },
    {"__builtin_type",    builtin_type    },
    {"__builtin_substr",  builtin_substr  },
    {"__builtin_streq",   builtin_streq   },
    {"__builtin_left",    builtin_left    },
    {"__builtin_right",   builtin_right   },
    {"__builtin_cmp",     builtin_cmp     },
    {"__builtin_chr",     builtin_chr     },
    {"__builtin_values",  builtin_values  },
    {"__builtin_open",    builtin_open    },
    {"__builtin_close",   builtin_close   },
    {"__builtin_read",    builtin_read    },
    {"__builtin_write",   builtin_write   },
    {"__builtin_seek",    builtin_seek    },
    {"__builtin_tell",    builtin_tell    },
    {"__builtin_readln",  builtin_readln  },
    {"__builtin_stat",    builtin_stat    },
    {"__builtin_eof",     builtin_eof     },
    {"__builtin_fld",     builtin_fld     },
    {"__builtin_sfld",    builtin_sfld    },
    {"__builtin_setfld",  builtin_setfld  },
    {"__builtin_buf",     builtin_buf     },
    {"__builtin_sleep",   builtin_sleep   },
    {"__builtin_pipe",    builtin_pipe    },
    {"__builtin_fork",    builtin_fork    },
    {"__builtin_waitpid", builtin_waitpid },
    {"__builtin_opendir", builtin_opendir },
    {"__builtin_readdir", builtin_readdir },
    {"__builtin_closedir",builtin_closedir},
    {"__builtin_chdir",   builtin_chdir   },
    {"__builtin_environ", builtin_environ },
    {"__builtin_getcwd",  builtin_getcwd  },
    {"__builtin_getenv",  builtin_getenv  },
    {"__builtin_dlopen",  builtin_dlopen  },
    {"__builtin_dlsym",   builtin_dlsym   },
    {"__builtin_dlclose", builtin_dlclose },
    {"__builtin_dlcall",  builtin_dlcall  },
    {"__builtin_platform",builtin_platform},
    {"__builtin_gc",      builtin_gc      },
    {"__builtin_md5",     builtin_md5     },
    {"__builtin_cocreate",builtin_cocreate},
    {"__builtin_coresume",builtin_coresume},
    {"__builtin_coyield", builtin_coyield },
    {"__builtin_costatus",builtin_costatus},
    {"__builtin_corun"   ,builtin_corun   },
    {"__builtin_millisec",builtin_millisec},
    {"__builtin_sysargv", builtin_sysargv },
    {"__builtin_logtime", builtin_logtime },
    {nullptr,             nullptr         }
};

inline void print_core(std::vector<nasal_ref>& elems)
{
    for(auto& i:elems)
        switch(i.type)
        {
            case vm_none: std::cout<<"null";           break;
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<i.num();          break;
            case vm_str:  std::cout<<i.str();          break;
            case vm_vec:  i.vec().print();             break;
            case vm_hash: i.hash().print();            break;
            case vm_func: std::cout<<"func(...){...}"; break;
            case vm_obj:  std::cout<<"<object>";       break;
            case vm_co:   std::cout<<"<coroutine>";    break;
        }
}
nasal_ref builtin_print(nasal_ref* local,nasal_gc& gc)
{
    // get arguments
    // local[0] is reserved for 'me'
    nasal_ref vec=local[1];
    // main process
    print_core(vec.vec().elems);
    std::cout<<std::flush;
    // generate return value
    return nil;
}
nasal_ref builtin_println(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref vec=local[1];
    print_core(vec.vec().elems);
    std::cout<<std::endl;
    return nil;
}
nasal_ref builtin_abort(nasal_ref* local,nasal_gc& gc)
{
    std::abort();
    return nil;
}
nasal_ref builtin_append(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref vec=local[1];
    nasal_ref elem=local[2];
    if(vec.type!=vm_vec)
        return builtin_err("append","\"vector\" must be vector");
    auto& ref_vec=vec.vec().elems;
    for(auto& i:elem.vec().elems)
        ref_vec.push_back(i);
    return nil;
}
nasal_ref builtin_setsize(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref vec=local[1];
    nasal_ref size=local[2];
    if(vec.type!=vm_vec)
        return builtin_err("setsize","\"vector\" must be vector");
    if(size.type!=vm_num)
        return builtin_err("setsize","\"size\" is not a number");
    int64_t num=(int64_t)size.num();
    if(num<0)
        return builtin_err("setsize","\"size\" must be greater than -1");
    vec.vec().elems.resize(num,nil);
    return nil;
}
nasal_ref builtin_system(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    if(str.type!=vm_str)
        return builtin_err("system","\"str\" must be string");
    return {vm_num,(double)system(str.str().c_str())};
}
nasal_ref builtin_input(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref ret=gc.alloc(vm_str);
    std::cin>>ret.str();
    return ret;
}
nasal_ref builtin_fin(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_str)
        return builtin_err("io.fin","\"filename\" must be string");
    std::string&  filename=val.str();
    std::ifstream fin(filename);
    if(!fin.fail())
    {
        std::stringstream rd;
        rd<<fin.rdbuf();
        return gc.newstr(rd.str());
    }
    return builtin_err("io.fin","cannot open \""+filename+"\"");
}
nasal_ref builtin_fout(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    nasal_ref str=local[2];
    if(val.type!=vm_str)
        return builtin_err("io.fout","\"filename\" must be string");
    if(str.type!=vm_str)
        return builtin_err("io.fout","\"str\" must be string");
    std::ofstream fout(val.str());
    if(fout.fail())
        return builtin_err("io.fout","cannot open \""+val.str()+"\"");
    fout<<str.str();
    return nil;
}
nasal_ref builtin_split(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref delimeter=local[1];
    nasal_ref str=local[2];
    if(delimeter.type!=vm_str)
        return builtin_err("split","\"separator\" must be string");
    if(str.type!=vm_str)
        return builtin_err("split","\"str\" must be string");
    const std::string& deli=delimeter.str();
    const std::string& s=str.str();

    // avoid being sweeped
    nasal_ref res=gc.temp=gc.alloc(vm_vec);
    std::vector<nasal_ref>& vec=res.vec().elems;

    if(!deli.length())
    {
        for(auto i:s)
            vec.push_back(gc.newstr(i));
        gc.temp=nil;
        return res;
    }
    size_t last=0;
    size_t pos=s.find(deli,last);
    while(pos!=std::string::npos)
    {
        if(pos>last)
            vec.push_back(gc.newstr(s.substr(last,pos-last)));
        last=pos+deli.length();
        pos=s.find(deli,last);
    }
    if(last!=s.length())
        vec.push_back(gc.newstr(s.substr(last,s.length()-1)));
    gc.temp=nil;
    return res;
}
nasal_ref builtin_rand(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num && val.type!=vm_nil)
        return builtin_err("rand","\"seed\" must be nil or number");
    if(val.type==vm_num)
    {
        srand((unsigned int)val.num());
        return nil;
    }
    double num=0;
    for(int i=0;i<5;++i)
        num=(num+rand())*(1.0/(RAND_MAX+1.0));
    return {vm_num,num};
}
nasal_ref builtin_id(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    char buf[32];
    if(val.type>vm_num)
        sprintf(buf,"%p",val.value.gcobj);
    else
        sprintf(buf,"0");
    return gc.newstr(buf);
}
nasal_ref builtin_int(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return nil;
    int number=(int)val.num();
    return {vm_num,(double)number};
}
nasal_ref builtin_floor(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    return {vm_num,std::floor(val.num())};
}
nasal_ref builtin_num(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type==vm_num)
        return val;
    if(val.type!=vm_str)
        return nil;
    double res=val.tonum();
    if(std::isnan(res))
        return nil;
    return {vm_num,res};
}
nasal_ref builtin_pop(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_vec)
        return builtin_err("pop","\"vector\" must be vector");
    auto& vec=val.vec().elems;
    if(vec.size())
    {
        nasal_ref tmp=vec.back();
        vec.pop_back();
        return tmp;
    }
    return nil;
}
nasal_ref builtin_str(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("str","\"number\" must be number");
    std::string tmp=std::to_string(val.num());
    tmp.erase(tmp.find_last_not_of('0')+1,std::string::npos);
    tmp.erase(tmp.find_last_not_of('.')+1,std::string::npos);
    return gc.newstr(tmp);
}
nasal_ref builtin_size(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    double num=0;
    switch(val.type)
    {
        case vm_num:  num=val.num();         break;
        case vm_str:  num=val.str().length();break;
        case vm_vec:  num=val.vec().size();  break;
        case vm_hash: num=val.hash().size(); break;
    }
    return {vm_num,num};
}
nasal_ref builtin_i32xor(nasal_ref* local,nasal_gc& gc)
{
    int a=(int)local[1].num();
    int b=(int)local[2].num();
    return {vm_num,(double)(a^b)};
}
nasal_ref builtin_i32and(nasal_ref* local,nasal_gc& gc)
{
    int a=(int)local[1].num();
    int b=(int)local[2].num();
    return {vm_num,(double)(a&b)};
}
nasal_ref builtin_i32or(nasal_ref* local,nasal_gc& gc)
{
    int a=(int)local[1].num();
    int b=(int)local[2].num();
    return {vm_num,(double)(a|b)};
}
nasal_ref builtin_i32nand(nasal_ref* local,nasal_gc& gc)
{
    int a=(int)local[1].num();
    int b=(int)local[2].num();
    return {vm_num,(double)(~(a&b))};
}
nasal_ref builtin_i32not(nasal_ref* local,nasal_gc& gc)
{
    int n=(int)local[1].num();
    return {vm_num,(double)(~n)};
}
nasal_ref builtin_u32xor(nasal_ref* local,nasal_gc& gc)
{
    uint32_t a=(uint32_t)local[1].num();
    uint32_t b=(uint32_t)local[2].num();
    return {vm_num,(double)(a^b)};
}
nasal_ref builtin_u32and(nasal_ref* local,nasal_gc& gc)
{
    uint32_t a=(uint32_t)local[1].num();
    uint32_t b=(uint32_t)local[2].num();
    return {vm_num,(double)(a&b)};
}
nasal_ref builtin_u32or(nasal_ref* local,nasal_gc& gc)
{
    uint32_t a=(uint32_t)local[1].num();
    uint32_t b=(uint32_t)local[2].num();
    return {vm_num,(double)(a|b)};
}
nasal_ref builtin_u32nand(nasal_ref* local,nasal_gc& gc)
{
    uint32_t a=(uint32_t)local[1].num();
    uint32_t b=(uint32_t)local[2].num();
    return {vm_num,(double)(~(a&b))};
}
nasal_ref builtin_u32not(nasal_ref* local,nasal_gc& gc)
{
    uint32_t n=(uint32_t)local[1].num();
    return {vm_num,(double)(~n)};
}
nasal_ref builtin_pow(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref x=local[1];
    nasal_ref y=local[2];
    if(x.type!=vm_num || y.type!=vm_num)
        return builtin_err("pow","\"x\" or \"y\" must be number");
    return {vm_num,std::pow(x.num(),y.num())};
}
nasal_ref builtin_sin(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("sin","\"x\" must be number");
    return {vm_num,sin(val.num())};
}
nasal_ref builtin_cos(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("cos","\"x\" must be number");
    return {vm_num,cos(val.num())};
}
nasal_ref builtin_tan(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("tan","\"x\" must be number");
    return {vm_num,tan(val.num())};
}
nasal_ref builtin_exp(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("exp","\"x\" must be number");
    return {vm_num,exp(val.num())};
}
nasal_ref builtin_lg(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("ln","\"x\" must be number");
    return {vm_num,log(val.num())/log(10.0)};
}
nasal_ref builtin_ln(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("ln","\"x\" must be number");
    return {vm_num,log(val.num())};
}
nasal_ref builtin_sqrt(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("sqrt","\"x\" must be number");
    return {vm_num,sqrt(val.num())};
}
nasal_ref builtin_atan2(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref x=local[1];
    nasal_ref y=local[2];
    if(x.type!=vm_num)
        return builtin_err("atan2","\"x\" must be number");
    if(y.type!=vm_num)
        return builtin_err("atan2","\"y\" must be number");
    return {vm_num,atan2(y.num(),x.num())};
}
nasal_ref builtin_isnan(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref x=local[1];
    return (x.type==vm_num && std::isnan(x.num()))?one:zero;
}
nasal_ref builtin_time(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("time","\"begin_time\" must be number");
    time_t begin_time=(time_t)val.num();
    return {vm_num,(double)time(&begin_time)};
}
nasal_ref builtin_contains(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    nasal_ref key=local[2];
    if(hash.type!=vm_hash)
        return builtin_err("contains","\"hash\" must be hash");
    if(key.type!=vm_str)
        return builtin_err("contains","\"key\" must be string");
    return hash.hash().elems.count(key.str())?one:zero;
}
nasal_ref builtin_delete(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    nasal_ref key=local[2];
    if(hash.type!=vm_hash)
        return builtin_err("delete","\"hash\" must be hash");
    if(key.type!=vm_str)
        return builtin_err("delete","\"key\" must be string");
    if(hash.hash().elems.count(key.str()))
        hash.hash().elems.erase(key.str());
    return nil;
}
nasal_ref builtin_keys(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    if(hash.type!=vm_hash)
        return builtin_err("keys","\"hash\" must be hash");
    // avoid being sweeped
    nasal_ref res=gc.temp=gc.alloc(vm_vec);
    auto& vec=res.vec().elems;
    for(auto& iter:hash.hash().elems)
        vec.push_back(gc.newstr(iter.first));
    gc.temp=nil;
    return res;
}
nasal_ref builtin_die(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    if(str.type!=vm_str)
        return builtin_err("die","\"str\" must be string");
    std::cerr<<"[vm] error: "<<str.str()<<'\n';
    return {vm_none};
}
nasal_ref builtin_find(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref needle=local[1];
    nasal_ref haystack=local[2];
    if(needle.type!=vm_str)
        return builtin_err("find","\"needle\" must be string");
    if(haystack.type!=vm_str)
        return builtin_err("find","\"haystack\" must be string");
    size_t pos=haystack.str().find(needle.str());
    if(pos==std::string::npos)
        return {vm_num,(double)-1};
    return {vm_num,(double)pos};
}
nasal_ref builtin_type(nasal_ref* local,nasal_gc& gc)
{
    switch(local[1].type)
    {
        case vm_none: return gc.newstr("undefined");break;
        case vm_nil:  return gc.newstr("nil");      break;
        case vm_num:  return gc.newstr("num");      break;
        case vm_str:  return gc.newstr("str");      break;
        case vm_vec:  return gc.newstr("vec");      break;
        case vm_hash: return gc.newstr("hash");     break;
        case vm_func: return gc.newstr("func");     break;
        case vm_obj:  return gc.newstr("obj");      break;
        case vm_co:   return gc.newstr("coroutine");break;
    }
    return nil;
}
nasal_ref builtin_substr(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    nasal_ref beg=local[2];
    nasal_ref len=local[3];
    if(str.type!=vm_str)
        return builtin_err("substr","\"str\" must be string");
    if(beg.type!=vm_num)
        return builtin_err("substr","\"begin\" must be number");
    if(len.type!=vm_num)
        return builtin_err("substr","\"length\" must be number");
    if(beg.num()<0)
        return builtin_err("substr","\"begin\" should be greater than or equal to zero");
    if(len.num()<0)
        return builtin_err("substr","\"length\" should be greater than or equal to zero");
    size_t begin=(size_t)beg.num();
    size_t length=(size_t)len.num();
    if(begin>=str.str().length() || begin+length>str.str().length())
        return builtin_err("susbtr","index out of range");
    return gc.newstr(str.str().substr(begin,length));
}
nasal_ref builtin_streq(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    nasal_ref b=local[2];
    return {vm_num,double((a.type!=vm_str || b.type!=vm_str)?0:(a.str()==b.str()))};
}
nasal_ref builtin_left(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    nasal_ref len=local[2];
    if(str.type!=vm_str)
        return builtin_err("left","\"string\" must be string");
    if(len.type!=vm_num)
        return builtin_err("left","\"length\" must be number");
    int length=(int)len.num();
    if(length<0)
        length=0;
    return gc.newstr(str.str().substr(0,length));
}
nasal_ref builtin_right(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    nasal_ref len=local[2];
    if(str.type!=vm_str)
        return builtin_err("right","\"string\" must be string");
    if(len.type!=vm_num)
        return builtin_err("right","\"length\" must be number");
    int length=(int)len.num();
    int srclen=str.str().length();
    if(length>srclen)
        length=srclen;
    if(length<0)
        length=0;
    return gc.newstr(str.str().substr(srclen-length, srclen));
}
nasal_ref builtin_cmp(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    nasal_ref b=local[2];
    if(a.type!=vm_str)
        return builtin_err("cmp","\"a\" must be string");
    if(b.type!=vm_str)
        return builtin_err("cmp","\"b\" must be string");
    return {vm_num,(double)strcmp(a.str().c_str(),b.str().c_str())};
}
nasal_ref builtin_chr(nasal_ref* local,nasal_gc& gc)
{
    const char* extend[]={
        "€"," ","‚","ƒ","„","…","†","‡",
        "ˆ","‰","Š","‹","Œ"," ","Ž"," ",
        " ","‘","’","“","”","•","–","—",
        "˜","™","š","›","œ"," ","ž","Ÿ",
        " ","¡","¢","£","¤","¥","¦","§",
        "¨","©","ª","«","¬"," ","®","¯",
        "°","±","²","³","´","µ","¶","·",
        "¸","¹","º","»","¼","½","¾","¿",
        "À","Á","Â","Ã","Ä","Å","Æ","Ç",
        "È","É","Ê","Ë","Ì","Í","Î","Ï",
        "Ð","Ñ","Ò","Ó","Ô","Õ","Ö","×",
        "Ø","Ù","Ú","Û","Ü","Ý","Þ","ß",
        "à","á","â","ã","ä","å","æ","ç",
        "è","é","ê","ë","ì","í","î","ï",
        "ð","ñ","ò","ó","ô","õ","ö","÷",
        "ø","ù","ú","û","ü","ý","þ","ÿ"
    };
    nasal_ref code=local[1];
    if(code.type!=vm_num)
        return builtin_err("chr","\"code\" must be number");
    int num=code.num();
    if(0<=num && num<128)
        return gc.newstr((char)num);
    else if(128<=num && num<256)
        return gc.newstr(extend[num-128]);
    return gc.newstr(" ");
}
nasal_ref builtin_values(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    if(hash.type!=vm_hash)
        return builtin_err("values","\"hash\" must be a hashmap");
    nasal_ref vec=gc.alloc(vm_vec);
    auto& v=vec.vec().elems;
    for(auto& i:hash.hash().elems)
        v.push_back(i.second);
    return vec;
}
void obj_file_destructor(void* ptr)
{
    fclose((FILE*)ptr);
}
nasal_ref builtin_open(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref name=local[1];
    nasal_ref mode=local[2];
    if(name.type!=vm_str)
        return builtin_err("open","\"filename\" must be string");
    if(mode.type!=vm_str)
        return builtin_err("open","\"mode\" must be string");
    FILE* res=fopen(name.str().c_str(),mode.str().c_str());
    if(!res)
        return builtin_err("open","failed to open file <"+name.str()+"> errno "+std::to_string(errno));
    nasal_ref ret=gc.alloc(vm_obj);
    ret.obj().type=nasal_obj::file;
    ret.obj().ptr=(void*)res;
    ret.obj().destructor=obj_file_destructor;
    return ret;
}
nasal_ref builtin_close(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref fd=local[1];
    if(!fd.objchk(nasal_obj::file))
        return builtin_err("close","not a valid filehandle");
    fclose((FILE*)fd.obj().ptr);
    fd.obj().ptr=nullptr;
    return nil;
}
nasal_ref builtin_read(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref fd=local[1];
    nasal_ref buf=local[2];
    nasal_ref len=local[3];
    if(!fd.objchk(nasal_obj::file))
        return builtin_err("read","not a valid filehandle");
    if(buf.type!=vm_str)
        return builtin_err("read","\"buf\" must be string");
    if(buf.value.gcobj->unmut)
        return builtin_err("read","\"buf\" is not a mutable string");
    if(len.type!=vm_num)
        return builtin_err("read","\"len\" must be number");
    if(len.num()<=0 || len.num()>=(1<<30))
        return builtin_err("read","\"len\" less than 1 or too large");
    char* buff=new char[(size_t)len.num()+1];
    if(!buff)
        return builtin_err("read","memory allocation error");
    double res=fread(buff,1,len.num(),(FILE*)fd.obj().ptr);
    buf.str()=buff;
    delete []buff;
    return {vm_num,res};
}
nasal_ref builtin_write(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref fd=local[1];
    nasal_ref str=local[2];
    if(!fd.objchk(nasal_obj::file))
        return builtin_err("write","not a valid filehandle");
    if(str.type!=vm_str)
        return builtin_err("write","\"str\" must be string");
    double res=(double)fwrite(str.str().c_str(),1,str.str().length(),(FILE*)fd.obj().ptr);
    return {vm_num,res};
}
nasal_ref builtin_seek(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref fd=local[1];
    nasal_ref pos=local[2];
    nasal_ref whence=local[3];
    if(!fd.objchk(nasal_obj::file))
        return builtin_err("seek","not a valid filehandle");
    if(pos.type!=vm_num)
        return builtin_err("seek","\"pos\" must be number");
    if(whence.type!=vm_num || whence.num()<0 || whence.num()>2)
        return builtin_err("seek","\"whence\" must be number between 0 and 2");
    double res=fseek((FILE*)fd.obj().ptr,pos.num(),whence.num());
    return {vm_num,res};
}
nasal_ref builtin_tell(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref fd=local[1];
    if(!fd.objchk(nasal_obj::file))
        return builtin_err("tell","not a valid filehandle");
    double res=ftell((FILE*)fd.obj().ptr);
    return {vm_num,res};
}
nasal_ref builtin_readln(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref fd=local[1];
    if(!fd.objchk(nasal_obj::file))
        return builtin_err("readln","not a valid filehandle");
    nasal_ref str=gc.alloc(vm_str);
    auto& s=str.str();
    char c;
    while((c=fgetc((FILE*)fd.obj().ptr))!=EOF)
    {
        if(c=='\r')
            continue;
        if(c=='\n')
            return str;
        s+=c;
    }
    if(s.length())
        return str;
    return nil;
}
nasal_ref builtin_stat(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref name=local[1];
    if(name.type!=vm_str)
        return builtin_err("stat","\"filename\" must be string");
    struct stat buf;
    if(stat(name.str().c_str(),&buf)<0)
        return builtin_err("stat","failed to open file <"+name.str()+">");
    nasal_ref ret=gc.alloc(vm_vec);
    ret.vec().elems={
        {vm_num,(double)buf.st_dev},
        {vm_num,(double)buf.st_ino},
        {vm_num,(double)buf.st_mode},
        {vm_num,(double)buf.st_nlink},
        {vm_num,(double)buf.st_uid},
        {vm_num,(double)buf.st_gid},
        {vm_num,(double)buf.st_rdev},
        {vm_num,(double)buf.st_size},
        {vm_num,(double)buf.st_atime},
        {vm_num,(double)buf.st_mtime},
        {vm_num,(double)buf.st_ctime}
    };
    return ret;
}
nasal_ref builtin_eof(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref fd=local[1];
    if(!fd.objchk(nasal_obj::file))
        return builtin_err("readln","not a valid filehandle");
    double res=feof((FILE*)fd.obj().ptr);
    return {vm_num,res};
}
nasal_ref builtin_fld(nasal_ref* local,nasal_gc& gc)
{
    // bits.fld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5)
    nasal_ref str=local[1];
    nasal_ref startbit=local[2];
    nasal_ref length=local[3];
    if(str.type!=vm_str || str.value.gcobj->unmut)
        return builtin_err("fld","\"str\" must be mutable string");
    if(startbit.type!=vm_num || length.type!=vm_num)
        return builtin_err("fld","\"startbit\",\"len\" must be number");
    uint32_t bit=(uint32_t)startbit.num();
    uint32_t len=(uint32_t)length.num();
    if(bit+len>8*str.str().length())
        return builtin_err("fld","bitfield out of bounds");
    uint32_t res=0;
    auto& s=str.str();
    for(uint32_t i=bit;i<bit+len;++i)
        if(s[i>>3]&(1<<(7-(i&7))))
            res|=1<<(bit+len-i-1);
    return {vm_num,(double)res};
}
nasal_ref builtin_sfld(nasal_ref* local,nasal_gc& gc)
{
    // bits.sfld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5) then this will be signed extended to
    // 11111101(-3)
    nasal_ref str=local[1];
    nasal_ref startbit=local[2];
    nasal_ref length=local[3];
    if(str.type!=vm_str || str.value.gcobj->unmut)
        return builtin_err("sfld","\"str\" must be mutable string");
    if(startbit.type!=vm_num || length.type!=vm_num)
        return builtin_err("sfld","\"startbit\",\"len\" must be number");
    uint32_t bit=(uint32_t)startbit.num();
    uint32_t len=(uint32_t)length.num();
    if(bit+len>8*str.str().length())
        return builtin_err("sfld","bitfield out of bounds");
    uint32_t res=0;
    auto& s=str.str();
    for(uint32_t i=bit;i<bit+len;++i)
        if(s[i>>3]&(1<<(7-(i&7))))
            res|=1<<(bit+len-i-1);
    if(res&(1<<(len-1)))
        res|=~((1<<len)-1);
    return {vm_num,(double)((int32_t)res)};
}
nasal_ref builtin_setfld(nasal_ref* local,nasal_gc& gc)
{
    // bits.setfld(s,0,8,69);
    // set 01000101(69) to string will get this:
    // 10100010(162)
    // so s[0]=162
    nasal_ref str=local[1];
    nasal_ref startbit=local[2];
    nasal_ref length=local[3];
    nasal_ref value=local[4];
    if(str.type!=vm_str  || str.value.gcobj->unmut)
        return builtin_err("setfld","\"str\" must be mutable string");
    if(startbit.type!=vm_num || length.type!=vm_num || value.type!=vm_num)
        return builtin_err("setfld","\"startbit\",\"len\",\"val\" must be number");
    uint32_t bit=(uint32_t)startbit.num();
    uint32_t len=(uint32_t)length.num();
    uint64_t val=(uint64_t)value.num();
    if(bit+len>8*str.str().length())
        return builtin_err("setfld","bitfield out of bounds");
    auto& s=str.str();
    for(uint32_t i=bit;i<bit+len;++i)
    {
        if(val&(1<<(i-bit)))
            s[i>>3]|=(1<<(7-(i&7)));
        else
            s[i>>3]&=~(1<<(7-(i&7)));
    }
    return nil;
}
nasal_ref builtin_buf(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref length=local[1];
    if(length.type!=vm_num || length.num()<=0)
        return builtin_err("buf","\"len\" must be a number greater than 9");
    nasal_ref str=gc.alloc(vm_str);
    auto& s=str.str();
    s.resize(length.num(),'\0');
    return str;
}
nasal_ref builtin_sleep(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("sleep","\"duration\" must be number");
    std::this_thread::sleep_for(std::chrono::microseconds(int64_t(val.num()*1e6)));
    return nil;
}
nasal_ref builtin_pipe(nasal_ref* local,nasal_gc& gc)
{
#ifndef _WIN32
    int fd[2];
    nasal_ref res=gc.alloc(vm_vec);
    if(pipe(fd)==-1)
        return builtin_err("pipe","failed to create pipe");
    res.vec().elems.push_back({vm_num,(double)fd[0]});
    res.vec().elems.push_back({vm_num,(double)fd[1]});
    return res;
#endif
    return builtin_err("pipe","not supported for windows platform");
}
nasal_ref builtin_fork(nasal_ref* local,nasal_gc& gc)
{
#ifndef _WIN32
    double res=fork();
    if(res<0)
        return builtin_err("fork","failed to fork a process");
    return {vm_num,(double)res};
#endif
    return builtin_err("fork","not supported for windows platform");
}
nasal_ref builtin_waitpid(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref pid=local[1];
    nasal_ref nohang=local[2];
    if(pid.type!=vm_num || nohang.type!=vm_num)
        return builtin_err("waitpid","pid and nohang must be number");
#ifndef _WIN32
    int ret_pid;
    int status;
    ret_pid=waitpid(pid.num(),&status,nohang.num()==0?0:WNOHANG);
    nasal_ref vec=gc.alloc(vm_vec);
    vec.vec().elems.push_back({vm_num,(double)ret_pid});
    vec.vec().elems.push_back({vm_num,(double)status});
    return vec;
#endif
    return builtin_err("waitpid","not supported for windows platform");
}
void obj_dir_destructor(void* ptr)
{
    closedir((DIR*)ptr);
}
nasal_ref builtin_opendir(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref path=local[1];
    if(path.type!=vm_str)
        return builtin_err("opendir","\"path\" must be string");
    DIR* p=opendir(path.str().c_str());
    if(!p)
        return builtin_err("opendir","cannot open dir <"+path.str()+"> errno "+std::to_string(errno));
    nasal_ref ret=gc.alloc(vm_obj);
    ret.obj().type=nasal_obj::dir;
    ret.obj().ptr=(void*)p;
    ret.obj().destructor=obj_dir_destructor;
    return ret;
}
nasal_ref builtin_readdir(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref handle=local[1];
    if(!handle.objchk(nasal_obj::dir))
        return builtin_err("readdir","not a valid dir handle");
    dirent* p=readdir((DIR*)handle.obj().ptr);
    if(!p)
        return nil;
    return gc.newstr(p->d_name);
}
nasal_ref builtin_closedir(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref handle=local[1];
    if(!handle.objchk(nasal_obj::dir))
        return builtin_err("closedir","not a valid dir handle");
    closedir((DIR*)handle.obj().ptr);
    handle.obj().ptr=nullptr;
    return nil;
}
nasal_ref builtin_chdir(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref path=local[1];
    if(path.type!=vm_str)
        return builtin_err("chdir","\"path\" must be string");
    if(chdir(path.str().c_str())<0)
        return builtin_err("chdir","failed to execute chdir");
    return nil;
}
nasal_ref builtin_environ(nasal_ref* local,nasal_gc& gc)
{
    char** env=environ;
    nasal_ref res=gc.temp=gc.alloc(vm_vec);
    auto& vec=res.vec().elems;
    while(*env)
    {
        vec.push_back(gc.newstr(*env));
        ++env;
    }
    gc.temp=nil;
    return res;
}
nasal_ref builtin_getcwd(nasal_ref* local,nasal_gc& gc)
{
    char buf[1024];
    if(!getcwd(buf,sizeof(buf)))
        return builtin_err("getcwd","failed to call getcwd");
    return gc.newstr(buf);
}
nasal_ref builtin_getenv(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref envvar=local[1];
    if(envvar.type!=vm_str)
        return builtin_err("getenv","\"envvar\" must be string");
    char* res=getenv(envvar.str().c_str());
    if(!res)
        return nil;
    return gc.newstr(res);
}
void obj_dylib_destructor(void* ptr)
{
#ifdef _WIN32
    FreeLibrary((HMODULE)ptr);
#else
    dlclose(ptr);
#endif
}
nasal_ref builtin_dlopen(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref dlname=local[1];
    if(dlname.type!=vm_str)
        return builtin_err("dlopen","\"libname\" must be string");
#ifdef _WIN32
    wchar_t* str=new wchar_t[dlname.str().size()+1];
    memset(str,0,sizeof(wchar_t)*dlname.str().size()+1);
    mbstowcs(str,dlname.str().c_str(),dlname.str().size()+1);
    void* ptr=LoadLibraryW(str);
    delete []str;
#else
    void* ptr=dlopen(dlname.str().c_str(),RTLD_LOCAL|RTLD_LAZY);
#endif
    if(!ptr)
        return builtin_err("dlopen","cannot open dynamic lib <"+dlname.str()+">");
    nasal_ref ret=gc.alloc(vm_obj);
    ret.obj().type=nasal_obj::dylib;
    ret.obj().ptr=ptr;
    ret.obj().destructor=obj_dylib_destructor;
    return ret;
}
nasal_ref builtin_dlsym(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref libptr=local[1];
    nasal_ref sym=local[2];
    if(!libptr.objchk(nasal_obj::dylib))
        return builtin_err("dlsym","\"lib\" is not a valid dynamic lib");
    if(sym.type!=vm_str)
        return builtin_err("dlsym","\"sym\" must be string");
#ifdef _WIN32
    void* func=(void*)GetProcAddress((HMODULE)libptr.obj().ptr,sym.str().c_str());
#else
    void* func=dlsym(libptr.obj().ptr,sym.str().c_str());
#endif
    if(!func)
        return builtin_err("dlsym","cannot find symbol \""+sym.str()+"\"");
    nasal_ref ret=gc.alloc(vm_obj);
    ret.obj().type=nasal_obj::faddr;
    ret.obj().ptr=func;
    return ret;
}
nasal_ref builtin_dlclose(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref libptr=local[1];
    if(!libptr.objchk(nasal_obj::dylib))
        return builtin_err("dlclose","\"lib\" is not a valid dynamic lib");
#ifdef _WIN32
    FreeLibrary((HMODULE)libptr.obj().ptr);
#else
    dlclose(libptr.obj().ptr);
#endif
    libptr.obj().ptr=nullptr;
    return nil;
}
nasal_ref builtin_dlcall(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref funcptr=local[1];
    nasal_ref args=local[2];
    if(!funcptr.objchk(nasal_obj::faddr))
        return builtin_err("dlcall","\"funcptr\" is not a valid function pointer");
    typedef nasal_ref (*externs)(std::vector<nasal_ref>&,nasal_gc&);
    externs func=(externs)funcptr.obj().ptr;
    return func(args.vec().elems,gc);
}
nasal_ref builtin_platform(nasal_ref* local,nasal_gc& gc)
{
#if defined _WIN32 || defined _WIN64
    return gc.newstr("windows");
#elif defined __linux__
    return gc.newstr("linux");
#elif defined __APPLE__
    return gc.newstr("macOS");
#endif
    return gc.newstr("unknown");
}
nasal_ref builtin_gc(nasal_ref* local,nasal_gc& gc)
{
    gc.mark();
    gc.sweep();
    return nil;
}

// md5 related functions
std::string tohex(uint32_t num)
{
    const char str16[]="0123456789abcdef";
    std::string str="";
    for(uint32_t i=0;i<4;i++,num>>=8)
    {
        std::string tmp="";
        for(uint32_t j=0,b=num&0xff;j<2;j++,b>>=4)
            tmp.insert(0,1,str16[b&0xf]);
        str+=tmp;
    }
    return str;
}
std::string md5(const std::string& source)
{
    std::vector<uint32_t> buff;
    uint32_t num=((source.length()+8)>>6)+1;
    uint32_t buffsize=num<<4;
    buff.resize(buffsize,0);
    for(uint32_t i=0;i<source.length();i++)
        buff[i>>2]|=((unsigned char)source[i])<<((i&0x3)<<3);
    buff[source.length()>>2]|=0x80<<(((source.length()%4))<<3);
    buff[buffsize-2]=(source.length()<<3)&0xffffffff;
    buff[buffsize-1]=((source.length()<<3)>>32)&0xffffffff;

    // uint32_t(abs(sin(i+1))*(2pow32))
    const uint32_t k[]={
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
        0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
        0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
        0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
        0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
        0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
        0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
    };

    // left shift bits
    const uint32_t s[]={
        7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
        5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
        6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
    };

    // index
    const uint32_t idx[]={
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, // g=i
        1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12, // g=(5*i+1)%16;
        5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2, // g=(3*i+5)%16;
        0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9  // g=(7*i)%16;
    };
    
#define shift(x,n)  (((x)<<(n))|((x)>>(32-(n)))) // cycle left shift
#define md5f(x,y,z) (((x)&(y))|((~x)&(z)))    
#define md5g(x,y,z) (((x)&(z))|((y)&(~z)))
#define md5h(x,y,z) ((x)^(y)^(z))
#define md5i(x,y,z) ((y)^((x)|(~z)))
    
    uint32_t atmp=0x67452301,btmp=0xefcdab89;
    uint32_t ctmp=0x98badcfe,dtmp=0x10325476;
    for(uint32_t i=0;i<buffsize;i+=16)
    {
        uint32_t f,a=atmp,b=btmp,c=ctmp,d=dtmp;
        for(uint32_t j=0;j<64;j++)
        {
            if(j<16)      f=md5f(b,c,d);
            else if(j<32) f=md5g(b,c,d);
            else if(j<48) f=md5h(b,c,d);
            else          f=md5i(b,c,d);
            uint32_t tmp=d;
            d=c;
            c=b;
            b=b+shift((a+f+k[j]+buff[i+idx[j]]),s[j]);
            a=tmp;
        }
        atmp+=a;
        btmp+=b;
        ctmp+=c;
        dtmp+=d;
    }
    return tohex(atmp)+tohex(btmp)+tohex(ctmp)+tohex(dtmp);
}
nasal_ref builtin_md5(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    if(str.type!=vm_str)
        return builtin_err("md5","\"str\" must be a string");
    return gc.newstr(md5(str.str()));
}

nasal_ref builtin_cocreate(nasal_ref* local,nasal_gc& gc)
{
    // +-----------------+
    // | old pc          | <- top[0]
    // +-----------------+
    // | old localr      | <- top[-1]
    // +-----------------+
    // | old upvalr      | <- top[-2]
    // +-----------------+
    // | local scope     |
    // | ...             |
    // +-----------------+ <- local pointer stored in localr
    // | old funcr       | <- old function stored in funcr
    // +-----------------+
    nasal_ref func=local[1];
    if(func.type!=vm_func)
        return builtin_err("coroutine::create","must use a function to create coroutine");
    if(gc.coroutine)
        return builtin_err("coroutine::create","cannot create another coroutine in a coroutine");
    nasal_ref co=gc.alloc(vm_co);
    nasal_co& coroutine=co.co();
    coroutine.pc=func.func().entry-1;

    coroutine.top[0]=nil;
    coroutine.localr=coroutine.top+1;
    coroutine.top=coroutine.localr+func.func().lsize;
    coroutine.localr[0]=func.func().local[0];
    coroutine.top[0]=nil; // old upvalr
    coroutine.top++;
    coroutine.top[0]={vm_addr,(nasal_ref*)nullptr}; // old localr
    coroutine.top++;
    coroutine.top[0]={vm_ret,(uint32_t)0}; // old pc, set to zero to make op_ret recognizing this as coroutine function

    coroutine.funcr=func; // make sure the coroutine function can use correct upvalues
    coroutine.status=nasal_co::suspended;
    
    return co;
}
nasal_ref builtin_coresume(nasal_ref* local,nasal_gc& gc)
{
    if(gc.coroutine)
        return builtin_err("coroutine::resume","cannot start another coroutine when one is running");
    nasal_ref co=local[1];
    if(co.type!=vm_co)
        return builtin_err("coroutine::resume","must use a coroutine object");
    if(co.co().status==nasal_co::dead)
        return nil;
    gc.ctxchg(co.co());
    return gc.top[0];
}
nasal_ref builtin_coyield(nasal_ref* local,nasal_gc& gc)
{
    if(!gc.coroutine)
        return builtin_err("coroutine::yield","no coroutine is running");
    gc.ctxreserve();
    // this will set to main stack top
    // then builtin_coresume will return it
    return local[1];
}
nasal_ref builtin_costatus(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref co=local[1];
    if(co.type!=vm_co)
        return builtin_err("coroutine::status","must use a coroutine object");
    switch(co.co().status)
    {
        case nasal_co::suspended: return gc.newstr("suspended");break;
        case nasal_co::running:   return gc.newstr("running");  break;
        case nasal_co::dead:      return gc.newstr("dead");     break;
    }
    return nil;
}
nasal_ref builtin_corun(nasal_ref* local,nasal_gc& gc)
{
    if(gc.coroutine)
        return one;
    return zero;
}
nasal_ref builtin_millisec(nasal_ref* local,nasal_gc& gc)
{
    double res=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return {vm_num,res};
}
nasal_ref builtin_sysargv(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref res=gc.alloc(vm_vec);
    res.vec().elems=gc.env_argv;
    return res;
}
nasal_ref builtin_logtime(nasal_ref* local,nasal_gc& gc)
{
    time_t t=time(nullptr);
    tm* tm_t=localtime(&t);
    char s[128];
    sprintf(s,"%d-%.2d-%.2d %.2d:%.2d:%.2d",tm_t->tm_year+1900,tm_t->tm_mon+1,tm_t->tm_mday,tm_t->tm_hour,tm_t->tm_min,tm_t->tm_sec);
    return gc.newstr(s);
}
#endif