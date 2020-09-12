#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__

int nasal_runtime::builtin_print(int local_scope_addr)
{
    int vector_value_addr=-1;
    if(local_scope_addr>=0)
        vector_value_addr=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address("elements");
    if(vector_value_addr<0 || nasal_vm.gc_get(vector_value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_print: cannot find values or wrong value type."<<std::endl;
        ++error;
        return -1;
    }
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
    int ret_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(ret_addr).set_type(vm_nil);
    return ret_addr;
}
int nasal_runtime::builtin_append(int local_scope_addr)
{
    int vector_value_addr=-1;
    if(local_scope_addr>=0)
        vector_value_addr=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address("vector");
    if(vector_value_addr<0 || nasal_vm.gc_get(vector_value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_append: cannot find values or wrong value type."<<std::endl;
        ++error;
        return -1;
    }
    int elem_value_addr=-1;
    if(local_scope_addr>=0)
        elem_value_addr=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address("elements");
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
    int vector_value_addr=-1;
    if(local_scope_addr>=0)
        vector_value_addr=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address("vector");
    if(vector_value_addr<0 || nasal_vm.gc_get(vector_value_addr).get_type()!=vm_vector)
    {
        std::cout<<">> [runtime] builtin_setsize: cannot find values or wrong value type."<<std::endl;
        ++error;
        return -1;
    }
    int size_value_addr=-1;
    if(local_scope_addr>=0)
        size_value_addr=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address("size");
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
            ref_vector.del_elem(number);
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

#endif