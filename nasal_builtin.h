#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__
/*
    builtin functions must be called inside a outer function like this:
    var print=func(elems...)
    {
        return __builtin_print(elems);
    }
    builtin function __builtin_print is wrapped up by print
*/

// declaration of builtin functions
// to add new builtin function,declare it here and write the definition below

nasal_ref builtin_print(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_append(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_setsize(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_system(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_input(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_sleep(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_fin(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_fout(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_split(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_rand(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_id(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_int(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_num(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_pop(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_str(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_size(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_xor(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_and(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_or(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_nand(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_not(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_sin(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_cos(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_tan(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_exp(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_ln(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_sqrt(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_atan2(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_isnan(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_time(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_contains(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_delete(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_keys(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_import(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_die(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_type(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_substr(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_streq(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_left(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_right(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_cmp(std::vector<nasal_ref>&,nasal_gc&);
nasal_ref builtin_chr(std::vector<nasal_ref>&,nasal_gc&);

nasal_ref builtin_err(const char* func_name,std::string info)
{
    std::cout<<"[vm] "<<func_name<<": "<<info<<".\n";
    return {vm_none};
}

// register builtin function's name and it's address here in this table below
// this table must end with {"",nullptr}
struct FUNC_TABLE
{
    const char* name;
    nasal_ref (*func)(std::vector<nasal_ref>&,nasal_gc&);
} builtin_func[]=
{
    {"__builtin_print",   builtin_print   },
    {"__builtin_append",  builtin_append  },
    {"__builtin_setsize", builtin_setsize },
    {"__builtin_system",  builtin_system  },
    {"__builtin_input",   builtin_input   },
    {"__builtin_sleep",   builtin_sleep   },
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
    {nullptr,             nullptr         }
};

nasal_ref builtin_print(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    // get arguments
    // local[0] is reserved for 'me'
    nasal_ref vec=local[1];
    // main process
    for(auto i:vec.vec()->elems)
        switch(i.type)
        {
            case vm_none: std::cout<<"undefined";      break;
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
    for(auto i:elem.vec()->elems)
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
    nasal_ref ret=gc.gc_alloc(vm_str);
    std::cin>>*ret.str();
    return ret;
}
nasal_ref builtin_sleep(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("sleep","\"duration\" must be number");
    // sleep in unistd.h will make this progress sleep sleep_time seconds.
    sleep((unsigned int)val.num());
    return gc.nil;
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
        nasal_ref ret=gc.gc_alloc(vm_str);
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
    local.push_back(gc.gc_alloc(vm_vec));

    std::vector<nasal_ref>& vec=local.back().vec()->elems;
    if(!delimeter_len)
    {
        for(int i=0;i<source_len;++i)
        {
            vec.push_back(gc.gc_alloc(vm_str));
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
                vec.push_back(gc.gc_alloc(vm_str));
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
        vec.push_back(gc.gc_alloc(vm_str));
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
    nasal_ref ret=gc.gc_alloc(vm_str);
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
    nasal_ref ret=gc.gc_alloc(vm_str);
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
nasal_ref builtin_ln(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    if(val.type!=vm_num)
        return builtin_err("ln","\"x\" must be number");
    return {vm_num,log(val.num())/log(2.7182818284590452354)};
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
    local.push_back(gc.gc_alloc(vm_vec));
    auto& vec=local.back().vec()->elems;
    for(auto& iter:hash.hash()->elems)
    {
        nasal_ref str=gc.gc_alloc(vm_str);
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
    std::cout<<"[vm] error: "<<*str.str()<<'\n';
    return nasal_ref(vm_none);
}
nasal_ref builtin_type(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref val=local[1];
    nasal_ref ret=gc.gc_alloc(vm_str);
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
    nasal_ref ret=gc.gc_alloc(vm_str);
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
    nasal_ref ret=gc.gc_alloc(vm_str);
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
    nasal_ref ret=gc.gc_alloc(vm_str);
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
    nasal_ref ret=gc.gc_alloc(vm_str);
    int num=code.num();
    if(0<=num && num<128)
        *ret.str()=(char)num;
    else if(128<=num && num<256)
        *ret.str()=extend[num-128];
    else
        *ret.str()=" ";
    return ret;
}

#endif