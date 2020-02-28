#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class nasal_runtime
{
    private:
        // local hash_map will be used when running
        sym_hash_map global_scope;
        
        // see detail of each enum type in function error_interrupt(const int)
        enum runtime_error_type
        {
            __incorrect_head_of_tree,
            __incorrect_head_of_func,
            __stack_overflow,
        };
        void error_interrupt (const int);
        void delete_total_scope(std::list<std::map<std::string,int> >&);
        void delete_last_scope (std::list<std::map<std::string,int> >&);
        void func_proc       (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void call_identifier (abstract_syntax_tree&);
        void calculation     (abstract_syntax_tree&);
        void assignment      (abstract_syntax_tree&);
        void definition      (abstract_syntax_tree&);
        void loop            (abstract_syntax_tree&);
        void conditional     (abstract_syntax_tree&);
    public:
        nasal_runtime()
        {
            global_scope.set_clear();
            nasal_gc.gc_init();
            return;
        }
        ~nasal_runtime()
        {
            global_scope.set_clear();
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

void nasal_runtime::delete_total_scope(std::list<std::map<std::string,int> >& scope)
{
    for(std::list<std::map<std::string,int> >::iterator i=scope.begin();i!=scope.end();++i)
        for(std::map<std::string,int>::iterator j=i->begin();j!=i->end();++j)
            nasal_gc.reference_delete(j->second);
    scope.clear();
    return;
}

void nasal_runtime::delete_last_scope(std::list<std::map<std::string,int> >& scope)
{
    if(scope.empty())
        return;
    std::list<std::map<std::string,int> >::iterator iter=scope.end();
    --iter;
    for(std::map<std::string,int>::iterator i=iter->begin();i!=iter->end();++i)
        nasal_gc.reference_delete(i->second);
    scope.pop_back();
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
            ;
        else if(node_type==__vector)
            ;
        else if(node_type==__hash)
            ;
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
        else if(node_type==__while)
            ;
        else if(node_type==__for)
            ;
        else if(node_type==__foreach)
            ;
        else if(node_type==__forindex)
            ;
        else if(node_type==__return)
            ;
    }
    return;
}

void nasal_runtime::main_proc(abstract_syntax_tree& root)
{
    time_t begin_time,end_time;
    begin_time=std::time(NULL);
    global_scope.set_clear();
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
        {
            for(std::list<abstract_syntax_tree>::iterator i=iter->get_children().begin();i!=iter->get_children().end();++i)
                ;
        }
        else if(node_type==__vector)
            ;
        else if(node_type==__hash)
            ;
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
        else if(node_type==__while)
            ;
        else if(node_type==__for)
            ;
        else if(node_type==__foreach)
            ;
        else if(node_type==__forindex)
            ;
    }

    end_time=std::time(NULL);
    std::cout<<">> [Runtime] process exited after "<<end_time-begin_time<<" s ."<<std::endl;
    return;
}

#endif