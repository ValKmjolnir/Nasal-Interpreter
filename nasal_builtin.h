#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__

#define in_builtin_find(value_name_string) (local_scope_addr>=0?nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address(value_name_string):-1)
#define in_builtin_check(value_addr,value_type) (nasal_vm.gc_get(value_addr).get_type()==(value_type))

int nasal_runtime::builtin_print(int local_scope_addr)
{
    // get arguments
    int vector_value_addr=in_builtin_find("elements");
    if(vector_value_addr<0 || !in_builtin_check(vector_value_addr,vm_vector))
    {
        std::cout<<">> [runtime] builtin_print: \"elements\" has wrong value type(must be vector).\n";
        ++error;
        return -1;
    }
    // main process
    nasal_vector& ref_vec=nasal_vm.gc_get(vector_value_addr).get_vector();
    int size=ref_vec.size();
    for(int i=0;i<size;++i)
    {
        nasal_scalar& tmp=nasal_vm.gc_get(ref_vec.get_value_address(i));
        switch(tmp.get_type())
        {
            case vm_nil:std::cout<<"nil";break;
            case vm_number:std::cout<<tmp.get_number();break;
            case vm_string:std::cout<<tmp.get_string();break;
            case vm_vector:std::cout<<"[...]";break;
            case vm_hash:std::cout<<"{...}";break;
            case vm_function:std::cout<<"func(...){...}";break;
            case vm_closure:std::cout<<"closure{{...}}";break;
        }
        if(i==size-1)
            std::cout<<'\n';
    }
    // generate return value
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}
int nasal_runtime::builtin_append(int local_scope_addr)
{
    int vector_value_addr=in_builtin_find("vector");
    int elem_value_addr=in_builtin_find("elements");
    if(vector_value_addr<0 || !in_builtin_check(vector_value_addr,vm_vector))
    {
        std::cout<<">> [runtime] builtin_append: \"vector\" has wrong value type(must be vector).\n";
        ++error;
        return -1;
    }
    if(elem_value_addr<0 || !in_builtin_check(elem_value_addr,vm_vector))
    {
        std::cout<<">> [runtime] builtin_append: \"elements\" has wrong value type(must be vector).\n";
        ++error;
        return -1;
    }
    nasal_vector& ref_vector=nasal_vm.gc_get(vector_value_addr).get_vector();
    nasal_vector& ref_elements=nasal_vm.gc_get(elem_value_addr).get_vector();
    int size=ref_elements.size();
    for(int i=0;i<size;++i)
    {
        int value_address=ref_elements.get_value_address(i);
        nasal_vm.add_reference(value_address);
        ref_vector.add_elem(value_address);
    }
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}
int nasal_runtime::builtin_setsize(int local_scope_addr)
{
    int vector_value_addr=in_builtin_find("vector");
    int size_value_addr=in_builtin_find("size");
    if(vector_value_addr<0 || nasal_vm.gc_get(vector_value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_setsize: \"vector\" has wrong value type(must be vector).\n";
        ++error;
        return -1;
    }
    if(size_value_addr<0)
    {
        std::cout<<">> [runtime] builtin_setsize: \"size\" has wrong value type(must be string or number).\n";
        ++error;
        return -1;
    }
    int type=nasal_vm.gc_get(size_value_addr).get_type();
    if(type!=vm_number && type!=vm_string)
    {
        std::cout<<">> [runtime] builtin_setsize: size is not a number.\n";
        ++error;
        return -1;
    }
    int number;
    if(type==vm_number)
        number=(int)nasal_vm.gc_get(size_value_addr).get_number();
    else
    {
        std::string str=nasal_vm.gc_get(size_value_addr).get_string();
        double tmp=trans_string_to_number(str);
        if(std::isnan(tmp))
        {
            std::cout<<">> [runtime] builtin_setsize: size is not a numerable string.\n";
            ++error;
            return -1;
        }
        number=(int)tmp;
    }
    if(number<0)
    {
        std::cout<<">> [runtime] builtin_setsize: size must be greater than -1.\n";
        ++error;
        return -1;
    }
    nasal_vector& ref_vector=nasal_vm.gc_get(vector_value_addr).get_vector();
    int vec_size=ref_vector.size();
    if(number<vec_size)
        for(int i=number;i<vec_size;++i)
        {
            int addr=ref_vector.del_elem();
            if(addr>=0)
                nasal_vm.del_reference(addr);
        }
    else if(number>vec_size)
        for(int i=vec_size;i<number;++i)
        {
            int new_val_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(new_val_addr).set_type(vm_nil);
            ref_vector.add_elem(new_val_addr);
        }
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}

int nasal_runtime::builtin_system(int local_scope_addr)
{
    int str_value_addr=in_builtin_find("str");
    if(str_value_addr<0 || nasal_vm.gc_get(str_value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_system: \"str\" has wrong value type(must be string).\n";
        ++error;
        return -1;
    }
    std::string str=nasal_vm.gc_get(str_value_addr).get_string();
    int size=str.length();
    char* command=new char[size+1];
    for(int i=0;i<size;++i)
        command[i]=str[i];
    command[size]='\0';
    system(command);
    delete []command;
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}

int nasal_runtime::builtin_input(int local_scope_addr)
{
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_string);
    std::string str;
    std::cin>>str;
    nasal_vm.gc_get(ret_addr).set_string(str);
    return ret_addr;
}

int nasal_runtime::builtin_sleep(int local_scope_addr)
{
    int value_addr=in_builtin_find("duration");
    if(value_addr<0 || (nasal_vm.gc_get(value_addr).get_type()!=vm_string && nasal_vm.gc_get(value_addr).get_type()!=vm_number))
    {
        std::cout<<">> [runtime] builtin_sleep: \"duration\" has wrong value type(must be string or number).\n";
        ++error;
        return -1;
    }
    unsigned long sleep_time=0;
    if(nasal_vm.gc_get(value_addr).get_type()==vm_string)
    {
        std::string str=nasal_vm.gc_get(value_addr).get_string();
        double number=trans_string_to_number(str);
        if(std::isnan(number))
        {
            std::cout<<">> [runtime] builtin_sleep: this is not a numerable string.\n";
            ++error;
            return -1;
        }sleep_time=(unsigned long)number;
    }
    else
        sleep_time=(unsigned long)nasal_vm.gc_get(value_addr).get_number();
    sleep(sleep_time); // sleep in unistd.h will make this progress sleep sleep_time seconds.
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}

int nasal_runtime::builtin_finput(int local_scope_addr)
{
    int value_addr=in_builtin_find("filename");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_finput: \"filename\" has wrong value type(must be string).\n";
        ++error;
        return -1;
    }
    std::string filename=nasal_vm.gc_get(value_addr).get_string();
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
        file_content="";
    fin.close();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_string);
    nasal_vm.gc_get(ret_addr).set_string(file_content);
    return ret_addr;
}

int nasal_runtime::builtin_foutput(int local_scope_addr)
{
    int value_addr=in_builtin_find("filename");
    int str_value_addr=in_builtin_find("str");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_foutput: \"filename\" has wrong value type(must be string).\n";
        ++error;
        return -1;
    }
    if(str_value_addr<0 || nasal_vm.gc_get(str_value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_foutput: \"str\" has wrong value type(must be string).\n";
        ++error;
        return -1;
    }
    std::string filename=nasal_vm.gc_get(value_addr).get_string();
    std::string file_content=nasal_vm.gc_get(str_value_addr).get_string();
    std::ofstream fout(filename);
    fout<<file_content;
    fout.close();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}

int nasal_runtime::builtin_split(int local_scope_addr)
{
    int delimeter_value_addr=in_builtin_find("delimeter");
    int string_value_addr=in_builtin_find("string");
    if(delimeter_value_addr<0 || nasal_vm.gc_get(delimeter_value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_split: \"delimeter\" has wrong value type(must be string).\n";
        ++error;
        return -1;
    }
    if(string_value_addr<0 || nasal_vm.gc_get(string_value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_split: \"string\" has wrong value type(must be string).\n";
        ++error;
        return -1;
    }
    std::string delimeter=nasal_vm.gc_get(delimeter_value_addr).get_string();
    std::string source=nasal_vm.gc_get(string_value_addr).get_string();
    int delimeter_len=delimeter.length();
    int source_len=source.length();

    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_vector);
    nasal_vector& ref_vec=nasal_vm.gc_get(ret_addr).get_vector();
    std::string tmp="";

    if(!delimeter_len)
    {
        for(int i=0;i<source_len;++i)
        {
            tmp+=source[i];
            int str_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(str_addr).set_type(vm_string);
            nasal_vm.gc_get(str_addr).set_string(tmp);
            ref_vec.add_elem(str_addr);
            tmp="";
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
            int str_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(str_addr).set_type(vm_string);
            nasal_vm.gc_get(str_addr).set_string(tmp);
            ref_vec.add_elem(str_addr);
            tmp="";
            i+=delimeter_len-1;
        }
        else
            tmp+=source[i];
    }
    if(tmp.length())
    {
        int str_addr=nasal_vm.gc_alloc();
        nasal_vm.gc_get(str_addr).set_type(vm_string);
        nasal_vm.gc_get(str_addr).set_string(tmp);
        ref_vec.add_elem(str_addr);
        tmp="";
    }
    return ret_addr;
}
int nasal_runtime::builtin_rand(int local_scope_addr)
{
    int value_addr=in_builtin_find("seed");
    if(value_addr<0 || (nasal_vm.gc_get(value_addr).get_type()!=vm_number && nasal_vm.gc_get(value_addr).get_type()!=vm_nil))
    {
        std::cout<<">> [runtime] builtin_rand: \"seed\" has wrong value type(must be nil or number).\n";
        ++error;
        return -1;
    }
    if(nasal_vm.gc_get(value_addr).get_type()==vm_number)
    {
        unsigned int number=(unsigned int)nasal_vm.gc_get(value_addr).get_number();
        srand(number);
        int ret_addr=nasal_vm.gc_alloc();
        nasal_vm.gc_get(ret_addr).set_type(vm_nil);
        return ret_addr;
    }
    double num=0;
    for(int i=0;i<5;++i)
        num=(num+rand())*(1.0/(RAND_MAX+1.0));
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(num);
    return ret_addr;
}
int nasal_runtime::builtin_id(int local_scope_addr)
{
    int value_addr=in_builtin_find("thing");
    if(value_addr<0)
    {
        std::cout<<">> [runtime] builtin_id: cannot find \"thing\".\n";
        ++error;
        return -1;
    }
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)value_addr);
    return ret_addr;
}
int nasal_runtime::builtin_int(int local_scope_addr)
{
    int value_addr=in_builtin_find("value");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_int: \"value\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    int number=(int)nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)number);
    return ret_addr;
}
int nasal_runtime::builtin_num(int local_scope_addr)
{
    int value_addr=in_builtin_find("value");
    if(value_addr<0 || !in_builtin_check(value_addr,vm_string))
    {
        std::cout<<">> [runtime] builtin_num: \"value\" has wrong value type(must be string).\n";
        ++error;
        return -1;
    }
    std::string str=nasal_vm.gc_get(value_addr).get_string();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(trans_string_to_number(str));
    return ret_addr;
}
int nasal_runtime::builtin_pop(int local_scope_addr)
{
    int value_addr=in_builtin_find("vector");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_pop: \"vector\" has wrong value type(must be vector).\n";
        ++error;
        return -1;
    }
    int ret_addr=nasal_vm.gc_get(value_addr).get_vector().del_elem();
    return ret_addr;
}
int nasal_runtime::builtin_str(int local_scope_addr)
{
    int value_addr=in_builtin_find("number");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_str: \"number\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    double number=nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_string);
    nasal_vm.gc_get(ret_addr).set_string(trans_number_to_string(number));
    return ret_addr;
}
int nasal_runtime::builtin_size(int local_scope_addr)
{
    int value_addr=in_builtin_find("object");
    if(value_addr<0)
    {
        std::cout<<">> [runtime] builtin_size: cannot find value \"object\".\n";
        ++error;
        return -1;
    }
    int type=nasal_vm.gc_get(value_addr).get_type();
    int number=-1;
    switch(type)
    {
        case vm_nil:
        case vm_number:
        case vm_function:
        case vm_closure:break;
        case vm_string:number=nasal_vm.gc_get(value_addr).get_string().length();break;
        case vm_vector:number=nasal_vm.gc_get(value_addr).get_vector().size();break;
        case vm_hash:number=nasal_vm.gc_get(value_addr).get_hash().size();break;
    }
    int ret_addr=nasal_vm.gc_alloc();
    if(number<0)
        nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    else
    {
        nasal_vm.gc_get(ret_addr).set_type(vm_number);
        nasal_vm.gc_get(ret_addr).set_number((double)number);
    }
    return ret_addr;
}
int nasal_runtime::builtin_xor(int local_scope_addr)
{
    int a_addr=in_builtin_find("a");
    int b_addr=in_builtin_find("b");
    if(a_addr<0 || nasal_vm.gc_get(a_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_xor: \"a\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    if(b_addr<0 || nasal_vm.gc_get(b_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_xor: \"b\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    int number_a=(int)nasal_vm.gc_get(a_addr).get_number();
    int number_b=(int)nasal_vm.gc_get(b_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)(number_a^number_b));
    return ret_addr;
}
int nasal_runtime::builtin_and(int local_scope_addr)
{
    int a_addr=in_builtin_find("a");
    int b_addr=in_builtin_find("b");
    if(a_addr<0 || nasal_vm.gc_get(a_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_and: \"a\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    if(b_addr<0 || nasal_vm.gc_get(b_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_and: \"b\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    int number_a=(int)nasal_vm.gc_get(a_addr).get_number();
    int number_b=(int)nasal_vm.gc_get(b_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)(number_a&number_b));
    return ret_addr;
}
int nasal_runtime::builtin_or(int local_scope_addr)
{
    int a_addr=in_builtin_find("a");
    int b_addr=in_builtin_find("b");
    if(a_addr<0 || nasal_vm.gc_get(a_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_or: \"a\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    if(b_addr<0 || nasal_vm.gc_get(b_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_or: \"b\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    int number_a=(int)nasal_vm.gc_get(a_addr).get_number();
    int number_b=(int)nasal_vm.gc_get(b_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)(number_a|number_b));
    return ret_addr;
}
int nasal_runtime::builtin_nand(int local_scope_addr)
{
    int a_addr=in_builtin_find("a");
    int b_addr=in_builtin_find("b");
    if(a_addr<0 || nasal_vm.gc_get(a_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_nand: \"a\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    if(b_addr<0 || nasal_vm.gc_get(b_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_nand: \"b\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    int number_a=(int)nasal_vm.gc_get(a_addr).get_number();
    int number_b=(int)nasal_vm.gc_get(b_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)(~(number_a&number_b)));
    return ret_addr;
}
int nasal_runtime::builtin_not(int local_scope_addr)
{
    int a_addr=in_builtin_find("a");
    if(a_addr<0 || nasal_vm.gc_get(a_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_not: \"a\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    int number=(int)nasal_vm.gc_get(a_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)(~number));
    return ret_addr;
}
int nasal_runtime::builtin_sin(int local_scope_addr)
{
    int value_addr=in_builtin_find("x");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_sin: \"x\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    double number=nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(sin(number));
    return ret_addr;
}
int nasal_runtime::builtin_cos(int local_scope_addr)
{
    int value_addr=in_builtin_find("x");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_cos: \"x\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    double number=nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(cos(number));
    return ret_addr;
}
int nasal_runtime::builtin_tan(int local_scope_addr)
{
    int value_addr=in_builtin_find("x");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_tan: \"x\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    double number=nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(tan(number));
    return ret_addr;
}
int nasal_runtime::builtin_exp(int local_scope_addr)
{
    int value_addr=in_builtin_find("x");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_exp: \"x\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    double number=nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(exp(number));
    return ret_addr;
}
int nasal_runtime::builtin_ln(int local_scope_addr)
{
    int value_addr=in_builtin_find("x");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_ln: \"x\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    double number=nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(log(number)/log(2.7182818284590452354));
    return ret_addr;
}
int nasal_runtime::builtin_sqrt(int local_scope_addr)
{
    int value_addr=in_builtin_find("x");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_sqrt: \"x\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    double number=nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(sqrt(number));
    return ret_addr;
}
int nasal_runtime::builtin_atan2(int local_scope_addr)
{
    int x_value_addr=in_builtin_find("x");
    int y_value_addr=in_builtin_find("y");
    if(x_value_addr<0 || nasal_vm.gc_get(x_value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_atan2: \"x\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    if(y_value_addr<0 || nasal_vm.gc_get(y_value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_atan2: \"y\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    double x=nasal_vm.gc_get(x_value_addr).get_number();
    double y=nasal_vm.gc_get(y_value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(atan2(y,x));
    return ret_addr;
}
int nasal_runtime::builtin_time(int local_scope_addr)
{
    int value_addr=in_builtin_find("begin_time");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_time: \"begin_time\" has wrong value type(must be number).\n";
        ++error;
        return -1;
    }
    time_t begin_time=(time_t)nasal_vm.gc_get(value_addr).get_number();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)time(&begin_time));
    return ret_addr;
}
int nasal_runtime::builtin_contains(int local_scope_addr)
{
    int hash_addr=in_builtin_find("hash");
    int key_addr=in_builtin_find("key");
    if(hash_addr<0 || !in_builtin_check(hash_addr,vm_hash))
    {
        std::cout<<">> [runtime] builtin_contains: \"hash\" has wrong type(must be hash).\n";
        ++error;
        return -1;
    }
    if(key_addr<0 || !in_builtin_check(key_addr,vm_string))
    {
        std::cout<<">> [runtime] builtin_contains: \"key\" has wrong type(must be string).\n";
        ++error;
        return -1;
    }
    std::string key=nasal_vm.gc_get(key_addr).get_string();
    bool contains=nasal_vm.gc_get(hash_addr).get_hash().check_contain(key);
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number((double)contains);
    return ret_addr;
}
int nasal_runtime::builtin_delete(int local_scope_addr)
{
    int hash_addr=in_builtin_find("hash");
    int key_addr=in_builtin_find("key");
    if(hash_addr<0 || !in_builtin_check(hash_addr,vm_hash))
    {
        std::cout<<">> [runtime] builtin_delete: \"hash\" has wrong type(must be hash).\n";
        ++error;
        return -1;
    }
    if(key_addr<0 || !in_builtin_check(key_addr,vm_string))
    {
        std::cout<<">> [runtime] builtin_delete: \"key\" has wrong type(must be string).\n";
        ++error;
        return -1;
    }
    std::string key=nasal_vm.gc_get(key_addr).get_string();
    nasal_vm.gc_get(hash_addr).get_hash().del_elem(key);
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}
int nasal_runtime::builtin_getkeys(int local_scope_addr)
{
    int hash_addr=in_builtin_find("hash");
    if(hash_addr<0 || !in_builtin_check(hash_addr,vm_hash))
    {
        std::cout<<">> [runtime] builtin_delete: \"hash\" has wrong type(must be hash).\n";
        ++error;
        return -1;
    }
    int ret_addr=nasal_vm.gc_get(hash_addr).get_hash().get_keys();
    return ret_addr;
}
int nasal_runtime::builtin_import(int local_scope_addr)
{
    // this function is used in preprocessing.
    // this function will return nothing when running.
    ++error;
    std::cout<<">> [runtime] builtin_import: cannot use import when running.\n";
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}
int nasal_runtime::builtin_die(int local_scope_addr)
{
    int str_addr=in_builtin_find("str");
    if(str_addr<0 || !in_builtin_check(str_addr,vm_string))
    {
        std::cout<<">> [runtime] builtin_die: \"str\" has wrong type(must be string).\n";
        ++error;
        return -1;
    }
    ++error;
    std::cout<<">> [runtime] error: "<<nasal_vm.gc_get(str_addr).get_string()<<'\n';
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}
int nasal_runtime::builtin_type(int local_scope_addr)
{
    int value_addr=in_builtin_find("object");
    if(value_addr<0)
    {
        std::cout<<">> [runtime] builtin_type: cannot find \"object\".\n";
        ++error;
        return -1;
    }
    int type=nasal_vm.gc_get(value_addr).get_type();
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_string);
    switch(type)
    {
        case vm_nil:      nasal_vm.gc_get(ret_addr).set_string("nil");break;
        case vm_number:   nasal_vm.gc_get(ret_addr).set_string("number");break;
        case vm_string:   nasal_vm.gc_get(ret_addr).set_string("string");break;
        case vm_vector:   nasal_vm.gc_get(ret_addr).set_string("vector");break;
        case vm_hash:     nasal_vm.gc_get(ret_addr).set_string("hash");break;
        case vm_function: nasal_vm.gc_get(ret_addr).set_string("function");break;
    }
    return ret_addr;
}
int nasal_runtime::builtin_substr(int local_scope_addr)
{
    int str_addr=in_builtin_find("str");
    int begin_addr=in_builtin_find("begin");
    int length_addr=in_builtin_find("length");
    if(str_addr<0 || !in_builtin_check(str_addr,vm_string))
    {
        std::cout<<">> [runtime] builtin_substr: cannot find \"str\" or wrong type(must be string).\n";
        ++error;
        return -1;
    }
    if(begin_addr<0 || !in_builtin_check(begin_addr,vm_number))
    {
        std::cout<<">> [runtime] builtin_substr: cannot find \"begin\" or wrong type(must be number).\n";
        ++error;
        return -1;
    }
    if(length_addr<0 || !in_builtin_check(length_addr,vm_number))
    {
        std::cout<<">> [runtime] builtin_substr: cannot find \"length\" or wrong type(must be number).\n";
        ++error;
        return -1;
    }
    std::string str=nasal_vm.gc_get(str_addr).get_string();
    int begin=(int)nasal_vm.gc_get(begin_addr).get_number();
    int len=(int)nasal_vm.gc_get(length_addr).get_number();
    if(begin>=str.length() || begin+len>=str.length())
    {
        std::cout<<">> [runtime] builtin_substr: index out of range.\n";
        ++error;
        return -1;
    }
    std::string tmp="";
    for(int i=begin;i<begin+len;++i)
        tmp+=str[i];
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_string);
    nasal_vm.gc_get(ret_addr).set_string(tmp);
    return ret_addr;
}
#endif