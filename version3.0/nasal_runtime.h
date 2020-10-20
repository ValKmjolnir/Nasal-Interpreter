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
    std::map<std::string,int (nasal_runtime::*)(int x)> builtin_func_hashmap;
    // function_return_address is an address in garbage_collector_memory
    int function_returned_address;
    // global_scope_address is an address in garbage_collector_memory
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
    int block_progress(nasal_ast&,int,bool);
    // run loop
    int before_for_loop(nasal_ast&,int);
    int after_each_for_loop(nasal_ast&,int);
    int loop_progress(nasal_ast&,int,bool);
    // run conditional
    bool check_condition(int);
    int conditional_progress(nasal_ast&,int,bool);
    // get scalars in complex data structure like vector/hash/function/closure(scope)
    int call_scalar(nasal_ast&,int);
    int call_vector(nasal_ast&,int,int);
    int call_hash(nasal_ast&,int,int);
    int call_function(nasal_ast&,std::string,int,int,int);
    int call_builtin_function(std::string,int);
    // get scalars' memory place in complex data structure like vector/hash/function/closure(scope)
    int call_scalar_mem(nasal_ast&,int);
    int call_vector_mem(nasal_ast&,int,int);
    int call_hash_mem(nasal_ast&,int,int);
    // calculate scalars
    int calculation(nasal_ast&,int);
    void definition(nasal_ast&,int);
    void multi_assignment(nasal_ast&,int);

    // builtin_func defined here
    int builtin_print(int);
    int builtin_append(int);
    int builtin_setsize(int);
    int builtin_system(int);
    int builtin_input(int);
    int builtin_sleep(int);
    int builtin_finput(int);
    int builtin_foutput(int);
    int builtin_split(int);
    int builtin_rand(int);
    int builtin_id(int);
    int builtin_int(int);
    int builtin_num(int);
    int builtin_pop(int);
    int builtin_str(int);
    int builtin_size(int);
    int builtin_xor(int);
    int builtin_and(int);
    int builtin_or(int);
    int builtin_nand(int);
    int builtin_not(int);
    int builtin_sin(int);
    int builtin_cos(int);
    int builtin_tan(int);
    int builtin_exp(int);
    int builtin_ln(int);
    int builtin_sqrt(int);
    int builtin_atan2(int);
    int builtin_time(int);
    int builtin_contains(int);
    int builtin_delete(int);
    int builtin_getkeys(int);
    int builtin_import(int);
    int builtin_die(int);
    void load_builtin_function();
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
    this->load_builtin_function();
    return;
}
nasal_runtime::~nasal_runtime()
{
    error=0;
    this->root.clear();
    this->global_scope_address=-1;
    this->builtin_func_hashmap.clear();
    return;
}
void nasal_runtime::load_builtin_function()
{
    struct FUNC_TABLE
    {
        std::string func_name;
        int (nasal_runtime::*func_pointer)(int x);
    } builtin_func_table[]=
    {
        {"nasal_call_builtin_std_cout",      nasal_runtime::builtin_print},
        {"nasal_call_builtin_push_back",     nasal_runtime::builtin_append},
        {"nasal_call_builtin_set_size",      nasal_runtime::builtin_setsize},
        {"nasal_call_builtin_system",        nasal_runtime::builtin_system},
        {"nasal_call_builtin_input",         nasal_runtime::builtin_input},
        {"nasal_call_builtin_sleep",         nasal_runtime::builtin_sleep},
        {"nasal_call_builtin_finput",        nasal_runtime::builtin_finput},
        {"nasal_call_builtin_foutput",       nasal_runtime::builtin_foutput},
        {"nasal_call_builtin_split",         nasal_runtime::builtin_split},
        {"nasal_call_builtin_rand",          nasal_runtime::builtin_rand},
        {"nasal_call_builtin_get_id",        nasal_runtime::builtin_id},
        {"nasal_call_builtin_trans_int",     nasal_runtime::builtin_int},
        {"nasal_call_builtin_trans_num",     nasal_runtime::builtin_num},
        {"nasal_call_builtin_pop_back",      nasal_runtime::builtin_pop},
        {"nasal_call_builtin_trans_str",     nasal_runtime::builtin_str},
        {"nasal_call_builtin_size",          nasal_runtime::builtin_size},
        {"nasal_call_builtin_xor",           nasal_runtime::builtin_xor},
        {"nasal_call_builtin_and",           nasal_runtime::builtin_and},
        {"nasal_call_builtin_or",            nasal_runtime::builtin_or},
        {"nasal_call_builtin_nand",          nasal_runtime::builtin_nand},
        {"nasal_call_builtin_not",           nasal_runtime::builtin_not},
        {"nasal_call_builtin_sin",           nasal_runtime::builtin_sin},
        {"nasal_call_builtin_cos",           nasal_runtime::builtin_cos},
        {"nasal_call_builtin_tan",           nasal_runtime::builtin_tan},
        {"nasal_call_builtin_exp",           nasal_runtime::builtin_exp},
        {"nasal_call_builtin_cpp_math_ln",   nasal_runtime::builtin_ln},
        {"nasal_call_builtin_cpp_math_sqrt", nasal_runtime::builtin_sqrt},
        {"nasal_call_builtin_cpp_atan2",     nasal_runtime::builtin_atan2},
        {"nasal_call_builtin_time",          nasal_runtime::builtin_time},
        {"nasal_call_builtin_contains",      nasal_runtime::builtin_contains},
        {"nasal_call_builtin_delete",        nasal_runtime::builtin_delete},
        {"nasal_call_builtin_get_keys",      nasal_runtime::builtin_getkeys},
        {"nasal_call_import",                nasal_runtime::builtin_import},
        {"nasal_call_builtin_die",           nasal_runtime::builtin_die},
        {"",                                 NULL}
    };
    for(int i=0;builtin_func_table[i].func_pointer;++i)
        builtin_func_hashmap[builtin_func_table[i].func_name]=builtin_func_table[i].func_pointer;
    return;
}
void nasal_runtime::set_root(nasal_ast& parse_result)
{
    this->root=parse_result;
    return;
}
void nasal_runtime::run()
{
    this->error=0;
    this->function_returned_address=-1;

    this->global_scope_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(global_scope_address).set_type(vm_closure);
    nasal_vm.gc_get(global_scope_address).get_closure().add_scope();

    time_t begin_time=std::time(NULL);
    int returned_statement=main_progress();
    time_t end_time=std::time(NULL);

    nasal_vm.gc_get(global_scope_address).get_closure().del_scope();
    nasal_vm.del_reference(global_scope_address);
    nasal_vm.clear();

    time_t total_run_time=end_time-begin_time;
    if(total_run_time>=1)
    std::cout<<">> [runtime] process exited after "<<total_run_time<<"s "<<(returned_statement==rt_exit_without_error?".\n":"with errors.\n");
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
        ref_of_this_vector.add_elem(calculation(node.get_children()[i],local_scope_addr));
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
        nasal_ast& ref_of_hash_member_value=node.get_children()[i].get_children()[1];
        std::string hash_member_name=node.get_children()[i].get_children()[0].get_str();
        ref_of_this_hash.add_elem(hash_member_name,calculation(ref_of_hash_member_value,local_scope_addr));
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
        ref_of_this_function.set_closure_addr(local_scope_addr);
    else
    {
        int new_closure=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_closure).set_type(vm_closure);
        ref_of_this_function.set_closure_addr(new_closure);
        nasal_vm.del_reference(new_closure);
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
            case ast_definition:definition(root.get_children()[i],-1);break;
            case ast_multi_assign:multi_assignment(root.get_children()[i],-1);break;
            case ast_conditional:ret_state=conditional_progress(root.get_children()[i],-1,false);break;
            case ast_while:case ast_for:case ast_forindex:case ast_foreach:
                ret_state=loop_progress(root.get_children()[i],-1,false);break;
            case ast_nil:case ast_number:case ast_string:case ast_function:break;
            case ast_identifier:
            case ast_vector:case ast_hash:
            case ast_call:
            case ast_equal:case ast_add_equal:case ast_sub_equal:case ast_mult_equal:case ast_div_equal:case ast_link_equal:
            case ast_unary_sub:case ast_unary_not:
            case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
            case ast_trinocular:nasal_vm.del_reference(calculation(root.get_children()[i],-1));break;
        }
        switch(ret_state)
        {
            case rt_break:std::cout<<">> [runtime] main_progress: cannot use break in main progress.\n";++error;break;
            case rt_continue:std::cout<<">> [runtime] main_progress: cannot use continue in main progress.\n";++error;break;
            case rt_return:std::cout<<">> [runtime] main_progress: cannot use return in main progress.\n";++error;break;
            case rt_error:std::cout<<">> [runtime] main_progress: error occurred when executing main progress.\n";++error;break;
        }
        if(error)
        {
            ret_state=rt_error;
            break;
        }
    }
    return ret_state;
}
int nasal_runtime::block_progress(nasal_ast& node,int local_scope_addr,bool allow_return)
{
    int ret_state=rt_exit_without_error;
    // if local_scope does not exist,create a new one.
    if(local_scope_addr<0)
    {
        local_scope_addr=nasal_vm.gc_alloc();
        nasal_vm.gc_get(local_scope_addr).set_type(vm_closure);
        nasal_vm.gc_get(local_scope_addr).get_closure().add_scope();
    }
    else
    {
        nasal_vm.add_reference(local_scope_addr);
        nasal_vm.gc_get(local_scope_addr).get_closure().add_scope();
    }
    int expr_number=node.get_children().size();
    int process_returned_value_addr=-1;
    for(int i=0;i<expr_number;++i)
    {
        nasal_ast& tmp_node=node.get_children()[i];
        int node_type=tmp_node.get_type();
        switch(node_type)
        {
            case ast_definition:definition(tmp_node,local_scope_addr);break;
            case ast_multi_assign:multi_assignment(tmp_node,local_scope_addr);break;
            case ast_conditional:ret_state=conditional_progress(tmp_node,local_scope_addr,allow_return);break;
            case ast_while:case ast_for:case ast_forindex:case ast_foreach:
                ret_state=loop_progress(tmp_node,local_scope_addr,allow_return);break;
            case ast_nil:case ast_number:case ast_string:case ast_function:break;
            case ast_identifier:
            case ast_vector:case ast_hash:
            case ast_call:
            case ast_equal:case ast_add_equal:case ast_sub_equal:case ast_mult_equal:case ast_div_equal:case ast_link_equal:
            case ast_unary_sub:case ast_unary_not:
            case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
            case ast_trinocular:nasal_vm.del_reference(calculation(tmp_node,local_scope_addr));break;
            case ast_break:ret_state=rt_break;break;
            case ast_continue:ret_state=rt_continue;break;
            case ast_return:
                ret_state=rt_return;
                if(allow_return)
                {
                    if(tmp_node.get_children().size())
                        function_returned_address=calculation(tmp_node.get_children()[0],local_scope_addr);
                    else
                    {
                        function_returned_address=nasal_vm.gc_alloc();
                        nasal_vm.gc_get(function_returned_address).set_type(vm_nil);
                    }
                } 
                else
                {
                    std::cout<<">> [runtime] block_progress: return expression is not allowed here.\n";
                    ++error;
                }
                break;
        }
        if(ret_state==rt_error)
        {
            std::cout<<">> [runtime] block_progress: error occurred when executing sub-progress.\n";
            ++error;
        }
        if(error || ret_state==rt_break || ret_state==rt_continue || ret_state==rt_return)
            break;
    }
    nasal_vm.gc_get(local_scope_addr).get_closure().del_scope();
    nasal_vm.del_reference(local_scope_addr);
    return ret_state;
}
int nasal_runtime::before_for_loop(nasal_ast& node,int local_scope_addr)
{
    int before_loop_node_type=node.get_type();
    switch(before_loop_node_type)
    {
        case ast_null:break;
        case ast_definition:definition(node,local_scope_addr);break;
        case ast_multi_assign:multi_assignment(node,local_scope_addr);break;
        case ast_nil:case ast_number:case ast_string:case ast_function:break;
        case ast_vector:case ast_hash:
        case ast_call:
        case ast_equal:case ast_add_equal:case ast_sub_equal:case ast_mult_equal:case ast_div_equal:case ast_link_equal:
        case ast_unary_sub:case ast_unary_not:
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
        case ast_trinocular:nasal_vm.del_reference(calculation(node,local_scope_addr));break;
        default:std::cout<<">> [runtime] before_for_loop: cannot use this expression before for-loop.\n";++error;break;
    }
    if(error)
        return rt_error;
    return rt_exit_without_error;
}
int nasal_runtime::after_each_for_loop(nasal_ast& node,int local_scope_addr)
{
    int node_type=node.get_type();
    switch(node_type)
    {
        case ast_null:break;
        case ast_definition:definition(node,local_scope_addr);break;
        case ast_multi_assign:multi_assignment(node,local_scope_addr);break;
        case ast_nil:case ast_number:case ast_string:case ast_function:break;
        case ast_vector:case ast_hash:
        case ast_call:
        case ast_equal:case ast_add_equal:case ast_sub_equal:case ast_mult_equal:case ast_div_equal:case ast_link_equal:
        case ast_unary_sub:case ast_unary_not:
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
        case ast_trinocular:nasal_vm.del_reference(calculation(node,local_scope_addr));break;
        default:std::cout<<">> [runtime] after_each_for_loop: cannot use this expression after each for-loop.\n";++error;break;
    }
    if(error)
        return rt_error;
    return rt_exit_without_error;
}
int nasal_runtime::loop_progress(nasal_ast& node,int local_scope_addr,bool allow_return)
{
    int ret_state=rt_exit_without_error;
    int loop_type=node.get_type();
    if(loop_type==ast_while)
    {
        nasal_ast& condition_node=node.get_children()[0];
        nasal_ast& run_block_node=node.get_children()[1];
        // create a new local scope to store iterator if local_scope_addr=-1
        int while_local_scope_addr=local_scope_addr;
        if(while_local_scope_addr<0)
        {
            while_local_scope_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(while_local_scope_addr).set_type(vm_closure);
            nasal_vm.gc_get(while_local_scope_addr).get_closure().add_scope();
        }
        else
        {
            nasal_vm.add_reference(local_scope_addr);
            nasal_vm.gc_get(while_local_scope_addr).get_closure().add_scope();
        }
        // check condition and begin loop
        int condition_value_addr=calculation(condition_node,while_local_scope_addr);
        bool result=check_condition(condition_value_addr);
        nasal_vm.del_reference(condition_value_addr);
        while(result)
        {
            // return expression will be checked in block_progress
            ret_state=block_progress(run_block_node,local_scope_addr,allow_return);
            if(ret_state==rt_break || ret_state==rt_error || ret_state==rt_return || error)
                break;
            condition_value_addr=calculation(condition_node,while_local_scope_addr);
            result=check_condition(condition_value_addr);
            nasal_vm.del_reference(condition_value_addr);
        }
        nasal_vm.gc_get(while_local_scope_addr).get_closure().del_scope();
        nasal_vm.del_reference(while_local_scope_addr);
    }
    else if(loop_type==ast_forindex || loop_type==ast_foreach)
    {
        nasal_ast& iter_node=node.get_children()[0];
        nasal_ast& vector_node=node.get_children()[1];
        nasal_ast& run_block_node=node.get_children()[2];
        // check vector to make sure iterator can be used without problem
        int vector_value_addr=calculation(vector_node,local_scope_addr);
        if(vector_value_addr<0 || nasal_vm.gc_get(vector_value_addr).get_type()!=vm_vector)
        {
            std::cout<<">> [runtime] loop_progress: "<<(loop_type==ast_forindex? "forindex":"foreach")<<" gets a value that is not a vector.\n";
            ++error;
            return rt_error;
        }
        // create a new local scope to store iterator if local_scope_addr=-1
        int forei_local_scope_addr=local_scope_addr;
        if(forei_local_scope_addr<0)
        {
            forei_local_scope_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(forei_local_scope_addr).set_type(vm_closure);
            nasal_vm.gc_get(forei_local_scope_addr).get_closure().add_scope();
        }
        else
        {
            nasal_vm.add_reference(local_scope_addr);
            nasal_vm.gc_get(forei_local_scope_addr).get_closure().add_scope();
        }
        // begin loop progress
        int mem_addr=-1;
        if(iter_node.get_type()==ast_new_iter)
        {
            int new_value_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(new_value_addr).set_type(vm_nil);
            std::string val_name=iter_node.get_children()[0].get_str();
            nasal_vm.gc_get(forei_local_scope_addr).get_closure().add_new_value(val_name,new_value_addr);
            mem_addr=nasal_vm.gc_get(forei_local_scope_addr).get_closure().get_mem_address(val_name);
        }
        else
            mem_addr=call_scalar_mem(iter_node,local_scope_addr);
        if(mem_addr<0)
        {
            std::cout<<">> [runtime] loop_progress: get null iterator.\n";
            ++error;
            return rt_error;
        }
        // ref_vector's size may change when running,so this loop will check size each time
        nasal_vector& ref_vector=nasal_vm.gc_get(vector_value_addr).get_vector();
        for(int i=0;i<ref_vector.size();++i)
        {
            // update iterator
            if(loop_type==ast_forindex)
            {
                int new_iter_val_addr=nasal_vm.gc_alloc();
                nasal_vm.gc_get(new_iter_val_addr).set_type(vm_number);
                nasal_vm.gc_get(new_iter_val_addr).set_number((double)i);
                nasal_vm.mem_change(mem_addr,new_iter_val_addr);
            }
            else
            {
                int value_addr=ref_vector.get_value_address(i);
                nasal_vm.add_reference(value_addr);
                nasal_vm.mem_change(mem_addr,value_addr);
            }
            ret_state=block_progress(run_block_node,forei_local_scope_addr,allow_return);
            if(ret_state==rt_break || ret_state==rt_return || ret_state==rt_error || error)
                break;
        }
        nasal_vm.del_reference(vector_value_addr);
        nasal_vm.gc_get(forei_local_scope_addr).get_closure().del_scope();
        nasal_vm.del_reference(forei_local_scope_addr);
    }
    else if(loop_type==ast_for)
    {
        nasal_ast& before_loop_node=node.get_children()[0];
        nasal_ast& condition_node=node.get_children()[1];
        nasal_ast& each_loop_do_node=node.get_children()[2];
        nasal_ast& run_block_node=node.get_children()[3];

        // set local scope if local_scope_addr=-1
        int for_local_scope_addr=local_scope_addr;
        if(for_local_scope_addr<0)
        {
            for_local_scope_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(for_local_scope_addr).set_type(vm_closure);
            nasal_vm.gc_get(for_local_scope_addr).get_closure().add_scope();
        }
        else
        {
            nasal_vm.add_reference(local_scope_addr);
            nasal_vm.gc_get(for_local_scope_addr).get_closure().add_scope();
        }
        // for progress
        ret_state=before_for_loop(before_loop_node,for_local_scope_addr);
        int condition_value_addr=calculation(condition_node,for_local_scope_addr);
        bool result=check_condition(condition_value_addr);
        nasal_vm.del_reference(condition_value_addr);
        while(result)
        {
            if(ret_state==rt_error)
                break;
            ret_state=block_progress(run_block_node,for_local_scope_addr,allow_return);
            if(ret_state==rt_error || ret_state==rt_return || ret_state==rt_break || error)
                break;
            ret_state=after_each_for_loop(each_loop_do_node,for_local_scope_addr);
            condition_value_addr=calculation(condition_node,for_local_scope_addr);
            result=check_condition(condition_value_addr);
            nasal_vm.del_reference(condition_value_addr);
        }
        nasal_vm.gc_get(for_local_scope_addr).get_closure().del_scope();
        nasal_vm.del_reference(for_local_scope_addr);
    }
    if(ret_state==rt_break || ret_state==rt_continue)
        ret_state=rt_exit_without_error;
    return ret_state;
}
bool nasal_runtime::check_condition(int value_addr)
{
    if(value_addr<0)
        return false;
    int type=nasal_vm.gc_get(value_addr).get_type();
    if(type==vm_vector || type==vm_hash || type==vm_function || type==vm_closure)
    {
        std::cout<<">> [runtime] check_condition: error value type when checking condition.\n";
        ++error;
        return false;
    }
    else if(type==vm_string)
    {
        std::string str=nasal_vm.gc_get(value_addr).get_string();
        if(!check_numerable_string(str))
        {
            std::cout<<">> [runtime] check_condition: error value type, \'"<<str<<"\' is not a numerable string.\n";
            ++error;
            return -1;
        }
        return (trans_string_to_number(str)!=0);
    }
    else if(type==vm_nil)
        return false;
    else if(type==vm_number)
        return (nasal_vm.gc_get(value_addr).get_number()!=0);
    return false;
}
int nasal_runtime::conditional_progress(nasal_ast& node,int local_scope_addr,bool allow_return)
{
    int ret_state=rt_exit_without_error;
    int size=node.get_children().size();
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp_node=node.get_children()[i];
        int type=tmp_node.get_type();
        if(type==ast_if || type==ast_elsif)
        {
            int condition_value_addr=calculation(tmp_node.get_children()[0],local_scope_addr);
            if(condition_value_addr<0)
                return rt_error;
            bool result=check_condition(condition_value_addr);
            nasal_vm.del_reference(condition_value_addr);
            if(result)
            {
                ret_state=block_progress(tmp_node.get_children()[1],local_scope_addr,allow_return);
                break;
            }
        }
        else
        {
            ret_state=block_progress(tmp_node.get_children()[0],local_scope_addr,allow_return);
            break;
        }
        if(error)
            break;
    }
    return ret_state;
}
int nasal_runtime::call_scalar(nasal_ast& node,int local_scope_addr)
{
    int value_address=-1;
    std::string val_name="";
    if(node.get_children()[0].get_type()==ast_identifier)
    {
        val_name=node.get_children()[0].get_str();
        if(local_scope_addr>=0)
            value_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address(val_name);
        if(value_address<0)
            value_address=nasal_vm.gc_get(global_scope_address).get_closure().get_value_address(val_name);
        if(value_address<0)
        {
            value_address=call_builtin_function(val_name,local_scope_addr);
            if(value_address>=0)
                return value_address;
        }
        if(value_address<0)
        {
            if(builtin_func_hashmap.find(val_name)!=builtin_func_hashmap.end())
                std::cout<<">> [runtime] call_scalar: call "<<val_name<<" failed.\n";
            else
                std::cout<<">> [runtime] call_scalar: cannot find value named \""<<val_name<<"\".\n";
            ++error;
            return -1;
        }
        nasal_vm.add_reference(value_address);
    }
    else
        value_address=calculation(node.get_children()[0],local_scope_addr);
    int call_expr_size=node.get_children().size();
    int last_call_hash_addr=-1;
    for(int i=1;i<call_expr_size;++i)
    {
        int tmp_value_addr=-1;
        nasal_ast& call_expr=node.get_children()[i];
        switch(call_expr.get_type())
        {
            case ast_call_vec:
                tmp_value_addr=call_vector(call_expr,value_address,local_scope_addr);
                last_call_hash_addr=-1;
                val_name="";
                break;
            case ast_call_hash:
                tmp_value_addr=call_hash(call_expr,value_address,local_scope_addr);
                last_call_hash_addr=value_address;
                val_name=call_expr.get_str();
                break;
            case ast_call_func:
                tmp_value_addr=call_function(call_expr,val_name,value_address,last_call_hash_addr,local_scope_addr);
                last_call_hash_addr=-1;
                val_name="";
                break;
        }
        nasal_vm.del_reference(value_address);
        value_address=tmp_value_addr;
        if(value_address<0)
            break;
    }
    return value_address;
}
int nasal_runtime::call_vector(nasal_ast& node,int base_value_addr,int local_scope_addr)
{
    int return_value_addr=-1;
    int base_value_type=nasal_vm.gc_get(base_value_addr).get_type();
    if(base_value_type!=vm_vector && base_value_type!=vm_hash && base_value_type!=vm_string)
    {
        std::cout<<">> [runtime] call_vector: incorrect value type,expected a vector/hash/string.\n";
        ++error;
        return -1;
    }
    int call_size=node.get_children().size();

    if(base_value_type==vm_vector)
    {
        std::vector<int> called_value_addrs;
        nasal_vector& reference_value=nasal_vm.gc_get(base_value_addr).get_vector();
        for(int i=0;i<call_size;++i)
        {
            if(node.get_children()[i].get_type()==ast_subvec)
            {
                nasal_ast& subvec_node=node.get_children()[i];
                int begin_value_addr=calculation(subvec_node.get_children()[0],local_scope_addr);
                int end_value_addr=calculation(subvec_node.get_children()[1],local_scope_addr);
                int begin_value_type=nasal_vm.gc_get(begin_value_addr).get_type();
                int end_value_type=nasal_vm.gc_get(end_value_addr).get_type();
                bool begin_is_nil=true,end_is_nil=true;
                int begin_index=0,end_index=0;
                if(begin_value_type!=vm_nil && begin_value_type!=vm_number && begin_value_type!=vm_string)
                {
                    std::cout<<">> [runtime] call_vector: begin index is not a number/numerable string.\n";
                    ++error;
                    return -1;
                }
                if(end_value_type!=vm_nil && end_value_type!=vm_number && end_value_type!=vm_string)
                {
                    std::cout<<">> [runtime] call_vector: end index is not a number/numerable string.\n";
                    ++error;
                    return -1;
                }
                if(begin_value_type==vm_string)
                {
                    std::string str=nasal_vm.gc_get(begin_value_addr).get_string();
                    if(!check_numerable_string(str))
                    {
                        std::cout<<">> [runtime] call_vector: begin index is not a numerable string.\n";
                        ++error;
                        return -1;
                    }
                    begin_index=(int)trans_string_to_number(str);
                    begin_is_nil=false;
                }
                else if(begin_value_type==vm_number)
                {
                    begin_index=(int)nasal_vm.gc_get(begin_value_addr).get_number();
                    begin_is_nil=false;
                }
                
                if(end_value_type==vm_string)
                {
                    std::string str=nasal_vm.gc_get(end_value_addr).get_string();
                    if(!check_numerable_string(str))
                    {
                        std::cout<<">> [runtime] call_vector: end index is not a numerable string.\n";
                        ++error;
                        return -1;
                    }
                    end_index=(int)trans_string_to_number(str);
                    end_is_nil=false;
                }
                else if(end_value_type==vm_number)
                {
                    end_index=(int)nasal_vm.gc_get(end_value_addr).get_number();
                    end_is_nil=false;
                }
                
                if(begin_is_nil && end_is_nil)
                {
                    begin_index=0;
                    end_index=reference_value.size()-1;
                }
                else if(begin_is_nil && !end_is_nil)
                {
                    begin_index=end_index<0? -reference_value.size():0;
                }
                else if(!begin_is_nil && end_is_nil)
                {
                    end_index=begin_index<0? -1:reference_value.size()-1;
                }
                else if(!begin_is_nil && !end_is_nil)
                {
                    if(begin_index>=end_index)
                    {
                        std::cout<<">> [runtime] call_vector: begin index must be less than end index.\n";
                        ++error;
                        return -1;
                    }
                }
                for(int i=begin_index;i<end_index;++i)
                    called_value_addrs.push_back(reference_value.get_value_address(i));
                nasal_vm.del_reference(begin_value_addr);
                nasal_vm.del_reference(end_value_addr);
            }
            else
            {
                int index_value_addr=calculation(node.get_children()[i],local_scope_addr);
                int index_value_type=nasal_vm.gc_get(index_value_addr).get_type();
                if(index_value_type!=vm_number && index_value_type!=vm_string)
                {
                    std::cout<<">> [runtime] call_vector: index is not a number/numerable string.\n";
                    ++error;
                    return -1;
                }
                int index_num=0;
                if(index_value_type==vm_string)
                {
                    std::string str=nasal_vm.gc_get(index_value_addr).get_string();
                    if(!check_numerable_string(str))
                    {
                        std::cout<<">> [runtime] call_vector: index is not a numerable string.\n";
                        ++error;
                        return -1;
                    }
                    index_num=(int)trans_string_to_number(str);
                }
                else
                    index_num=(int)nasal_vm.gc_get(index_value_addr).get_number();
                nasal_vm.del_reference(index_value_addr);
                called_value_addrs.push_back(reference_value.get_value_address(index_num));
            }
        }
        // generate sub-vector
        if(called_value_addrs.size()==1)
        {
            int value_addr=called_value_addrs[0];
            nasal_vm.add_reference(value_addr);
            return_value_addr=value_addr;
        }
        else
        {
            return_value_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(return_value_addr).set_type(vm_vector);
            nasal_vector& return_vector=nasal_vm.gc_get(return_value_addr).get_vector();
            int vec_size=called_value_addrs.size();
            for(int i=0;i<vec_size;++i)
            {
                int value_addr=called_value_addrs[i];
                nasal_vm.add_reference(value_addr);
                return_vector.add_elem(value_addr);
            }
        }
    }
    else if(base_value_type==vm_hash)
    {
        if(call_size>1)
        {
            std::cout<<">> [runtime] call_vector: when calling a hash,only one key is alowed.\n";
            ++error;
            return -1;
        }
        if(node.get_children()[0].get_type()==ast_subvec)
        {
            std::cout<<">> [runtime] call_vector: cannot slice hash.\n";
            ++error;
            return -1;
        }
        std::string str=node.get_children()[0].get_str();
        int value_addr=nasal_vm.gc_get(base_value_addr).get_hash().get_value_address(str);
        nasal_vm.add_reference(value_addr);
        return_value_addr=value_addr;
    }
    else
    {
        if(call_size>1)
        {
            std::cout<<">> [runtime] call_vector: when calling a string,only one index is alowed.\n";
            ++error;
            return -1;
        }
        if(node.get_children()[0].get_type()==ast_subvec)
        {
            std::cout<<">> [runtime] call_vector: cannot slice string.\n";
            ++error;
            return -1;
        }
        int index_value_addr=calculation(node.get_children()[0],local_scope_addr);
        int index_value_type=nasal_vm.gc_get(index_value_addr).get_type();
        if(index_value_type!=vm_number && index_value_type!=vm_string)
        {
            std::cout<<">> [runtime] call_vector: index is not a number/numerable string.\n";
            ++error;
            return -1;
        }
        int index_num=0;
        if(index_value_type==vm_string)
        {
            std::string str=nasal_vm.gc_get(index_value_addr).get_string();
            if(!check_numerable_string(str))
            {
                std::cout<<">> [runtime] call_vector: index is not a numerable string.\n";
                ++error;
                return -1;
            }
            index_num=(int)trans_string_to_number(str);
        }
        else
            index_num=(int)nasal_vm.gc_get(index_value_addr).get_number();
        nasal_vm.del_reference(index_value_addr);
        return_value_addr=nasal_vm.gc_alloc();
        nasal_vm.gc_get(return_value_addr).set_type(vm_number);
        std::string str=nasal_vm.gc_get(base_value_addr).get_string();
        int str_size=str.length();
        if(index_num>=str_size || index_num<-str_size)
        {
            std::cout<<">> [runtime] call_vector: index out of range.\n";
            ++error;
            return -1;
        }
        nasal_vm.gc_get(return_value_addr).set_number((double)str[(index_num+str_size)%str_size]);
    }
    return return_value_addr;
}
int nasal_runtime::call_hash(nasal_ast& node,int base_value_addr,int local_scope_addr)
{
    int value_type=nasal_vm.gc_get(base_value_addr).get_type();
    if(value_type!=vm_hash)
    {
        std::cout<<">> [runtime] call_hash: incorrect value type,expected a hash.\n";
        ++error;
        return -1;
    }
    int ret_value_addr=nasal_vm.gc_get(base_value_addr).get_hash().get_value_address(node.get_str());
    nasal_vm.add_reference(ret_value_addr);
    return ret_value_addr;
}
int nasal_runtime::call_function(nasal_ast& node,std::string func_name,int base_value_addr,int last_call_hash_addr,int local_scope_addr)
{
    int ret_value_addr=-1;
    int value_type=nasal_vm.gc_get(base_value_addr).get_type();
    if(value_type!=vm_function)
    {
        std::cout<<">> [runtime] call_function: incorrect value type,expected a function.\n";
        ++error;
        return -1;
    }
    nasal_function& reference_of_func=nasal_vm.gc_get(base_value_addr).get_func();
    int run_closure_addr=reference_of_func.get_closure_addr();
    nasal_closure& run_closure=nasal_vm.gc_get(run_closure_addr).get_closure();
    run_closure.add_scope();
    // set self
    if(func_name.length())
    {
        nasal_vm.add_reference(base_value_addr);
        run_closure.add_new_value(func_name,base_value_addr);
    }
    // set hash.me
    if(last_call_hash_addr>=0)
    {
        nasal_vm.add_reference(last_call_hash_addr);
        run_closure.add_new_value("me",last_call_hash_addr);
    }
    nasal_ast& argument_format=reference_of_func.get_arguments();
    if(!node.get_children().size())
    {
        if(argument_format.get_children().size() && argument_format.get_children()[0].get_type()!=ast_default_arg && argument_format.get_children()[0].get_type()!=ast_dynamic_id)
        {
            int size=argument_format.get_children().size();
            int sum=0;
            for(int i=0;i<size;++i)
                sum+=(argument_format.get_children()[i].get_type()!=ast_default_arg);
            std::cout<<">> [runtime] call_function: lack at least "<<sum<<" argument(s) but get 0.\n";
            ++error;
            return -1;
        }
        else if(argument_format.get_children().size() && argument_format.get_children()[0].get_type()==ast_dynamic_id)
        {
            // load null dynamic-id
            int vector_value_addr=nasal_vm.gc_alloc();
            nasal_vm.gc_get(vector_value_addr).set_type(vm_vector);
            run_closure.add_new_value(argument_format.get_children()[0].get_str(),vector_value_addr);
        }
        else if(argument_format.get_children().size() && argument_format.get_children()[0].get_type()==ast_default_arg)
        {
            // load default values
            int size=argument_format.get_children().size();
            for(int i=0;i<size;++i)
            {
                int tmp_value_addr=calculation(argument_format.get_children()[i].get_children()[1],local_scope_addr);
                if(tmp_value_addr<0)
                    return -1;
                run_closure.add_new_value(argument_format.get_children()[i].get_children()[0].get_str(),tmp_value_addr);
            }
        }
    }
    else if(node.get_children()[0].get_type()==ast_hashmember)
    {
        std::map<std::string,bool> args_usage_table;        // check arguments in argument_format is correctly used
        std::map<std::string,bool> default_args_table;      // check default arguments
        std::map<std::string,nasal_ast*> default_args_node; // if one of default arguments is not in use,use default value
        // load arguments' name.
        int arg_format_size=argument_format.get_children().size();
        for(int i=0;i<arg_format_size;++i)
        {
            nasal_ast& tmp_node=argument_format.get_children()[i];
            std::string id_name=(tmp_node.get_type()==ast_default_arg? tmp_node.get_children()[0].get_str():tmp_node.get_str());
            args_usage_table[id_name]=false;
            if(tmp_node.get_type()==ast_default_arg)
            {
                default_args_table[id_name]=false;
                default_args_node[id_name]=&tmp_node.get_children()[1];
            }
        }
        // initialize normal arguments.
        int args_size=node.get_children().size();
        for(int i=0;i<args_size;++i)
        {
            nasal_ast& tmp_node=node.get_children()[i];
            std::string id_name=tmp_node.get_children()[0].get_str();
            if(args_usage_table.find(id_name)==args_usage_table.end())
            {
                std::cout<<">> [runtime] call_function: identifier named \'"<<id_name<<"\' does not exist.\n";
                ++error;
                return -1;
            }
            else
            {
                args_usage_table[id_name]=true;
                if(default_args_table.find(id_name)!=default_args_table.end())
                    default_args_table[id_name]=true;
            }
            int value_addr=calculation(tmp_node.get_children()[1],local_scope_addr);
            if(value_addr<0)
                return -1;
            run_closure.add_new_value(id_name,value_addr);
        }
        // use default value if a default-argument haven't been initialized.
        for(std::map<std::string,bool>::iterator i=default_args_table.begin();i!=default_args_table.end();++i)
            if(!i->second)
            {
                int value_addr=calculation(*default_args_node[i->first],local_scope_addr);
                if(value_addr<0)
                    return -1;
                run_closure.add_new_value(i->first,value_addr);
                args_usage_table[i->first]=true;
            }
        // use null vector if dynamic-identifier haven't been initialized.
        if(argument_format.get_children().back().get_type()==ast_dynamic_id)
        {
            std::string dyn_str=argument_format.get_children().back().get_str();
            if(!args_usage_table[dyn_str])
            {
                args_usage_table[dyn_str]=true;
                int vector_value_addr=nasal_vm.gc_alloc();
                nasal_vm.gc_get(vector_value_addr).set_type(vm_vector);
                run_closure.add_new_value(dyn_str,vector_value_addr);
            }
        }
        // check if each argument is initialized.
        for(std::map<std::string,bool>::iterator i=args_usage_table.begin();i!=args_usage_table.end();++i)
            if(!i->second)
            {
                std::cout<<">> [runtime] call_function: argument named \'"<<i->first<<"\' is not in use.\n";
                ++error;
                return -1;
            }
    }
    else
    {
        std::vector<int> args; // store value address of input arguments
        int size=node.get_children().size();
        for(int i=0;i<size;++i)
        {
            int tmp_val_addr=calculation(node.get_children()[i],local_scope_addr);
            if(tmp_val_addr<0)
            {
                std::cout<<">> [runtime] call_function: error value address when generating argument list.\n";
                ++error;
                return -1;
            }
            args.push_back(tmp_val_addr);
        }
        int arg_format_size=argument_format.get_children().size();
        if(size>arg_format_size && argument_format.get_children().back().get_type()!=ast_dynamic_id)
        {
            std::cout<<">> [runtime] call_function: too much arguments.\n";
            ++error;
            return -1;
        }
        for(int i=0;i<arg_format_size;++i)
        {
            nasal_ast& tmp_node=argument_format.get_children()[i];
            if(tmp_node.get_type()==ast_identifier || tmp_node.get_type()==ast_dynamic_id)
            {
                if(tmp_node.get_type()==ast_dynamic_id)
                {
                    int vector_value_addr=nasal_vm.gc_alloc();
                    nasal_vm.gc_get(vector_value_addr).set_type(vm_vector);
                    nasal_vector& ref_vec=nasal_vm.gc_get(vector_value_addr).get_vector();
                    for(int j=i;j<size;++j)
                        ref_vec.add_elem(args[j]);
                    run_closure.add_new_value(tmp_node.get_str(),vector_value_addr);
                    break;
                }
                if(i<size)
                    run_closure.add_new_value(tmp_node.get_str(),args[i]);
                else
                {
                    std::cout<<">> [runtime] call_function: lack argument(s).stop.\n";
                    ++error;
                    return -1;
                }
            }
            else // default_args
            {
                if(i<size)
                    run_closure.add_new_value(tmp_node.get_children()[0].get_str(),args[i]);
                else
                {
                    int tmp_val_addr=calculation(tmp_node.get_children()[1],local_scope_addr);
                    if(tmp_val_addr<0)
                        return -1;
                    run_closure.add_new_value(tmp_node.get_children()[0].get_str(),tmp_val_addr);
                }
            }
        }
    }
    int ret_state=block_progress(reference_of_func.get_run_block(),run_closure_addr,true);
    run_closure.del_scope();

    if(ret_state==rt_break || ret_state==rt_continue)
    {
        std::cout<<">> [runtime] call_function: break and continue are not allowed to be used here.\n";
        ++error;
        return -1;
    }
    else if(ret_state==rt_error)
        return -1;

    if(function_returned_address>=0)
    {
        ret_value_addr=function_returned_address;
        function_returned_address=-1;
    }
    else
    {
        ret_value_addr=nasal_vm.gc_alloc();
        nasal_vm.gc_get(ret_value_addr).set_type(vm_nil);
    }
    return ret_value_addr;
}
int nasal_runtime::call_builtin_function(std::string val_name,int local_scope_addr)
{
    int ret_value_addr=-1;
    int builtin_func_num=-1;
    if(builtin_func_hashmap.find(val_name)!=builtin_func_hashmap.end())
        ret_value_addr=(this->*builtin_func_hashmap[val_name])(local_scope_addr);
    return ret_value_addr;
}
int nasal_runtime::call_scalar_mem(nasal_ast& node,int local_scope_addr)
{
    int mem_address=-1;
    if(node.get_type()==ast_identifier)
    {
        if(local_scope_addr>=0)
            mem_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_mem_address(node.get_str());
        if(mem_address<0)
            mem_address=nasal_vm.gc_get(global_scope_address).get_closure().get_mem_address(node.get_str());
        if(mem_address<0)
        {
            std::cout<<">> [runtime] call_scalar_mem: cannot find value named \'"<<node.get_str()<<"\'.\n";
            ++error;
            return -1;
        }
        return mem_address;
    }
    if(local_scope_addr>=0)
        mem_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_mem_address(node.get_children()[0].get_str());
    if(mem_address<0)
        mem_address=nasal_vm.gc_get(global_scope_address).get_closure().get_mem_address(node.get_children()[0].get_str());
    if(mem_address<0)
    {
        std::cout<<">> [runtime] call_scalar_mem: cannot find value named \'"<<node.get_children()[0].get_str()<<"\'.\n";
        ++error;
        return -1;
    }
    int call_expr_size=node.get_children().size();
    for(int i=1;i<call_expr_size;++i)
    {
        int tmp_mem_addr=-1;
        nasal_ast& call_expr=node.get_children()[i];
        switch(call_expr.get_type())
        {
            case ast_call_vec:  tmp_mem_addr=call_vector_mem(call_expr,mem_address,local_scope_addr);break;
            case ast_call_hash: tmp_mem_addr=call_hash_mem(call_expr,mem_address,local_scope_addr);break;
            case ast_call_func:
                std::cout<<">> [runtime] call_scalar_mem: cannot change the value that function returns.\n";
                ++error;
                return -1;
                break;
        }
        mem_address=tmp_mem_addr;
        if(mem_address<0)
        {
            ++error;
            std::cout<<">> [runtime] call_scalar_mem: cannot find correct memory space.\n";
            break;
        }
    }
    return mem_address;
}
int nasal_runtime::call_vector_mem(nasal_ast& node,int base_mem_addr,int local_scope_addr)
{
    int return_mem_addr=-1;
    int base_value_addr=nasal_vm.mem_get(base_mem_addr);
    int base_value_type=nasal_vm.gc_get(base_value_addr).get_type();
    if(base_value_type!=vm_vector && base_value_type!=vm_hash)
    {
        std::cout<<">> [runtime] call_vector_mem: incorrect value type,expected a vector/hash.\n";
        ++error;
        return -1;
    }
    int call_size=node.get_children().size();
    if(call_size>1)
    {
        std::cout<<">> [runtime] call_vector_mem: when searching a memory space in a vector,only one index is alowed.\n";
        ++error;
        return -1;
    }
    if(base_value_type==vm_vector)
    {
        nasal_vector& reference_value=nasal_vm.gc_get(base_value_addr).get_vector();
        if(node.get_children()[0].get_type()==ast_subvec)
        {
            std::cout<<">> [runtime] call_vector_mem: sub-vector in this progress is a temporary value and cannot be changed.\n";
            ++error;
            return -1;
        }
        int index_value_addr=calculation(node.get_children()[0],local_scope_addr);
        int index_value_type=nasal_vm.gc_get(index_value_addr).get_type();
        if(index_value_type!=vm_number && index_value_type!=vm_string)
        {
            std::cout<<">> [runtime] call_vector_mem: index is not a number/numerable string.\n";
            ++error;
            return -1;
        }
        int index_num=0;
        if(index_value_type==vm_string)
        {
            std::string str=nasal_vm.gc_get(index_value_addr).get_string();
            if(!check_numerable_string(str))
            {
                std::cout<<">> [runtime] call_vector_mem: index is not a numerable string.\n";
                ++error;
                return -1;
            }
            index_num=(int)trans_string_to_number(str);
        }
        else
            index_num=(int)nasal_vm.gc_get(index_value_addr).get_number();
        nasal_vm.del_reference(index_value_addr);
        return_mem_addr=reference_value.get_mem_address(index_num);
    }
    else
    {
        if(call_size>1)
        {
            std::cout<<">> [runtime] call_vector_mem: when calling a hash,only one key is alowed.\n";
            ++error;
            return -1;
        }
        std::string str=node.get_children()[0].get_str();
        return_mem_addr=nasal_vm.gc_get(base_value_addr).get_hash().get_mem_address(str);
    }
    return return_mem_addr;
}
int nasal_runtime::call_hash_mem(nasal_ast& node,int base_mem_addr,int local_scope_addr)
{
    int base_value_addr=nasal_vm.mem_get(base_mem_addr);
    int value_type=nasal_vm.gc_get(base_value_addr).get_type();
    if(value_type!=vm_hash)
    {
        std::cout<<">> [runtime] call_hash_mem: incorrect value type,expected a hash.\n";
        ++error;
        return -1;
    }
    int ret_mem_addr=nasal_vm.gc_get(base_value_addr).get_hash().get_mem_address(node.get_str());
    return ret_mem_addr;
}
int nasal_runtime::calculation(nasal_ast& node,int local_scope_addr)
{
    // after this process, a new address(in nasal_vm.garbage_collector_memory) will be returned
    int ret_address=-1;
    int calculation_type=node.get_type();
    if(calculation_type==ast_nil)
    {
        ret_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(ret_address).set_type(vm_nil);
    }
    else if(calculation_type==ast_number)
        ret_address=number_generation(node);
    else if(calculation_type==ast_string)
        ret_address=string_generation(node);
    else if(calculation_type==ast_identifier)
    {
        if(local_scope_addr>=0)
            ret_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address(node.get_str());
        if(ret_address<0)
            ret_address=nasal_vm.gc_get(global_scope_address).get_closure().get_value_address(node.get_str());
        if(ret_address<0)
        {
            std::cout<<">> [runtime] calculation: cannot find value named \'"<<node.get_str()<<"\'.\n";
            ++error;
            return -1;
        }
        nasal_vm.add_reference(ret_address);
    }
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
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_sub)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_sub(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_mult)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_mult(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_div)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_div(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_link)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_link(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_cmp_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_equal(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_cmp_not_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_not_equal(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_less_than)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_less(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_less_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_less_or_equal(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_greater_than)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_greater(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_greater_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_greater_or_equal(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_and)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_and(left_gc_addr,right_gc_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(left_gc_addr);
        nasal_vm.del_reference(right_gc_addr);
    }
    else if(calculation_type==ast_or)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        if(check_condition(left_gc_addr))
            ret_address=left_gc_addr;
        else
        {
            int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
            ret_address=nasal_scalar_calculator.nasal_scalar_or(left_gc_addr,right_gc_addr);
            // delete the reference of temporary values
            nasal_vm.del_reference(left_gc_addr);
            nasal_vm.del_reference(right_gc_addr);
        }
    }
    else if(calculation_type==ast_unary_not)
    {
        int addr=calculation(node.get_children()[0],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_unary_not(addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(addr);
    }
    else if(calculation_type==ast_unary_sub)
    {
        int addr=calculation(node.get_children()[0],local_scope_addr);
        ret_address=nasal_scalar_calculator.nasal_scalar_unary_sub(addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(addr);
    }
    else if(calculation_type==ast_trinocular)
    {
        int condition_addr=calculation(node.get_children()[0],local_scope_addr);
        if(check_condition(condition_addr))
            ret_address=calculation(node.get_children()[1],local_scope_addr);
        else
            ret_address=calculation(node.get_children()[2],local_scope_addr);
        // delete the reference of temporary values
        nasal_vm.del_reference(condition_addr);
    }
    else if(calculation_type==ast_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        nasal_vm.mem_change(scalar_mem_space,new_scalar_gc_addr);// this progress will delete the reference to old gc_addr in scalar_mem_space
        nasal_vm.add_reference(new_scalar_gc_addr);// this reference is reserved for ret_address
        ret_address=new_scalar_gc_addr;
    }
    else if(calculation_type==ast_add_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_add(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.del_reference(new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);// this progress will delete the reference to old gc_addr in scalar_mem_space
        nasal_vm.add_reference(result_val_address);// this reference is reserved for ret_address
        ret_address=result_val_address;
    }
    else if(calculation_type==ast_sub_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_sub(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.del_reference(new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);// this progress will delete the reference to old gc_addr in scalar_mem_space
        nasal_vm.add_reference(result_val_address);// this reference is reserved for ret_address
        ret_address=result_val_address;
    }
    else if(calculation_type==ast_div_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_div(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.del_reference(new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);// this progress will delete the reference to old gc_addr in scalar_mem_space
        nasal_vm.add_reference(result_val_address);// this reference is reserved for ret_address
        ret_address=result_val_address;
    }
    else if(calculation_type==ast_mult_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_mult(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.del_reference(new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);// this progress will delete the reference to old gc_addr in scalar_mem_space
        nasal_vm.add_reference(result_val_address);// this reference is reserved for ret_address
        ret_address=result_val_address;
    }
    else if(calculation_type==ast_link_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0],local_scope_addr);
        int scalar_val_space=nasal_vm.mem_get(scalar_mem_space);
        int new_scalar_gc_addr=calculation(node.get_children()[1],local_scope_addr);
        int result_val_address=nasal_scalar_calculator.nasal_scalar_link(scalar_val_space,new_scalar_gc_addr);
        nasal_vm.del_reference(new_scalar_gc_addr);
        nasal_vm.mem_change(scalar_mem_space,result_val_address);// this progress will delete the reference to old gc_addr in scalar_mem_space
        nasal_vm.add_reference(result_val_address);// this reference is reserved for ret_address
        ret_address=result_val_address;
    }
    else
    {
        std::cout<<">> [runtime] calculation: this expression cannot be calculated.expression type:"<<ast_str(node.get_type())<<".\n";
        ++error;
        return -1;
    }
    if(ret_address<0)
    {
        std::cout<<">> [runtime] calculation: incorrect values are used in calculation.\n";
        ++error;
    }
    return ret_address;
}
void nasal_runtime::definition(nasal_ast& node,int local_scope_addr)
{
    nasal_ast& define_node=node.get_children()[0];
    nasal_ast& value_node=node.get_children()[1];
    if(define_node.get_type()==ast_identifier && value_node.get_type()==ast_multi_scalar)
    {
        std::cout<<">> [runtime] definition: one identifier cannot accept too many values.\n";
        ++error;
        return;
    }
    if(define_node.get_type()==ast_identifier)
    {
        std::string new_name=define_node.get_str();
        int value_addr=calculation(value_node,local_scope_addr);
        if(value_addr<0)
            return;
        nasal_vm.gc_get(local_scope_addr<0?global_scope_address:local_scope_addr).get_closure().add_new_value(new_name,value_addr);
    }
    else
    {
        std::vector<std::string> identifier_table;
        int id_size=define_node.get_children().size();
        for(int i=0;i<id_size;++i)
            identifier_table.push_back(define_node.get_children()[i].get_str());
        if(value_node.get_type()==ast_multi_scalar)
        {
            int val_size=value_node.get_children().size();
            if(id_size!=val_size)
            {
                std::cout<<">> [runtime] definition: size of identifiers and size of values do not match.\n";
                ++error;
                return;
            }
            for(int i=0;i<val_size;++i)
            {
                int tmp_addr=calculation(value_node.get_children()[i],local_scope_addr);
                nasal_vm.gc_get(local_scope_addr<0?global_scope_address:local_scope_addr).get_closure().add_new_value(identifier_table[i],tmp_addr);
            }
        }
        else
        {
            int value_addr=calculation(value_node,local_scope_addr);
            if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_vector)
            {
                std::cout<<">> [runtime] definition: must use vector in multi-definition.\n";
                ++error;
                return;
            }
            nasal_vector& ref_vector=nasal_vm.gc_get(value_addr).get_vector();
            if(ref_vector.size()!=id_size)
            {
                std::cout<<">> [runtime] definition: size of identifiers and size of values do not match.\n";
                ++error;
                return;
            }
            for(int i=0;i<id_size;++i)
            {
                int tmp_addr=ref_vector.get_value_address(i);
                nasal_vm.gc_get(local_scope_addr<0?global_scope_address:local_scope_addr).get_closure().add_new_value(identifier_table[i],tmp_addr);
            }
            nasal_vm.del_reference(value_addr);
        }
    }
    return;
}
void nasal_runtime::multi_assignment(nasal_ast& node,int local_scope_addr)
{
    nasal_ast& multi_call_node=node.get_children()[0];
    nasal_ast& value_node=node.get_children()[1];
    std::vector<int> mem_table;
    int id_size=multi_call_node.get_children().size();
    for(int i=0;i<id_size;++i)
    {
        nasal_ast& tmp_node=multi_call_node.get_children()[i];
        if(tmp_node.get_type()!=ast_call && tmp_node.get_type()!=ast_identifier)
        {
            std::cout<<">> [runtime] multi_assignment: multi-assignment must use available memory address.\n";
            ++error;
            return;
        }
        mem_table.push_back(call_scalar_mem(tmp_node,local_scope_addr));
    }
    if(value_node.get_type()==ast_multi_scalar)
    {
        int val_size=value_node.get_children().size();
        if(id_size!=val_size)
        {
            std::cout<<">> [runtime] multi_assignment: size of calls and size of values do not match.\n";
            ++error;
            return;
        }
        std::vector<int> value_table;
        for(int i=0;i<val_size;++i)
            value_table.push_back(calculation(value_node.get_children()[i],local_scope_addr));
        for(int i=0;i<val_size;++i)
            nasal_vm.mem_change(mem_table[i],value_table[i]);
    }
    else
    {
        int value_addr=calculation(value_node,local_scope_addr);
        if(value_addr<0)
            return;
        if(nasal_vm.gc_get(value_addr).get_type()!=vm_vector)
        {
            std::cout<<">> [runtime] multi_assignment: must use vector in multi-assignment.\n";
            ++error;
            return;
        }
        nasal_vector& ref_vector=nasal_vm.gc_get(value_addr).get_vector();
        if(ref_vector.size()!=id_size)
        {
            std::cout<<">> [runtime] multi_assignment: size of calls and size of values do not match.\n";
            ++error;
            return;
        }
        std::vector<int> value_table;
        for(int i=0;i<id_size;++i)
        {
            int tmp_addr=ref_vector.get_value_address(i);
            nasal_vm.add_reference(tmp_addr);
            value_table.push_back(tmp_addr);
        }
        for(int i=0;i<id_size;++i)
            nasal_vm.mem_change(mem_table[i],value_table[i]);
        nasal_vm.del_reference(value_addr);
    }
    return;
}

#endif