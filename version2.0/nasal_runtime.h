#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class nasal_runtime
{
    private:
        // global scope is a hash_map
        // main_local_scope is used in main block 
        // when calling a loop or conditional expression
        // when adding a new identifier into scope,you need to check if this identifier is already in the scope.
        std::map<std::string,int> global_scope;
        std::list<std::map<std::string,int> > main_local_scope;
        
        // see detail of each enum type in function error_interrupt(const int)
        enum runtime_error_type
        {
            __incorrect_head_of_tree,
            __incorrect_head_of_func,
            __not_numerable_str,
            __error_value_type,
            __stack_overflow,
        };
        int  runtime_error_exit_mark;
        void error_interrupt    (const int,const int);
        int  number_generation  (abstract_syntax_tree&);
        int  string_generation  (abstract_syntax_tree&);
        int  vector_generation  (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  hash_generation    (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  function_generation(std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  calculation        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  call_identifier    (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void definition         (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void loop_expr          (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void conditional        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  func_proc          (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
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

void nasal_runtime::error_interrupt(const int type,const int line)
{
    std::cout<<">> [Runtime] line "<<line<<": ";
    switch (type)
    {
        case __incorrect_head_of_tree:
            std::cout<<"lib hasn\'t been loaded."<<std::endl;break;
        case __incorrect_head_of_func:
            std::cout<<"called identifier is not a function."<<std::endl;break;
        case __not_numerable_str:
            std::cout<<"this string is not a numerable one."<<std::endl;break;
        case __stack_overflow:
            std::cout<<"stack overflow."<<std::endl;break;
        case __error_value_type:
            std::cout<<"operator gets error value type."<<std::endl;break;
        default:
            std::cout<<"unknown error."<<std::endl;break;
    }
    runtime_error_exit_mark=type;
    return;
}

int nasal_runtime::number_generation(abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_number);
    nasal_gc.get_scalar(addr).get_number().set_number(node.get_var_number());
    return addr;
}
int nasal_runtime::string_generation(abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_string);
    nasal_gc.get_scalar(addr).get_string().set_string(node.get_var_string());
    return addr;
}
int nasal_runtime::vector_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_vector);
    std::list<abstract_syntax_tree>::iterator call_node=node.get_children().end();
    for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
    {
        int var_type=i->get_node_type();
        if(var_type==__number)
            nasal_gc.get_scalar(addr).get_vector().vec_push(number_generation(*i));
        else if(var_type==__string)
            nasal_gc.get_scalar(addr).get_vector().vec_push(string_generation(*i));
        else if(var_type==__vector)
            nasal_gc.get_scalar(addr).get_vector().vec_push(vector_generation(local_scope,*i));
        else if(var_type==__hash)
            nasal_gc.get_scalar(addr).get_vector().vec_push(hash_generation(local_scope,*i));
        else if(var_type==__function)
            nasal_gc.get_scalar(addr).get_vector().vec_push(function_generation(local_scope,*i));
        else if(var_type==__id)
            nasal_gc.get_scalar(addr).get_vector().vec_push(call_identifier(local_scope,*i));
        else if(var_type==__add_operator  || var_type==__sub_operator || var_type==__mul_operator  || var_type==__div_operator || var_type==__link_operator ||
                var_type==__cmp_equal || var_type==__cmp_less || var_type==__cmp_more || var_type==__cmp_not_equal || var_type==__cmp_less_or_equal || var_type==__cmp_more_or_equal ||
                var_type==__and_operator || var_type==__or_operator || var_type==__ques_mark ||
                var_type==__equal || var_type==__add_equal || var_type==__sub_equal || var_type==__div_equal || var_type==__mul_equal || var_type==__link_equal)
            nasal_gc.get_scalar(addr).get_vector().vec_push(calculation(local_scope,*i));
        else
        {
            call_node=i;
            break;
        }
    }
    for(;call_node!=node.get_children().end();++call_node)
    {
        ;
    }
    return addr;
}
int nasal_runtime::hash_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_hash);
    nasal_gc.get_scalar(addr).get_hash().set_self_addr(addr);
    std::list<abstract_syntax_tree>::iterator call_node=node.get_children().end();
    for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
    {
        if(i->get_node_type()!=__hash_member)
        {
            call_node=i;
            break;
        }
        else
        {
            std::string member_name=i->get_children().front().get_var_string();
            int var_type=i->get_children().back().get_node_type();
            if(var_type==__number)
                nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,number_generation(*i));
            else if(var_type==__string)
                nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,string_generation(*i));
            else if(var_type==__vector)
                nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,vector_generation(local_scope,*i));
            else if(var_type==__hash)
                nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,hash_generation(local_scope,*i));
            else if(var_type==__function)
            {
                // hash's function must get a parent_hash_addr 
                // this address will be given to identifier 'me'
                nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,function_generation(local_scope,i->get_children().back()));
                nasal_gc.get_scalar(nasal_gc.get_scalar(addr).get_hash().get_hash_member(member_name)).get_function().set_parent_hash_addr(addr);
            }
            else if(var_type==__add_operator  || var_type==__sub_operator || var_type==__mul_operator  || var_type==__div_operator || var_type==__link_operator ||
                    var_type==__cmp_equal || var_type==__cmp_less || var_type==__cmp_more || var_type==__cmp_not_equal || var_type==__cmp_less_or_equal || var_type==__cmp_more_or_equal ||
                    var_type==__and_operator || var_type==__or_operator || var_type==__ques_mark ||
                    var_type==__equal || var_type==__add_equal || var_type==__sub_equal || var_type==__div_equal || var_type==__mul_equal || var_type==__link_equal)
                nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,calculation(local_scope,*i));
        }
    }
    for(;call_node!=node.get_children().end();++call_node)
    {
        ;
    }
    return addr;
}
int nasal_runtime::function_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_function);
    nasal_gc.get_scalar(addr).get_function().set_local_scope(local_scope);
    nasal_gc.get_scalar(addr).get_function().set_statement_block(node);
    return addr;
}
int nasal_runtime::calculation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    // calculation will return a value that points to a new area in memory
    int node_type=node.get_node_type();
    if(node_type==__number)
        return number_generation(node);
    else if(node_type==__string)
        return string_generation(node);
    else if(node_type==__vector)
        return vector_generation(local_scope,node);
    else if(node_type==__hash)
        return hash_generation(local_scope,node);
    else if(node_type==__function)
        return function_generation(local_scope,node);
    else if(node_type==__id)
        return call_identifier(local_scope,node);
    else if(node_type==__add_operator || node_type==__sub_operator || node_type==__mul_operator || node_type==__div_operator)
    {
        // scalar_nil canot be used here
        int ret_addr=-1;
        int addr_1=-1;
        int addr_2=-1;
        if(node.get_children().size()==1)
        {
            // note: sub operator and nor operator may mean that this is unary calculation
            addr_1=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(addr_1).set_type(scalar_number);
            nasal_gc.get_scalar(addr_1).get_number().set_number(0);
            addr_2=calculation(local_scope,node.get_children().front());
        }
        else
        {
            addr_1=calculation(local_scope,node.get_children().front());
            addr_2=calculation(local_scope,node.get_children().back());
        }
        // check if the address is available
        if(addr_1<0 || addr_2<0)
            return -1;
        int type_1=nasal_gc.get_scalar(addr_1).get_type();
        int type_2=nasal_gc.get_scalar(addr_2).get_type();
        double num_1=0;
        double num_2=0;
        if(type_1==scalar_number && type_2==scalar_number)
        {
            num_1=nasal_gc.get_scalar(addr_1).get_number().get_number();
            num_2=nasal_gc.get_scalar(addr_2).get_number().get_number();
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            num_1=nasal_gc.get_scalar(addr_1).get_number().get_number();
            std::string tmp_str=nasal_gc.get_scalar(addr_2).get_string().get_string();
            if(check_numerable_string(tmp_str))
                num_2=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_node_line());
                return -1;
            }
        }
        else if(type_1==scalar_string && type_2==scalar_number)
        {
            std::string tmp_str=nasal_gc.get_scalar(addr_1).get_string().get_string();
            num_2=nasal_gc.get_scalar(addr_2).get_number().get_number();
            if(check_numerable_string(tmp_str))
                num_1=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_node_line());
                return -1;
            }
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            std::string tmp_str_1=nasal_gc.get_scalar(addr_1).get_string().get_string();
            std::string tmp_str_2=nasal_gc.get_scalar(addr_2).get_string().get_string();
            if(check_numerable_string(tmp_str_1) && check_numerable_string(tmp_str_2))
            {
                num_1=trans_string_to_number(tmp_str_1);
                num_2=trans_string_to_number(tmp_str_2);
            }
            else
            {
                error_interrupt(__not_numerable_str,node.get_node_line());
                return -1;
            }
        }
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        ret_addr=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
        switch(node_type)
        {
            case __add_operator:nasal_gc.get_scalar(ret_addr).get_number().set_number(num_1+num_2);break;
            case __sub_operator:nasal_gc.get_scalar(ret_addr).get_number().set_number(num_1-num_2);break;
            case __mul_operator:nasal_gc.get_scalar(ret_addr).get_number().set_number(num_1*num_2);break;
            case __div_operator:nasal_gc.get_scalar(ret_addr).get_number().set_number(num_1/num_2);break;
        }
        nasal_gc.reference_delete(addr_1);
        nasal_gc.reference_delete(addr_2);
        return ret_addr;
    }
    else if(node_type==__link_operator)
    {
        // scalar_nil cannot be used here
        int ret_addr=-1;
        int addr_1=calculation(local_scope,node.get_children().front());
        int addr_2=calculation(local_scope,node.get_children().back());
        // check if the address is available
        if(addr_1<0 || addr_2<0)
            return -1;
        int type_1=nasal_gc.get_scalar(addr_1).get_type();
        int type_2=nasal_gc.get_scalar(addr_2).get_type();
        std::string str_1;
        std::string str_2;
        if(type_1==scalar_number && type_2==scalar_number)
        {
            str_1=trans_number_to_string(nasal_gc.get_scalar(addr_1).get_number().get_number());
            str_2=trans_number_to_string(nasal_gc.get_scalar(addr_2).get_number().get_number());
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            str_1=trans_number_to_string(nasal_gc.get_scalar(addr_1).get_number().get_number());
            str_2=nasal_gc.get_scalar(addr_2).get_string().get_string();
        }
        else if(type_1==scalar_string && type_2==scalar_number)
        {
            str_1=nasal_gc.get_scalar(addr_1).get_string().get_string();
            str_2=trans_number_to_string(nasal_gc.get_scalar(addr_2).get_number().get_number());
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            str_1=nasal_gc.get_scalar(addr_1).get_string().get_string();
            str_2=nasal_gc.get_scalar(addr_2).get_string().get_string();
        }
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        ret_addr=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(ret_addr).set_type(scalar_string);
        nasal_gc.get_scalar(ret_addr).get_string().set_string(str_1+str_2);
        nasal_gc.reference_delete(addr_1);
        nasal_gc.reference_delete(addr_2);
        return ret_addr;
    }
    else if(node_type==__cmp_equal || node_type==__cmp_not_equal || node_type==__cmp_less || node_type==__cmp_less_or_equal || node_type==__cmp_more || node_type==__cmp_more_or_equal)
    {
        int ret_addr=-1;
        int addr_1=calculation(local_scope,node.get_children().front());
        int addr_2=calculation(local_scope,node.get_children().back());
        // check if the address is available
        if(addr_1<0 || addr_2<0)
            return -1;
        int type_1=nasal_gc.get_scalar(addr_1).get_type();
        int type_2=nasal_gc.get_scalar(addr_2).get_type();
        // note: cmp operator change strings into numbers then making comparation
        if(type_1==scalar_number && type_2==scalar_number)
        {
            ;
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            ;
        }
        else if(type_1==scalar_string && type_2==scalar_number)
        {
            ;
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            ;
        }
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        ret_addr=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
        //
        nasal_gc.reference_delete(addr_1);
        nasal_gc.reference_delete(addr_2);
        return ret_addr;
    }
    else if(node_type==__equal)
    {
        ;
    }
    else if(node_type==__add_equal || node_type==__sub_equal || node_type==__mul_equal || node_type==__div_equal || node_type==__link_equal)
    {
        // scalar_nil also cannot be used here
        ;
    }
    else if(node_type==__and_operator || node_type==__or_operator || node_type==__nor_operator)
    {
        // or will return the first value that is not null
        // and will return the last value that is not null
        // nor will return 0 if the value is not null
        // nor will return 1 if the value is null(0,nil,'0',"0")
        // if or gets all 0,or returns 0
        // if and gets a 0,and returns 0
        ;
    }
    return -1;
}
int nasal_runtime::call_identifier(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=-1;
    std::string tmp_id_name=node.get_var_name();
    if(global_scope.find(tmp_id_name)!=global_scope.end())
        addr=global_scope[tmp_id_name];
    for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
        if(iter->find(tmp_id_name)!=iter->end())
            addr=(*iter)[tmp_id_name];
    return -1;
}
void nasal_runtime::definition(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    if(local_scope.empty())
        ;
    return;
}
void nasal_runtime::loop_expr(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);

    return;
}
void nasal_runtime::conditional(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    
    return;
}

int nasal_runtime::func_proc(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& func_root)
{
    if(func_root.get_node_type()!=__function)
    {
        error_interrupt(__incorrect_head_of_func,func_root.get_node_line());
        return -1;
    }
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    // loading parameters
    for(std::list<abstract_syntax_tree>::iterator iter=func_root.get_children().front().get_children().begin();iter!=func_root.get_children().front().get_children().end();++iter)
    {

    }
    // process
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
            this->vector_generation(local_scope,*iter);
        else if(node_type==__hash)
            this->hash_generation(local_scope,*iter);
        else if(node_type==__function)
            this->function_generation(local_scope,*iter);
        else if(node_type==__add_operator  || node_type==__sub_operator || node_type==__mul_operator  || node_type==__div_operator || node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more || node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark ||
                node_type==__equal || node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
            this->calculation(local_scope,*iter);
        else if(node_type==__definition)
            this->definition(local_scope,*iter);
        else if(node_type==__conditional)
            this->conditional(local_scope,*iter);
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            this->loop_expr(local_scope,*iter);
        if(runtime_error_exit_mark>=0)
            break;
    }
    return -1;
}

void nasal_runtime::main_proc(abstract_syntax_tree& root)
{
    time_t begin_time,end_time;
    begin_time=std::time(NULL);

    // initializing global scope and nasal_gc
    // runtime_error_exit_mark is set to -1,if runtime_error_exit_mark >=0,this means an error occurred
    global_scope.clear();
    nasal_gc.gc_init();
    runtime_error_exit_mark=-1;

    if(root.get_node_type()!=__root)
    {
        error_interrupt(__incorrect_head_of_tree,root.get_node_line());
        return;
    }
    for(std::list<abstract_syntax_tree>::iterator iter=root.get_children().begin();iter!=root.get_children().end();++iter)
    {
        // use local value node_type to avoid calling function too many times.
        int node_type=iter->get_node_type();
        if(node_type==__number || node_type==__string)
            ;
        else if(node_type==__id)
            this->call_identifier(main_local_scope,*iter);
        else if(node_type==__vector)
            this->vector_generation(main_local_scope,*iter);
        else if(node_type==__hash)
            this->hash_generation(main_local_scope,*iter);
        else if(node_type==__function)
            this->function_generation(main_local_scope,*iter);
        else if(node_type==__add_operator  || node_type==__sub_operator || node_type==__mul_operator  || node_type==__div_operator || node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more || node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark ||
                node_type==__equal || node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
            this->calculation(main_local_scope,*iter);
        else if(node_type==__definition)
            this->definition(main_local_scope,*iter);
        else if(node_type==__conditional)
            this->conditional(main_local_scope,*iter);
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            this->loop_expr(main_local_scope,*iter);
        if(runtime_error_exit_mark>=0)
            break;
    }

    end_time=std::time(NULL);
    std::cout<<">> [Runtime] process exited after "<<end_time-begin_time<<" s ."<<std::endl;
    return;
}

#endif
