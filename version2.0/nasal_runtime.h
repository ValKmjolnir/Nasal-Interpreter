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
            __undefined_identifier,
            __multi_assign_incorrect_value_number,
            __multi_define_incorrect_value_number,
            __invalid_vector_member,
            __invalid_hash_member,
            __memory_overflow,
            __not_callable_vector,
            __not_callable_hash,
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
        int* get_identifier_addr(std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  call_identifier    (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void definition         (std::list<std::map<std::string,int> >&,std::map<std::string,int>&,abstract_syntax_tree&);
        void loop_expr          (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void conditional        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  func_proc          (std::list<std::map<std::string,int> >&,abstract_syntax_tree&,abstract_syntax_tree&);
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
        case __undefined_identifier:
            std::cout<<"undefined identifier."<<std::endl;break;
        case __multi_assign_incorrect_value_number:
        	std::cout<<"numbers of assigned identifiers and values are not the same."<<std::endl;break;
        case __multi_define_incorrect_value_number:
            std::cout<<"numbers of defined identifiers and values are not the same.";break;
        case __invalid_vector_member:
            std::cout<<"this member of vector does not exist."<<std::endl;break;
        case __invalid_hash_member:
            std::cout<<"this member of hash does not exist."<<std::endl;break;
        case __memory_overflow:
            std::cout<<"memory overflow occurred when calling vector or hash."<<std::endl;break;
        case __not_callable_vector:
            std::cout<<"called a value that is not a vector."<<std::endl;break;
        case __not_callable_hash:
            std::cout<<"called a value that is not a hash."<<std::endl;break;
        case __not_numerable_str:
            std::cout<<"this string is not a numerable one."<<std::endl;break;
        case __stack_overflow:
            std::cout<<"stack overflow."<<std::endl;break;
        case __error_value_type:
            std::cout<<"incorrect value type."<<std::endl;break;
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
        switch(call_node->get_node_type())
        {
            case __call_vector:break;
            case __call_hash:break;
            case __call_function:break;
        }
    }
    return addr;
}
int nasal_runtime::hash_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_hash);
    // self address must be set here
    // if not,then 'me' in function-type member will not get a correct address
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
            // note:how does nasal_function change the address of me when a hash calls it?
            // because before calling this function,hash gives the address if itself as a parameter to the function,
            // and this parameter's name is 'me'
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
                nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,function_generation(local_scope,i->get_children().back()));
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
    // function
    //     parameters
    //     block
    //     calls...
    std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();
    nasal_gc.get_scalar(addr).get_function().set_paramemter_list(*i);
    ++i;
    nasal_gc.get_scalar(addr).get_function().set_statement_block(*i);
    ++i;
    for(;i!=node.get_children().end();++i)
    {
        ;
    }
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
        // scalar_nil cannot be used here
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
                error_interrupt(__not_numerable_str,node.get_children().back().get_node_line());
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
                error_interrupt(__not_numerable_str,node.get_children().front().get_node_line());
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
    else if(node_type==__cmp_equal || node_type==__cmp_not_equal)
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
        ret_addr=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
        if(type_1==scalar_number && type_2==scalar_number)
        {
            double num_1=nasal_gc.get_scalar(addr_1).get_number().get_number();
            double num_2=nasal_gc.get_scalar(addr_2).get_number().get_number();
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type==__cmp_equal?num_1==num_2:num_1!=num_2)));
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            double num_1=nasal_gc.get_scalar(addr_1).get_number().get_number();
            double num_2=0;
            std::string tmp_str=nasal_gc.get_scalar(addr_2).get_string().get_string();
            if(check_numerable_string(tmp_str))
                num_2=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_children().back().get_node_line());
                return -1;
            }
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type==__cmp_equal?num_1==num_2:num_1!=num_2)));
        }
        else if(type_1==scalar_string && type_2==scalar_number)
        {
            double num_1=0;
            double num_2=nasal_gc.get_scalar(addr_2).get_number().get_number();
            std::string tmp_str=nasal_gc.get_scalar(addr_1).get_string().get_string();
            if(check_numerable_string(tmp_str))
                num_1=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_children().front().get_node_line());
                return -1;
            }
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type==__cmp_equal?num_1==num_2:num_1!=num_2)));
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            std::string str_1=nasal_gc.get_scalar(addr_1).get_string().get_string();
            std::string str_2=nasal_gc.get_scalar(addr_2).get_string().get_string();
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type==__cmp_equal?str_1==str_2:str_1!=str_2)));
        }
        else if(type_1==scalar_vector && type_2==scalar_vector)
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type==__cmp_equal?addr_1==addr_2:addr_1!=addr_2)));
        else if(type_1==scalar_hash && type_2==scalar_hash)
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type==__cmp_equal?addr_1==addr_2:addr_1!=addr_2)));
        else if(type_1==scalar_function && type_2==scalar_function)
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type==__cmp_equal?addr_1==addr_2:addr_1!=addr_2)));
        else if(type_1!=type_2)
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type!=__cmp_equal)));
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        nasal_gc.reference_delete(addr_1);
        nasal_gc.reference_delete(addr_2);
        return ret_addr;
    }
    else if(node_type==__cmp_less || node_type==__cmp_less_or_equal || node_type==__cmp_more || node_type==__cmp_more_or_equal)
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
        // only numbers and strings can take these calculations
        ret_addr=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
        if(type_1==scalar_number && type_2==scalar_number)
        {
            double num_1=nasal_gc.get_scalar(addr_1).get_number().get_number();
            double num_2=nasal_gc.get_scalar(addr_2).get_number().get_number();
            switch(node_type)
            {
                case __cmp_less:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<num_2)));break;
                case __cmp_less_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<=num_2)));break;
                case __cmp_more:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>num_2)));break;
                case __cmp_more_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>=num_2)));break;
            }
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            double num_1=nasal_gc.get_scalar(addr_1).get_number().get_number();
            double num_2=0;
            std::string tmp_str=nasal_gc.get_scalar(addr_2).get_string().get_string();
            if(check_numerable_string(tmp_str))
                num_2=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_children().back().get_node_line());
                return -1;
            }
            switch(node_type)
            {
                case __cmp_less:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<num_2)));break;
                case __cmp_less_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<=num_2)));break;
                case __cmp_more:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>num_2)));break;
                case __cmp_more_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>=num_2)));break;
            }
        }
        else if(type_1==scalar_string && type_2==scalar_number)
        {
            double num_1=0;
            double num_2=nasal_gc.get_scalar(addr_2).get_number().get_number();
            std::string tmp_str=nasal_gc.get_scalar(addr_1).get_string().get_string();
            if(check_numerable_string(tmp_str))
                num_1=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_children().front().get_node_line());
                return -1;
            }
            switch(node_type)
            {
                case __cmp_less:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<num_2)));break;
                case __cmp_less_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<=num_2)));break;
                case __cmp_more:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>num_2)));break;
                case __cmp_more_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>=num_2)));break;
            }
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            std::string str_1=nasal_gc.get_scalar(addr_1).get_string().get_string();
            std::string str_2=nasal_gc.get_scalar(addr_2).get_string().get_string();
            switch(node_type)
            {
                case __cmp_less:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(str_1<str_2)));break;
                case __cmp_less_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(str_1<=str_2)));break;
                case __cmp_more:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(str_1>str_2)));break;
                case __cmp_more_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(str_1>=str_2)));break;
            }
        }
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        nasal_gc.reference_delete(addr_1);
        nasal_gc.reference_delete(addr_2);
        return ret_addr;
    }
    else if(node_type==__equal)
    {
        // scalar_function needs 'me' changement here
        // but now it is unfinished
        int child_node_type_1=node.get_children().front().get_node_type();
        int child_node_type_2=node.get_children().back().get_node_type();
        if(child_node_type_1==__multi_scalar)
        {
        	std::vector<int*> assigned_addrs;
        	for(std::list<abstract_syntax_tree>::iterator i=node.get_children().front().get_children().begin();i!=node.get_children().front().get_children().end();++i)
			{
				assigned_addrs.push_back(get_identifier_addr(local_scope,*i));
				if(!assigned_addrs.back())
				{
					assigned_addrs.clear();
					break;
				}
			}
			if(assigned_addrs.empty())
				return -1;
			if(child_node_type_2==__multi_scalar)
			{
				std::vector<int> data_addrs;
				for(std::list<abstract_syntax_tree>::iterator i=node.get_children().back().get_children().begin();i!=node.get_children().back().get_children().end();++i)
				{
                    int new_addr=calculation(local_scope,*i);
                    if(new_addr<0)
                        return -1;
                    data_addrs.push_back(new_addr);
                }
				if(data_addrs.size()!=assigned_addrs.size())
				{
					error_interrupt(__multi_assign_incorrect_value_number,node.get_children().back().get_node_line());
					return -1;
				}
                // multi assignment doesn't have returned value
                // because multi assignment cannot be used in calculation expression
				for(int i=0;i<assigned_addrs.size();++i)
				{
                    int new_data_addr=-1;
                    switch(nasal_gc.get_scalar(data_addrs[i]).get_type())
                    {
                        case scalar_nil:
                            new_data_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_data_addr).set_type(scalar_nil);
                            *assigned_addrs[i]=new_data_addr;
                            break;
                        case scalar_number:
                            new_data_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_data_addr).set_type(scalar_number);
                            nasal_gc.get_scalar(new_data_addr).get_number().deep_copy(nasal_gc.get_scalar(data_addrs[i]).get_number());
                            *assigned_addrs[i]=new_data_addr;
                            break;
                        case scalar_string:
                            new_data_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_data_addr).set_type(scalar_string);
                            nasal_gc.get_scalar(new_data_addr).get_string().deep_copy(nasal_gc.get_scalar(data_addrs[i]).get_string());
                            *assigned_addrs[i]=new_data_addr;
                            break;
                        case scalar_function:
                            new_data_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_data_addr).set_type(scalar_function);
                            nasal_gc.get_scalar(new_data_addr).get_function().deep_copy(nasal_gc.get_scalar(data_addrs[i]).get_function());
                            *assigned_addrs[i]=new_data_addr;
                            break;
                        case scalar_vector:
                        case scalar_hash:
                            *assigned_addrs[i]=data_addrs[i];
                            nasal_gc.reference_add(data_addrs[i]);
                            break;
                    }
                }
                for(int i=0;i<data_addrs.size();++i)
                    nasal_gc.reference_delete(data_addrs[i]);
			}
			else
			{
				int data_addr=calculation(local_scope,node.get_children().back());
                if(data_addr<0)
                    return -1;
				if(nasal_gc.get_scalar(data_addr).get_type()!=scalar_vector)
				{
					error_interrupt(__error_value_type,node.get_children().back().get_node_line());
					return -1;
				}
				else if(nasal_gc.get_scalar(data_addr).get_vector().get_size()!=assigned_addrs.size())
				{
					error_interrupt(__multi_assign_incorrect_value_number,node.get_children().back().get_node_line());
					return -1;
				}
				for(int i=0;i<assigned_addrs.size();++i)
				{
                    int new_data_addr=-1;
                    int vector_member_addr=nasal_gc.get_scalar(data_addr).get_vector().get_elem(i);
                    if(vector_member_addr<0)
                    {
                        error_interrupt(__invalid_vector_member,node.get_children().back().get_node_line());
                        return -1;
                    }
                    switch(nasal_gc.get_scalar(vector_member_addr).get_type())
                    {
                        case scalar_nil:
                            new_data_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_data_addr).set_type(scalar_nil);
                            *assigned_addrs[i]=new_data_addr;
                            break;
                        case scalar_number:
                            new_data_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_data_addr).set_type(scalar_number);
                            nasal_gc.get_scalar(new_data_addr).get_number().deep_copy(nasal_gc.get_scalar(vector_member_addr).get_number());
                            *assigned_addrs[i]=new_data_addr;
                            break;
                        case scalar_string:
                            new_data_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_data_addr).set_type(scalar_string);
                            nasal_gc.get_scalar(new_data_addr).get_string().deep_copy(nasal_gc.get_scalar(vector_member_addr).get_string());
                            *assigned_addrs[i]=new_data_addr;
                            break;
                        case scalar_function:
                            new_data_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_data_addr).set_type(scalar_function);
                            nasal_gc.get_scalar(new_data_addr).get_function().deep_copy(nasal_gc.get_scalar(vector_member_addr).get_function());
                            *assigned_addrs[i]=new_data_addr;
                            break;
                        case scalar_vector:
                        case scalar_hash:
                            *assigned_addrs[i]=vector_member_addr;
                            nasal_gc.reference_add(vector_member_addr);
                            break;
                    }
                }
                nasal_gc.reference_delete(data_addr);
			}
            return -1;
		}
        else
        {
        	int ret_addr=-1;
        	int* assigned_addr=get_identifier_addr(local_scope,node.get_children().front());
        	if(!assigned_addr)
        		return -1;
        	int data_addr=calculation(local_scope,node.get_children().back());
            if(data_addr<0)
                return -1;
        	int new_data_addr=-1;
        	nasal_gc.reference_delete(*assigned_addr);
        	switch(nasal_gc.get_scalar(data_addr).get_type())
        	{
        		case scalar_nil:
        			new_data_addr=nasal_gc.gc_alloc();
        			nasal_gc.get_scalar(new_data_addr).set_type(scalar_nil);
        			*assigned_addr=new_data_addr;
        			nasal_gc.reference_add(new_data_addr);
        			ret_addr=new_data_addr;
        			break;
        		case scalar_number:
        			new_data_addr=nasal_gc.gc_alloc();
        			nasal_gc.get_scalar(new_data_addr).set_type(scalar_number);
        			nasal_gc.get_scalar(new_data_addr).get_number().deep_copy(nasal_gc.get_scalar(data_addr).get_number());
        			*assigned_addr=new_data_addr;
        			nasal_gc.reference_add(new_data_addr);
        			ret_addr=new_data_addr;
        			break;
        		case scalar_string:
        			new_data_addr=nasal_gc.gc_alloc();
        			nasal_gc.get_scalar(new_data_addr).set_type(scalar_string);
        			nasal_gc.get_scalar(new_data_addr).get_string().deep_copy(nasal_gc.get_scalar(data_addr).get_string());
        			*assigned_addr=new_data_addr;
        			nasal_gc.reference_add(new_data_addr);
        			ret_addr=new_data_addr;
        			break;
        		case scalar_function:
        			new_data_addr=nasal_gc.gc_alloc();
        			nasal_gc.get_scalar(new_data_addr).set_type(scalar_function);
        			nasal_gc.get_scalar(new_data_addr).get_function().deep_copy(nasal_gc.get_scalar(data_addr).get_function());
        			*assigned_addr=new_data_addr;
        			nasal_gc.reference_add(new_data_addr);
        			ret_addr=new_data_addr;
        			break;
        		case scalar_vector:
        		case scalar_hash:
        			*assigned_addr=data_addr;
        			nasal_gc.reference_add(data_addr);
        			ret_addr=data_addr;
        			break;
			}
            nasal_gc.reference_delete(data_addr);
        	return ret_addr;
		}
		return -1;
    }
    else if(node_type==__add_equal || node_type==__sub_equal || node_type==__mul_equal || node_type==__div_equal)
    {
        // scalar_nil also cannot be used here
        // += -= *= /= ~= only works on scalar_number and scalar_string
        int id_addr=calculation(local_scope,node.get_children().front());
        int value_addr=calculation(local_scope,node.get_children().back());
        if(id_addr<0 || value_addr<0)
            return -1;
        int type_1=nasal_gc.get_scalar(id_addr).get_type();
        int type_2=nasal_gc.get_scalar(value_addr).get_type();
        if(type_1==scalar_number && type_2==scalar_number)
        {
            double num_1=nasal_gc.get_scalar(id_addr).get_number().get_number();
            double num_2=nasal_gc.get_scalar(value_addr).get_number().get_number();
            switch(node_type)
            {
                case __add_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1+num_2);break;
                case __sub_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1-num_2);break;
                case __mul_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1*num_2);break;
                case __div_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1/num_2);break;
            }
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            double num_1=nasal_gc.get_scalar(id_addr).get_number().get_number();
            double num_2=0;
            std::string tmp_str=nasal_gc.get_scalar(value_addr).get_string().get_string();
            if(check_numerable_string(tmp_str))
                num_2=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_children().back().get_node_line());
                return -1;
            }
            switch(node_type)
            {
                case __add_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1+num_2);break;
                case __sub_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1-num_2);break;
                case __mul_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1*num_2);break;
                case __div_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1/num_2);break;
            }
        }
        else if(type_1==scalar_string && type_2==scalar_number)
        {
            double num_1=0;
            double num_2=nasal_gc.get_scalar(value_addr).get_number().get_number();
            std::string tmp_str=nasal_gc.get_scalar(id_addr).get_string().get_string();
            if(check_numerable_string(tmp_str))
                num_1=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_children().front().get_node_line());
                return -1;
            }
            nasal_gc.get_scalar(id_addr).get_string().set_clear();
            nasal_gc.get_scalar(id_addr).set_type(scalar_number);
            switch(node_type)
            {
                case __add_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1+num_2);break;
                case __sub_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1-num_2);break;
                case __mul_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1*num_2);break;
                case __div_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1/num_2);break;
            }
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            double num_1=0;
            double num_2=0;
            std::string tmp_str_1=nasal_gc.get_scalar(id_addr).get_string().get_string();
            std::string tmp_str_2=nasal_gc.get_scalar(value_addr).get_string().get_string();
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
            nasal_gc.get_scalar(id_addr).get_string().set_clear();
            nasal_gc.get_scalar(id_addr).set_type(scalar_number);
            switch(node_type)
            {
                case __add_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1+num_2);break;
                case __sub_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1-num_2);break;
                case __mul_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1*num_2);break;
                case __div_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1/num_2);break;
            }
        }
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        nasal_gc.reference_delete(value_addr);
        return id_addr;
    }
    else if(node_type==__link_equal)
    {
        // scalar_nil also cannot be used here
        // += -= *= /= ~= only works on scalar_number and scalar_string
        int id_addr=calculation(local_scope,node.get_children().front());
        int value_addr=calculation(local_scope,node.get_children().back());
        if(id_addr<0 || value_addr<0)
            return -1;
        int type_1=nasal_gc.get_scalar(id_addr).get_type();
        int type_2=nasal_gc.get_scalar(value_addr).get_type();
        double num_1,num_2;
        std::string str_1,str_2;
        if(type_1==scalar_number && type_2==scalar_number)
        {
            num_1=nasal_gc.get_scalar(id_addr).get_number().get_number();
            num_2=nasal_gc.get_scalar(value_addr).get_number().get_number();
            str_1=trans_number_to_string(num_1);
            str_2=trans_number_to_string(num_2);
            nasal_gc.get_scalar(id_addr).get_number().set_clear();
            nasal_gc.get_scalar(id_addr).set_type(scalar_string);
            nasal_gc.get_scalar(id_addr).get_string().set_string(str_1+str_2);
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            num_1=nasal_gc.get_scalar(id_addr).get_number().get_number();
            str_1=trans_number_to_string(num_1);
            str_2=nasal_gc.get_scalar(value_addr).get_string().get_string();
            nasal_gc.get_scalar(id_addr).get_number().set_clear();
            nasal_gc.get_scalar(id_addr).set_type(scalar_string);
            nasal_gc.get_scalar(id_addr).get_string().set_string(str_1+str_2);
        }
        else if(type_1==scalar_string && type_2==scalar_number)
        {
            num_2=nasal_gc.get_scalar(value_addr).get_number().get_number();
            str_1=nasal_gc.get_scalar(id_addr).get_string().get_string();
            str_2=trans_number_to_string(num_2);
            nasal_gc.get_scalar(id_addr).get_string().set_string(str_1+str_2);
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            str_1=nasal_gc.get_scalar(id_addr).get_string().get_string();
            str_2=nasal_gc.get_scalar(value_addr).get_string().get_string();
            nasal_gc.get_scalar(id_addr).get_string().set_string(str_1+str_2);
        }
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        nasal_gc.reference_delete(value_addr);
        return id_addr;
    }
    else if(node_type==__and_operator)
    {
        // and will return the last value that is not null
        // if and gets a null,and returns 0
        // vector,hash,function is not allowed here
        int ret_addr=-1;
        int addr=calculation(local_scope,node.get_children().front());
        if(addr<0)
            return -1;
        int type=nasal_gc.get_scalar(addr).get_type();
        if(type==scalar_number)
        {
            double num=nasal_gc.get_scalar(addr).get_number().get_number();
            if(num==0)
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(addr);
                return ret_addr;
            }
            else
                nasal_gc.reference_delete(addr);
        }
        else if(type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(addr).get_string().get_string();
            if(str.length()==0 || (check_numerable_string(str) && trans_string_to_number(str)==0))
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(addr);
                return ret_addr;
            }
            else
                nasal_gc.reference_delete(addr);
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().front().get_node_line());
            return -1;
        }
        addr=calculation(local_scope,node.get_children().back());
        if(addr<0)
            return -1;
        type=nasal_gc.get_scalar(addr).get_type();
        if(type==scalar_number)
        {
            double num=nasal_gc.get_scalar(addr).get_number().get_number();
            if(num==0)
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(addr);
                return ret_addr;
            }
            else
                ret_addr=addr;
        }
        else if(type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(addr).get_string().get_string();
            if(str.length()==0 || (check_numerable_string(str) && trans_string_to_number(str)==0))
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(addr);
                return ret_addr;
            }
            else
                ret_addr=addr;
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().front().get_node_line());
            return -1;
        }
        return ret_addr;
    }
    else if(node_type==__or_operator)
    {
        // or will return the first value that is not null
        // if or gets all null(0,'0',nil,"0",'',""),or returns 0
        // vector,hash,function is not allowed here
        int ret_addr=-1;
        int addr=calculation(local_scope,node.get_children().front());
        if(addr<0)
            return -1;
        int type=nasal_gc.get_scalar(addr).get_type();
        if(type==scalar_number)
        {
            double num=nasal_gc.get_scalar(addr).get_number().get_number();
            if(num!=0)
                return addr;
            else
                nasal_gc.reference_delete(addr);
        }
        else if(type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(addr).get_string().get_string();
            if(str.length()!=0 && (!check_numerable_string(str) || !trans_string_to_number(str)==0))
                return addr;
            else
                nasal_gc.reference_delete(addr);
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().front().get_node_line());
            return -1;
        }
        addr=calculation(local_scope,node.get_children().back());
        if(addr<0)
            return -1;
        type=nasal_gc.get_scalar(addr).get_type();
        if(type==scalar_number)
        {
            double num=nasal_gc.get_scalar(addr).get_number().get_number();
            if(num==0)
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(addr);
                return ret_addr;
            }
            else
                return addr;
        }
        else if(type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(addr).get_string().get_string();
            if(str.length()==0 || (check_numerable_string(str) && trans_string_to_number(str)==0))
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(addr);
                return ret_addr;
            }
            else
                return addr;
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().front().get_node_line());
            return -1;
        }
        return -1;
    }
    else if(node_type==__nor_operator)
    {
        // nor will return 0 if the value is not null
        // nor will return 1 if the value is null(0,nil,'0',"0",'',"")
        // vector,hash,function is not allowed here
        int ret_addr=-1;
        int addr=calculation(local_scope,node.get_children().front());
        int type=nasal_gc.get_scalar(addr).get_type();
        if(type==scalar_number)
        {
            double num=nasal_gc.get_scalar(addr).get_number().get_number();
            ret_addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num==0)));
        }
        else if(type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(addr).get_string().get_string();
            ret_addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
            if(str.length()==0 || (check_numerable_string(str) && trans_string_to_number(str)==0))
                nasal_gc.get_scalar(ret_addr).get_number().set_number(1);
            else
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().front().get_node_line());
            return -1;
        }
        nasal_gc.reference_delete(addr);
        return ret_addr;
    }
    else if(node_type==__ques_mark)
    {
    	// ?:
    	// this will return the first element if the condition is true
    	// this will return the second element if the condition is null(0,nil,"0",'0',"",'',"0x0","0o0")
        int condition_addr=calculation(local_scope,node.get_children().front());
        int condition_type=nasal_gc.get_scalar(condition_addr).get_type();
        std::list<abstract_syntax_tree>::iterator ptr=node.get_children().begin();
        ++ptr;
        if(condition_type==scalar_number && nasal_gc.get_scalar(condition_addr).get_number().get_number()==0)
        {
        	++ptr;
        	nasal_gc.reference_delete(condition_addr);
        	return calculation(local_scope,*ptr);
		}
		else if(condition_type==scalar_string
				&& check_numerable_string(nasal_gc.get_scalar(condition_addr).get_string().get_string())
				&& trans_string_to_number(nasal_gc.get_scalar(condition_addr).get_string().get_string())==0)
		{
			++ptr;
        	nasal_gc.reference_delete(condition_addr);
        	return calculation(local_scope,*ptr);
		}
		else
		{
			nasal_gc.reference_delete(condition_addr);
        	return calculation(local_scope,*ptr);
		}
		return -1;
    }
    return -1;
}
int* nasal_runtime::get_identifier_addr(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int* ret_addr=NULL;
    return ret_addr;
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
    if(addr<0)
    {
        error_interrupt(__undefined_identifier,node.get_node_line());
        return -1;
    }
    int last_hash_addr=-1;
    for(std::list<abstract_syntax_tree>::iterator iter=node.get_children().begin();iter!=node.get_children().end();++iter)
    {
        if(nasal_gc.get_scalar(addr).get_type()==scalar_hash)
            last_hash_addr=addr;
        int node_type=iter->get_node_type();
        if(node_type==__call_vector)
        {
            int called_type=nasal_gc.get_scalar(addr).get_type();
            if(called_type!=scalar_vector && called_type!=scalar_hash)
            {
                error_interrupt(__error_value_type,iter->get_node_line());
                return -1;
            }
            int data_addr=calculation(local_scope,iter->get_children().front());
            if(called_type==scalar_vector)
            {
                ;
            }
            else if(called_type==scalar_hash)
            {
                
                if(nasal_gc.get_scalar(data_addr).get_type()!=scalar_string)
                {
                    error_interrupt(__error_value_type,iter->get_node_line());
                    return -1;
                }
                std::string key_str=nasal_gc.get_scalar(data_addr).get_string().get_string();
                addr=nasal_gc.get_scalar(addr).get_hash().get_hash_member(key_str);
                if(addr<0)
                {
                    error_interrupt(__invalid_hash_member,iter->get_node_line());
                    return -1;
                }   
            }
        }
        else if(node_type==__call_hash)
        {
            if(nasal_gc.get_scalar(addr).get_type()!=scalar_hash)
            {
                error_interrupt(__error_value_type,iter->get_node_line());
                return -1;
            }
            addr=nasal_gc.get_scalar(addr).get_hash().get_hash_member(iter->get_var_name());
            if(addr<0)
            {
                error_interrupt(__invalid_hash_member,iter->get_node_line());
                return -1;
            }
        }
        else if(node_type==__call_function)
            ;
    }
    // after calculation or assignment/definition,reference counter of this address will -1
    // to avoid nasal_gc collecting this address,reference counter+1 here
    nasal_gc.reference_add(addr);
    return addr;
}
void nasal_runtime::definition(std::list<std::map<std::string,int> >&local_scope,std::map<std::string,int>& now_scope,abstract_syntax_tree& node)
{
    if(node.get_children().front().get_node_type()==__multi_id)
    {
        std::vector<std::string> id_names;
        for(std::list<abstract_syntax_tree>::iterator i=node.get_children().front().get_children().begin();i!=node.get_children().front().get_children().end();++i)
            id_names.push_back(i->get_var_name());
        int child_node_type=node.get_children().back().get_node_type();
        if(child_node_type==__multi_scalar)
        {
            std::vector<int> data_addrs;
            for(std::list<abstract_syntax_tree>::iterator i=node.get_children().back().get_children().begin();i!=node.get_children().back().get_children().end();++i)
            {
                int new_addr=calculation(local_scope,*i);
                if(new_addr<0)
                    return;
                data_addrs.push_back(new_addr);
            }
            if(data_addrs.size()!=id_names.size())
            {
                error_interrupt(__multi_define_incorrect_value_number,node.get_children().back().get_node_line());
                return;
            }
            for(int i=0;i<id_names.size();++i)
            {
                int new_data_addr=-1;
                switch(nasal_gc.get_scalar(data_addrs[i]).get_type())
                {
                    case scalar_nil:
                        new_data_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_data_addr).set_type(scalar_nil);
                        break;
                    case scalar_number:
                        new_data_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_data_addr).set_type(scalar_number);
                        nasal_gc.get_scalar(new_data_addr).get_number().deep_copy(nasal_gc.get_scalar(data_addrs[i]).get_number());
                        break;
                    case scalar_string:
                        new_data_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_data_addr).set_type(scalar_string);
                        nasal_gc.get_scalar(new_data_addr).get_string().deep_copy(nasal_gc.get_scalar(data_addrs[i]).get_string());
                        break;
                    case scalar_function:
                        new_data_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_data_addr).set_type(scalar_function);
                        nasal_gc.get_scalar(new_data_addr).get_function().deep_copy(nasal_gc.get_scalar(data_addrs[i]).get_function());
                        break;
                    case scalar_vector:
                    case scalar_hash:
                        new_data_addr=data_addrs[i];
                        nasal_gc.reference_add(data_addrs[i]);
                        break;
                }
                now_scope[id_names[i]]=new_data_addr;
            }
            for(int i=0;i<data_addrs.size();++i)
                nasal_gc.reference_delete(data_addrs[i]);
        }
        else
        {
            int data_addr=calculation(local_scope,node.get_children().back());
            if(data_addr<0)
                return;
            if(nasal_gc.get_scalar(data_addr).get_type()!=scalar_vector)
            {
                error_interrupt(__error_value_type,node.get_children().back().get_node_line());
                return;
            }
            else if(nasal_gc.get_scalar(data_addr).get_vector().get_size()!=id_names.size())
            {
                error_interrupt(__multi_define_incorrect_value_number,node.get_children().back().get_node_line());
                return;
            }
            for(int i=0;i<id_names.size();++i)
            {
                int new_data_addr=-1;
                int vector_member_addr=nasal_gc.get_scalar(data_addr).get_vector().get_elem(i);
                if(vector_member_addr<0)
                {
                    error_interrupt(__invalid_vector_member,node.get_children().back().get_node_line());
                    return;
                }
                switch(nasal_gc.get_scalar(vector_member_addr).get_type())
                {
                    case scalar_nil:
                        new_data_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_data_addr).set_type(scalar_nil);
                        break;
                    case scalar_number:
                        new_data_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_data_addr).set_type(scalar_number);
                        nasal_gc.get_scalar(new_data_addr).get_number().deep_copy(nasal_gc.get_scalar(vector_member_addr).get_number());
                        break;
                    case scalar_string:
                        new_data_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_data_addr).set_type(scalar_string);
                        nasal_gc.get_scalar(new_data_addr).get_string().deep_copy(nasal_gc.get_scalar(vector_member_addr).get_string());
                        break;
                    case scalar_function:
                        new_data_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_data_addr).set_type(scalar_function);
                        nasal_gc.get_scalar(new_data_addr).get_function().deep_copy(nasal_gc.get_scalar(vector_member_addr).get_function());
                        break;
                    case scalar_vector:
                    case scalar_hash:
                        new_data_addr=vector_member_addr;
                        nasal_gc.reference_add(vector_member_addr);
                        break;
                }
                now_scope[id_names[i]]=new_data_addr;
            }
            nasal_gc.reference_delete(data_addr);
        }
    }
    else
    {
        std::string id_name=node.get_children().front().get_var_name();
        if(now_scope.find(id_name)!=now_scope.end())
            nasal_gc.reference_delete(now_scope[id_name]);
        int data_addr=calculation(local_scope,node.get_children().back());
        if(data_addr<0)
            return;
        int new_data_addr=-1;
        switch(nasal_gc.get_scalar(data_addr).get_type())
        {
            case scalar_nil:
                new_data_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(new_data_addr).set_type(scalar_nil);
                break;
            case scalar_number:
                new_data_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(new_data_addr).set_type(scalar_number);
                nasal_gc.get_scalar(new_data_addr).get_number().deep_copy(nasal_gc.get_scalar(data_addr).get_number());
                break;
            case scalar_string:
                new_data_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(new_data_addr).set_type(scalar_string);
                nasal_gc.get_scalar(new_data_addr).get_string().deep_copy(nasal_gc.get_scalar(data_addr).get_string());
                break;
            case scalar_function:
                new_data_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(new_data_addr).set_type(scalar_function);
                nasal_gc.get_scalar(new_data_addr).get_function().deep_copy(nasal_gc.get_scalar(data_addr).get_function());
                break;
            case scalar_vector:
            case scalar_hash:
                new_data_addr=data_addr;
                nasal_gc.reference_add(new_data_addr);
                break;
        }
        nasal_gc.reference_delete(data_addr);
        now_scope[id_name]=new_data_addr;
    }
    return;
}
void nasal_runtime::loop_expr(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    int loop_type=node.get_node_type();
    if(loop_type==__while)
    {
        ;
    }
    else if(loop_type==__foreach)
    {
        ;
    }
    else if(loop_type==__forindex)
    {
        ;
    }
    else if(loop_type==__for)
    {
        ;
    }
    for(std::map<std::string,int>::iterator i=local_scope.back().begin();i!=local_scope.back().end();++i)
        nasal_gc.reference_delete(i->second);
    local_scope.pop_back();
    return;
}
void nasal_runtime::conditional(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
    {
        int condition_node_type=i->get_node_type();
        if(condition_node_type==__if)
            ;
        else if(condition_node_type==__elsif)
            ;
        else
            ;
    }
    for(std::map<std::string,int>::iterator i=local_scope.back().begin();i!=local_scope.back().end();++i)
        nasal_gc.reference_delete(i->second);
    local_scope.pop_back();
    return;
}

int nasal_runtime::func_proc(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& parameter_list,abstract_syntax_tree& func_root)
{
    if(func_root.get_node_type()!=__function)
    {
        error_interrupt(__incorrect_head_of_func,func_root.get_node_line());
        return -1;
    }
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    // loading parameters
    for(std::list<abstract_syntax_tree>::iterator iter=parameter_list.get_children().begin();iter!=parameter_list.get_children().end();++iter)
    {

    }
    // process
    for(std::list<abstract_syntax_tree>::iterator iter=func_root.get_children().begin();iter!=func_root.get_children().end();++iter)
    {
        // use local value node_type to avoid calling function too many times.
        int node_type=iter->get_node_type();
        if(node_type==__number || node_type==__string)
            ;
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
            this->definition(local_scope,local_scope.back(),*iter);
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
            this->definition(main_local_scope,global_scope,*iter);
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
