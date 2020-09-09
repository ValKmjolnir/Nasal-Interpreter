#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__

int nasal_runtime::builtin_print(int local_scope_addr)
{
    int vector_value_addr=-1;
    if(local_scope_addr>=0)
        vector_value_addr=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address("dyn");
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

#endif