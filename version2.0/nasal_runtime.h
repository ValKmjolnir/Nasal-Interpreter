#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class nasal_runtime
{
    private:
        // global scope is a hash_map
        // null_local_scope is used when function need reference of local scope
        // but this function is called from global scope
        std::map<std::string,int> global_scope;
        std::list<std::map<std::string,int> > null_local_scope;
        
        // see detail of each enum type in function error_interrupt(const int)
        enum runtime_error_type
        {
            __incorrect_head_of_tree,
            __incorrect_head_of_func,
            __stack_overflow,
        };
        void error_interrupt   (const int);
        void vector_generation (abstract_syntax_tree&);
        void hash_generation   (abstract_syntax_tree&);
        void calculation       (abstract_syntax_tree&);
        void call_identifier   (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void assignment        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void definition        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void loop_expr         (abstract_syntax_tree&);
        void conditional       (abstract_syntax_tree&);
        void func_proc         (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
    public:
        nasal_runtime()
        {
            global_scope.clear();
            nasal_gc.gc_init();
            return;
        }
        ~nasal_runtime()
        {
            global_scope.clear();
            nasal_gc.gc_init();
            return;
        }
        void main_proc(abstract_syntax_tree&);
};

void nasal_runtime::error_interrupt(const int type)
{
    std::string error_head=">> [Runtime] fatal error: ";
    switch (type)
    {
        case __incorrect_head_of_tree:
            std::cout<<error_head<<"the abstract syntax tree's root type is not \'__root\' ."<<std::endl;break;
        case __incorrect_head_of_func:
            std::cout<<error_head<<"called identifier is not a function ."<<std::endl;break;
        case __stack_overflow:
            std::cout<<error_head<<"stack overflow ."<<std::endl;break;
        default:break;
    }
    return;
}

void nasal_runtime::vector_generation(abstract_syntax_tree& node)
{
    return;
}
void nasal_runtime::hash_generation(abstract_syntax_tree& node)
{
    return;
}
void nasal_runtime::calculation(abstract_syntax_tree& node)
{
    return;
}
void nasal_runtime::call_identifier(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    if(local_scope.empty())
        ;
    return;
}
void nasal_runtime::assignment(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    if(local_scope.empty())
        ;
    return;
}
void nasal_runtime::definition(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    if(local_scope.empty())
        ;
    return;
}
void nasal_runtime::loop_expr(abstract_syntax_tree& node)
{
    std::list<std::map<std::string,int> > local_scope;
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);

    return;
}
void nasal_runtime::conditional(abstract_syntax_tree& node)
{
    std::list<std::map<std::string,int> > local_scope;
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    
    return;
}

void nasal_runtime::func_proc(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& func_root)
{
    if(func_root.get_node_type()!=__function)
    {
        error_interrupt(__incorrect_head_of_func);
        return;
    }
    for(std::list<abstract_syntax_tree>::iterator iter=func_root.get_children().back().get_children().begin();iter!=func_root.get_children().back().get_children().end();++iter)
    {
        // use local value node_type to avoid calling function too many times.
        int node_type=iter->get_node_type();
        if(node_type==__number || node_type==__string)
            ;
        // only number or string
        else if(node_type==__id)
            this->call_identifier(local_scope,*iter);
        else if(node_type==__vector)
            this->vector_generation(*iter);
        else if(node_type==__hash)
            this->hash_generation(*iter);
        else if(node_type==__function)
            ;
        else if(node_type==__add_operator  || node_type==__sub_operator ||
                node_type==__mul_operator  || node_type==__div_operator ||
                node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more ||
                node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark)
            ;
        else if(node_type==__equal)
            ;
        else if(node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
            ;
        else if(node_type==__definition)
            ;
        else if(node_type==__conditional)
            ;
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            ;
    }
    return;
}

void nasal_runtime::main_proc(abstract_syntax_tree& root)
{
    time_t begin_time,end_time;
    begin_time=std::time(NULL);
    global_scope.clear();
    nasal_gc.gc_init();
    if(root.get_node_type()!=__root)
    {
        error_interrupt(__incorrect_head_of_tree);
        return;
    }
    for(std::list<abstract_syntax_tree>::iterator iter=root.get_children().begin();iter!=root.get_children().end();++iter)
    {
        // use local value node_type to avoid calling function too many times.
        int node_type=iter->get_node_type();
        if(node_type==__number || node_type==__string)
            ;
        else if(node_type==__id)
            this->call_identifier(null_local_scope,*iter);
        else if(node_type==__vector)
            this->vector_generation(*iter);
        else if(node_type==__hash)
            this->hash_generation(*iter);
        else if(node_type==__function)
        {
            nasal_scalar temp_function;
            temp_function.set_type(scalar_function);
        }
        else if(node_type==__add_operator  || node_type==__sub_operator ||
                node_type==__mul_operator  || node_type==__div_operator ||
                node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more ||
                node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark)
            ;
        else if(node_type==__equal)
            ;
        else if(node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
            ;
        else if(node_type==__definition)
            ;
        else if(node_type==__conditional)
            ;
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            ;
    }

    end_time=std::time(NULL);
    std::cout<<">> [Runtime] process exited after "<<end_time-begin_time<<" s ."<<std::endl;
    return;
}

#endif