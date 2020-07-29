#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class nasal_runtime
{
private:
    nasal_ast root;
    std::stack<int> function_position;
    std::stack<int> running_stack;
    std::stack<int> closure_stack;
public:
    nasal_runtime();
    ~nasal_runtime();
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

#endif