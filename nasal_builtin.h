#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__
/*
    builtin functions must be called inside a function like this:
    var print=func(elems...){
        return __builtin_print(elems);
    }
    builtin function __builtin_print is wrapped up by print
*/
enum obj_type
{
    obj_file=1,
    obj_dir,
    obj_dylib,
    obj_extern
};
// declaration of builtin functions
// to add new builtin function, declare it here and write the definition below
#define nas_native(name) nasal_ref name(std::vector<nasal_ref>&,nasal_gc&)
nas_native(builtin_print);
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
nas_native(builtin_num);
nas_native(builtin_pop);
nas_native(builtin_str);
nas_native(builtin_size);
nas_native(builtin_xor);
nas_native(builtin_and);
nas_native(builtin_or);
nas_native(builtin_nand);
nas_native(builtin_not);
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
nas_native(builtin_import);
nas_native(builtin_die);
nas_native(builtin_type);
nas_native(builtin_substr);
nas_native(builtin_streq);
nas_native(builtin_left);
nas_native(builtin_right);
nas_native(builtin_cmp);
nas_native(builtin_chr);
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
nas_native(builtin_opendir);
nas_native(builtin_readdir);
nas_native(builtin_closedir);
nas_native(builtin_chdir);
nas_native(builtin_getcwd);
nas_native(builtin_getenv);
nas_native(builtin_dlopen);
nas_native(builtin_dlsym);
nas_native(builtin_dlclose);
nas_native(builtin_dlcall);
nas_native(builtin_platform);

nasal_ref builtin_err(const char* func_name,std::string info)
{
    std::cerr<<"[vm] "<<func_name<<": "<<info<<".\n";
    return {vm_none,nullptr};
}

// register builtin function's name and it's address here in this table below
// this table must end with {nullptr,nullptr}
struct
{
    const char* name;
    nasal_ref (*func)(std::vector<nasal_ref>&,nasal_gc&);
} builtin[]=
{
    {"__builtin_print",   builtin_print   },
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
    {"__builtin_num",     builtin_num     },
    {"__builtin_pop",     builtin_pop     },
    {"__builtin_str",     builtin_str     },
    {"__builtin_size",    builtin_size    },
    {"__builtin_xor",     builtin_xor     },
    {"__builtin_and",     builtin_and     },
    {"__builtin_or",      builtin_or      },
    {"__builtin_nand",    builtin_nand    },
    {"__builtin_not",     builtin_not     },
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
    {"__builtin_import",  builtin_import  },
    {"__builtin_die",     builtin_die     },
    {"__builtin_type",    builtin_type    },
    {"__builtin_substr",  builtin_substr  },
    {"__builtin_streq",   builtin_streq   },
    {"__builtin_left",    builtin_left    },
    {"__builtin_right",   builtin_right   },
    {"__builtin_cmp",     builtin_cmp     },
    {"__builtin_chr",     builtin_chr     },
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
    {"__builtin_opendir", builtin_opendir },
    {"__builtin_readdir", builtin_readdir },
    {"__builtin_closedir",builtin_closedir},
    {"__builtin_chdir",   builtin_chdir   },
    {"__builtin_getcwd",  builtin_getcwd  },
    {"__builtin_getenv",  builtin_getenv  },
    {"__builtin_dlopen",  builtin_dlopen  },
    {"__builtin_dlsym",   builtin_dlsym   },
    {"__builtin_dlclose", builtin_dlclose },
    {"__builtin_dlcall",  builtin_dlcall  },
    {"__builtin_platform",builtin_platform},
    {nullptr,             nullptr         }
};

nasal_ref builtin_print(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    // get arguments
    // local[0] is reserved for 'me'
    nasal_ref vec=local[1];
    // main process
    for(auto& i:vec.vec()->elems)
        switch(i.type)
        {
            case vm_none: std::cout<<"null";           break;
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<i.num();          break;
            case vm_str:  std::cout<<*i.str();         break;
            case vm_vec:  i.vec()->print();            break;
            case vm_hash: i.hash()->print();           break;
            case vm_func: std::cout<<"func(...){...}"; break;
            case vm_obj:  std::cout<<"<object>";       break;
        }
    std::cout<<std::flush;
    // generate return value
    return gc.nil;
}
nasal_ref builtin_append(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref vec=local[1];
    nasal_ref elem=local[2];
    if(vec.type!=vm_vec)
        return builtin_err("append","\"vector\" must be vector");
    auto& ref_vec=vec.vec()->elems;
    for(auto& i:elem.vec()->elems)
        ref_vec.push_back(i);
    return gc.nil;
}
nasal_ref builtin_setsize(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref vec=local[1];
    nasal_ref size=local[2];
    if(vec.type!=vm_vec)
        return builtin_err("setsize","\"vector\" must be vector");
    if(size.type!=vm_num)
        return builtin_err("setsize","\"size\" is not a number");
    int num=(int)size.num();
    if(num<0)
        return builtin_err("setsize","\"size\" must be greater than -1");
    vec.vec()->elems.resize(num,gc.nil);
    return gc.nil;
}
nasal_ref builtin_system(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    if(str.type!=vm_str)
        return builtin_err("system","\"str\" must be string");
    return {vm_num,(double)system(str.str()->c_str())};
}
nasal_ref builtin_input(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref ret=gc.alloc(vm_str);
    std::cin>>*ret.str();
    return ret;
}
nasal_ref builtin_fin(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_str)
        return builtin_err("io.fin","\"filename\" must be string");
    std::string&  filename=*val.str();
    std::ifstream fin(filename);
    if(!fin.fail())
    {
        nasal_ref ret=gc.alloc(vm_str);
        std::stringstream rd;
        rd<<fin.rdbuf();
        *ret.str()=rd.str();
        return ret;
    }
    return builtin_err("io.fin","cannot open \""+filename+"\"");
}
nasal_ref builtin_fout(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    nasal_ref str=local[2];
    if(val.type!=vm_str)
        return builtin_err("io.fout","\"filename\" must be string");
    if(str.type!=vm_str)
        return builtin_err("io.fout","\"str\" must be string");
    std::ofstream fout(*val.str());
    if(fout.fail())
        return builtin_err("io.fout","cannot open \""+*val.str()+"\"");
    fout<<*str.str();
    return gc.nil;
}
nasal_ref builtin_split(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref deli_val=local[1];
    nasal_ref str_val=local[2];
    if(deli_val.type!=vm_str)
        return builtin_err("split","\"delimeter\" must be string");
    if(str_val.type!=vm_str)
        return builtin_err("split","\"string\" must be string");
    std::string& delimeter=*deli_val.str();
    std::string& source=*str_val.str();
    size_t delimeter_len=delimeter.length();
    size_t source_len=source.length();

    // push it to local scope to avoid being sweeped
    local.push_back(gc.alloc(vm_vec));

    std::vector<nasal_ref>& vec=local.back().vec()->elems;
    if(!delimeter_len)
    {
        for(int i=0;i<source_len;++i)
        {
            vec.push_back(gc.alloc(vm_str));
            *vec.back().str()=source[i];
        }
        return local.back();
    }

    std::string tmp="";
    for(int i=0;i<source_len;++i)
    {
        bool check_delimeter=false;
        if(source[i]==delimeter[0])
            for(int j=0;j<delimeter_len;++j)
            {
                if(i+j>=source_len || source[i+j]!=delimeter[j])
                    break;
                if(j==delimeter_len-1)
                    check_delimeter=true;
            }
        if(check_delimeter)
        {
            if(tmp.length())
            {
                vec.push_back(gc.alloc(vm_str));
                *vec.back().str()=tmp;
                tmp="";
            }
            i+=delimeter_len-1;
        }
        else
            tmp+=source[i];
    }
    if(tmp.length())
    {
        vec.push_back(gc.alloc(vm_str));
        *vec.back().str()=tmp;
        tmp="";
    }
    return local.back();
}
nasal_ref builtin_rand(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num && val.type!=vm_nil)
        return builtin_err("rand","\"seed\" must be nil or number");
    if(val.type==vm_num)
    {
        srand((unsigned int)val.num());
        return gc.nil;
    }
    double num=0;
    for(int i=0;i<5;++i)
        num=(num+rand())*(1.0/(RAND_MAX+1.0));
    return {vm_num,num};
}
nasal_ref builtin_id(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    nasal_ref ret=gc.alloc(vm_str);
    char buf[32];
    if(val.type>vm_num)
        sprintf(buf,"%p",val.value.gcobj);
    else
        sprintf(buf,"0");
    *ret.str()=buf;
    return ret;
}
nasal_ref builtin_int(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return gc.nil;
    int number=(int)val.num();
    return {vm_num,(double)number};
}
nasal_ref builtin_num(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_str)
        return gc.nil;
    return {vm_num,val.to_number()};
}
nasal_ref builtin_pop(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_vec)
        return builtin_err("pop","\"vector\" must be vector");
    if(val.vec()->elems.size())
    {
        nasal_ref tmp=val.vec()->elems.back();
        val.vec()->elems.pop_back();
        return tmp;
    }
    return gc.nil;
}
nasal_ref builtin_str(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("str","\"number\" must be number");
    nasal_ref ret=gc.alloc(vm_str);
    *ret.str()=std::to_string(val.num());
    return ret;
}
nasal_ref builtin_size(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    double num;
    switch(val.type)
    {
        case vm_num:  num=val.num();               break;
        case vm_str:  num=val.str()->length();     break;
        case vm_vec:  num=val.vec()->elems.size(); break;
        case vm_hash: num=val.hash()->elems.size();break;
    }
    return {vm_num,num};
}
nasal_ref builtin_xor(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    nasal_ref b=local[2];
    if(a.type!=vm_num)
        return builtin_err("xor","\"a\" must be number");
    if(b.type!=vm_num)
        return builtin_err("xor","\"b\" must be number");
    int number_a=(int)a.num();
    int number_b=(int)b.num();
    return {vm_num,(double)(number_a^number_b)};
}
nasal_ref builtin_and(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    nasal_ref b=local[2];
    if(a.type!=vm_num)
        return builtin_err("and","\"a\" must be number");
    if(b.type!=vm_num)
        return builtin_err("and","\"b\" must be number");
    int number_a=(int)a.num();
    int number_b=(int)b.num();
    return {vm_num,(double)(number_a&number_b)};
}
nasal_ref builtin_or(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    nasal_ref b=local[2];
    if(a.type!=vm_num)
        return builtin_err("or","\"a\" must be number");
    if(b.type!=vm_num)
        return builtin_err("or","\"b\" must be number");
    int number_a=(int)a.num();
    int number_b=(int)b.num();
    return {vm_num,(double)(number_a|number_b)};
}
nasal_ref builtin_nand(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    nasal_ref b=local[2];
    if(a.type!=vm_num)
        return builtin_err("nand","\"a\" must be number");
    if(b.type!=vm_num)
        return builtin_err("nand","\"b\" must be number");
    int number_a=(int)a.num();
    int number_b=(int)b.num();
    return {vm_num,(double)(~(number_a&number_b))};
}
nasal_ref builtin_not(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    if(a.type!=vm_num)
        return builtin_err("not","\"a\" must be number");
    int number=(int)a.num();
    return {vm_num,(double)(~number)};
}
nasal_ref builtin_sin(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("sin","\"x\" must be number");
    return {vm_num,sin(val.num())};
}
nasal_ref builtin_cos(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("cos","\"x\" must be number");
    return {vm_num,cos(val.num())};
}
nasal_ref builtin_tan(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("tan","\"x\" must be number");
    return {vm_num,tan(val.num())};
}
nasal_ref builtin_exp(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("exp","\"x\" must be number");
    return {vm_num,exp(val.num())};
}
nasal_ref builtin_lg(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("ln","\"x\" must be number");
    return {vm_num,log(val.num())/log(10.0)};
}
nasal_ref builtin_ln(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("ln","\"x\" must be number");
    return {vm_num,log(val.num())};
}
nasal_ref builtin_sqrt(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("sqrt","\"x\" must be number");
    return {vm_num,sqrt(val.num())};
}
nasal_ref builtin_atan2(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref x=local[1];
    nasal_ref y=local[2];
    if(x.type!=vm_num)
        return builtin_err("atan2","\"x\" must be number");
    if(y.type!=vm_num)
        return builtin_err("atan2","\"y\" must be number");
    return {vm_num,atan2(y.num(),x.num())};
}
nasal_ref builtin_isnan(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref x=local[1];
    return (x.type==vm_num && std::isnan(x.num()))?gc.one:gc.zero;
}
nasal_ref builtin_time(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("time","\"begin_time\" must be number");
    time_t begin_time=(time_t)val.num();
    return {vm_num,(double)time(&begin_time)};
}
nasal_ref builtin_contains(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    nasal_ref key=local[2];
    if(hash.type!=vm_hash)
        return builtin_err("contains","\"hash\" must be hash");
    if(key.type!=vm_str)
        return builtin_err("contains","\"key\" must be string");
    return hash.hash()->elems.count(*key.str())?gc.one:gc.zero;
}
nasal_ref builtin_delete(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    nasal_ref key=local[2];
    if(hash.type!=vm_hash)
        return builtin_err("delete","\"hash\" must be hash");
    if(key.type!=vm_str)
        return builtin_err("delete","\"key\" must be string");
    if(hash.hash()->elems.count(*key.str()))
        hash.hash()->elems.erase(*key.str());
    return gc.nil;
}
nasal_ref builtin_keys(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    if(hash.type!=vm_hash)
        return builtin_err("keys","\"hash\" must be hash");
    // push vector into local scope to avoid being sweeped
    local.push_back(gc.alloc(vm_vec));
    auto& vec=local.back().vec()->elems;
    for(auto& iter:hash.hash()->elems)
    {
        nasal_ref str=gc.alloc(vm_str);
        *str.str()=iter.first;
        vec.push_back(str);
    }
    return local.back();
}
nasal_ref builtin_import(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    // this function is used in preprocessing.
    // this function will return nothing when running.
    return builtin_err("import","must use this function in global scope");
}
nasal_ref builtin_die(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    if(str.type!=vm_str)
        return builtin_err("die","\"str\" must be string");
    std::cerr<<"[vm] error: "<<*str.str()<<'\n';
    return nasal_ref(vm_none);
}
nasal_ref builtin_type(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    nasal_ref ret=gc.alloc(vm_str);
    switch(val.type)
    {
        case vm_none: *ret.str()="undefined";break;
        case vm_nil:  *ret.str()="nil";      break;
        case vm_num:  *ret.str()="num";      break;
        case vm_str:  *ret.str()="str";      break;
        case vm_vec:  *ret.str()="vec";      break;
        case vm_hash: *ret.str()="hash";     break;
        case vm_func: *ret.str()="func";     break;
        case vm_obj:  *ret.str()="obj";      break;
    }
    return ret;
}
nasal_ref builtin_substr(std::vector<nasal_ref>& local,nasal_gc& gc)
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
    int begin=(int)beg.num();
    int length=(int)len.num();
    if(begin>=str.str()->length() || begin+length-1>=str.str()->length())
        return builtin_err("susbtr","index out of range");
    if(length<0)
        length=0;
    nasal_ref ret=gc.alloc(vm_str);
    *ret.str()=str.str()->substr(begin,length);
    return ret;
}
nasal_ref builtin_streq(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    nasal_ref b=local[2];
    return {vm_num,double((a.type!=vm_str || b.type!=vm_str)?0:(*a.str()==*b.str()))};
}
nasal_ref builtin_left(std::vector<nasal_ref>& local,nasal_gc& gc)
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
    nasal_ref ret=gc.alloc(vm_str);
    *ret.str()=str.str()->substr(0,length);
    return ret;
}
nasal_ref builtin_right(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref str=local[1];
    nasal_ref len=local[2];
    if(str.type!=vm_str)
        return builtin_err("right","\"string\" must be string");
    if(len.type!=vm_num)
        return builtin_err("right","\"length\" must be number");
    int length=(int)len.num();
    int srclen=str.str()->length();
    if(length>srclen)
        length=srclen;
    if(length<0)
        length=0;
    nasal_ref ret=gc.alloc(vm_str);
    *ret.str()=str.str()->substr(srclen-length, srclen);
    return ret;
}
nasal_ref builtin_cmp(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref a=local[1];
    nasal_ref b=local[2];
    if(a.type!=vm_str)
        return builtin_err("cmp","\"a\" must be string");
    if(b.type!=vm_str)
        return builtin_err("cmp","\"b\" must be string");
    return {vm_num,(double)strcmp(a.str()->c_str(),b.str()->c_str())};
}
nasal_ref builtin_chr(std::vector<nasal_ref>& local,nasal_gc& gc)
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
    nasal_ref ret=gc.alloc(vm_str);
    int num=code.num();
    if(0<=num && num<128)
        *ret.str()=(char)num;
    else if(128<=num && num<256)
        *ret.str()=extend[num-128];
    else
        *ret.str()=" ";
    return ret;
}
nasal_ref builtin_open(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filename=local[1];
    nasal_ref mode=local[2];
    if(filename.type!=vm_str)
        return builtin_err("open","\"filename\" must be string");
    if(mode.type!=vm_str)
        return builtin_err("open","\"mode\" must be string");
    FILE* res=fopen(filename.str()->c_str(),mode.str()->c_str());
    if(!res)
        return builtin_err("open","failed to open file <"+*filename.str()+"> errno "+std::to_string(errno));
    nasal_ref ret=gc.alloc(vm_obj);
    ret.obj()->type=obj_file;
    ret.obj()->ptr=(void*)res;
    return ret;
}
nasal_ref builtin_close(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filehandle=local[1];
    if(filehandle.type!=vm_obj || filehandle.obj()->type!=obj_file)
        return builtin_err("close","not a correct filehandle");
    fclose((FILE*)filehandle.obj()->ptr);
    return gc.nil;
}
nasal_ref builtin_read(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filehandle=local[1];
    nasal_ref buf=local[2];
    nasal_ref len=local[3];
    if(filehandle.type!=vm_obj || filehandle.obj()->type!=obj_file)
        return builtin_err("read","not a correct filehandle");
    if(buf.type!=vm_str)
        return builtin_err("read","\"buf\" must be string");
    if(len.type!=vm_num)
        return builtin_err("read","\"len\" must be number");
    if(len.num()<=0 || len.num()>=(1<<30))
        return builtin_err("read","\"len\" less than 1 or too large");
    char* buff=new char[(size_t)len.num()+1];
    if(!buff)
        return builtin_err("read","memory allocation error");
    double res=fread(buff,1,len.num(),(FILE*)filehandle.obj()->ptr);
    *buf.str()=buff;
    delete []buff;
    return {vm_num,res};
}
nasal_ref builtin_write(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filehandle=local[1];
    nasal_ref str=local[2];
    if(filehandle.type!=vm_obj || filehandle.obj()->type!=obj_file)
        return builtin_err("write","not a correct filehandle");
    if(str.type!=vm_str)
        return builtin_err("write","\"str\" must be string");
    double res=(double)fwrite(str.str()->c_str(),1,str.str()->length(),(FILE*)filehandle.obj()->ptr);
    return {vm_num,res};
}
nasal_ref builtin_seek(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filehandle=local[1];
    nasal_ref position=local[2];
    nasal_ref whence=local[3];
    if(filehandle.type!=vm_obj || filehandle.obj()->type!=obj_file)
        return builtin_err("seek","not a correct filehandle");
    if(position.type!=vm_num)
        return builtin_err("seek","\"pos\" must be number");
    if(whence.type!=vm_num || whence.num()<0 || whence.num()>2)
        return builtin_err("seek","\"whence\" must be number between 0 and 2");
    double res=fseek((FILE*)filehandle.obj()->ptr,position.num(),whence.num());
    return {vm_num,res};
}
nasal_ref builtin_tell(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filehandle=local[1];
    if(filehandle.type!=vm_obj || filehandle.obj()->type!=obj_file)
        return builtin_err("tell","not a correct filehandle");
    double res=ftell((FILE*)filehandle.obj()->ptr);
    return {vm_num,res};
}
nasal_ref builtin_readln(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filehandle=local[1];
    if(filehandle.type!=vm_obj || filehandle.obj()->type!=obj_file)
        return builtin_err("readln","not a correct filehandle");
    nasal_ref str=gc.alloc(vm_str);
    auto& s=*str.str();
    char c;
    while((c=fgetc((FILE*)filehandle.obj()->ptr))!=EOF)
    {
        if(c=='\r')
            continue;
        if(c=='\n')
            return str;
        s+=c;
    }
    if(s.length())
        return str;
    return gc.nil;
}
nasal_ref builtin_stat(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filename=local[1];
    if(filename.type!=vm_str)
        return builtin_err("stat","\"filename\" must be string");
    struct stat buf;
    if(stat(filename.str()->c_str(),&buf)<0)
        return builtin_err("stat","failed to open file <"+*filename.str()+">");
    nasal_ref ret=gc.alloc(vm_vec);
    ret.vec()->elems={
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
nasal_ref builtin_eof(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref filehandle=local[1];
    if(filehandle.type!=vm_obj || filehandle.obj()->type!=obj_file)
        return builtin_err("readln","not a correct filehandle");
    double res=feof((FILE*)filehandle.obj()->ptr);
    return {vm_num,res};
}
nasal_ref builtin_fld(std::vector<nasal_ref>& local,nasal_gc& gc)
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
    if(bit+len>8*str.str()->length())
        return builtin_err("fld","bitfield out of bounds");
    uint32_t res=0;
    auto& s=*str.str();
    for(uint32_t i=bit;i<bit+len;++i)
        if(s[i>>3]&(1<<(7-(i&7))))
            res|=1<<(bit+len-i-1);
    return {vm_num,(double)res};
}
nasal_ref builtin_sfld(std::vector<nasal_ref>& local,nasal_gc& gc)
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
    if(bit+len>8*str.str()->length())
        return builtin_err("sfld","bitfield out of bounds");
    uint32_t res=0;
    auto& s=*str.str();
    for(uint32_t i=bit;i<bit+len;++i)
        if(s[i>>3]&(1<<(7-(i&7))))
            res|=1<<(bit+len-i-1);
    if(res&(1<<(len-1)))
        res|=~((1<<len)-1);
    return {vm_num,(double)((int32_t)res)};
}
nasal_ref builtin_setfld(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    // bits.setfld(s,0,8,69);
    // set 1000101(69) to string will get this:
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
    if(bit+len>8*str.str()->length())
        return builtin_err("setfld","bitfield out of bounds");
    auto& s=*str.str();
    for(uint32_t i=bit;i<bit+len;++i)
    {
        if(val&(1<<(i-bit)))
            s[i>>3]|=(1<<(7-(i&7)));
        else
            s[i>>3]&=~(1<<(7-(i&7)));
    }
    return gc.nil;
}
nasal_ref builtin_buf(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref length=local[1];
    if(length.type!=vm_num || length.num()<=0)
        return builtin_err("buf","\"len\" must be a number greater than 9");
    nasal_ref str=gc.alloc(vm_str);
    auto& s=*str.str();
    s.resize(length.num(),'\0');
    return str;
}
nasal_ref builtin_sleep(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("sleep","\"duration\" must be number");
    usleep((useconds_t)(val.num()*1e6));
    return gc.nil;
}
nasal_ref builtin_opendir(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref path=local[1];
    if(path.type!=vm_str)
        return builtin_err("opendir","\"path\" must be string");
    DIR* p=opendir(path.str()->c_str());
    if(!p)
        return builtin_err("opendir","cannot open dir <"+*path.str()+">");
    nasal_ref ret=gc.alloc(vm_obj);
    ret.obj()->type=obj_dir;
    ret.obj()->ptr=(void*)p;
    return ret;
}
nasal_ref builtin_readdir(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref handle=local[1];
    if(handle.type!=vm_obj || handle.obj()->type!=obj_dir)
        return builtin_err("readdir","not a correct dir handle");
    dirent* p=readdir((DIR*)handle.obj()->ptr);
    if(!p)
        return gc.nil;
    nasal_ref ret=gc.alloc(vm_str);
    *ret.str()=p->d_name;
    return ret;
}
nasal_ref builtin_closedir(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref handle=local[1];
    if(handle.type!=vm_obj || handle.obj()->type!=obj_dir)
        return builtin_err("closedir","not a correct dir handle");
    closedir((DIR*)handle.obj()->ptr);
    return gc.nil;
}
nasal_ref builtin_chdir(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref path=local[1];
    if(path.type!=vm_str)
        return builtin_err("chdir","\"path\" must be string");
    if(chdir(path.str()->c_str())<0)
        return builtin_err("chdir","failed to execute chdir");
    return gc.nil;
}
nasal_ref builtin_getcwd(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    char buf[1024];
    getcwd(buf,sizeof(buf));
    nasal_ref str=gc.alloc(vm_str);
    *str.str()=buf;
    return str;
}
nasal_ref builtin_getenv(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref envvar=local[1];
    if(envvar.type!=vm_str)
        return builtin_err("getenv","\"envvar\" must be string");
    char* res=getenv(envvar.str()->c_str());
    if(!res)
        return gc.nil;
    nasal_ref str=gc.alloc(vm_str);
    *str.str()=res;
    return str;
}
nasal_ref builtin_dlopen(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref dlname=local[1];
    if(dlname.type!=vm_str)
        return builtin_err("dlopen","\"libname\" must be string");
#ifdef _WIN32
    // wchar_t* str=new wchar_t[dlname.str()->size()+1];
    // memset(str,0,sizeof(wchar_t)*dlname.str()->size()+1);
    // mbstowcs(str,dlname.str()->c_str(),dlname.str()->size()+1);
    // void* ptr=LoadLibrary(str);
    // delete []str;
    void* ptr=LoadLibrary(dlname.str()->c_str());
#else
    void* ptr=dlopen(dlname.str()->c_str(),RTLD_LOCAL|RTLD_LAZY);
#endif
    if(!ptr)
        return builtin_err("dlopen","cannot open dynamic lib \""+*dlname.str()+"\"");
    nasal_ref ret=gc.alloc(vm_obj);
    ret.obj()->type=obj_dylib;
    ret.obj()->ptr=ptr;
    return ret;
}
nasal_ref builtin_dlsym(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref libptr=local[1];
    nasal_ref sym=local[2];
    if(libptr.type!=vm_obj || libptr.obj()->type!=obj_dylib)
        return builtin_err("dlsym","\"lib\" is not a correct dynamic lib entry");
    if(sym.type!=vm_str)
        return builtin_err("dlsym","\"sym\" must be string");
#ifdef _WIN32
    void* func=(void*)GetProcAddress((HMODULE)libptr.obj()->ptr,sym.str()->c_str());
#else
    void* func=dlsym(libptr.obj()->ptr,sym.str()->c_str());
#endif
    if(!func)
        return builtin_err("dlsym","cannot find symbol \""+*sym.str()+"\"");
    nasal_ref ret=gc.alloc(vm_obj);
    ret.obj()->type=obj_extern;
    ret.obj()->ptr=func;
    return ret;
}
nasal_ref builtin_dlclose(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref libptr=local[1];
    if(libptr.type!=vm_obj || libptr.obj()->type!=obj_dylib)
        return builtin_err("dlclose","\"lib\" is not a correct dynamic lib entry");
#ifdef _WIN32
    FreeLibrary((HMODULE)libptr.obj()->ptr);
#else
    dlclose(libptr.obj()->ptr);
#endif
    return gc.nil;
}
nasal_ref builtin_dlcall(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref funcptr=local[1];
    nasal_ref args=local[2];
    if(funcptr.type!=vm_obj || funcptr.obj()->type!=obj_extern)
        return builtin_err("dlcall","\"funcptr\" is not a correct function pointer");
    typedef nasal_ref (*extern_func)(std::vector<nasal_ref>&,nasal_gc&);
    extern_func func=(extern_func)funcptr.obj()->ptr;
    return func(args.vec()->elems,gc);
}
nasal_ref builtin_platform(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref ret=gc.alloc(vm_str);
#if defined _WIN32 || defined _WIN64
    *ret.str()="windows";
#elif defined __linux__
    *ret.str()="linux";
#elif defined __APPLE__
    *ret.str()="macOS";
#endif
    return ret;
}
#endif