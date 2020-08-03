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
    // process functions are private
    int number_generation(nasal_ast&);
    int string_generation(nasal_ast&);
    int vector_generation(nasal_ast&);
    int hash_generation(nasal_ast&);
    int function_generation(nasal_ast&);

    int main_progress();
    int block_progress();
    int loop_progress();
    int conditional_progress();
    int function_progress();
    int calculation();
public:
    nasal_runtime();
    ~nasal_runtime();
    void set_root(nasal_ast&);
    void run();
};

nasal_runtime::nasal_runtime()
{
    this->root.clear();
    this->global_scope_address=-1;
    return;
}
nasal_runtime::~nasal_runtime()
{
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
    this->global_scope_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(global_scope_address).set_type(vm_closure);
    nasal_vm.gc_get(global_scope_address).get_closure().add_scope();

    nasal_vm.gc_get(global_scope_address).get_closure().del_scope();
    nasal_vm.del_reference(global_scope_address);
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
int nasal_runtime::vector_generation(nasal_ast& node)
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
            case ast_vector:   nasal_vm.mem_init(new_memory_space,vector_generation(node.get_children()[i]));break;
            case ast_hash:     nasal_vm.mem_init(new_memory_space,hash_generation(node.get_children()[i]));break;
            case ast_function: nasal_vm.mem_init(new_memory_space,function_generation(node.get_children()[i]));break;
        }
    }
    return new_addr;
}
int nasal_runtime::hash_generation(nasal_ast& node)
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
            case ast_vector:   nasal_vm.mem_init(new_memory_space,vector_generation(ref_of_hash_member_value));break;
            case ast_hash:     nasal_vm.mem_init(new_memory_space,hash_generation(ref_of_hash_member_value));break;
            case ast_function: nasal_vm.mem_init(new_memory_space,function_generation(ref_of_hash_member_value));break;
        }
    }
    return new_addr;
}
int nasal_runtime::function_generation(nasal_ast& node)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_function);
    nasal_function& ref_of_this_function=nasal_vm.gc_get(new_addr).get_func();

    ref_of_this_function.set_arguments(node.get_children()[0]);
    ref_of_this_function.set_run_block(node.get_children()[1]);
    return new_addr;
}
int nasal_runtime::main_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::block_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::loop_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::conditional_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::function_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::calculation()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}

#endif