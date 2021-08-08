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

nasal_val* builtin_print(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_append(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_setsize(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_system(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_input(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_sleep(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_fin(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_fout(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_split(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_rand(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_id(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_int(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_num(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_pop(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_str(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_size(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_xor(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_and(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_or(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_nand(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_not(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_sin(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_cos(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_tan(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_exp(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_ln(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_sqrt(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_atan2(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_time(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_contains(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_delete(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_keys(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_import(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_die(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_type(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_substr(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_streq(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_left(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_right(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_cmp(std::vector<nasal_val*>&,nasal_gc&);
nasal_val* builtin_chr(std::vector<nasal_val*>&,nasal_gc&);

#define builtin_err(func_name,info) std::cout<<"[vm] "<<func_name<<": "<<info<<".\n"

// register builtin function's name and it's address here in this table below
// this table must end with {"",nullptr}
struct FUNC_TABLE
{
    const char* name;
    nasal_val* (*func)(std::vector<nasal_val*>&,nasal_gc&);
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

nasal_val* builtin_print(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    // get arguments
    // local_scope[0] is reserved for 'me'
    nasal_val* vec_addr=local_scope[1];
    // main process
    for(auto i:vec_addr->ptr.vec->elems)
        switch(i->type)
        {
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<i->ptr.num;       break;
            case vm_str:  std::cout<<*i->ptr.str;      break;
            case vm_vec:  i->ptr.vec->print();         break;
            case vm_hash: i->ptr.hash->print();        break;
            case vm_func: std::cout<<"func(...){...}"; break;
        }
    std::cout<<std::flush;
    // generate return value
    return gc.nil_addr;
}
nasal_val* builtin_append(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* vec_addr=local_scope[1];
    nasal_val* elem_addr=local_scope[2];
    if(vec_addr->type!=vm_vec)
    {
        builtin_err("append","\"vector\" must be vector");
        return nullptr;
    }
    std::vector<nasal_val*>& ref_vec=vec_addr->ptr.vec->elems;
    for(auto i:elem_addr->ptr.vec->elems)
        ref_vec.push_back(i);
    return gc.nil_addr;
}
nasal_val* builtin_setsize(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* vec_addr=local_scope[1];
    nasal_val* size_addr=local_scope[2];
    if(vec_addr->type!=vm_vec)
    {
        builtin_err("setsize","\"vector\" must be vector");
        return nullptr;
    }
    if(size_addr->type!=vm_num)
    {
        builtin_err("setsize","\"size\" is not a number");
        return nullptr;
    }
    int num=size_addr->ptr.num;
    if(num<0)
    {
        builtin_err("setsize","\"size\" must be greater than -1");
        return nullptr;
    }
    vec_addr->ptr.vec->elems.resize(num,gc.nil_addr);
    return gc.nil_addr;
}

nasal_val* builtin_system(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    nasal_val* str_addr=local_scope[1];
    if(str_addr->type!=vm_str)
    {
        builtin_err("system","\"str\" must be string");
        return nullptr;
    }
    ret_addr->ptr.num=(double)system(str_addr->ptr.str->c_str());
    return ret_addr;
}

nasal_val* builtin_input(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    std::cin>>*ret_addr->ptr.str;
    return ret_addr;
}

nasal_val* builtin_sleep(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("sleep","\"duration\" must be number");
        return nullptr;
    }
    // sleep in unistd.h will make this progress sleep sleep_time seconds.
    sleep((unsigned int)val_addr->ptr.num);
    return gc.nil_addr;
}

nasal_val* builtin_fin(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_str)
    {
        builtin_err("io.fin","\"filename\" must be string");
        return nullptr;
    }
    std::string&  filename=*val_addr->ptr.str;
    std::ifstream fin(filename);
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    *ret_addr->ptr.str="";
    if(!fin.fail())
        while(!fin.eof())
        {
            char c=fin.get();
            if(fin.eof())
                break;
            ret_addr->ptr.str->push_back(c);
        }
    else
        builtin_err("io.fin","cannot open \""+filename+"\"");
    fin.close();
    return ret_addr;
}

nasal_val* builtin_fout(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    nasal_val* str_addr=local_scope[2];
    if(val_addr->type!=vm_str)
    {
        builtin_err("io.fout","\"filename\" must be string");
        return nullptr;
    }
    if(str_addr->type!=vm_str)
    {
        builtin_err("io.fout","\"str\" must be string");
        return nullptr;
    }
    std::ofstream fout(*val_addr->ptr.str);
    if(fout.fail())
    {
        builtin_err("io.fout","cannot open \""+*val_addr->ptr.str+"\"");
        return nullptr;
    }
    fout<<*str_addr->ptr.str;
    fout.close();
    return gc.nil_addr;
}

nasal_val* builtin_split(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* deli_val_addr=local_scope[1];
    nasal_val* str_val_addr=local_scope[2];
    if(deli_val_addr->type!=vm_str)
    {
        builtin_err("split","\"delimeter\" must be string");
        return nullptr;
    }
    if(str_val_addr->type!=vm_str)
    {
        builtin_err("split","\"string\" must be string");
        return nullptr;
    }
    std::string& delimeter=*deli_val_addr->ptr.str;
    std::string& source=*str_val_addr->ptr.str;
    size_t delimeter_len=delimeter.length();
    size_t source_len=source.length();

    // push it to local scope to avoid being sweeped
    local_scope.push_back(gc.gc_alloc(vm_vec));

    std::vector<nasal_val*>& vec=local_scope.back()->ptr.vec->elems;
    if(!delimeter_len)
    {
        for(int i=0;i<source_len;++i)
        {
            vec.push_back(gc.gc_alloc(vm_str));
            *vec.back()->ptr.str=source[i];
        }
        return local_scope.back();
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
                *vec.back()->ptr.str=tmp;
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
        *vec.back()->ptr.str=tmp;
        tmp="";
    }
    return local_scope.back();
}
nasal_val* builtin_rand(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num && val_addr->type!=vm_nil)
    {
        builtin_err("rand","\"seed\" must be nil or number");
        return nullptr;
    }
    if(val_addr->type==vm_num)
    {
        srand((unsigned int)val_addr->ptr.num);
        return gc.nil_addr;
    }

    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=0;
    for(int i=0;i<5;++i)
        ret_addr->ptr.num=(ret_addr->ptr.num+rand())*(1.0/(RAND_MAX+1.0));
    return ret_addr;
}
nasal_val* builtin_id(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    char buf[32];
    sprintf(buf,"%p",val_addr);
    *ret_addr->ptr.str=buf;
    return ret_addr;
}
nasal_val* builtin_int(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("int","\"value\" must be number");
        return nullptr;
    }
    int number=(int)val_addr->ptr.num;
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=(double)number;
    return ret_addr;
}
nasal_val* builtin_num(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_str)
    {
        builtin_err("num","\"value\" must be string");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=val_addr->to_number();
    return ret_addr;
}
nasal_val* builtin_pop(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_vec)
    {
        builtin_err("pop","\"vector\" must be vector");
        return nullptr;
    }
    if(val_addr->ptr.vec->elems.size())
    {
        nasal_val* tmp=val_addr->ptr.vec->elems.back();
        val_addr->ptr.vec->elems.pop_back();
        return tmp;
    }
    return gc.nil_addr;
}
nasal_val* builtin_str(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("str","\"number\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    *ret_addr->ptr.str=val_addr->to_string();
    return ret_addr;
}
nasal_val* builtin_size(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    switch(val_addr->type)
    {
        case vm_nil:  ret_addr->ptr.num=0;                               break;
        case vm_num:  ret_addr->ptr.num=val_addr->ptr.num;               break;
        case vm_func: ret_addr->ptr.num=0;                               break;
        case vm_str:  ret_addr->ptr.num=val_addr->ptr.str->length();     break;
        case vm_vec:  ret_addr->ptr.num=val_addr->ptr.vec->elems.size(); break;
        case vm_hash: ret_addr->ptr.num=val_addr->ptr.hash->elems.size();break;
    }
    return ret_addr;
}
nasal_val* builtin_xor(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* a_addr=local_scope[1];
    nasal_val* b_addr=local_scope[2];
    if(a_addr->type!=vm_num)
    {
        builtin_err("xor","\"a\" must be number");
        return nullptr;
    }
    if(b_addr->type!=vm_num)
    {
        builtin_err("xor","\"b\" must be number");
        return nullptr;
    }
    int number_a=(int)a_addr->ptr.num;
    int number_b=(int)b_addr->ptr.num;
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=(number_a^number_b);
    return ret_addr;
}
nasal_val* builtin_and(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* a_addr=local_scope[1];
    nasal_val* b_addr=local_scope[2];
    if(a_addr->type!=vm_num)
    {
        builtin_err("and","\"a\" must be number");
        return nullptr;
    }
    if(b_addr->type!=vm_num)
    {
        builtin_err("and","\"b\" must be number");
        return nullptr;
    }
    int number_a=(int)a_addr->ptr.num;
    int number_b=(int)b_addr->ptr.num;
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=(number_a&number_b);
    return ret_addr;
}
nasal_val* builtin_or(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* a_addr=local_scope[1];
    nasal_val* b_addr=local_scope[2];
    if(a_addr->type!=vm_num)
    {
        builtin_err("or","\"a\" must be number");
        return nullptr;
    }
    if(b_addr->type!=vm_num)
    {
        builtin_err("or","\"b\" must be number");
        return nullptr;
    }
    int number_a=(int)a_addr->ptr.num;
    int number_b=(int)b_addr->ptr.num;
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=(number_a|number_b);
    return ret_addr;
}
nasal_val* builtin_nand(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* a_addr=local_scope[1];
    nasal_val* b_addr=local_scope[2];
    if(a_addr->type!=vm_num)
    {
        builtin_err("nand","\"a\" must be number");
        return nullptr;
    }
    if(b_addr->type!=vm_num)
    {
        builtin_err("nand","\"b\" must be number");
        return nullptr;
    }
    int number_a=(int)a_addr->ptr.num;
    int number_b=(int)b_addr->ptr.num;
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=(~(number_a&number_b));
    return ret_addr;
}
nasal_val* builtin_not(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* a_addr=local_scope[1];
    if(a_addr->type!=vm_num)
    {
        builtin_err("not","\"a\" must be number");
        return nullptr;
    }
    int number=(int)a_addr->ptr.num;
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=(~number);
    return ret_addr;
}
nasal_val* builtin_sin(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("sin","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=sin(val_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_cos(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("cos","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=cos(val_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_tan(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("tan","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=tan(val_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_exp(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("exp","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=exp(val_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_ln(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("ln","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=(log(val_addr->ptr.num)/log(2.7182818284590452354));
    return ret_addr;
}
nasal_val* builtin_sqrt(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("sqrt","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=sqrt(val_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_atan2(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* x_val_addr=local_scope[1];
    nasal_val* y_val_addr=local_scope[2];
    if(x_val_addr->type!=vm_num)
    {
        builtin_err("atan2","\"x\" must be number");
        return nullptr;
    }
    if(y_val_addr->type!=vm_num)
    {
        builtin_err("atan2","\"y\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=atan2(y_val_addr->ptr.num,x_val_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_time(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    if(val_addr->type!=vm_num)
    {
        builtin_err("time","\"begin_time\" must be number");
        return nullptr;
    }
    time_t begin_time=(time_t)val_addr->ptr.num;
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=time(&begin_time);
    return ret_addr;
}
nasal_val* builtin_contains(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* hash_addr=local_scope[1];
    nasal_val* key_addr=local_scope[2];
    if(hash_addr->type!=vm_hash)
    {
        builtin_err("contains","\"hash\" must be hash");
        return nullptr;
    }
    if(key_addr->type!=vm_str)
    {
        builtin_err("contains","\"key\" must be string");
        return nullptr;
    }
    return hash_addr->ptr.hash->elems.count(*key_addr->ptr.str)?gc.one_addr:gc.zero_addr;
}
nasal_val* builtin_delete(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* hash_addr=local_scope[1];
    nasal_val* key_addr=local_scope[2];
    if(hash_addr->type!=vm_hash)
    {
        builtin_err("delete","\"hash\" must be hash");
        return nullptr;
    }
    if(key_addr->type!=vm_str)
    {
        builtin_err("delete","\"key\" must be string");
        return nullptr;
    }
    if(hash_addr->ptr.hash->elems.count(*key_addr->ptr.str))
        hash_addr->ptr.hash->elems.erase(*key_addr->ptr.str);
    return gc.nil_addr;
}
nasal_val* builtin_keys(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* hash_addr=local_scope[1];
    if(hash_addr->type!=vm_hash)
    {
        builtin_err("keys","\"hash\" must be hash");
        return nullptr;
    }

    // push vector into local scope to avoid being sweeped
    local_scope.push_back(gc.gc_alloc(vm_vec));
    std::vector<nasal_val*>& vec=local_scope.back()->ptr.vec->elems;
    for(auto iter:hash_addr->ptr.hash->elems)
    {
        nasal_val* str_addr=gc.gc_alloc(vm_str);
        *str_addr->ptr.str=iter.first;
        vec.push_back(str_addr);
    }
    return local_scope.back();
}
nasal_val* builtin_import(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    // this function is used in preprocessing.
    // this function will return nothing when running.
    builtin_err("import","must use this function in global scope");
    return nullptr;
}
nasal_val* builtin_die(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* str_addr=local_scope[1];
    if(str_addr->type!=vm_str)
    {
        builtin_err("die","\"str\" must be string");
        return nullptr;
    }
    std::cout<<">> [vm] error: "<<*str_addr->ptr.str<<'\n';
    return nullptr;
}
nasal_val* builtin_type(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* val_addr=local_scope[1];
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    switch(val_addr->type)
    {
        case vm_nil:  *ret_addr->ptr.str="nil";  break;
        case vm_num:  *ret_addr->ptr.str="num";  break;
        case vm_str:  *ret_addr->ptr.str="str";  break;
        case vm_vec:  *ret_addr->ptr.str="vec";  break;
        case vm_hash: *ret_addr->ptr.str="hash"; break;
        case vm_func: *ret_addr->ptr.str="func"; break;
    }
    return ret_addr;
}
nasal_val* builtin_substr(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* str_addr=local_scope[1];
    nasal_val* beg_addr=local_scope[2];
    nasal_val* len_addr=local_scope[3];
    if(str_addr->type!=vm_str)
    {
        builtin_err("substr","\"str\" must be string");
        return nullptr;
    }
    if(beg_addr->type!=vm_num)
    {
        builtin_err("substr","\"begin\" must be number");
        return nullptr;
    }
    if(len_addr->type!=vm_num)
    {
        builtin_err("substr","\"length\" must be number");
        return nullptr;
    }
    std::string& str=*str_addr->ptr.str;
    int beg=(int)beg_addr->ptr.num;
    int len=(int)len_addr->ptr.num;
    if(beg>=str.length() || beg+len-1>=str.length())
    {
        builtin_err("susbtr","index out of range");
        return nullptr;
    }
    if(len<0)
        len=0;
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    *ret_addr->ptr.str=str.substr(beg,len);
    return ret_addr;
}
nasal_val* builtin_streq(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* a_addr=local_scope[1];
    nasal_val* b_addr=local_scope[2];
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=(a_addr->type!=vm_str || b_addr->type!=vm_str)?0:(*a_addr->ptr.str==*b_addr->ptr.str);
    return ret_addr;
}
nasal_val* builtin_left(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* str_addr=local_scope[1];
    nasal_val* len_addr=local_scope[2];
    if(str_addr->type!=vm_str)
    {
        builtin_err("left","\"string\" must be string");
        return nullptr;
    }
    if(len_addr->type!=vm_num)
    {
        builtin_err("left","\"length\" must be number");
        return nullptr;
    }
    std::string& str=*str_addr->ptr.str;
    int len=(int)len_addr->ptr.num;
    if(len<0)
        len=0;
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    *ret_addr->ptr.str=str.substr(0, len);
    return ret_addr;
}
nasal_val* builtin_right(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* str_addr=local_scope[1];
    nasal_val* len_addr=local_scope[2];
    if(str_addr->type!=vm_str)
    {
        builtin_err("right","\"string\" must be string");
        return nullptr;
    }
    if(len_addr->type!=vm_num)
    {
        builtin_err("right","\"length\" must be number");
        return nullptr;
    } 
    std::string& str=*str_addr->ptr.str;
    int len=(int)len_addr->ptr.num;
    int srclen=str.length();
    if(len>srclen)
        len=srclen;
    if(len<0)
        len=0;
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    *ret_addr->ptr.str=str.substr(srclen-len, srclen);
    return ret_addr;
}
nasal_val* builtin_cmp(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    nasal_val* a_addr=local_scope[1];
    nasal_val* b_addr=local_scope[2];
    if(a_addr->type!=vm_str)
    {
        builtin_err("cmp","\"a\" must be string");
        return nullptr;
    }
    if(b_addr->type!=vm_str)
    {
        builtin_err("cmp","\"b\" must be string");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->ptr.num=strcmp(a_addr->ptr.str->c_str(),b_addr->ptr.str->c_str());
    return ret_addr;
}
nasal_val* builtin_chr(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
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
    nasal_val* code_addr=local_scope[1];
    if(code_addr->type!=vm_num)
    {
        builtin_err("chr","\"code\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    int num=code_addr->ptr.num;
    if(0<=num && num<128)
        *ret_addr->ptr.str=(char)num;
    else if(128<=num && num<256)
        *ret_addr->ptr.str=extend[num-128];
    else
        *ret_addr->ptr.str=" ";
    return ret_addr;
}

#endif