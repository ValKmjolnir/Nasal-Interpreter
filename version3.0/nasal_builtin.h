#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__

#define in_builtin_find(value_name_string) (local_scope_addr>=0?nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address(value_name_string):-1)

int nasal_runtime::builtin_print(int local_scope_addr)
{
    // get arguments
    int vector_value_addr=in_builtin_find("elements");
    if(vector_value_addr<0 || nasal_vm.gc_get(vector_value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_print: cannot find values or wrong value type."<<std::endl;
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
            std::cout<<std::endl;
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
    if(vector_value_addr<0 || nasal_vm.gc_get(vector_value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_append: cannot find values or wrong value type."<<std::endl;
        ++error;
        return -1;
    }
    if(elem_value_addr<0 || nasal_vm.gc_get(elem_value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_append: cannot find values or wrong value type."<<std::endl;
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
        std::cout<<">> [runtime] builtin_setsize: cannot find values or wrong value type."<<std::endl;
        ++error;
        return -1;
    }
    if(size_value_addr<0)
    {
        std::cout<<">> [runtime] builtin_setsize: cannot find values or wrong value type."<<std::endl;
        ++error;
        return -1;
    }
    int type=nasal_vm.gc_get(size_value_addr).get_type();
    if(type!=vm_number && type!=vm_string)
    {
        std::cout<<">> [runtime] builtin_setsize: size is not a number."<<std::endl;
        ++error;
        return -1;
    }
    int number;
    if(type==vm_number)
        number=(int)nasal_vm.gc_get(size_value_addr).get_number();
    else
    {
        std::string str=nasal_vm.gc_get(size_value_addr).get_string();
        if(check_numerable_string(str))
            number=(int)trans_string_to_number(str);
        else
        {
            std::cout<<">> [runtime] builtin_setsize: size is not a numerable string."<<std::endl;
            ++error;
            return -1;
        }
    }
    if(number<0)
    {
        std::cout<<">> [runtime] builtin_setsize: size must be greater than -1."<<std::endl;
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
        std::cout<<">> [runtime] builtin_system: cannot find values or wrong value type(must be string)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_sleep: cannot find values or wrong value type(must be string or numebr)."<<std::endl;
        ++error;
        return -1;
    }
    unsigned long sleep_time=0;
    if(nasal_vm.gc_get(value_addr).get_type()==vm_string)
    {
        std::string str=nasal_vm.gc_get(value_addr).get_string();
        if(check_numerable_string(str))
            sleep_time=(unsigned long)trans_string_to_number(str);
        else
        {
            std::cout<<">> [runtime] builtin_sleep: this is not a numerable string."<<std::endl;
            ++error;
            return -1;
        }
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
        std::cout<<">> [runtime] builtin_finput: cannot find values or wrong value type(must be string)."<<std::endl;
        ++error;
        return -1;
    }
    std::string filename=nasal_vm.gc_get(value_addr).get_string();
    std::ifstream fin(filename);
    std::string file_content="";
    if(!fin.fail())
        while(!fin.eof())
        {
            file_content.push_back(fin.get());
            if(fin.eof())
                break;
        }
    else
        file_content="cannot open file named \'"+filename+"\'.";
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
        std::cout<<">> [runtime] builtin_foutput: cannot find values or wrong value type(filename must be string)."<<std::endl;
        ++error;
        return -1;
    }
    if(str_value_addr<0 || nasal_vm.gc_get(str_value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_foutput: cannot find values or wrong value type(file's content must be string)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_split: cannot find values or wrong value type(must be string)."<<std::endl;
        ++error;
        return -1;
    }
    if(string_value_addr<0 || nasal_vm.gc_get(string_value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_split: cannot find values or wrong value type(must be string)."<<std::endl;
        ++error;
        return -1;
    }
    std::string delimeter=nasal_vm.gc_get(delimeter_value_addr).get_string();
    std::string source=nasal_vm.gc_get(string_value_addr).get_string();
    int delimeter_len=delimeter.length();
    if(delimeter_len<1)
    {
        std::cout<<">> [runtime] builtin_split: delimeter's length must be greater than 0."<<std::endl;
        ++error;
        return -1;
    }
    int source_len=source.length();

    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_vector);
    nasal_vector& ref_vec=nasal_vm.gc_get(ret_addr).get_vector();
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
        std::cout<<">> [runtime] builtin_rand: cannot find values or wrong value type(must be nil or number)."<<std::endl;
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
    int value_addr=in_builtin_find("id");
    if(value_addr<0)
    {
        std::cout<<">> [runtime] builtin_id: cannot find this value."<<std::endl;
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
        std::cout<<">> [runtime] builtin_int: cannot find this value or wrong value type(must be number)."<<std::endl;
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
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_string)
    {
        std::cout<<">> [runtime] builtin_num: cannot find this value or wrong value type(must be string)."<<std::endl;
        ++error;
        return -1;
    }
    std::string str=nasal_vm.gc_get(value_addr).get_string();
    if(!check_numerable_string(str))
    {
        std::cout<<">> [runtime] builtin_num: this is not a numerable string."<<std::endl;
        ++error;
        return -1;
    }
    double number=trans_string_to_number(str);
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_number);
    nasal_vm.gc_get(ret_addr).set_number(number);
    return ret_addr;
}
int nasal_runtime::builtin_pop(int local_scope_addr)
{
    int value_addr=in_builtin_find("vector");
    if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_pop: cannot find this value or wrong value type(must be vector)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_str: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_size: cannot find values."<<std::endl;
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
        std::cout<<">> [runtime] builtin_xor: cannot find values or wrong value type(must be number)."<<std::endl;
        ++error;
        return -1;
    }
    if(b_addr<0 || nasal_vm.gc_get(b_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_xor: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_and: cannot find values or wrong value type(must be number)."<<std::endl;
        ++error;
        return -1;
    }
    if(b_addr<0 || nasal_vm.gc_get(b_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_and: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_or: cannot find values or wrong value type(must be number)."<<std::endl;
        ++error;
        return -1;
    }
    if(b_addr<0 || nasal_vm.gc_get(b_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_or: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_nand: cannot find values or wrong value type(must be number)."<<std::endl;
        ++error;
        return -1;
    }
    if(b_addr<0 || nasal_vm.gc_get(b_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_nand: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_not: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_sin: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_cos: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_tan: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_exp: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_ln: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_sqrt: cannot find values or wrong value type(must be number)."<<std::endl;
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
        std::cout<<">> [runtime] builtin_atan2: cannot find values or wrong value type(must be number)."<<std::endl;
        ++error;
        return -1;
    }
    if(y_value_addr<0 || nasal_vm.gc_get(y_value_addr).get_type()!=vm_number)
    {
        std::cout<<">> [runtime] builtin_atan2: cannot find values or wrong value type(must be number)."<<std::endl;
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
#endif