#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class nasal_runtime
{
private:
    // global_scope_address is address in garbage_collector_memory,NOT memory_manager_memory
    int global_scope_address;
    nasal_ast root;
    std::stack<int> function_position;
    std::stack<int> running_stack;
    std::stack<int> closure_stack;
public:
    nasal_runtime();
    ~nasal_runtime();
    void run();
};

nasal_runtime::nasal_runtime()
{
    root.clear();
    while(!function_position.empty())
        function_position.pop();
    while(!running_stack.empty())
        running_stack.pop();
    while(!closure_stack.empty())
        closure_stack.pop();
    return;
}
nasal_runtime::~nasal_runtime()
{
    root.clear();
    while(!function_position.empty())
        function_position.pop();
    while(!running_stack.empty())
        running_stack.pop();
    while(!closure_stack.empty())
        closure_stack.pop();
    return;
}
void nasal_runtime::run()
{
    global_scope_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(global_scope_address).set_type(vm_closure);
    nasal_vm.gc_get(global_scope_address).get_closure().add_scope();

    nasal_vm.gc_get(global_scope_address).get_closure().del_scope();
    nasal_vm.del_reference(global_scope_address);
    return;
}

#endif