#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

enum runtime_returned_state
{
    rt_return=1,
    rt_break,
    rt_continue,
    rt_error,
    rt_exit_without_error
};

class nasal_runtime
{
private:
    // global_scope_address and function_return_address are addresses in garbage_collector_memory
    int function_returned_address;
    int global_scope_address;
    nasal_ast root;

    // if error occurred,this value will add 1
    int error;

    // generate number and return gc place of this number
    int number_generation(nasal_ast&);
    // generate string and return gc place of this string
    int string_generation(nasal_ast&);
    // generate vector and return gc place of this vector
    int vector_generation(nasal_ast&,int);
    // generate hash and return gc place of this hash
    int hash_generation(nasal_ast&,int);
    // generate function and return gc place of this function
    int function_generation(nasal_ast&,int);

    /*
        functions after this note may have parameter named 'local_scope_addr'
        if no local scope existing when calling these functions,use -1
    */

    // main expression block running process
    int main_progress();
    // function/loop/conditional expression block running process
    int block_progress(nasal_ast&,int);
    // run loop
    int loop_progress(nasal_ast&,int);
    // run conditional
    int conditional_progress(nasal_ast&,int);
    // run function
    int function_progress(nasal_ast&,int);
    // get scalars in complex data structure like vector/hash/function/closure(scope)
    int call_scalar(nasal_ast&,int);
    int call_vector(nasal_ast&,int,int);
    int call_hash(nasal_ast&,int,int);
    int call_function(nasal_ast&,int,int);
    // get scalars' memory place in complex data structure like vector/hash/function/closure(scope)
    int call_scalar_mem(nasal_ast&,int);
    int call_vector_mem(nasal_ast&,int,int);
    int call_hash_mem(nasal_ast&,int,int);
    int call_function_mem(nasal_ast&,int,int);
    // calculate scalars
    int calculation(nasal_ast&,int);
public:
    nasal_runtime();
    ~nasal_runtime();
    void set_root(nasal_ast&);
    void run();
};

nasal_runtime::nasal_runtime()
{
    error=0;
    this->root.clear();
    this->global_scope_address=-1;
    return;
}
nasal_runtime::~nasal_runtime()
{
    error=0;
    this->root.clear();
    this->global_scope_address=-1;
    return;
}
void nasal_runtime::set_root(nasal_ast& parse_result)
{
    this->root=parse_result;
    return;
}
void nasal_runtime::run()
{
    int returned_statement;
    time_t begin_time,end_time;
    this->global_scope_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(global_scope_address).set_type(vm_closure);
    nasal_vm.gc_get(global_scope_address).get_closure().add_scope();
    begin_time=std::time(NULL);
    returned_statement=main_progress();
    end_time=std::time(NULL);
    nasal_vm.gc_get(global_scope_address).get_closure().del_scope();
    nasal_vm.del_reference(global_scope_address);
    std::cout<<">> [runtime] process exited after "<<end_time-begin_time<<"s with returned value "<<returned_statement<<'.'<<std::endl;
    return;
}

// private functions
int nasal_runtime::number_generation(nasal_ast& node)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_number);
    nasal_vm.gc_get(new_addr).set_number(trans_string_to_number(node.get_str()));
    return new_addr;
}
int nasal_runtime::string_generation(nasal_ast& node)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_string);
    nasal_vm.gc_get(new_addr).set_string(node.get_str());
    return new_addr;
}
int nasal_runtime::vector_generation(nasal_ast& node,int local_scope_addr)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_vector);
    nasal_vector& ref_of_this_vector=nasal_vm.gc_get(new_addr).get_vector();

    int node_children_size=node.get_children().size();
    for(int i=0;i<node_children_size;++i)
    {
        int new_memory_space=nasal_vm.mem_alloc();
        ref_of_this_vector.add_elem(new_memory_space);
        switch(node.get_children()[i].get_type())
        {
            case ast_number:   nasal_vm.mem_init(new_memory_space,number_generation(node.get_children()[i]));break;
            case ast_string:   nasal_vm.mem_init(new_memory_space,string_generation(node.get_children()[i]));break;
            case ast_vector:   nasal_vm.mem_init(new_memory_space,vector_generation(node.get_children()[i],local_scope_addr));break;
            case ast_hash:     nasal_vm.mem_init(new_memory_space,hash_generation(node.get_children()[i],local_scope_addr));break;
            case ast_function: nasal_vm.mem_init(new_memory_space,function_generation(node.get_children()[i],local_scope_addr));break;
        }
    }
    return new_addr;
}
int nasal_runtime::hash_generation(nasal_ast& node,int local_scope_addr)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_hash);
    nasal_hash& ref_of_this_hash=nasal_vm.gc_get(new_addr).get_hash();

    int node_children_size=node.get_children().size();
    for(int i=0;i<node_children_size;++i)
    {
        int new_memory_space=nasal_vm.mem_alloc();
        std::string hash_member_name=node.get_children()[i].get_children()[0].get_str();
        ref_of_this_hash.add_elem(hash_member_name,new_memory_space);
        nasal_ast& ref_of_hash_member_value=node.get_children()[i].get_children()[1];
        switch(ref_of_hash_member_value.get_type())
        {
            case ast_number:   nasal_vm.mem_init(new_memory_space,number_generation(ref_of_hash_member_value));break;
            case ast_string:   nasal_vm.mem_init(new_memory_space,string_generation(ref_of_hash_member_value));break;
            case ast_vector:   nasal_vm.mem_init(new_memory_space,vector_generation(ref_of_hash_member_value,local_scope_addr));break;
            case ast_hash:     nasal_vm.mem_init(new_memory_space,hash_generation(ref_of_hash_member_value,local_scope_addr));break;
            case ast_function: nasal_vm.mem_init(new_memory_space,function_generation(ref_of_hash_member_value,local_scope_addr));break;
        }
    }
    return new_addr;
}
int nasal_runtime::function_generation(nasal_ast& node,int local_scope_addr)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_function);
    nasal_function& ref_of_this_function=nasal_vm.gc_get(new_addr).get_func();

    ref_of_this_function.set_arguments(node.get_children()[0]);
    ref_of_this_function.set_run_block(node.get_children()[1]);
    if(local_scope_addr>=0)
    {
        nasal_vm.add_reference(local_scope_addr);
        ref_of_this_function.set_closure_addr(local_scope_addr);
    }
    return new_addr;
}
int nasal_runtime::main_progress()
{
    int ret_state=rt_exit_without_error;
    int expr_number=root.get_children().size();
    int process_returned_value_addr=-1;
    for(int i=0;i<expr_number;++i)
    {
        int node_type=root.get_children()[i].get_type();
        switch(node_type)
        {
            case ast_definition:break;
            case ast_multi_assign:break;
            case ast_conditional:
                ret_state=conditional_progress(root.get_children()[i],-1);
                break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:
                ret_state=loop_progress(root.get_children()[i],-1);
                break;
            case ast_number:break;
            case ast_string:break;
            case ast_add_equal:
            case ast_sub_equal:
            case ast_mult_equal:
            case ast_div_equal:
            case ast_link_equal:
            case ast_unary_sub:
            case ast_unary_not:
            case ast_add:
            case ast_sub:
            case ast_mult:
            case ast_div:
            case ast_link:
            case ast_trinocular:
                process_returned_value_addr=calculation(root.get_children()[i],-1);
                nasal_vm.del_reference(process_returned_value_addr);
                break;
            case ast_break:
                ret_state=rt_break;
                break;
            case ast_continue:
                ret_state=rt_continue;
                break;
        }
        switch(ret_state)
        {
            case rt_break:
                std::cout<<">> [runtime] main_progress:cannot use break in main progress."<<std::endl;
                ++error;
                break;
            case rt_continue:
                std::cout<<">> [runtime] main_progress:cannot use continue in main progress."<<std::endl;
                ++error;
                break;
            case rt_error:
                std::cout<<">> [runtime] main_progress:error occurred when executing main progress."<<std::endl;
                ++error;
                break;
        }
        if(error)
            break;
    }
    return ret_state;
}
int nasal_runtime::block_progress(nasal_ast& node,int local_scope_addr)
{
    int ret_state=rt_exit_without_error;
    if(local_scope_addr<0)
    {
        local_scope_addr=nasal_vm.gc_alloc();
        nasal_vm.gc_get(local_scope_addr).set_type(vm_closure);
        nasal_vm.gc_get(local_scope_addr).get_closure().add_scope();
    }
    else
        nasal_vm.add_reference(local_scope_addr);
    int expr_number=node.get_children().size();
    int process_returned_value_addr=-1;
    for(int i=0;i<expr_number;++i)
    {
        int node_type=node.get_children()[i].get_type();
        switch(node_type)
        {
            case ast_definition:break;
            case ast_multi_assign:break;
            case ast_conditional:
                ret_state=conditional_progress(node.get_children()[i],local_scope_addr);
                break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:
                ret_state=loop_progress(node.get_children()[i],local_scope_addr);
                break;
            case ast_number:break;
            case ast_string:break;
            case ast_add_equal:
            case ast_sub_equal:
            case ast_mult_equal:
            case ast_div_equal:
            case ast_link_equal:
            case ast_unary_sub:
            case ast_unary_not:
            case ast_add:
            case ast_sub:
            case ast_mult:
            case ast_div:
            case ast_link:
            case ast_trinocular:
                process_returned_value_addr=calculation(root.get_children()[i],local_scope_addr);
                nasal_vm.del_reference(process_returned_value_addr);
                break;
            case ast_break:
                ret_state=rt_break;
                break;
            case ast_continue:
                ret_state=rt_continue;
                break;
        }
        if(ret_state==rt_error)
        {
            std::cout<<">> [runtime] block_progress:error occurred when executing sub-progress."<<std::endl;
            ++error;
        }
        if(error)
            break;
    }
    nasal_vm.del_reference(local_scope_addr);
    return ret_state;
}
int nasal_runtime::loop_progress(nasal_ast& node,int local_scope_addr)
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::conditional_progress(nasal_ast& node,int local_scope_addr)
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::function_progress(nasal_ast& node,int local_scope_addr)
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::call_scalar(nasal_ast& node,int local_scope_addr)
{
    int value_address=-1;
    if(local_scope_addr>=0)
        value_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address(node.get_children()[0].get_str());
    if(value_address<0)
        value_address=nasal_vm.gc_get(global_scope_address).get_closure().get_value_address(node.get_children()[0].get_str());
    if(value_address<0)
    {
        std::cout<<">> [runtime] call_nasal_scalar:cannot find value named \'"<<node.get_children()[0].get_str()<<"\'."<<std::endl;
        return -1;
    }
    // unfinished
    return -1;
}
int nasal_runtime::call_vector(nasal_ast& node,int base_value_addr,int local_scope_addr)
{
    return -1;
}
int nasal_runtime::call_hash(nasal_ast& node,int base_value_addr,int local_scope_addr)
{
    return -1;
}
int nasal_runtime::call_function(nasal_ast& node,int base_value_addr,int local_scope_addr)
{
    return -1;
}
int nasal_runtime::call_scalar_mem(nasal_ast& node,int local_scope_addr)
{
    int mem_address=-1;
    if(local_scope_addr>=0)
        mem_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_mem_address(node.get_children()[0].get_str());
    if(mem_address<0)
        mem_address=nasal_vm.gc_get(global_scope_address).get_closure().get_mem_address(node.get_children()[0].get_str());
    if(mem_address<0)
    {
        std::cout<<">> [runtime] call_nasal_mem:cannot find value named \'"<<node.get_children()[0].get_str()<<"\'."<<std::endl;
        return -1;
    }
    // unfinished
    return mem_address;
}
int nasal_runtime::call_vector_mem(nasal_ast& node,int base_mem_addr,int local_scope_addr)
{
    return -1;
}
int nasal_runtime::call_hash_mem(nasal_ast& node,int base_mem_addr,int local_scope_addr)
{
    return -1;
}
int nasal_runtime::call_function_mem(nasal_ast& node,int base_mem_addr,int local_scope_addr)
{
    return -1;
}
int nasal_runtime::calculation(nasal_ast& node,int local_scope_addr)
{
    // after this process, a new address(in nasal_vm.garbage_collector_memory) will be returned
    int ret_address=-1;
    int calculation_type=node.get_type();
    if(calculation_type==ast_number)
        ret_address=number_generation(node);
    else if(calculation_type==ast_string)
        ret_address=string_generation(node);
    else if(calculation_type==ast_vector)
        ret_address=vector_generation(node,local_scope_addr);
    else if(calculation_type==ast_hash)
        ret_address=hash_generation(node,local_scope_addr);
    else if(calculation_type==ast_function)
        ret_address=function_generation(node,local_scope_addr);
    else if(calculation_type==ast_call)
        ret_address=call_scalar(node,local_scope_addr);
    else if(calculation_type==ast_add)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_add(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_sub)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_sub(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_mult)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_mult(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_div)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_div(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_link)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_link(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_cmp_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_equal(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_cmp_not_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_not_equal(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_less_than)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_less(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_less_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_less_or_equal(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_greater_than)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_greater(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_greater_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_greater_or_equal(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_unary_not)
    {
        int addr=calculation(node.get_children()[0],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_unary_not(addr);
    }
    else if(calculation_type==ast_unary_sub)
    {
        int addr=calculation(node.get_children()[0],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_unary_sub(addr);
    }
    else if(calculation_type==ast_trinocular)
    {
        int condition_addr=calculation(node.get_children()[0],local_scope_addr);
        int ret_1_addr=calculation(node.get_children()[1],local_scope_addr);
        int ret_2_addr=calculation(node.get_children()[2],local_scope_addr);
        int check_null=nasal_scalar_calculator.nasal_scalar_unary_not(condition_addr);
        if(nasal_vm.gc_get(check_null).get_number()==0)
        {
            ret_address=ret_1_addr;
            nasal_vm.del_reference(ret_2_addr);
        }
        else
        {
            ret_address=ret_2_addr;
            nasal_vm.del_reference(ret_1_addr);
        }
        nasal_vm.del_reference(condition_addr);
        nasal_vm.del_reference(check_null);
    }
    else if(calculation_type==ast_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        nasal_vm.mem_change(scalar_mem_space,new_scalar_gc_addr);
        nasal_vm.add_reference(new_scalar_gc_addr);
        ret_address=new_scalar_gc_addr;
    }
    else if(calculation_type==ast_add_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_add(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);
        nasal_vm.add_reference(result_val_address);
        ret_address=result_val_address;
    }
    else if(calculation_type==ast_sub_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_sub(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);
        nasal_vm.add_reference(result_val_address);
        ret_address=result_val_address;
    }
    else if(calculation_type==ast_div_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_div(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);
        nasal_vm.add_reference(result_val_address);
        ret_address=result_val_address;
    }
    else if(calculation_type==ast_mult_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_mult(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);
        nasal_vm.add_reference(result_val_address);
        ret_address=result_val_address;
    }
    else if(calculation_type==ast_link_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_link(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);
        nasal_vm.add_reference(result_val_address);
        ret_address=result_val_address;
    }
    if(ret_address<0)
    {
        std::cout<<">> [runtime] calculation:incorrect values are used in calculation."<<std::endl;
        ++error;
    }
    return ret_address;
}

#endif