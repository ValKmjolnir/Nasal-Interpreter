#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

enum runtime_returned_state
{
    rt_return=1,
    rt_error,
    rt_break,
    rt_continue,
    rt_exit_without_error
};

class nasal_runtime
{
private:
    std::map<std::string,int (*)(int x)> builtin_func_hashmap;
    // function_return_address is an address in garbage_collector_memory
    int function_returned_address;
    // global_scope_address is an address in garbage_collector_memory
    int global_scope_address;

    nasal_ast root;

    // if error occurred,this value will add 1
    int error;
    void die(int,std::string);

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
    void main_progress();
    // function/loop/conditional expression block running process
    int block_progress(nasal_ast&,int);
    // run loop
    void before_for_loop(nasal_ast&,int);
    void after_each_for_loop(nasal_ast&,int);
    int loop_progress(nasal_ast&,int);
    // run conditional
    bool check_condition(int);
    int conditional_progress(nasal_ast&,int);
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
void nasal_runtime::die(int line,std::string info)
{
    ++error;
    std::cout<<">> [runtime] line "<<line<<": "+info<<".\n";
    return;
}
void nasal_runtime::load_builtin_function()
{
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
    // this state is reserved for builtin_die
    builtin_die_state=0;

    this->error=0;
    this->function_returned_address=-1;

    this->global_scope_address=nasal_vm.gc_alloc(vm_closure);
    nasal_vm.gc_get(global_scope_address).get_closure().add_scope();

    time_t begin_time=std::time(NULL);
    main_progress();
    time_t end_time=std::time(NULL);

    nasal_vm.gc_get(global_scope_address).get_closure().del_scope();
    nasal_vm.del_reference(global_scope_address);
    nasal_vm.clear();

    time_t total_run_time=end_time-begin_time;
    if(total_run_time>=1)
    std::cout<<">> [runtime] process exited after "<<total_run_time<<"s.\n";
    return;
}

// private functions
int nasal_runtime::vector_generation(nasal_ast& node,int local_scope_addr)
{
    int new_addr=nasal_vm.gc_alloc(vm_vector);
    nasal_vector& ref_of_this_vector=nasal_vm.gc_get(new_addr).get_vector();

    int node_children_size=node.get_children().size();
    for(int i=0;i<node_children_size;++i)
        ref_of_this_vector.add_elem(calculation(node.get_children()[i],local_scope_addr));
    return new_addr;
}
int nasal_runtime::hash_generation(nasal_ast& node,int local_scope_addr)
{
    int new_addr=nasal_vm.gc_alloc(vm_hash);
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
    int new_addr=nasal_vm.gc_alloc(vm_function);
    nasal_function& ref_of_this_function=nasal_vm.gc_get(new_addr).get_func();

    ref_of_this_function.set_arguments(node.get_children()[0]);
    ref_of_this_function.set_run_block(node.get_children()[1]);

    if(local_scope_addr>=0)
        ref_of_this_function.set_closure_addr(local_scope_addr);
    else
    {
        int new_closure=nasal_vm.gc_alloc(vm_closure);
        ref_of_this_function.set_closure_addr(new_closure);
        nasal_vm.del_reference(new_closure);
    }
    return new_addr;
}
void nasal_runtime::main_progress()
{
    int expr_number=root.get_children().size();
    int process_returned_value_addr=-1;
    for(int i=0;i<expr_number;++i)
    {
        nasal_ast& node=root.get_children()[i];
        int node_type=node.get_type();
        switch(node_type)
        {
            case ast_definition:definition(node,-1);break;
            case ast_multi_assign:multi_assignment(node,-1);break;
            case ast_conditional:conditional_progress(node,-1);break;
            case ast_while:case ast_for:case ast_forindex:case ast_foreach:
                loop_progress(node,-1);break;
            case ast_nil:case ast_number:case ast_string:case ast_function:break;
            case ast_identifier:
            case ast_vector:case ast_hash:
            case ast_call:
            case ast_equal:case ast_add_equal:case ast_sub_equal:case ast_mult_equal:case ast_div_equal:case ast_link_equal:
            case ast_unary_sub:case ast_unary_not:
            case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
            case ast_trinocular:nasal_vm.del_reference(calculation(node,-1));break;
        }
        if(error)
            break;
    }
    return;
}
int nasal_runtime::block_progress(nasal_ast& node,int local_scope_addr)
{
    int ret_state=rt_exit_without_error;
    // if local_scope does not exist,create a new one.
    if(local_scope_addr<0)
    {
        local_scope_addr=nasal_vm.gc_alloc(vm_closure);
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
            case ast_conditional:ret_state=conditional_progress(tmp_node,local_scope_addr);break;
            case ast_while:case ast_for:case ast_forindex:case ast_foreach:
                ret_state=loop_progress(tmp_node,local_scope_addr);break;
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
                if(tmp_node.get_children().size())
                    function_returned_address=calculation(tmp_node.get_children()[0],local_scope_addr);
                else
                    function_returned_address=nasal_vm.gc_alloc(vm_nil);
                break;
        }
        if(error || ret_state==rt_break || ret_state==rt_continue || ret_state==rt_return)
            break;
    }
    nasal_vm.gc_get(local_scope_addr).get_closure().del_scope();
    nasal_vm.del_reference(local_scope_addr);
    return ret_state;
}
void nasal_runtime::before_for_loop(nasal_ast& node,int local_scope_addr)
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
        default:die(node.get_line(),"cannot use this expression before for-loop");break;
    }
    return;
}
void nasal_runtime::after_each_for_loop(nasal_ast& node,int local_scope_addr)
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
        default:die(node.get_line(),"cannot use this expression after each for-loop");break;
    }
    return;
}
int nasal_runtime::loop_progress(nasal_ast& node,int local_scope_addr)
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
            while_local_scope_addr=nasal_vm.gc_alloc(vm_closure);
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
            ret_state=block_progress(run_block_node,local_scope_addr);
            if(ret_state==rt_break || ret_state==rt_return || error)
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
            die(vector_node.get_line(),"must use vector in forindex/foreach");
            return rt_error;
        }
        // create a new local scope to store iterator if local_scope_addr=-1
        int forei_local_scope_addr=local_scope_addr;
        if(forei_local_scope_addr<0)
        {
            forei_local_scope_addr=nasal_vm.gc_alloc(vm_closure);
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
            int new_value_addr=nasal_vm.gc_alloc(vm_nil);
            std::string val_name=iter_node.get_children()[0].get_str();
            nasal_vm.gc_get(forei_local_scope_addr).get_closure().add_new_value(val_name,new_value_addr);
            mem_addr=nasal_vm.gc_get(forei_local_scope_addr).get_closure().get_mem_address(val_name);
        }
        else
            mem_addr=call_scalar_mem(iter_node,local_scope_addr);
        if(mem_addr<0)
        {
            die(iter_node.get_line(),"get null iterator");
            return rt_error;
        }
        // ref_vector's size may change when running,so this loop will check size each time
        nasal_vector& ref_vector=nasal_vm.gc_get(vector_value_addr).get_vector();
        for(int i=0;i<ref_vector.size();++i)
        {
            // update iterator
            if(loop_type==ast_forindex)
            {
                int new_iter_val_addr=nasal_vm.gc_alloc(vm_number);
                nasal_vm.gc_get(new_iter_val_addr).set_number((double)i);
                nasal_vm.mem_change(mem_addr,new_iter_val_addr);
            }
            else
            {
                int value_addr=ref_vector.get_value_address(i);
                nasal_vm.add_reference(value_addr);
                nasal_vm.mem_change(mem_addr,value_addr);
            }
            ret_state=block_progress(run_block_node,forei_local_scope_addr);
            if(ret_state==rt_break || ret_state==rt_return || error)
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
            for_local_scope_addr=nasal_vm.gc_alloc(vm_closure);
            nasal_vm.gc_get(for_local_scope_addr).get_closure().add_scope();
        }
        else
        {
            nasal_vm.add_reference(local_scope_addr);
            nasal_vm.gc_get(for_local_scope_addr).get_closure().add_scope();
        }
        // for progress
        before_for_loop(before_loop_node,for_local_scope_addr);
        int condition_value_addr=calculation(condition_node,for_local_scope_addr);
        bool result=check_condition(condition_value_addr);
        nasal_vm.del_reference(condition_value_addr);
        while(result)
        {
            if(error)
                break;
            ret_state=block_progress(run_block_node,for_local_scope_addr);
            if(ret_state==rt_return || ret_state==rt_break || error)
                break;
            after_each_for_loop(each_loop_do_node,for_local_scope_addr);
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
    if(type==vm_nil || type==vm_vector || type==vm_hash || type==vm_function)
        return false;
    else if(type==vm_string)
    {
        std::string str=nasal_vm.gc_get(value_addr).get_string();
        double number=trans_string_to_number(str);
        if(std::isnan(number))
            return str.length()!=0;
        return (number!=0);
    }
    else if(type==vm_number)
        return (nasal_vm.gc_get(value_addr).get_number()!=0);
    return false;
}
int nasal_runtime::conditional_progress(nasal_ast& node,int local_scope_addr)
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
            {
                die(tmp_node.get_children()[0].get_line(),"cannot check condition of this value");
                return rt_error;
            }
            bool result=check_condition(condition_value_addr);
            nasal_vm.del_reference(condition_value_addr);
            if(result)
            {
                ret_state=block_progress(tmp_node.get_children()[1],local_scope_addr);
                break;
            }
        }
        else
        {
            ret_state=block_progress(tmp_node.get_children()[0],local_scope_addr);
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
            else
            {
                if(builtin_func_hashmap.find(val_name)!=builtin_func_hashmap.end())
                    die(node.get_children()[0].get_line(),"call "+val_name+" failed");
                else
                    die(node.get_children()[0].get_line()," cannot find \""+val_name+"\"");
                return -1;
            }
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
        die(node.get_line(),"cannot find elements by index in nil/number/function");
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
                    die(subvec_node.get_children()[0].get_line(),"begin index must be nil/number/string");
                    return -1;
                }
                if(end_value_type!=vm_nil && end_value_type!=vm_number && end_value_type!=vm_string)
                {
                    die(subvec_node.get_children()[1].get_line(),"end index must be nil/number/string");
                    return -1;
                }
                if(begin_value_type==vm_string)
                {
                    std::string str=nasal_vm.gc_get(begin_value_addr).get_string();
                    double number=trans_string_to_number(str);
                    if(std::isnan(number))
                    {
                        die(subvec_node.get_children()[0].get_line(),"begin index is not a numerable string");
                        return -1;
                    }
                    begin_index=(int)number;
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
                    double number=trans_string_to_number(str);
                    if(std::isnan(number))
                    {
                        die(subvec_node.get_children()[1].get_line(),"end index is not a numerable string");
                        return -1;
                    }
                    begin_index=(int)number;
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
                else if(!begin_is_nil && !end_is_nil && begin_index>=end_index)
                {
                    die(subvec_node.get_children()[0].get_line(),"begin index is greater than end index");
                    return -1;
                }
                for(int i=begin_index;i<=end_index;++i)
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
                    die(node.get_children()[i].get_line(),"index must be number/string");
                    return -1;
                }
                int index_num=0;
                if(index_value_type==vm_string)
                {
                    std::string str=nasal_vm.gc_get(index_value_addr).get_string();
                    double number=trans_string_to_number(str);
                    if(std::isnan(number))
                    {
                        die(node.get_children()[i].get_line(),"index is not a numerable string");
                        return -1;
                    }
                    index_num=(int)number;
                }
                else
                    index_num=(int)nasal_vm.gc_get(index_value_addr).get_number();
                nasal_vm.del_reference(index_value_addr);
                called_value_addrs.push_back(reference_value.get_value_address(index_num));
            }
        }
        // generate sub-vector
        if(call_size==1 && node.get_children()[0].get_type()!=ast_subvec)
        {
            int value_addr=called_value_addrs[0];
            nasal_vm.add_reference(value_addr);
            return_value_addr=value_addr;
        }
        else
        {
            return_value_addr=nasal_vm.gc_alloc(vm_vector);
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
            die(node.get_line(),"use only one key to call a hash");
            return -1;
        }
        nasal_ast& tmp=node.get_children()[0];
        if(tmp.get_type()==ast_subvec)
        {
            die(tmp.get_line(),"cannot slice hash");
            return -1;
        }
        int str_addr=calculation(tmp,local_scope_addr);
        if(str_addr<0 || nasal_vm.gc_get(str_addr).get_type()!=vm_string)
        {
            die(tmp.get_line(),"must use string as the key");
            return -1;
        }
        std::string str=nasal_vm.gc_get(str_addr).get_string();
        int value_addr=nasal_vm.gc_get(base_value_addr).get_hash().get_value_address(str);
        nasal_vm.add_reference(value_addr);
        return_value_addr=value_addr;
    }
    else
    {
        if(call_size>1)
        {
            die(node.get_line(),"use only one index to call a string");
            return -1;
        }
        nasal_ast& tmp=node.get_children()[0];
        if(tmp.get_type()==ast_subvec)
        {
            die(tmp.get_line(),"cannot slice string (use substr)");
            return -1;
        }
        int index_value_addr=calculation(tmp,local_scope_addr);
        int index_value_type=nasal_vm.gc_get(index_value_addr).get_type();
        if(index_value_type!=vm_number && index_value_type!=vm_string)
        {
            die(tmp.get_line(),"index is not a number/numerable string");
            return -1;
        }
        int index_num=0;
        if(index_value_type==vm_string)
        {
            std::string str=nasal_vm.gc_get(index_value_addr).get_string();
            double number=trans_string_to_number(str);
            if(std::isnan(number))
            {
                die(tmp.get_line(),"index is not a numerable string");
                return -1;
            }
            index_num=(int)number;
        }
        else
            index_num=(int)nasal_vm.gc_get(index_value_addr).get_number();
        nasal_vm.del_reference(index_value_addr);
        return_value_addr=nasal_vm.gc_alloc(vm_number);
        std::string str=nasal_vm.gc_get(base_value_addr).get_string();
        int str_size=str.length();
        if(index_num>=str_size || index_num<-str_size)
        {
            die(tmp.get_line(),"index out of range");
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
        die(node.get_line(),"called a value that is not a hash");
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
        die(node.get_line(),"called a value that is not a function");
        return -1;
    }
    nasal_function& reference_of_func=nasal_vm.gc_get(base_value_addr).get_func();
    int run_closure_addr=reference_of_func.get_closure_addr();
    nasal_closure& run_closure=nasal_vm.gc_get(run_closure_addr).get_closure();
    run_closure.add_scope();
    // set hash.me
    if(last_call_hash_addr>=0)
    {
        nasal_vm.add_reference(last_call_hash_addr);
        run_closure.add_new_value("me",last_call_hash_addr);
    }
    else if(func_name.length())
    {
        // when hash.me does not exist,set self
        nasal_vm.add_reference(base_value_addr);
        run_closure.add_new_value(func_name,base_value_addr);
    }
    nasal_ast& argument_format=reference_of_func.get_arguments();
    if(!node.get_children().size())
    {
        if(argument_format.get_children().size() && argument_format.get_children()[0].get_type()!=ast_default_arg && argument_format.get_children()[0].get_type()!=ast_dynamic_id)
        {
            die(node.get_line(),"lack argument(s)");
            return -1;
        }
        else if(argument_format.get_children().size() && argument_format.get_children()[0].get_type()==ast_dynamic_id)
        {
            // load null dynamic-id
            int vector_value_addr=nasal_vm.gc_alloc(vm_vector);
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
                die(tmp_node.get_line(),"argument named \""+id_name+"\" does not exist");
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
                int vector_value_addr=nasal_vm.gc_alloc(vm_vector);
                run_closure.add_new_value(dyn_str,vector_value_addr);
            }
        }
        // check if each argument is initialized.
        for(std::map<std::string,bool>::iterator i=args_usage_table.begin();i!=args_usage_table.end();++i)
            if(!i->second)
            {
                die(node.get_line(),"argument named \""+i->first+"\" is not in use");
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
                return -1;
            args.push_back(tmp_val_addr);
        }
        int arg_format_size=argument_format.get_children().size();
        if(size>arg_format_size && argument_format.get_children().back().get_type()!=ast_dynamic_id)
        {
            die(node.get_line(),"too much arguments");
            return -1;
        }
        for(int i=0;i<arg_format_size;++i)
        {
            nasal_ast& tmp_node=argument_format.get_children()[i];
            if(tmp_node.get_type()==ast_identifier || tmp_node.get_type()==ast_dynamic_id)
            {
                if(tmp_node.get_type()==ast_dynamic_id)
                {
                    int vector_value_addr=nasal_vm.gc_alloc(vm_vector);
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
                    die(node.get_line(),"lack argument(s)");
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
    block_progress(reference_of_func.get_run_block(),run_closure_addr);
    run_closure.del_scope();

    if(function_returned_address>=0)
    {
        ret_value_addr=function_returned_address;
        function_returned_address=-1;
    }
    else
        ret_value_addr=nasal_vm.gc_alloc(vm_nil);
    return ret_value_addr;
}
int nasal_runtime::call_builtin_function(std::string val_name,int local_scope_addr)
{
    int ret_value_addr=-1;
    int builtin_func_num=-1;
    if(builtin_func_hashmap.find(val_name)!=builtin_func_hashmap.end())
    {
        ret_value_addr=(*builtin_func_hashmap[val_name])(local_scope_addr);
        error+=builtin_die_state;
    }
    return ret_value_addr;
}
int nasal_runtime::call_scalar_mem(nasal_ast& node,int local_scope_addr)
{
    int mem_address=-1;
    if(node.get_type()==ast_identifier)
    {
        std::string id_name=node.get_str();
        if(local_scope_addr>=0)
            mem_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_mem_address(id_name);
        if(mem_address<0)
            mem_address=nasal_vm.gc_get(global_scope_address).get_closure().get_mem_address(id_name);
        if(mem_address<0)
        {
            die(node.get_line(),"cannot find \""+id_name+"\"");
            return -1;
        }
        return mem_address;
    }
    std::string id_name=node.get_children()[0].get_str();
    if(local_scope_addr>=0)
        mem_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_mem_address(id_name);
    if(mem_address<0)
        mem_address=nasal_vm.gc_get(global_scope_address).get_closure().get_mem_address(id_name);
    if(mem_address<0)
    {
        die(node.get_children()[0].get_line(),"cannot find \""+id_name+"\"");
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
                die(call_expr.get_line(),"cannot change function returned value");
                return -1;
                break;
        }
        mem_address=tmp_mem_addr;
        if(mem_address<0)
        {
            die(call_expr.get_line(),"incorrect memory space");
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
        die(node.get_line(),"incorrect value type,must be vector/hash");
        return -1;
    }
    int call_size=node.get_children().size();
    if(call_size>1)
    {
        die(node.get_line(),"only one index/key is allowed to get memory space of vector/hash");
        return -1;
    }
    if(base_value_type==vm_vector)
    {
        nasal_vector& reference_value=nasal_vm.gc_get(base_value_addr).get_vector();
        nasal_ast& tmp=node.get_children()[0];
        if(tmp.get_type()==ast_subvec)
        {
            die(tmp.get_line(),"temporary sliced vector");
            return -1;
        }
        int index_value_addr=calculation(tmp,local_scope_addr);
        int index_value_type=nasal_vm.gc_get(index_value_addr).get_type();
        if(index_value_type!=vm_number && index_value_type!=vm_string)
        {
            die(tmp.get_line(),"index is not a number/numerable string");
            return -1;
        }
        int index_num=0;
        if(index_value_type==vm_string)
        {
            std::string str=nasal_vm.gc_get(index_value_addr).get_string();
            double number=trans_string_to_number(str);
            if(std::isnan(number))
            {
                die(tmp.get_line(),"index is not a numerable string");
                return -1;
            }
            index_num=(int)number;
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
            die(node.get_line(),"use only one key to call a hash");
            return -1;
        }
        nasal_ast& tmp=node.get_children()[0];
        if(tmp.get_type()==ast_subvec)
        {
            die(tmp.get_line(),"cannot slice hash");
            return -1;
        }
        int str_addr=calculation(tmp,local_scope_addr);
        if(str_addr<0 || nasal_vm.gc_get(str_addr).get_type()!=vm_string)
        {
            die(tmp.get_line(),"must use string as the key");
            return -1;
        }
        std::string str=nasal_vm.gc_get(str_addr).get_string();
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
        die(node.get_line(),"called a value that is not a hash");
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
    if(calculation_type==ast_null)
    {
        ret_address=nasal_vm.gc_alloc(vm_number);
        nasal_vm.gc_get(ret_address).set_number(1);
    }
    else if(calculation_type==ast_nil)
        ret_address=nasal_vm.gc_alloc(vm_nil);
    else if(calculation_type==ast_number)
    {
        ret_address=nasal_vm.gc_alloc(vm_number);
        nasal_vm.gc_get(ret_address).set_number(node.get_num());
    }
    else if(calculation_type==ast_string)
    {
        ret_address=nasal_vm.gc_alloc(vm_string);
        nasal_vm.gc_get(ret_address).set_string(node.get_str());
    }
    else if(calculation_type==ast_identifier)
    {
        if(local_scope_addr>=0)
            ret_address=nasal_vm.gc_get(local_scope_addr).get_closure().get_value_address(node.get_str());
        if(ret_address<0)
            ret_address=nasal_vm.gc_get(global_scope_address).get_closure().get_value_address(node.get_str());
        if(ret_address<0)
        {
            die(node.get_line(),"cannot find \""+node.get_str()+"\"");
            ret_address=nasal_vm.gc_alloc(vm_nil);
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
        if(!check_condition(left_gc_addr))
        {
            nasal_vm.del_reference(left_gc_addr);
            ret_address=nasal_vm.gc_alloc(vm_number);
            nasal_vm.gc_get(ret_address).set_number(0);
        }
        else
        {
            int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
            if(!check_condition(right_gc_addr))
            {
                nasal_vm.del_reference(left_gc_addr);
                nasal_vm.del_reference(right_gc_addr);
                ret_address=nasal_vm.gc_alloc(vm_number);
                nasal_vm.gc_get(ret_address).set_number(0);
            }
            else
            {
                nasal_vm.del_reference(left_gc_addr);
                ret_address=right_gc_addr;
            }
        }
    }
    else if(calculation_type==ast_or)
    {
        int left_gc_addr=calculation(node.get_children()[0],local_scope_addr);
        if(check_condition(left_gc_addr))
            ret_address=left_gc_addr;
        else
        {
            int right_gc_addr=calculation(node.get_children()[1],local_scope_addr);
            if(check_condition(right_gc_addr))
            {
                nasal_vm.del_reference(left_gc_addr);
                ret_address=right_gc_addr;
            }
            else
            {
                nasal_vm.del_reference(left_gc_addr);
                nasal_vm.del_reference(right_gc_addr);
                ret_address=nasal_vm.gc_alloc(vm_number);
                nasal_vm.gc_get(ret_address).set_number(0);
            }
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
        die(node.get_line(),"cannot calculate expression: "+ast_str(node.get_type()));
        ret_address=nasal_vm.gc_alloc(vm_nil);
    }
    return ret_address;
}
void nasal_runtime::definition(nasal_ast& node,int local_scope_addr)
{
    nasal_ast& define_node=node.get_children()[0];
    nasal_ast& value_node=node.get_children()[1];
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
                die(value_node.get_line(),"size of identifiers and size of values do not match");
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
                die(value_node.get_line(),"must use vector in multi-definition");
                return;
            }
            nasal_vector& ref_vector=nasal_vm.gc_get(value_addr).get_vector();
            if(ref_vector.size()!=id_size)
            {
                die(value_node.get_line(),"size of identifiers and size of values do not match");
                return;
            }
            for(int i=0;i<id_size;++i)
            {
                int tmp_addr=ref_vector.get_value_address(i);
                nasal_vm.add_reference(tmp_addr);
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
        mem_table.push_back(call_scalar_mem(tmp_node,local_scope_addr));
    }
    if(value_node.get_type()==ast_multi_scalar)
    {
        int val_size=value_node.get_children().size();
        if(id_size!=val_size)
        {
            die(value_node.get_line(),"size of calls and size of values do not match");
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
        if(value_addr<0 || nasal_vm.gc_get(value_addr).get_type()!=vm_vector)
        {
            die(value_node.get_line(),"must use vector in multi-assignment");
            return;
        }
        nasal_vector& ref_vector=nasal_vm.gc_get(value_addr).get_vector();
        if(ref_vector.size()!=id_size)
        {
            die(value_node.get_line(),"size of calls and size of values do not match");
            return;
        }
        // use value_table to store values temprorarily
        // so that (a,b)=(b,a) can run correctly
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