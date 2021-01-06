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
#define in_builtin_find(value_name_string) (local_scope_addr->get_closure().get_value_address(builtin_use_string_table[value_name_string]))

// declaration of builtin functions
// to add new builtin function,declare it here and write the definition below

nasal_scalar* builtin_print(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_append(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_setsize(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_system(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_input(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_sleep(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_finput(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_foutput(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_split(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_rand(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_id(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_int(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_num(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_pop(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_str(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_size(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_xor(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_and(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_or(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_nand(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_not(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_sin(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_cos(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_tan(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_exp(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_ln(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_sqrt(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_atan2(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_time(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_contains(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_delete(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_getkeys(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_import(nasal_scalar*,nasal_virtual_machine&);
bool          builtin_die_state;// used in builtin_die
nasal_scalar* builtin_die(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_type(nasal_scalar*,nasal_virtual_machine&);
nasal_scalar* builtin_substr(nasal_scalar*,nasal_virtual_machine&);

void builtin_error_occurred(std::string func_name,std::string info)
{
    builtin_die_state=true;
    std::cout<<">> [vm] "<<func_name<<": "<<info<<".\n";
    return;
}

// register builtin function's name and it's address here in this table below
// this table must and with {"",NULL}
struct FUNC_TABLE
{
    std::string func_name;
    nasal_scalar* (*func_pointer)(nasal_scalar* x,nasal_virtual_machine& nasal_vm);
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
    {"",                                 NULL}
};

nasal_scalar* builtin_print(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    // get arguments
    nasal_scalar* vector_value_addr=in_builtin_find("elements");
    // main process
    nasal_vector& ref_vec=vector_value_addr->get_vector();
    int size=ref_vec.size();
    for(int i=0;i<size;++i)
    {
        nasal_scalar* tmp=ref_vec[i];
        switch(tmp->get_type())
        {
            case vm_nil:      std::cout<<"nil";             break;
            case vm_number:   std::cout<<tmp->get_number(); break;
            case vm_string:   std::cout<<tmp->get_string(); break;
            case vm_vector:   tmp->get_vector().print();    break;
            case vm_hash:     tmp->get_hash().print();      break;
            case vm_function: std::cout<<"func(...){...}";  break;
        }
    }
    std::cout<<"\n";
    // generate return value
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}
nasal_scalar* builtin_append(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* vector_value_addr=in_builtin_find("vector");
    nasal_scalar* elem_value_addr=in_builtin_find("elements");
    if(vector_value_addr->get_type()!=vm_vector)
    {
        builtin_error_occurred("append","\"vector\" must be vector");
        return NULL;
    }
    nasal_vector& ref_vector=vector_value_addr->get_vector();
    nasal_vector& ref_elements=elem_value_addr->get_vector();
    int size=ref_elements.size();
    for(int i=0;i<size;++i)
    {
        nasal_scalar* value_address=ref_elements.get_value_address(i);
        nasal_vm.add_reference(value_address);
        ref_vector.add_elem(value_address);
    }
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}
nasal_scalar* builtin_setsize(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* vector_value_addr=in_builtin_find("vector");
    nasal_scalar* size_value_addr=in_builtin_find("size");
    if(vector_value_addr->get_type()!=vm_vector)
    {
        builtin_error_occurred("setsize","\"vector\" must be vector");
        return NULL;
    }
    int type=size_value_addr->get_type();
    if(type!=vm_number)
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
    nasal_vector& ref_vector=vector_value_addr->get_vector();
    int vec_size=ref_vector.size();
    if(number<vec_size)
        for(int i=number;i<vec_size;++i)
        {
            nasal_scalar* addr=ref_vector.del_elem();
            if(addr) nasal_vm.del_reference(addr);
        }
    else if(number>vec_size)
        for(int i=vec_size;i<number;++i)
        {
            nasal_scalar* new_val_addr=nasal_vm.gc_alloc(vm_nil);
            ref_vector.add_elem(new_val_addr);
        }
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}

nasal_scalar* builtin_system(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* str_value_addr=in_builtin_find("str");
    if(str_value_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("system","\"str\" must be string");
        return NULL;
    }
    std::string str=str_value_addr->get_string();
    system(str.data());
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}

nasal_scalar* builtin_input(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_string);
    std::string str;
    std::cin>>str;
    ret_addr->set_string(str);
    return ret_addr;
}

nasal_scalar* builtin_sleep(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("duration");
    int type=value_addr->get_type();
    if(type!=vm_number)
    {
        builtin_error_occurred("sleep","\"duration\" must be number");
        return NULL;
    }
    unsigned long sleep_time=0;
    sleep_time=(unsigned long)value_addr->get_number();
    sleep(sleep_time); // sleep in unistd.h will make this progress sleep sleep_time seconds.
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}

nasal_scalar* builtin_finput(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("filename");
    if(value_addr->get_type()!=vm_string)
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
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_string);
    ret_addr->set_string(file_content);
    return ret_addr;
}

nasal_scalar* builtin_foutput(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("filename");
    nasal_scalar* str_value_addr=in_builtin_find("str");
    if(value_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("io.fout","\"filename\" must be string");
        return NULL;
    }
    if(str_value_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("io.fout","\"str\" must be string");
        return NULL;
    }
    std::string filename=value_addr->get_string();
    std::string file_content=str_value_addr->get_string();
    std::ofstream fout(filename);
    fout<<file_content;
    fout.close();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}

nasal_scalar* builtin_split(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* delimeter_value_addr=in_builtin_find("delimeter");
    nasal_scalar* string_value_addr=in_builtin_find("string");
    if(delimeter_value_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("split","\"delimeter\" must be string");
        return NULL;
    }
    if(string_value_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("split","\"string\" must be string");
        return NULL;
    }
    std::string delimeter=delimeter_value_addr->get_string();
    std::string source=string_value_addr->get_string();
    int delimeter_len=delimeter.length();
    int source_len=source.length();

    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_vector);
    nasal_vector& ref_vec=ret_addr->get_vector();
    std::string tmp="";

    if(!delimeter_len)
    {
        for(int i=0;i<source_len;++i)
        {
            tmp=source[i];
            nasal_scalar* str_addr=nasal_vm.gc_alloc(vm_string);
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
            nasal_scalar* str_addr=nasal_vm.gc_alloc(vm_string);
            str_addr->set_string(tmp);
            ref_vec.add_elem(str_addr);
            tmp="";
            i+=delimeter_len-1;
        }
        else
            tmp+=source[i];
    }
    if(tmp.length())
    {
        nasal_scalar* str_addr=nasal_vm.gc_alloc(vm_string);
        str_addr->set_string(tmp);
        ref_vec.add_elem(str_addr);
        tmp="";
    }
    return ret_addr;
}
nasal_scalar* builtin_rand(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("seed");
    if(value_addr->get_type()!=vm_number && value_addr->get_type()!=vm_nil)
    {
        builtin_error_occurred("rand","\"seed\" must be nil or number");
        return NULL;
    }
    if(value_addr->get_type()==vm_number)
    {
        unsigned int number=(unsigned int)value_addr->get_number();
        srand(number);
        nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
        return ret_addr;
    }
    double num=0;
    for(int i=0;i<5;++i)
        num=(num+rand())*(1.0/(RAND_MAX+1.0));
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(num);
    return ret_addr;
}
nasal_scalar* builtin_id(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("thing");
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_string);
    char buf[32];
    sprintf(buf,"0x%p",value_addr);
    ret_addr->set_string(buf);
    return ret_addr;
}
nasal_scalar* builtin_int(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("value");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("int","\"value\" must be number");
        return NULL;
    }
    int number=(int)value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)number);
    return ret_addr;
}
nasal_scalar* builtin_num(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("value");
    if(value_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("num","\"value\" must be string");
        return NULL;
    }
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(value_addr->to_number());
    return ret_addr;
}
nasal_scalar* builtin_pop(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("vector");
    if(value_addr->get_type()!=vm_vector)
    {
        builtin_error_occurred("pop","\"vector\" must be vector");
        return NULL;
    }
    nasal_scalar* ret_addr=value_addr->get_vector().del_elem();
    return ret_addr;
}
nasal_scalar* builtin_str(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("number");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("str","\"number\" must be number");
        return NULL;
    }
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_string(value_addr->to_string());
    return ret_addr;
}
nasal_scalar* builtin_size(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("object");
    int type=value_addr->get_type();
    int number=-1;
    switch(type)
    {
        case vm_nil:
        case vm_number:
        case vm_function: 
        case vm_closure: return nasal_vm.gc_alloc(vm_nil);         break;
        case vm_string:  number=value_addr->get_string().length(); break;
        case vm_vector:  number=value_addr->get_vector().size();   break;
        case vm_hash:    number=value_addr->get_hash().size();     break;
    }
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)number);
    return ret_addr;
}
nasal_scalar* builtin_xor(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* a_addr=in_builtin_find("a");
    nasal_scalar* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("xor","\"a\" must be number");
        return NULL;
    }
    if(b_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("xor","\"b\" must be number");
        return NULL;
    }
    int number_a=(int)a_addr->get_number();
    int number_b=(int)b_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)(number_a^number_b));
    return ret_addr;
}
nasal_scalar* builtin_and(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* a_addr=in_builtin_find("a");
    nasal_scalar* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("and","\"a\" must be number");
        return NULL;
    }
    if(b_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("and","\"b\" must be number");
        return NULL;
    }
    int number_a=(int)a_addr->get_number();
    int number_b=(int)b_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)(number_a&number_b));
    return ret_addr;
}
nasal_scalar* builtin_or(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* a_addr=in_builtin_find("a");
    nasal_scalar* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("or","\"a\" must be number");
        return NULL;
    }
    if(b_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("or","\"b\" must be number");
        return NULL;
    }
    int number_a=(int)a_addr->get_number();
    int number_b=(int)b_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)(number_a|number_b));
    return ret_addr;
}
nasal_scalar* builtin_nand(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* a_addr=in_builtin_find("a");
    nasal_scalar* b_addr=in_builtin_find("b");
    if(a_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("nand","\"a\" must be number");
        return NULL;
    }
    if(b_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("nand","\"b\" must be number");
        return NULL;
    }
    int number_a=(int)a_addr->get_number();
    int number_b=(int)b_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)(~(number_a&number_b)));
    return ret_addr;
}
nasal_scalar* builtin_not(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* a_addr=in_builtin_find("a");
    if(a_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("not","\"a\" must be number");
        return NULL;
    }
    int number=(int)a_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)(~number));
    return ret_addr;
}
nasal_scalar* builtin_sin(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("sin","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(sin(number));
    return ret_addr;
}
nasal_scalar* builtin_cos(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("cos","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(cos(number));
    return ret_addr;
}
nasal_scalar* builtin_tan(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("tan","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(tan(number));
    return ret_addr;
}
nasal_scalar* builtin_exp(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("exp","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(exp(number));
    return ret_addr;
}
nasal_scalar* builtin_ln(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("ln","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(log(number)/log(2.7182818284590452354));
    return ret_addr;
}
nasal_scalar* builtin_sqrt(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("x");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("sqrt","\"x\" must be number");
        return NULL;
    }
    double number=value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(sqrt(number));
    return ret_addr;
}
nasal_scalar* builtin_atan2(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* x_value_addr=in_builtin_find("x");
    nasal_scalar* y_value_addr=in_builtin_find("y");
    if(x_value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("atan2","\"x\" must be number");
        return NULL;
    }
    if(y_value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("atan2","\"y\" must be number");
        return NULL;
    }
    double x=x_value_addr->get_number();
    double y=y_value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number(atan2(y,x));
    return ret_addr;
}
nasal_scalar* builtin_time(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("begin_time");
    if(value_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("time","\"begin_time\" must be number");
        return NULL;
    }
    time_t begin_time=(time_t)value_addr->get_number();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)time(&begin_time));
    return ret_addr;
}
nasal_scalar* builtin_contains(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* hash_addr=in_builtin_find("hash");
    nasal_scalar* key_addr=in_builtin_find("key");
    if(hash_addr->get_type()!=vm_hash)
    {
        builtin_error_occurred("contains","\"hash\" must be hash");
        return NULL;
    }
    if(key_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("contains","\"key\" must be string");
        return NULL;
    }
    bool contains=hash_addr->get_hash().check_contain(key_addr->get_string());
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_number);
    ret_addr->set_number((double)contains);
    return ret_addr;
}
nasal_scalar* builtin_delete(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* hash_addr=in_builtin_find("hash");
    nasal_scalar* key_addr=in_builtin_find("key");
    if(hash_addr->get_type()!=vm_hash)
    {
        builtin_error_occurred("delete","\"hash\" must be hash");
        return NULL;
    }
    if(key_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("delete","\"key\" must be string");
        return NULL;
    }
    hash_addr->get_hash().del_elem(key_addr->get_string());
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}
nasal_scalar* builtin_getkeys(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* hash_addr=in_builtin_find("hash");
    if(hash_addr->get_type()!=vm_hash)
    {
        builtin_error_occurred("keys","\"hash\" must be hash");
        return NULL;
    }
    nasal_scalar* ret_addr=hash_addr->get_hash().get_keys();
    return ret_addr;
}
nasal_scalar* builtin_import(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    // this function is used in preprocessing.
    // this function will return nothing when running.
    builtin_error_occurred("import","cannot use import when running");
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}
nasal_scalar* builtin_die(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* str_addr=in_builtin_find("str");
    if(str_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("die","\"str\" must be string");
        return NULL;
    }
    builtin_die_state=true;
    std::cout<<">> [vm] error: "<<str_addr->get_string()<<'\n';
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_addr;
}
nasal_scalar* builtin_type(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* value_addr=in_builtin_find("object");
    int type=value_addr->get_type();
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_string);
    switch(type)
    {
        case vm_nil:      ret_addr->set_string("nil");break;
        case vm_number:   ret_addr->set_string("number");break;
        case vm_string:   ret_addr->set_string("string");break;
        case vm_vector:   ret_addr->set_string("vector");break;
        case vm_hash:     ret_addr->set_string("hash");break;
        case vm_function: ret_addr->set_string("function");break;
    }
    return ret_addr;
}
nasal_scalar* builtin_substr(nasal_scalar* local_scope_addr,nasal_virtual_machine& nasal_vm)
{
    nasal_scalar* str_addr=in_builtin_find("str");
    nasal_scalar* begin_addr=in_builtin_find("begin");
    nasal_scalar* length_addr=in_builtin_find("length");
    if(str_addr->get_type()!=vm_string)
    {
        builtin_error_occurred("substr","\"str\" must be string");
        return NULL;
    }
    if(begin_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("substr","\"begin\" must be number");
        return NULL;
    }
    if(length_addr->get_type()!=vm_number)
    {
        builtin_error_occurred("substr","\"length\" must be number");
        return NULL;
    }
    std::string str=str_addr->get_string();
    int begin=(int)begin_addr->get_number();
    int len=(int)length_addr->get_number();
    if(begin+len>=str.length())
    {
        builtin_error_occurred("susbtr","index out of range");
        return NULL;
    }
    std::string tmp="";
    for(int i=begin;i<begin+len;++i)
        tmp+=str[i];
    nasal_scalar* ret_addr=nasal_vm.gc_alloc(vm_string);
    ret_addr->set_string(tmp);
    return ret_addr;
}
#endif