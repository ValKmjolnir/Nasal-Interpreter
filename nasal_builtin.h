#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__

// builtin functions must be called inside a outer function like this:
// var print=func(elements...)
// {
//     nasal_call_builtin_std_cout(elements);
//     return nil;
// }
// builtin function nasal_call_builtin_std_cout is wrapped up by print

std::map<std::string,int> builtin_use_string_table;
// used to find values that builtin function uses
#define in_builtin_find(value_name_string) \
(\
    local_scope_addr\
    ->get_closure()\
    .get_value_address\
    (builtin_use_string_table[value_name_string])\
)

// declaration of builtin functions
// to add new builtin function,declare it here and write the definition below

nasal_val* builtin_print(nasal_val*,nasal_gc&);
nasal_val* builtin_append(nasal_val*,nasal_gc&);
nasal_val* builtin_setsize(nasal_val*,nasal_gc&);
nasal_val* builtin_system(nasal_val*,nasal_gc&);
nasal_val* builtin_input(nasal_val*,nasal_gc&);
nasal_val* builtin_sleep(nasal_val*,nasal_gc&);
nasal_val* builtin_finput(nasal_val*,nasal_gc&);
nasal_val* builtin_foutput(nasal_val*,nasal_gc&);
nasal_val* builtin_split(nasal_val*,nasal_gc&);
nasal_val* builtin_rand(nasal_val*,nasal_gc&);
nasal_val* builtin_id(nasal_val*,nasal_gc&);
nasal_val* builtin_int(nasal_val*,nasal_gc&);
nasal_val* builtin_num(nasal_val*,nasal_gc&);
nasal_val* builtin_pop(nasal_val*,nasal_gc&);
nasal_val* builtin_str(nasal_val*,nasal_gc&);
nasal_val* builtin_size(nasal_val*,nasal_gc&);
nasal_val* builtin_xor(nasal_val*,nasal_gc&);
nasal_val* builtin_and(nasal_val*,nasal_gc&);
nasal_val* builtin_or(nasal_val*,nasal_gc&);
nasal_val* builtin_nand(nasal_val*,nasal_gc&);
nasal_val* builtin_not(nasal_val*,nasal_gc&);
nasal_val* builtin_sin(nasal_val*,nasal_gc&);
nasal_val* builtin_cos(nasal_val*,nasal_gc&);
nasal_val* builtin_tan(nasal_val*,nasal_gc&);
nasal_val* builtin_exp(nasal_val*,nasal_gc&);
nasal_val* builtin_ln(nasal_val*,nasal_gc&);
nasal_val* builtin_sqrt(nasal_val*,nasal_gc&);
nasal_val* builtin_atan2(nasal_val*,nasal_gc&);
nasal_val* builtin_time(nasal_val*,nasal_gc&);
nasal_val* builtin_contains(nasal_val*,nasal_gc&);
nasal_val* builtin_delete(nasal_val*,nasal_gc&);
nasal_val* builtin_getkeys(nasal_val*,nasal_gc&);
nasal_val* builtin_import(nasal_val*,nasal_gc&);
bool       builtin_die_state;// used in builtin_die
nasal_val* builtin_die(nasal_val*,nasal_gc&);
nasal_val* builtin_type(nasal_val*,nasal_gc&);
nasal_val* builtin_substr(nasal_val*,nasal_gc&);
nasal_val* builtin_streq(nasal_val*,nasal_gc&);
nasal_val* builtin_left(nasal_val*,nasal_gc&);
nasal_val* builtin_right(nasal_val*,nasal_gc&);
nasal_val* builtin_cmp(nasal_val*,nasal_gc&);
nasal_val* builtin_chr(nasal_val*,nasal_gc&);

void builtin_error_occurred(std::string func_name,std::string info)
{
    builtin_die_state=true;
    std::cout<<">> [vm] "<<func_name<<": "<<info<<".\n";
    return;
}

// register builtin function's name and it's address here in this table below
// this table must end with {"",NULL}
struct FUNC_TABLE
{
    std::string func_name;
    nasal_val* (*func_pointer)(nasal_val* x,nasal_gc& gc);
} builtin_func_table[]=
{
    {"nasal_call_builtin_std_cout",      builtin_print},
    {"nasal_call_builtin_push_back",     builtin_append},
    {"nasal_call_builtin_set_size",      builtin_setsize},
    {"nasal_call_builtin_system",        builtin_system},
    {"nasal_call_builtin_input",         builtin_input},
    {"nasal_call_builtin_sleep",         builtin_sleep},
    {"nasal_call_builtin_finput",        builtin_finput},
    {"nasal_call_builtin_foutput",       builtin_foutput},
    {"nasal_call_builtin_split",         builtin_split},
    {"nasal_call_builtin_rand",          builtin_rand},
    {"nasal_call_builtin_get_id",        builtin_id},
    {"nasal_call_builtin_trans_int",     builtin_int},
    {"nasal_call_builtin_trans_num",     builtin_num},
    {"nasal_call_builtin_pop_back",      builtin_pop},
    {"nasal_call_builtin_trans_str",     builtin_str},
    {"nasal_call_builtin_size",          builtin_size},
    {"nasal_call_builtin_xor",           builtin_xor},
    {"nasal_call_builtin_and",           builtin_and},
    {"nasal_call_builtin_or",            builtin_or},
    {"nasal_call_builtin_nand",          builtin_nand},
    {"nasal_call_builtin_not",           builtin_not},
    {"nasal_call_builtin_sin",           builtin_sin},
    {"nasal_call_builtin_cos",           builtin_cos},
    {"nasal_call_builtin_tan",           builtin_tan},
    {"nasal_call_builtin_exp",           builtin_exp},
    {"nasal_call_builtin_cpp_math_ln",   builtin_ln},
    {"nasal_call_builtin_cpp_math_sqrt", builtin_sqrt},
    {"nasal_call_builtin_cpp_atan2",     builtin_atan2},
    {"nasal_call_builtin_time",          builtin_time},
    {"nasal_call_builtin_contains",      builtin_contains},
    {"nasal_call_builtin_delete",        builtin_delete},
    {"nasal_call_builtin_get_keys",      builtin_getkeys},
    {"nasal_call_import",                builtin_import},
    {"nasal_call_builtin_die",           builtin_die},
    {"nasal_call_builtin_type",          builtin_type},
    {"nasal_call_builtin_substr",        builtin_substr},
    {"nasal_call_builtin_streq",         builtin_streq},
    {"nasal_call_builtin_left",          builtin_left},
    {"nasal_call_builtin_right",         builtin_right},
    {"nasal_call_builtin_cmp",           builtin_cmp},
    {"nasal_call_builtin_chr",           builtin_chr},
    {"",                                 NULL}
};

nasal_val* builtin_print(nasal_val* local_scope_addr,nasal_gc& gc)
{
    // get arguments
    nasal_val* vector_value_addr=in_builtin_find("elements");
    // main process
    nasal_vec& ref_vec=vector_value_addr->get_vector();
    int size=ref_vec.size();
    for(int i=0;i<size;++i)
    {
        nasal_val* tmp=ref_vec[i];
        switch(tmp->get_type())
        {
            case vm_nil:  std::cout<<"nil";             break;
            case vm_num:  std::cout<<tmp->get_number(); break;
            case vm_str:  std::cout<<tmp->get_string(); break;
            case vm_vec:  tmp->get_vector().print();    break;
            case vm_hash: tmp->get_hash().print();      break;
            case vm_func: std::cout<<"func(...){...}";  break;
        }
    }
    // generate return value
    return gc.gc_alloc(vm_nil);
}
nasal_val* builtin_append(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* vector_value_addr=in_builtin_find("vector");
    nasal_val* elem_value_addr=in_builtin_find("elements");
    if(vector_value_addr->get_type()!=vm_vec)
    {
        builtin_error_occurred("append","\"vector\" must be vector");
        return NULL;
    }
    nasal_vec& ref_vector=vector_value_addr->get_vector();
    nasal_vec& ref_elements=elem_value_addr->get_vector();
    int size=ref_elements.size();
    for(int i=0;i<size;++i)
    {
        nasal_val* value_address=ref_elements.get_value_address(i);
        gc.add_reference(value_address);
        ref_vector.add_elem(value_address);
    }
    return gc.gc_alloc(vm_nil);
}
nasal_val* builtin_setsize(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* vector_value_addr=in_builtin_find("vector");
    nasal_val* size_value_addr=in_builtin_find("size");
    if(vector_value_addr->get_type()!=vm_vec)
    {
        builtin_error_occurred("setsize","\"vector\" must be vector");
        return NULL;
    }
    int type=size_value_addr->get_type();
    if(type!=vm_num)
    {
        builtin_error_occurred("setsize","\"size\" is not a number");
        return NULL;
    }
    int number=size_value_addr->get_number();
    if(number<0)
    {
        builtin_error_occurred("setsize","\"size\" must be greater than -1");
        return NULL;
    }
    nasal_vec& ref_vector=vector_value_addr->get_vector();
    int vec_size=ref_vector.size();
    if(number<vec_size)
        for(int i=number;i<vec_size;++i)
        {
            nasal_val* addr=ref_vector.del_elem();
            if(addr) gc.del_reference(addr);
        }
    else if(number>vec_size)
        for(int i=vec_size;i<number;++i)
        {
            nasal_val* new_val_addr=gc.gc_alloc(vm_nil);
            ref_vector.add_elem(new_val_addr);
        }
    nasal_val* ret_addr=gc.gc_alloc(vm_nil);
    return ret_addr;
}

nasal_val* builtin_system(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* str_value_addr=in_builtin_find("str");
    if(str_value_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("system","\"str\" must be string");
        return NULL;
    }
    std::string str=str_value_addr->get_string();
    system(str.data());
    nasal_val* ret_addr=gc.gc_alloc(vm_nil);
    return ret_addr;
}

nasal_val* builtin_input(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    std::string str;
    std::cin>>str;
    ret_addr->set_string(str);
    return ret_addr;
}

nasal_val* builtin_sleep(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("duration");
    int type=value_addr->get_type();
    if(type!=vm_num)
    {
        builtin_error_occurred("sleep","\"duration\" must be number");
        return NULL;
    }
    // sleep in unistd.h will make this progress sleep sleep_time seconds.
    sleep((unsigned long)value_addr->get_number());
    nasal_val* ret_addr=gc.gc_alloc(vm_nil);
    return ret_addr;
}

nasal_val* builtin_finput(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("filename");
    if(value_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("io.fin","\"filename\" must be string");
        return NULL;
    }
    std::string filename=value_addr->get_string();
    std::ifstream fin(filename);
    std::string file_content="";
    if(!fin.fail())
        while(!fin.eof())
        {
            char c=fin.get();
            if(fin.eof())
                break;
            file_content.push_back(c);
        }
    else
        builtin_error_occurred("io.fin","cannot open \""+filename+"\".");
    fin.close();
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    ret_addr->set_string(file_content);
    return ret_addr;
}

nasal_val* builtin_foutput(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("filename");
    nasal_val* str_value_addr=in_builtin_find("str");
    if(value_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("io.fout","\"filename\" must be string");
        return NULL;
    }
    if(str_value_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("io.fout","\"str\" must be string");
        return NULL;
    }
    std::string filename=value_addr->get_string();
    std::string file_content=str_value_addr->get_string();
    std::ofstream fout(filename);
    fout<<file_content;
    fout.close();
    nasal_val* ret_addr=gc.gc_alloc(vm_nil);
    return ret_addr;
}

nasal_val* builtin_split(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* delimeter_value_addr=in_builtin_find("delimeter");
    nasal_val* string_value_addr=in_builtin_find("string");
    if(delimeter_value_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("split","\"delimeter\" must be string");
        return NULL;
    }
    if(string_value_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("split","\"string\" must be string");
        return NULL;
    }
    std::string delimeter=delimeter_value_addr->get_string();
    std::string source=string_value_addr->get_string();
    int delimeter_len=delimeter.length();
    int source_len=source.length();

    nasal_val* ret_addr=gc.gc_alloc(vm_vec);
    nasal_vec& ref_vec=ret_addr->get_vector();
    std::string tmp="";

    if(!delimeter_len)
    {
        for(int i=0;i<source_len;++i)
        {
            tmp=source[i];
            nasal_val* str_addr=gc.gc_alloc(vm_str);
            str_addr->set_string(tmp);
            ref_vec.add_elem(str_addr);
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
                nasal_val* str_addr=gc.gc_alloc(vm_str);
                str_addr->set_string(tmp);
                ref_vec.add_elem(str_addr);
                tmp="";
            }
            i+=delimeter_len-1;
        }
        else
            tmp+=source[i];
    }
    if(tmp.length())
    {
        nasal_val* str_addr=gc.gc_alloc(vm_str);
        str_addr->set_string(tmp);
        ref_vec.add_elem(str_addr);
        tmp="";
    }
    return ret_addr;
}
nasal_val* builtin_rand(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("seed");
    if(value_addr->get_type()!=vm_num && value_addr->get_type()!=vm_nil)
    {
        builtin_error_occurred("rand","\"seed\" must be nil or number");
        return NULL;
    }
    if(value_addr->get_type()==vm_num)
    {
        unsigned int number=(unsigned int)value_addr->get_number();
        srand(number);
        nasal_val* ret_addr=gc.gc_alloc(vm_nil);
        return ret_addr;
    }
    double num=0;
    for(int i=0;i<5;++i)
        num=(num+rand())*(1.0/(RAND_MAX+1.0));
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(num);
    return ret_addr;
}
nasal_val* builtin_id(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("thing");
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    char buf[32];
    sprintf(buf,"0x%p",value_addr);
    ret_addr->set_string(buf);
    return ret_addr;
}
nasal_val* builtin_int(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("value");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("int","\"value\" must be number");
        return NULL;
    }
    int number=(int)value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)number);
    return ret_addr;
}
nasal_val* builtin_num(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("value");
    if(value_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("num","\"value\" must be string");
        return NULL;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(value_addr->to_number());
    return ret_addr;
}
nasal_val* builtin_pop(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("vector");
    if(value_addr->get_type()!=vm_vec)
    {
        builtin_error_occurred("pop","\"vector\" must be vector");
        return NULL;
    }
    nasal_val* ret_addr=value_addr->get_vector().del_elem();
    return ret_addr;
}
nasal_val* builtin_str(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("number");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("str","\"number\" must be number");
        return NULL;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_string(value_addr->to_string());
    return ret_addr;
}
nasal_val* builtin_size(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("object");
    int type=value_addr->get_type();
    int number=-1;
    switch(type)
    {
        case vm_nil:
        case vm_num:
        case vm_func: 
        case vm_scop: return gc.gc_alloc(vm_nil);               break;
        case vm_str:  number=value_addr->get_string().length(); break;
        case vm_vec:  number=value_addr->get_vector().size();   break;
        case vm_hash: number=value_addr->get_hash().size();     break;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)number);
    return ret_addr;
}
nasal_val* builtin_xor(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("xor","\"a\" must be number");
        return NULL;
    }
    if(b_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("xor","\"b\" must be number");
        return NULL;
    }
    int number_a=(int)a_addr->get_number();
    int number_b=(int)b_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)(number_a^number_b));
    return ret_addr;
}
nasal_val* builtin_and(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("and","\"a\" must be number");
        return NULL;
    }
    if(b_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("and","\"b\" must be number");
        return NULL;
    }
    int number_a=(int)a_addr->get_number();
    int number_b=(int)b_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)(number_a&number_b));
    return ret_addr;
}
nasal_val* builtin_or(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("or","\"a\" must be number");
        return NULL;
    }
    if(b_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("or","\"b\" must be number");
        return NULL;
    }
    int number_a=(int)a_addr->get_number();
    int number_b=(int)b_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)(number_a|number_b));
    return ret_addr;
}
nasal_val* builtin_nand(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("nand","\"a\" must be number");
        return NULL;
    }
    if(b_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("nand","\"b\" must be number");
        return NULL;
    }
    int number_a=(int)a_addr->get_number();
    int number_b=(int)b_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)(~(number_a&number_b)));
    return ret_addr;
}
nasal_val* builtin_not(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* a_addr=in_builtin_find("a");
    if(a_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("not","\"a\" must be number");
        return NULL;
    }
    int number=(int)a_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)(~number));
    return ret_addr;
}
nasal_val* builtin_sin(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("sin","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(sin(number));
    return ret_addr;
}
nasal_val* builtin_cos(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("cos","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(cos(number));
    return ret_addr;
}
nasal_val* builtin_tan(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("tan","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(tan(number));
    return ret_addr;
}
nasal_val* builtin_exp(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("exp","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(exp(number));
    return ret_addr;
}
nasal_val* builtin_ln(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("ln","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(log(number)/log(2.7182818284590452354));
    return ret_addr;
}
nasal_val* builtin_sqrt(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("sqrt","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(sqrt(number));
    return ret_addr;
}
nasal_val* builtin_atan2(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* x_value_addr=in_builtin_find("x");
    nasal_val* y_value_addr=in_builtin_find("y");
    if(x_value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("atan2","\"x\" must be number");
        return NULL;
    }
    if(y_value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("atan2","\"y\" must be number");
        return NULL;
    }
    double x=x_value_addr->get_number();
    double y=y_value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(atan2(y,x));
    return ret_addr;
}
nasal_val* builtin_time(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("begin_time");
    if(value_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("time","\"begin_time\" must be number");
        return NULL;
    }
    time_t begin_time=(time_t)value_addr->get_number();
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)time(&begin_time));
    return ret_addr;
}
nasal_val* builtin_contains(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* hash_addr=in_builtin_find("hash");
    nasal_val* key_addr=in_builtin_find("key");
    if(hash_addr->get_type()!=vm_hash)
    {
        builtin_error_occurred("contains","\"hash\" must be hash");
        return NULL;
    }
    if(key_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("contains","\"key\" must be string");
        return NULL;
    }
    bool contains=hash_addr->get_hash().check_contain(key_addr->get_string());
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number((double)contains);
    return ret_addr;
}
nasal_val* builtin_delete(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* hash_addr=in_builtin_find("hash");
    nasal_val* key_addr=in_builtin_find("key");
    if(hash_addr->get_type()!=vm_hash)
    {
        builtin_error_occurred("delete","\"hash\" must be hash");
        return NULL;
    }
    if(key_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("delete","\"key\" must be string");
        return NULL;
    }
    hash_addr->get_hash().del_elem(key_addr->get_string());
    nasal_val* ret_addr=gc.gc_alloc(vm_nil);
    return ret_addr;
}
nasal_val* builtin_getkeys(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* hash_addr=in_builtin_find("hash");
    if(hash_addr->get_type()!=vm_hash)
    {
        builtin_error_occurred("keys","\"hash\" must be hash");
        return NULL;
    }
    nasal_val* ret_addr=hash_addr->get_hash().get_keys();
    return ret_addr;
}
nasal_val* builtin_import(nasal_val* local_scope_addr,nasal_gc& gc)
{
    // this function is used in preprocessing.
    // this function will return nothing when running.
    builtin_error_occurred("import","cannot use import when running");
    nasal_val* ret_addr=gc.gc_alloc(vm_nil);
    return ret_addr;
}
nasal_val* builtin_die(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* str_addr=in_builtin_find("str");
    if(str_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("die","\"str\" must be string");
        return NULL;
    }
    builtin_die_state=true;
    std::cout<<">> [vm] error: "<<str_addr->get_string()<<'\n';
    nasal_val* ret_addr=gc.gc_alloc(vm_nil);
    return ret_addr;
}
nasal_val* builtin_type(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* value_addr=in_builtin_find("object");
    int type=value_addr->get_type();
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    switch(type)
    {
        case vm_nil:      ret_addr->set_string("nil");break;
        case vm_num:   ret_addr->set_string("number");break;
        case vm_str:   ret_addr->set_string("string");break;
        case vm_vec:   ret_addr->set_string("vector");break;
        case vm_hash:     ret_addr->set_string("hash");break;
        case vm_func: ret_addr->set_string("function");break;
    }
    return ret_addr;
}
nasal_val* builtin_substr(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* str_addr=in_builtin_find("str");
    nasal_val* begin_addr=in_builtin_find("begin");
    nasal_val* length_addr=in_builtin_find("length");
    if(str_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("substr","\"str\" must be string");
        return NULL;
    }
    if(begin_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("substr","\"begin\" must be number");
        return NULL;
    }
    if(length_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("substr","\"length\" must be number");
        return NULL;
    }
    std::string str=str_addr->get_string();
    int begin=(int)begin_addr->get_number();
    int len=(int)length_addr->get_number();
    if(begin+len>str.length())
    {
        builtin_error_occurred("susbtr","index out of range");
        return NULL;
    }
    std::string tmp="";
    for(int i=begin;i<begin+len;++i)
        tmp+=str[i];
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    ret_addr->set_string(tmp);
    return ret_addr;
}
nasal_val* builtin_streq(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    if(a_addr->get_type()!=vm_str || b_addr->get_type()!=vm_str)
    {
        ret_addr->set_number((double)0);
        return ret_addr;
    }
    int ret=(a_addr->get_string()==b_addr->get_string());
    ret_addr->set_number((double)ret);
    return ret_addr;
}
nasal_val* builtin_left(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* string_addr=in_builtin_find("string");
    nasal_val* length_addr=in_builtin_find("length");
    if(string_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("left","\"string\" must be string");
        return NULL;
    }
    if(length_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("left","\"length\" must be number");
        return NULL;
    }
    std::string str=string_addr->get_string();
    int len=(int)length_addr->get_number();
    if(len<0)
        len=0;
    std::string tmp=str.substr(0, len);
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    ret_addr->set_string(tmp);
    return ret_addr;
}
nasal_val* builtin_right(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* string_addr=in_builtin_find("string");
    nasal_val* length_addr=in_builtin_find("length");
    if(string_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("right","\"string\" must be string");
        return NULL;
    }
    if(length_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("right","\"length\" must be number");
        return NULL;
    } 
    std::string str=string_addr->get_string();
    int len=(int)length_addr->get_number();
    int srclen=str.length();
    if(len>srclen)
        len=srclen;
    if(len<0)
        len=0;
    std::string tmp=str.substr(srclen-len, srclen);
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    ret_addr->set_string(tmp);
    return ret_addr;
}
nasal_val* builtin_cmp(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* a_addr=in_builtin_find("a");
    nasal_val* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("cmp","\"a\" must be string");
        return NULL;
    }
    if(b_addr->get_type()!=vm_str)
    {
        builtin_error_occurred("cmp","\"b\" must be string");
        return NULL;
    }
    nasal_val* ret_addr=gc.gc_alloc(vm_num);
    ret_addr->set_number(strcmp(a_addr->get_string().data(),b_addr->get_string().data()));
    return ret_addr;
}
nasal_val* builtin_chr(nasal_val* local_scope_addr,nasal_gc& gc)
{
    nasal_val* code_addr=in_builtin_find("code");
    if(code_addr->get_type()!=vm_num)
    {
        builtin_error_occurred("chr","\"code\" must be number");
        return NULL;
    }
    char chr[2]={(char)code_addr->get_number(),'\0'};
    nasal_val* ret_addr=gc.gc_alloc(vm_str);
    ret_addr->set_string(chr);
    return ret_addr;
}
#endif