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