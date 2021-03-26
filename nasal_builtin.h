#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__
/*
    builtin functions must be called inside a outer function like this:
    var print=func(elements...)
    {
        __builtin_std_cout(elements);
        return nil;
    }
    builtin function __builtin_std_cout is wrapped up by print
*/

std::unordered_map<std::string,int> builtin_use_str_table;
// used to find values that builtin function uses
#define in_builtin_find(val_name) \
(\
    local_scope_addr\
    ->ptr.scop\
    ->get_val\
    (builtin_use_str_table[val_name])\
)

// declaration of builtin functions
// to add new builtin function,declare it here and write the definition below

nasal_val* builtin_print(nasal_val*);
nasal_val* builtin_append(nasal_val*);
nasal_val* builtin_setsize(nasal_val*);
nasal_val* builtin_system(nasal_val*);
nasal_val* builtin_input(nasal_val*);
nasal_val* builtin_sleep(nasal_val*);
nasal_val* builtin_fin(nasal_val*);
nasal_val* builtin_fout(nasal_val*);
nasal_val* builtin_split(nasal_val*);
nasal_val* builtin_rand(nasal_val*);
nasal_val* builtin_id(nasal_val*);
nasal_val* builtin_int(nasal_val*);
nasal_val* builtin_num(nasal_val*);
nasal_val* builtin_pop(nasal_val*);
nasal_val* builtin_str(nasal_val*);
nasal_val* builtin_size(nasal_val*);
nasal_val* builtin_xor(nasal_val*);
nasal_val* builtin_and(nasal_val*);
nasal_val* builtin_or(nasal_val*);
nasal_val* builtin_nand(nasal_val*);
nasal_val* builtin_not(nasal_val*);
nasal_val* builtin_sin(nasal_val*);
nasal_val* builtin_cos(nasal_val*);
nasal_val* builtin_tan(nasal_val*);
nasal_val* builtin_exp(nasal_val*);
nasal_val* builtin_ln(nasal_val*);
nasal_val* builtin_sqrt(nasal_val*);
nasal_val* builtin_atan2(nasal_val*);
nasal_val* builtin_time(nasal_val*);
nasal_val* builtin_contains(nasal_val*);
nasal_val* builtin_delete(nasal_val*);
nasal_val* builtin_getkeys(nasal_val*);
nasal_val* builtin_import(nasal_val*);
nasal_val* builtin_die(nasal_val*);
nasal_val* builtin_type(nasal_val*);
nasal_val* builtin_substr(nasal_val*);
nasal_val* builtin_streq(nasal_val*);
nasal_val* builtin_left(nasal_val*);
nasal_val* builtin_right(nasal_val*);
nasal_val* builtin_cmp(nasal_val*);
nasal_val* builtin_chr(nasal_val*);

void builtin_error_occurred(std::string func_name,std::string info)
{
    std::cout<<">> [vm] "<<func_name<<": "<<info<<".\n";
    return;
}

// register builtin function's name and it's address here in this table below
// this table must end with {"",NULL}
struct FUNC_TABLE
{
    std::string name;
    nasal_val* (*func)(nasal_val* x);
} builtin_func[]=
{
    {"__builtin_std_cout", builtin_print   },
    {"__builtin_push_back",builtin_append  },
    {"__builtin_set_size", builtin_setsize },
    {"__builtin_system",   builtin_system  },
    {"__builtin_input",    builtin_input   },
    {"__builtin_sleep",    builtin_sleep   },
    {"__builtin_fin",      builtin_fin     },
    {"__builtin_fout",     builtin_fout    },
    {"__builtin_split",    builtin_split   },
    {"__builtin_rand",     builtin_rand    },
    {"__builtin_get_id",   builtin_id      },
    {"__builtin_int",      builtin_int     },
    {"__builtin_num",      builtin_num     },
    {"__builtin_pop_back", builtin_pop     },
    {"__builtin_str",      builtin_str     },
    {"__builtin_size",     builtin_size    },
    {"__builtin_xor",      builtin_xor     },
    {"__builtin_and",      builtin_and     },
    {"__builtin_or",       builtin_or      },
    {"__builtin_nand",     builtin_nand    },
    {"__builtin_not",      builtin_not     },
    {"__builtin_sin",      builtin_sin     },
    {"__builtin_cos",      builtin_cos     },
    {"__builtin_tan",      builtin_tan     },
    {"__builtin_exp",      builtin_exp     },
    {"__builtin_ln",       builtin_ln      },
    {"__builtin_sqrt",     builtin_sqrt    },
    {"__builtin_atan2",    builtin_atan2   },
    {"__builtin_time",     builtin_time    },
    {"__builtin_contains", builtin_contains},
    {"__builtin_delete",   builtin_delete  },
    {"__builtin_get_keys", builtin_getkeys },
    {"__builtin_import",   builtin_import  },
    {"__builtin_die",      builtin_die     },
    {"__builtin_type",     builtin_type    },
    {"__builtin_substr",   builtin_substr  },
    {"__builtin_streq",    builtin_streq   },
    {"__builtin_left",     builtin_left    },
    {"__builtin_right",    builtin_right   },
    {"__builtin_cmp",      builtin_cmp     },
    {"__builtin_chr",      builtin_chr     },
    {"",                   nullptr         }
};

nasal_val* builtin_print(nasal_val* local_scope_addr)
{
    // get arguments
    nasal_val* vector_value_addr=in_builtin_find("elements");
    // main process
    std::vector<nasal_val*>& ref_vec=vector_value_addr->ptr.vec->elems;
    int size=ref_vec.size();
    for(int i=0;i<size;++i)
    {
        nasal_val* tmp=ref_vec[i];
        switch(tmp->type)
        {
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<tmp->ptr.num;     break;
            case vm_str:  std::cout<<*tmp->ptr.str;    break;
            case vm_vec:  tmp->ptr.vec->print();       break;
            case vm_hash: tmp->ptr.hash->print();      break;
            case vm_func: std::cout<<"func(...){...}"; break;
        }
    }
    // generate return value
    return nil_addr;
}
nasal_val* builtin_append(nasal_val* local_scope_addr)
{
    nasal_val* vector_value_addr=in_builtin_find("vector");
    nasal_val* elem_value_addr=in_builtin_find("elements");
    if(vector_value_addr->type!=vm_vec)
    {
        builtin_error_occurred("append","\"vector\" must be vector");
        return nullptr;
    }
    nasal_vec& ref_vector=*vector_value_addr->ptr.vec;
    nasal_vec& ref_elements=*elem_value_addr->ptr.vec;
    int size=ref_elements.elems.size();
    for(int i=0;i<size;++i)
    {
        nasal_val* value_address=ref_elements.get_val(i);
        ref_vector.elems.push_back(value_address);
    }
    return nil_addr;
}
nasal_val* builtin_setsize(nasal_val* local_scope_addr)
{
    nasal_val* vector_value_addr=in_builtin_find("vector");
    nasal_val* size_value_addr=in_builtin_find("size");
    if(vector_value_addr->type!=vm_vec)
    {
        builtin_error_occurred("setsize","\"vector\" must be vector");
        return nullptr;
    }
    int type=size_value_addr->type;
    if(type!=vm_num)
    {
        builtin_error_occurred("setsize","\"size\" is not a number");
        return nullptr;
    }
    int number=size_value_addr->ptr.num;
    if(number<0)
    {
        builtin_error_occurred("setsize","\"size\" must be greater than -1");
        return nullptr;
    }
    nasal_vec& ref_vector=*vector_value_addr->ptr.vec;
    int vec_size=ref_vector.elems.size();
    if(number<vec_size)
        for(int i=number;i<vec_size;++i)
            ref_vector.elems.pop_back();
    else if(number>vec_size)
        for(int i=vec_size;i<number;++i)
        {
            nasal_val* new_val_addr=nil_addr;
            ref_vector.elems.push_back(new_val_addr);
        }
    return nil_addr;
}

nasal_val* builtin_system(nasal_val* local_scope_addr)
{
    nasal_val* str_value_addr=in_builtin_find("str");
    if(str_value_addr->type!=vm_str)
    {
        builtin_error_occurred("system","\"str\" must be string");
        return nullptr;
    }
    system(str_value_addr->ptr.str->data());
    nasal_val* ret_addr=nil_addr;
    return ret_addr;
}

nasal_val* builtin_input(nasal_val* local_scope_addr)
{
    nasal_val* ret_addr=gc_alloc(vm_str);
    std::cin>>*ret_addr->ptr.str;
    return ret_addr;
}

nasal_val* builtin_sleep(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("duration");
    int type=value_addr->type;
    if(type!=vm_num)
    {
        builtin_error_occurred("sleep","\"duration\" must be number");
        return nullptr;
    }
    // sleep in unistd.h will make this progress sleep sleep_time seconds.
    sleep((unsigned long)value_addr->ptr.num);
    return nil_addr;
}

nasal_val* builtin_fin(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("filename");
    if(value_addr->type!=vm_str)
    {
        builtin_error_occurred("io.fin","\"filename\" must be string");
        return nullptr;
    }
    std::string&  filename=*value_addr->ptr.str;
    std::ifstream fin(filename);
    nasal_val* ret_addr=gc_alloc(vm_str);
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
        builtin_error_occurred("io.fin","cannot open \""+filename+"\"");
    fin.close();
    return ret_addr;
}

nasal_val* builtin_fout(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("filename");
    nasal_val* str_value_addr=in_builtin_find("str");
    if(value_addr->type!=vm_str)
    {
        builtin_error_occurred("io.fout","\"filename\" must be string");
        return nullptr;
    }
    if(str_value_addr->type!=vm_str)
    {
        builtin_error_occurred("io.fout","\"str\" must be string");
        return nullptr;
    }
    std::ofstream fout(*value_addr->ptr.str);
    if(fout.fail())
    {
        builtin_error_occurred("io.fout","cannot open \""+*value_addr->ptr.str+"\"");
        return nullptr;
    }
    fout<<*str_value_addr->ptr.str;
    fout.close();
    return nil_addr;
}

nasal_val* builtin_split(nasal_val* local_scope_addr)
{
    nasal_val* delimeter_value_addr=in_builtin_find("delimeter");
    nasal_val* string_value_addr=in_builtin_find("string");
    if(delimeter_value_addr->type!=vm_str)
    {
        builtin_error_occurred("split","\"delimeter\" must be string");
        return nullptr;
    }
    if(string_value_addr->type!=vm_str)
    {
        builtin_error_occurred("split","\"string\" must be string");
        return nullptr;
    }
    std::string delimeter=*delimeter_value_addr->ptr.str;
    std::string source=*string_value_addr->ptr.str;
    int delimeter_len=delimeter.length();
    int source_len=source.length();

    nasal_val* ret_addr=gc_alloc(vm_vec);
    nasal_vec& ref_vec=*ret_addr->ptr.vec;
    std::string tmp="";

    if(!delimeter_len)
    {
        for(int i=0;i<source_len;++i)
        {
            nasal_val* str_addr=gc_alloc(vm_str);
            *str_addr->ptr.str=source[i];
            ref_vec.elems.push_back(str_addr);
        }
        return ret_addr;
    }
    
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
                nasal_val* str_addr=gc_alloc(vm_str);
                *str_addr->ptr.str=tmp;
                ref_vec.elems.push_back(str_addr);
                tmp="";
            }
            i+=delimeter_len-1;
        }
        else
            tmp+=source[i];
    }
    if(tmp.length())
    {
        nasal_val* str_addr=gc_alloc(vm_str);
        *str_addr->ptr.str=tmp;
        ref_vec.elems.push_back(str_addr);
        tmp="";
    }
    return ret_addr;
}
nasal_val* builtin_rand(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("seed");
    if(value_addr->type!=vm_num && value_addr->type!=vm_nil)
    {
        builtin_error_occurred("rand","\"seed\" must be nil or number");
        return nullptr;
    }
    if(value_addr->type==vm_num)
    {
        unsigned int number=(unsigned int)value_addr->ptr.num;
        srand(number);
        nasal_val* ret_addr=nil_addr;
        return ret_addr;
    }
    double num=0;
    for(int i=0;i<5;++i)
        num=(num+rand())*(1.0/(RAND_MAX+1.0));
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=num;
    return ret_addr;
}
nasal_val* builtin_id(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("thing");
    nasal_val* ret_addr=gc_alloc(vm_str);
    char buf[32];
    sprintf(buf,"0x%p",value_addr);
    *ret_addr->ptr.str=buf;
    return ret_addr;
}
nasal_val* builtin_int(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("value");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("int","\"value\" must be number");
        return nullptr;
    }
    int number=(int)value_addr->ptr.num;
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=(double)number;
    return ret_addr;
}
nasal_val* builtin_num(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("value");
    if(value_addr->type!=vm_str)
    {
        builtin_error_occurred("num","\"value\" must be string");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=value_addr->to_number();
    return ret_addr;
}
nasal_val* builtin_pop(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("vector");
    if(value_addr->type!=vm_vec)
    {
        builtin_error_occurred("pop","\"vector\" must be vector");
        return nullptr;
    }
    if(value_addr->ptr.vec->elems.size())
    {
        nasal_val* tmp=value_addr->ptr.vec->elems.back();
        value_addr->ptr.vec->elems.pop_back();
        return tmp;
    }
    return nil_addr;
}
nasal_val* builtin_str(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("number");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("str","\"number\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    *ret_addr->ptr.str=value_addr->to_string();
    return ret_addr;
}
nasal_val* builtin_size(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("object");
    int type=value_addr->type;
    int number=-1;
    switch(type)
    {
        case vm_nil:
        case vm_num:
        case vm_func: 
        case vm_scop: return nil_addr;                          break;
        case vm_str:  number=value_addr->ptr.str->length();     break;
        case vm_vec:  number=value_addr->ptr.vec->elems.size(); break;
        case vm_hash: number=value_addr->ptr.hash->elems.size();break;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=number;
    return ret_addr;
}
nasal_val* builtin_xor(nasal_val* local_scope_addr)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->type!=vm_num)
    {
        builtin_error_occurred("xor","\"a\" must be number");
        return nullptr;
    }
    if(b_addr->type!=vm_num)
    {
        builtin_error_occurred("xor","\"b\" must be number");
        return nullptr;
    }
    int number_a=(int)a_addr->ptr.num;
    int number_b=(int)b_addr->ptr.num;
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=(number_a^number_b);
    return ret_addr;
}
nasal_val* builtin_and(nasal_val* local_scope_addr)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->type!=vm_num)
    {
        builtin_error_occurred("and","\"a\" must be number");
        return nullptr;
    }
    if(b_addr->type!=vm_num)
    {
        builtin_error_occurred("and","\"b\" must be number");
        return nullptr;
    }
    int number_a=(int)a_addr->ptr.num;
    int number_b=(int)b_addr->ptr.num;
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=(number_a&number_b);
    return ret_addr;
}
nasal_val* builtin_or(nasal_val* local_scope_addr)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->type!=vm_num)
    {
        builtin_error_occurred("or","\"a\" must be number");
        return nullptr;
    }
    if(b_addr->type!=vm_num)
    {
        builtin_error_occurred("or","\"b\" must be number");
        return nullptr;
    }
    int number_a=(int)a_addr->ptr.num;
    int number_b=(int)b_addr->ptr.num;
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=(number_a|number_b);
    return ret_addr;
}
nasal_val* builtin_nand(nasal_val* local_scope_addr)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->type!=vm_num)
    {
        builtin_error_occurred("nand","\"a\" must be number");
        return nullptr;
    }
    if(b_addr->type!=vm_num)
    {
        builtin_error_occurred("nand","\"b\" must be number");
        return nullptr;
    }
    int number_a=(int)a_addr->ptr.num;
    int number_b=(int)b_addr->ptr.num;
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=(~(number_a&number_b));
    return ret_addr;
}
nasal_val* builtin_not(nasal_val* local_scope_addr)
{
    nasal_val* a_addr=in_builtin_find("a");
    if(a_addr->type!=vm_num)
    {
        builtin_error_occurred("not","\"a\" must be number");
        return nullptr;
    }
    int number=(int)a_addr->ptr.num;
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=(~number);
    return ret_addr;
}
nasal_val* builtin_sin(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("sin","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=sin(value_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_cos(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("cos","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=cos(value_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_tan(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("tan","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=tan(value_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_exp(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("exp","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=exp(value_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_ln(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("ln","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=(log(value_addr->ptr.num)/log(2.7182818284590452354));
    return ret_addr;
}
nasal_val* builtin_sqrt(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("sqrt","\"x\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=sqrt(value_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_atan2(nasal_val* local_scope_addr)
{
    nasal_val* x_value_addr=in_builtin_find("x");
    nasal_val* y_value_addr=in_builtin_find("y");
    if(x_value_addr->type!=vm_num)
    {
        builtin_error_occurred("atan2","\"x\" must be number");
        return nullptr;
    }
    if(y_value_addr->type!=vm_num)
    {
        builtin_error_occurred("atan2","\"y\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=atan2(y_value_addr->ptr.num,x_value_addr->ptr.num);
    return ret_addr;
}
nasal_val* builtin_time(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("begin_time");
    if(value_addr->type!=vm_num)
    {
        builtin_error_occurred("time","\"begin_time\" must be number");
        return nullptr;
    }
    time_t begin_time=(time_t)value_addr->ptr.num;
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=time(&begin_time);
    return ret_addr;
}
nasal_val* builtin_contains(nasal_val* local_scope_addr)
{
    nasal_val* hash_addr=in_builtin_find("hash");
    nasal_val* key_addr=in_builtin_find("key");
    if(hash_addr->type!=vm_hash)
    {
        builtin_error_occurred("contains","\"hash\" must be hash");
        return nullptr;
    }
    if(key_addr->type!=vm_str)
    {
        builtin_error_occurred("contains","\"key\" must be string");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=hash_addr->ptr.hash->check_contain(*key_addr->ptr.str);
    return ret_addr;
}
nasal_val* builtin_delete(nasal_val* local_scope_addr)
{
    nasal_val* hash_addr=in_builtin_find("hash");
    nasal_val* key_addr=in_builtin_find("key");
    if(hash_addr->type!=vm_hash)
    {
        builtin_error_occurred("delete","\"hash\" must be hash");
        return nullptr;
    }
    if(key_addr->type!=vm_str)
    {
        builtin_error_occurred("delete","\"key\" must be string");
        return nullptr;
    }
    if(hash_addr->ptr.hash->elems.count(*key_addr->ptr.str))
        hash_addr->ptr.hash->elems.erase(*key_addr->ptr.str);
    return nil_addr;
}
nasal_val* builtin_getkeys(nasal_val* local_scope_addr)
{
    nasal_val* hash_addr=in_builtin_find("hash");
    if(hash_addr->type!=vm_hash)
    {
        builtin_error_occurred("keys","\"hash\" must be hash");
        return nullptr;
    }
    return hash_addr->ptr.hash->get_keys();
}
nasal_val* builtin_import(nasal_val* local_scope_addr)
{
    // this function is used in preprocessing.
    // this function will return nothing when running.
    builtin_error_occurred("import","cannot use import when running");
    return nullptr;
}
nasal_val* builtin_die(nasal_val* local_scope_addr)
{
    nasal_val* str_addr=in_builtin_find("str");
    if(str_addr->type!=vm_str)
    {
        builtin_error_occurred("die","\"str\" must be string");
        return nullptr;
    }
    std::cout<<">> [vm] error: "<<str_addr->ptr.str<<'\n';
    return nullptr;
}
nasal_val* builtin_type(nasal_val* local_scope_addr)
{
    nasal_val* value_addr=in_builtin_find("object");
    nasal_val* ret_addr=gc_alloc(vm_str);
    switch(value_addr->type)
    {
        case vm_nil:  *ret_addr->ptr.str="nil";      break;
        case vm_num:  *ret_addr->ptr.str="number";   break;
        case vm_str:  *ret_addr->ptr.str="string";   break;
        case vm_vec:  *ret_addr->ptr.str="vector";   break;
        case vm_hash: *ret_addr->ptr.str="hash";     break;
        case vm_func: *ret_addr->ptr.str="function"; break;
    }
    return ret_addr;
}
nasal_val* builtin_substr(nasal_val* local_scope_addr)
{
    nasal_val* str_addr=in_builtin_find("str");
    nasal_val* begin_addr=in_builtin_find("begin");
    nasal_val* length_addr=in_builtin_find("length");
    if(str_addr->type!=vm_str)
    {
        builtin_error_occurred("substr","\"str\" must be string");
        return nullptr;
    }
    if(begin_addr->type!=vm_num)
    {
        builtin_error_occurred("substr","\"begin\" must be number");
        return nullptr;
    }
    if(length_addr->type!=vm_num)
    {
        builtin_error_occurred("substr","\"length\" must be number");
        return nullptr;
    }
    std::string str=*str_addr->ptr.str;
    int begin=(int)begin_addr->ptr.num;
    int len=(int)length_addr->ptr.num;
    if(begin+len>str.length())
    {
        builtin_error_occurred("susbtr","index out of range");
        return nullptr;
    }
    std::string tmp="";
    for(int i=begin;i<begin+len;++i)
        tmp+=str[i];
    nasal_val* ret_addr=gc_alloc(vm_str);
    *ret_addr->ptr.str=tmp;
    return ret_addr;
}
nasal_val* builtin_streq(nasal_val* local_scope_addr)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    nasal_val* ret_addr=gc_alloc(vm_num);
    if(a_addr->type!=vm_str || b_addr->type!=vm_str)
    {
        ret_addr->ptr.num=0;
        return ret_addr;
    }
    ret_addr->ptr.num=(a_addr->ptr.str==b_addr->ptr.str);
    return ret_addr;
}
nasal_val* builtin_left(nasal_val* local_scope_addr)
{
    nasal_val* string_addr=in_builtin_find("string");
    nasal_val* length_addr=in_builtin_find("length");
    if(string_addr->type!=vm_str)
    {
        builtin_error_occurred("left","\"string\" must be string");
        return nullptr;
    }
    if(length_addr->type!=vm_num)
    {
        builtin_error_occurred("left","\"length\" must be number");
        return nullptr;
    }
    std::string str=*string_addr->ptr.str;
    int len=(int)length_addr->ptr.num;
    if(len<0)
        len=0;
    std::string tmp=str.substr(0, len);
    nasal_val* ret_addr=gc_alloc(vm_str);
    *ret_addr->ptr.str=tmp;
    return ret_addr;
}
nasal_val* builtin_right(nasal_val* local_scope_addr)
{
    nasal_val* string_addr=in_builtin_find("string");
    nasal_val* length_addr=in_builtin_find("length");
    if(string_addr->type!=vm_str)
    {
        builtin_error_occurred("right","\"string\" must be string");
        return nullptr;
    }
    if(length_addr->type!=vm_num)
    {
        builtin_error_occurred("right","\"length\" must be number");
        return nullptr;
    } 
    std::string str=*string_addr->ptr.str;
    int len=(int)length_addr->ptr.num;
    int srclen=str.length();
    if(len>srclen)
        len=srclen;
    if(len<0)
        len=0;
    std::string tmp=str.substr(srclen-len, srclen);
    nasal_val* ret_addr=gc_alloc(vm_str);
    *ret_addr->ptr.str=tmp;
    return ret_addr;
}
nasal_val* builtin_cmp(nasal_val* local_scope_addr)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->type!=vm_str)
    {
        builtin_error_occurred("cmp","\"a\" must be string");
        return nullptr;
    }
    if(b_addr->type!=vm_str)
    {
        builtin_error_occurred("cmp","\"b\" must be string");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_num);
    ret_addr->ptr.num=strcmp(a_addr->ptr.str->data(),b_addr->ptr.str->data());
    return ret_addr;
}
nasal_val* builtin_chr(nasal_val* local_scope_addr)
{
    nasal_val* code_addr=in_builtin_find("code");
    if(code_addr->type!=vm_num)
    {
        builtin_error_occurred("chr","\"code\" must be number");
        return nullptr;
    }
    nasal_val* ret_addr=gc_alloc(vm_str);
    *ret_addr->ptr.str=(char)code_addr->ptr.num;
    return ret_addr;
}

#endif