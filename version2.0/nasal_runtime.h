#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__
#define NAS_LIB_FUNC_NUM 32
std::string inline_func_name[NAS_LIB_FUNC_NUM]=
{
    //base.nas
    "nasal_call_builtin_push_back",// append
    "nasal_call_builtin_set_size", // setsize
    "nasal_call_builtin_subvec",
    "nasal_call_builtin_contains",
    "nasal_call_builtin_delete",
    "nasal_call_builtin_trans_int",
    "nasal_call_builtin_trans_num",
    "nasal_call_builtin_get_keys",
    "nasal_call_builtin_pop_back",
    "nasal_call_builtin_sizeof",
    "nasal_call_builtin_str_cmp_equal",
    "nasal_call_builtin_cmp",
    "nasal_call_builtin_cpp_sort",
    "nasal_call_builtin_substr",
    "nasal_call_builtin_sprintf",
    "nasal_call_builtin_find_first_occur",
    "nasal_call_builtin_split",
    "nasal_call_builtin_rand",
    "nasal_call_builtin_get_id",
    //bits.nas
    "nasal_call_builtin_bitcalc",
    "nasal_call_builtin_sbitcalc",
    "nasal_call_builtin_setbit",
    "nasal_call_builtin_null_string_gen",
    //io.nas
    "nasal_call_builtin_c_std_puts",// print
    //math.nas
    "nasal_call_builtin_sin",
    "nasal_call_builtin_cos",
    "nasal_call_builtin_tan",
    "nasal_call_builtin_exp",
    "nasal_call_builtin_cpp_math_ln",
    "nasal_call_builtin_cpp_math_sqrt",
    "nasal_call_builtin_cpp_atan2",
    //system.nas
    "nasal_call_builtin_scalar_type"//system.type
};

class nasal_runtime
{
    private:
        // global scope is a hash_map
        // main_local_scope is used in main block 
        // when calling a loop or conditional expression
        // when adding a new identifier into scope,you need to check if this identifier is already in the scope.
        std::map<std::string,int> global_scope;
        std::list<std::map<std::string,int> > main_local_scope;

        // enum of state type used by loop/conditional
        enum state_stack_member_type
        {
            __state_error = 0,
            __state_no_operation,
            __state_continue,
            __state_break,
            __state_return,
        };
        
        // see detail of each enum type in function error_interrupt(const int)
        enum runtime_error_type
        {
            __incorrect_head_of_tree,
            __undefined_identifier,
            __multi_assign_incorrect_value_number,
            __multi_define_incorrect_value_number,
            __invalid_vector_member,
            __invalid_hash_member,
            __class_inheritence_need_vector,
            __memory_overflow,
            __not_callable_vector,
            __not_callable_hash,
            __not_callable_function,
            __not_numerable_str,
            __error_value_type,
            __error_value_type_when_calling_vector,
            __error_value_type_when_calling_hash,
            __special_call_vector_negative_value,
            __special_call_vector_too_large_value,
            __normal_call_vector_too_large_value,
            __function_returned_value_be_assigned,
            __subvec_value_be_assigned,
            __call_function_lack_para,
            __forindex_foreach_not_vector,
            __break_not_used_in_loop,
            __continue_not_used_in_loop,
            __return_not_used_in_function,
            __stack_overflow,
        };
        int  runtime_error_exit_mark;
        int  function_returned_addr;
        void error_interrupt    (const int,const int);
        int  number_generation  (abstract_syntax_tree&);
        int  string_generation  (abstract_syntax_tree&);
        // sub-process of call_identifier/vector_generation/hash_generation/function_generation
        int  bracket_call_vector(std::list<std::map<std::string,int> >&,std::list<abstract_syntax_tree>::iterator&,int);
        int  dot_call_hash      (std::list<std::map<std::string,int> >&,std::list<abstract_syntax_tree>::iterator&,int);
        int  call_function      (std::list<std::map<std::string,int> >&,std::list<abstract_syntax_tree>::iterator&,int,int);

        int  vector_generation  (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  hash_generation    (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  function_generation(std::list<std::map<std::string,int> >&,abstract_syntax_tree&);

        void update_closure     (std::vector<int>&,int);
        bool check_condition    (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  calculation        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  assignment         (std::list<std::map<std::string,int> >&,abstract_syntax_tree&,int);
        int  call_identifier    (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void definition         (std::list<std::map<std::string,int> >&,std::map<std::string,int>&,abstract_syntax_tree&);
        int  loop_expr          (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  conditional        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  block_proc         (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  func_proc          (std::list<std::map<std::string,int> >&,std::list<std::map<std::string,int> >&,abstract_syntax_tree&,abstract_syntax_tree&,abstract_syntax_tree&,int);// checked
        int  builtin_function   (std::list<std::map<std::string,int> >&,std::string);
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
        case __class_inheritence_need_vector:
            std::cout<<"class inheritence needs vector type."<<std::endl;break;
        case __memory_overflow:
            std::cout<<"memory overflow occurred when calling vector or hash."<<std::endl;break;
        case __not_callable_vector:
            std::cout<<"called a value that is not a vector."<<std::endl;break;
        case __not_callable_hash:
            std::cout<<"called a value that is not a hash."<<std::endl;break;
        case __not_callable_function:
            std::cout<<"called a value that is not a function."<<std::endl;break;
        case __not_numerable_str:
            std::cout<<"this string is not a numerable one."<<std::endl;break;
        case __error_value_type:
            std::cout<<"incorrect value type."<<std::endl;break;
        case __error_value_type_when_calling_vector:
            std::cout<<"incorrect value type when calling a vector."<<std::endl;break;
        case __error_value_type_when_calling_hash:
            std::cout<<"incorrect value type when calling a hash."<<std::endl;break;
        case __special_call_vector_negative_value:
            std::cout<<"negative value occurred when calling sub-vector."<<std::endl;break;
        case __special_call_vector_too_large_value:
            std::cout<<"the number used to call the sub-vector is too large(over 0x7fffffff)."<<std::endl;break;
        case __normal_call_vector_too_large_value:
            std::cout<<"the number used to call the vector is too large(over 0x7fffffff)."<<std::endl;break;
        case __function_returned_value_be_assigned:
            std::cout<<"cannot assign a value that function returns."<<std::endl;break;
        case __subvec_value_be_assigned:
            std::cout<<"cannot assign a subvec value."<<std::endl;break;
        case __call_function_lack_para:
            std::cout<<"lack parameter(s) when calling a function."<<std::endl;break;
        case __forindex_foreach_not_vector:
            std::cout<<"forindex and foreach need vector here."<<std::endl;break;
        case __break_not_used_in_loop:
            std::cout<<"break must be used in a loop."<<std::endl;break;
        case __continue_not_used_in_loop:
            std::cout<<"continue must be used in a loop."<<std::endl;break;
        case __return_not_used_in_function:
            std::cout<<"return expr must be used in a function."<<std::endl;break;
        case __stack_overflow:
            std::cout<<"stack overflow."<<std::endl;break;
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
int  nasal_runtime::bracket_call_vector(std::list<std::map<std::string,int> >& local_scope,std::list<abstract_syntax_tree>::iterator& call_node,int addr)
{
    // check the scalar type of called identifier here
    int called_type=nasal_gc.get_scalar(addr).get_type();
    if(call_node->get_children().size()==1 && called_type!=scalar_vector && called_type!=scalar_hash)
    {
        error_interrupt(__error_value_type_when_calling_vector,call_node->get_node_line());
        return -1;
    }
    if(call_node->get_children().size()>1 && called_type!=scalar_vector)
    {
        error_interrupt(__error_value_type_when_calling_vector,call_node->get_node_line());
        return -1;
    }
    if(call_node->get_children().size()==1)
    {
        if(call_node->get_children().front().get_node_type()==__sub_vector)
        {
            if(called_type==scalar_hash)
            {
                error_interrupt(__not_callable_vector,call_node->get_node_line());
                return -1;
            }
            int num1_addr=-1;
            int num2_addr=-1;
            // identifier[num1:];
            if(call_node->get_children().front().get_children().size()==1)
            {
                num1_addr=calculation(local_scope,call_node->get_children().front().get_children().front());
                if(num1_addr<0)
                    return -1;
            }
            // identifier[num1:num2];
            else
            {
                num1_addr=calculation(local_scope,call_node->get_children().front().get_children().front());
                num2_addr=calculation(local_scope,call_node->get_children().front().get_children().back());
                if(num1_addr<0 || num2_addr<0)
                    return -1;
            }
            if(nasal_gc.get_scalar(num1_addr).get_type()!=scalar_number
                && nasal_gc.get_scalar(num1_addr).get_type()!=scalar_string)
            {
                error_interrupt(__error_value_type_when_calling_vector,call_node->get_children().front().get_children().front().get_node_line());
                return -1;
            }
            if(num2_addr>=0
                && (nasal_gc.get_scalar(num2_addr).get_type()!=scalar_number
                && nasal_gc.get_scalar(num2_addr).get_type()!=scalar_string
                && nasal_gc.get_scalar(num2_addr).get_type()!=scalar_nil))
            {
                error_interrupt(__error_value_type_when_calling_vector,call_node->get_children().front().get_children().back().get_node_line());
                return -1;
            }
            if(nasal_gc.get_scalar(num1_addr).get_type()==scalar_string)
            {
                if(check_numerable_string(nasal_gc.get_scalar(num1_addr).get_string().get_string()))
                {
                    double tmp_num=trans_string_to_number(nasal_gc.get_scalar(num1_addr).get_string().get_string());
                    if(tmp_num<0)
                    {
                        error_interrupt(__special_call_vector_negative_value,call_node->get_children().front().get_children().front().get_node_line());
                        return -1;
                    }
                    nasal_gc.reference_delete(num1_addr);
                    num1_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(num1_addr).set_type(scalar_number);
                    nasal_gc.get_scalar(num1_addr).get_number().set_number(tmp_num);
                }
                else
                {
                    error_interrupt(__not_numerable_str,call_node->get_children().front().get_children().front().get_node_line());
                    return -1;
                }
            }
            if(num2_addr>=0 && nasal_gc.get_scalar(num2_addr).get_type()==scalar_string)
            {
                if(check_numerable_string(nasal_gc.get_scalar(num2_addr).get_string().get_string()))
                {
                    double tmp_num=trans_string_to_number(nasal_gc.get_scalar(num2_addr).get_string().get_string());
                    if(tmp_num<0)
                    {
                        error_interrupt(__special_call_vector_negative_value,call_node->get_children().front().get_children().back().get_node_line());
                        return -1;
                    }
                    nasal_gc.reference_delete(num2_addr);
                    num2_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(num2_addr).set_type(scalar_number);
                    nasal_gc.get_scalar(num2_addr).get_number().set_number(tmp_num);
                }
                else
                {
                    error_interrupt(__not_numerable_str,call_node->get_children().front().get_children().back().get_node_line());
                    return -1;
                }
            }
            if(nasal_gc.get_scalar(num1_addr).get_number().get_number()>2147483647)
            {
                error_interrupt(__special_call_vector_too_large_value,call_node->get_children().front().get_children().front().get_node_line());
                return -1;
            }
            if(num2_addr>=0 && nasal_gc.get_scalar(num2_addr).get_number().get_number()>2147483647)
            {
                error_interrupt(__special_call_vector_too_large_value,call_node->get_children().front().get_children().back().get_node_line());
                return -1;
            }
            int begin_num=(int)nasal_gc.get_scalar(num1_addr).get_number().get_number();
            int end_num=0;
            if(num2_addr<0 || nasal_gc.get_scalar(num2_addr).get_type()==scalar_nil)
                end_num=nasal_gc.get_scalar(addr).get_vector().get_size();
            else
                end_num=(int)nasal_gc.get_scalar(num2_addr).get_number().get_number();
            if(num1_addr>=0)
                nasal_gc.reference_delete(num1_addr);
            if(num2_addr>=0)
                nasal_gc.reference_delete(num2_addr);
            std::vector<int> subvec_result;
            for(int i=begin_num;i<end_num;++i)
            {
                // addr used here
                int tmp_data_addr=nasal_gc.get_scalar(addr).get_vector().get_elem(i);
                int new_addr=-1;
                if(tmp_data_addr<0)
                {
                    error_interrupt(__invalid_vector_member,call_node->get_children().front().get_children().front().get_node_line());
                    return -1;
                }
                switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
                {
                    case scalar_nil:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                        break;
                    case scalar_number:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                        nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
                        break;
                    case scalar_string:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                        nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
                        break;
                    case scalar_function:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                        nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
                        break;
                    case scalar_vector:
                    case scalar_hash:
                        new_addr=tmp_data_addr;
                        nasal_gc.reference_add(new_addr);
                        break;
                }
                subvec_result.push_back(new_addr);
            }
            int tmp_addr=addr;
            addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(addr).set_type(scalar_vector);
            for(int i=0;i<subvec_result.size();++i)
                nasal_gc.get_scalar(addr).get_vector().vec_push(subvec_result[i]);
            nasal_gc.reference_delete(tmp_addr);
        }// end sub-vector
        else
        {
            // normal vector/hash calling
            int data_addr=calculation(local_scope,call_node->get_children().front());
            if(data_addr<0)
                return -1;
            if(nasal_gc.get_scalar(data_addr).get_type()!=scalar_number && nasal_gc.get_scalar(data_addr).get_type()!=scalar_string)
            {
                error_interrupt(__error_value_type_when_calling_vector,call_node->get_children().front().get_node_line());
                return -1;
            }
            if(called_type==scalar_vector)
            {
                double place_num=0;
                if(nasal_gc.get_scalar(data_addr).get_type()==scalar_string)
                {
                    if(check_numerable_string(nasal_gc.get_scalar(data_addr).get_string().get_string()))
                        place_num=(int)trans_string_to_number(nasal_gc.get_scalar(data_addr).get_string().get_string());
                    else
                    {
                        error_interrupt(__not_numerable_str,call_node->get_children().front().get_node_line());
                        return -1;
                    }
                }
                else if(nasal_gc.get_scalar(data_addr).get_type()==scalar_number)
                    place_num=nasal_gc.get_scalar(data_addr).get_number().get_number();
                if(place_num>2147483647 || place_num<-2147483648)
                {
                    error_interrupt(__normal_call_vector_too_large_value,call_node->get_children().front().get_node_line());
                    return -1;
                }
                int tmp_data_addr=nasal_gc.get_scalar(addr).get_vector().get_elem((int)place_num);
                if(tmp_data_addr<0)
                {
                    error_interrupt(__invalid_vector_member,call_node->get_children().front().get_node_line());
                    return -1;
                }
                int new_addr=-1;
                switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
                {
                    case scalar_nil:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                        break;
                    case scalar_number:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                        nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
                        break;
                    case scalar_string:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                        nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
                        break;
                    case scalar_function:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                        nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
                        break;
                    case scalar_vector:
                    case scalar_hash:
                        new_addr=tmp_data_addr;
                        nasal_gc.reference_add(new_addr);
                        break;
                }
                int tmp_addr=addr;
                addr=new_addr;
                nasal_gc.reference_delete(tmp_addr);
            }
            else if(called_type==scalar_hash)
            {
                if(nasal_gc.get_scalar(data_addr).get_type()!=scalar_string)
                {
                    error_interrupt(__error_value_type_when_calling_hash,call_node->get_children().front().get_node_line());
                    return -1;
                }
                int tmp_data_addr=nasal_gc.get_scalar(addr).get_hash().get_hash_member(nasal_gc.get_scalar(data_addr).get_string().get_string());
                if(tmp_data_addr<0)
                {
                    error_interrupt(__invalid_hash_member,call_node->get_children().front().get_node_line());
                    return -1;
                }
                int new_addr=-1;
                switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
                {
                    case scalar_nil:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                        break;
                    case scalar_number:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                        nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
                        break;
                    case scalar_string:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                        nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
                        break;
                    case scalar_function:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                        nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
                        break;
                    case scalar_vector:
                    case scalar_hash:
                        new_addr=tmp_data_addr;
                        nasal_gc.reference_add(new_addr);
                        break;
                }
                int tmp_addr=addr;
                addr=new_addr;
                nasal_gc.reference_delete(tmp_addr);
            }
            nasal_gc.reference_delete(data_addr);
        }
    }
    else if(call_node->get_children().size()>1)
    {
        int generated_subvec_addr=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(generated_subvec_addr).set_type(scalar_vector);
        for(std::list<abstract_syntax_tree>::iterator i=call_node->get_children().begin();i!=call_node->get_children().end();++i)
        {
            if(i->get_node_type()==__sub_vector)
            {
                if(called_type==scalar_hash)
                {
                    error_interrupt(__not_callable_vector,i->get_node_line());
                    return -1;
                }
                int num1_addr=-1;
                int num2_addr=-1;
                // identifier[num1:];
                if(i->get_children().size()==1)
                {
                    num1_addr=calculation(local_scope,i->get_children().front());
                    if(num1_addr<0)
                        return -1;
                }
                // identifier[num1:num2];
                else
                {
                    num1_addr=calculation(local_scope,i->get_children().front());
                    num2_addr=calculation(local_scope,i->get_children().back());
                    if(num1_addr<0 || num2_addr<0)
                        return -1;
                }
                if(nasal_gc.get_scalar(num1_addr).get_type()!=scalar_number
                    && nasal_gc.get_scalar(num1_addr).get_type()!=scalar_string)
                {
                    error_interrupt(__error_value_type_when_calling_vector,i->get_children().front().get_node_line());
                    return -1;
                }
                if(num2_addr>=0
                    && (nasal_gc.get_scalar(num2_addr).get_type()!=scalar_number
                    && nasal_gc.get_scalar(num2_addr).get_type()!=scalar_string
                    && nasal_gc.get_scalar(num2_addr).get_type()!=scalar_nil))
                {
                    error_interrupt(__error_value_type_when_calling_vector,i->get_children().back().get_node_line());
                    return -1;
                }
                if(nasal_gc.get_scalar(num1_addr).get_type()==scalar_string)
                {
                    if(check_numerable_string(nasal_gc.get_scalar(num1_addr).get_string().get_string()))
                    {
                        double tmp_num=trans_string_to_number(nasal_gc.get_scalar(num1_addr).get_string().get_string());
                        if(tmp_num<0)
                        {
                            error_interrupt(__special_call_vector_negative_value,i->get_children().front().get_node_line());
                            return -1;
                        }
                        nasal_gc.reference_delete(num1_addr);
                        num1_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(num1_addr).set_type(scalar_number);
                        nasal_gc.get_scalar(num1_addr).get_number().set_number(tmp_num);
                    }
                    else
                    {
                        error_interrupt(__not_numerable_str,i->get_children().front().get_node_line());
                        return -1;
                    }
                }
                if(num2_addr>=0 && nasal_gc.get_scalar(num2_addr).get_type()==scalar_string)
                {
                    if(check_numerable_string(nasal_gc.get_scalar(num2_addr).get_string().get_string()))
                    {
                        double tmp_num=trans_string_to_number(nasal_gc.get_scalar(num2_addr).get_string().get_string());
                        if(tmp_num<0)
                        {
                            error_interrupt(__special_call_vector_negative_value,i->get_children().back().get_node_line());
                            return -1;
                        }
                        nasal_gc.reference_delete(num2_addr);
                        num2_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(num2_addr).set_type(scalar_number);
                        nasal_gc.get_scalar(num2_addr).get_number().set_number(tmp_num);
                    }
                    else
                    {
                        error_interrupt(__not_numerable_str,i->get_children().back().get_node_line());
                        return -1;
                    }
                }
                if(nasal_gc.get_scalar(num1_addr).get_number().get_number()>2147483647)
                {
                    error_interrupt(__special_call_vector_too_large_value,i->get_children().front().get_node_line());
                    return -1;
                }
                if(num2_addr>=0 && nasal_gc.get_scalar(num2_addr).get_number().get_number()>2147483647)
                {
                    error_interrupt(__special_call_vector_too_large_value,i->get_children().back().get_node_line());
                    return -1;
                }
                int begin_num=(int)nasal_gc.get_scalar(num1_addr).get_number().get_number();
                int end_num=0;
                if(num2_addr<0 || nasal_gc.get_scalar(num2_addr).get_type()==scalar_nil)
                    end_num=nasal_gc.get_scalar(addr).get_vector().get_size();
                else
                    end_num=(int)nasal_gc.get_scalar(num2_addr).get_number().get_number();
                if(num1_addr>=0)
                    nasal_gc.reference_delete(num1_addr);
                if(num2_addr>=0)
                    nasal_gc.reference_delete(num2_addr);
                std::vector<int> subvec_result;
                for(int j=begin_num;j<end_num;++j)
                {
                    // addr used here
                    int tmp_data_addr=nasal_gc.get_scalar(addr).get_vector().get_elem(j);
                    int new_addr=-1;
                    if(tmp_data_addr<0)
                    {
                        error_interrupt(__invalid_vector_member,i->get_children().front().get_node_line());
                        return -1;
                    }
                    switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
                    {
                        case scalar_nil:
                            new_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                            break;
                        case scalar_number:
                            new_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                            nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
                            break;
                        case scalar_string:
                            new_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                            nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
                            break;
                        case scalar_function:
                            new_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                            nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
                            break;
                        case scalar_vector:
                        case scalar_hash:
                            new_addr=tmp_data_addr;
                            nasal_gc.reference_add(new_addr);
                            break;
                    }
                    subvec_result.push_back(new_addr);
                }
                for(int j=0;j<subvec_result.size();++j)
                    nasal_gc.get_scalar(generated_subvec_addr).get_vector().vec_push(subvec_result[j]);
            }// end sub-vector
            else
            {
                // normal vector calling
                int data_addr=calculation(local_scope,*i);
                if(data_addr<0)
                    return -1;
                if(nasal_gc.get_scalar(data_addr).get_type()!=scalar_number && nasal_gc.get_scalar(data_addr).get_type()!=scalar_string)
                {
                    error_interrupt(__error_value_type_when_calling_vector,i->get_node_line());
                    return -1;
                }
                double place_num=0;
                if(nasal_gc.get_scalar(data_addr).get_type()==scalar_string)
                {
                    if(check_numerable_string(nasal_gc.get_scalar(data_addr).get_string().get_string()))
                        place_num=(int)trans_string_to_number(nasal_gc.get_scalar(data_addr).get_string().get_string());
                    else
                    {
                        error_interrupt(__not_numerable_str,i->get_node_line());
                        return -1;
                    }
                }
                else if(nasal_gc.get_scalar(data_addr).get_type()==scalar_number)
                    place_num=nasal_gc.get_scalar(data_addr).get_number().get_number();
                if(place_num>2147483647 || place_num<-2147483648)
                {
                    error_interrupt(__normal_call_vector_too_large_value,i->get_node_line());
                    return -1;
                }
                int tmp_data_addr=nasal_gc.get_scalar(addr).get_vector().get_elem((int)place_num);
                if(tmp_data_addr<0)
                {
                    error_interrupt(__invalid_vector_member,i->get_node_line());
                    return -1;
                }
                int new_addr=-1;
                switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
                {
                    case scalar_nil:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                        break;
                    case scalar_number:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                        nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
                        break;
                    case scalar_string:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                        nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
                        break;
                    case scalar_function:
                        new_addr=nasal_gc.gc_alloc();
                        nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                        nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
                        break;
                    case scalar_vector:
                    case scalar_hash:
                        new_addr=tmp_data_addr;
                        nasal_gc.reference_add(new_addr);
                        break;
                }
                nasal_gc.get_scalar(generated_subvec_addr).get_vector().vec_push(new_addr);
                nasal_gc.reference_delete(data_addr);
            }
        }
        int tmp_addr=addr;
        addr=generated_subvec_addr;
        nasal_gc.reference_delete(tmp_addr);
    }
    return addr;
}
int  nasal_runtime::dot_call_hash(std::list<std::map<std::string,int> >& local_scope,std::list<abstract_syntax_tree>::iterator& call_node,int addr)
{
    if(nasal_gc.get_scalar(addr).get_type()!=scalar_hash)
    {
        error_interrupt(__not_callable_hash,call_node->get_node_line());
        return -1;
    }
    int tmp_data_addr=nasal_gc.get_scalar(addr).get_hash().get_hash_member(call_node->get_var_name());
    if(tmp_data_addr<0)
    {
        error_interrupt(__invalid_hash_member,call_node->get_node_line());
        return -1;
    }
    int new_addr=-1;
    switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
    {
        case scalar_nil:
            new_addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
            break;
        case scalar_number:
            new_addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(new_addr).set_type(scalar_number);
            nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
            break;
        case scalar_string:
            new_addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(new_addr).set_type(scalar_string);
            nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
            break;
        case scalar_function:
            new_addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(new_addr).set_type(scalar_function);
            nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
            break;
        case scalar_vector:
        case scalar_hash:
            new_addr=tmp_data_addr;
            nasal_gc.reference_add(new_addr);
            break;
    }
    int tmp_addr=addr;
    addr=new_addr;
    nasal_gc.reference_delete(tmp_addr);
    return addr;
}
int  nasal_runtime::call_function(std::list<std::map<std::string,int> >& local_scope,std::list<abstract_syntax_tree>::iterator& call_node,int addr,int last_hash_addr)
{
    if(nasal_gc.get_scalar(addr).get_type()!=scalar_function)
    {
        error_interrupt(__not_callable_function,call_node->get_node_line());
        return -1;
    }
    int tmp_addr=addr;
    addr=func_proc(
        local_scope,
        nasal_gc.get_scalar(nasal_gc.get_scalar(addr).get_function().get_local_scope()).get_closure().get_local_scope(),
        nasal_gc.get_scalar(addr).get_function().get_parameter_list(),
        nasal_gc.get_scalar(addr).get_function().get_statement_block(),
        *call_node,
        last_hash_addr
    );
    if(addr<0)
        return -1;
    nasal_gc.reference_delete(tmp_addr);
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
        if(var_type==__number)        nasal_gc.get_scalar(addr).get_vector().vec_push(number_generation(*i));
        else if(var_type==__string)   nasal_gc.get_scalar(addr).get_vector().vec_push(string_generation(*i));
        else if(var_type==__id)       nasal_gc.get_scalar(addr).get_vector().vec_push(call_identifier(local_scope,*i));
        else if(var_type==__vector)   nasal_gc.get_scalar(addr).get_vector().vec_push(vector_generation(local_scope,*i));
        else if(var_type==__hash)     nasal_gc.get_scalar(addr).get_vector().vec_push(hash_generation(local_scope,*i));
        else if(var_type==__function) nasal_gc.get_scalar(addr).get_vector().vec_push(function_generation(local_scope,*i));
        else if(var_type==__id)       nasal_gc.get_scalar(addr).get_vector().vec_push(call_identifier(local_scope,*i));
        else if(var_type==__add_operator  || var_type==__sub_operator || var_type==__mul_operator  || var_type==__div_operator || var_type==__link_operator
            || var_type==__cmp_equal || var_type==__cmp_less || var_type==__cmp_more || var_type==__cmp_not_equal || var_type==__cmp_less_or_equal || var_type==__cmp_more_or_equal
            || var_type==__and_operator || var_type==__or_operator || var_type==__ques_mark
            || var_type==__equal || var_type==__add_equal || var_type==__sub_equal || var_type==__div_equal || var_type==__mul_equal || var_type==__link_equal)
            nasal_gc.get_scalar(addr).get_vector().vec_push(calculation(local_scope,*i));
        else
        {
            call_node=i;
            break;
        }
    }
    int last_hash_addr=-1;
    for(;call_node!=node.get_children().end();++call_node)
    {
        if(nasal_gc.get_scalar(addr).get_type()==scalar_hash)
        {
            int tmp_hash_addr=last_hash_addr;
            last_hash_addr=addr;
            nasal_gc.reference_add(last_hash_addr);
            if(tmp_hash_addr>=0)
                nasal_gc.reference_delete(tmp_hash_addr);
        }
        // call vector/special call hash/subvec
        // the special type of calling hash like a["name"] is also generated as calling vector
        if(call_node->get_node_type()==__call_vector)
            addr=bracket_call_vector(local_scope,call_node,addr);
        // call hash identifier.identifier
        else if(call_node->get_node_type()==__call_hash)
            addr=dot_call_hash(local_scope,call_node,addr);
        // call function identifier(...)
        else if(call_node->get_node_type()==__call_function)
            addr=call_function(local_scope,call_node,addr,last_hash_addr);
        
        if(addr<0)
            break;
    }
    return addr;
}
int nasal_runtime::hash_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_hash);
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
            std::string member_name=i->get_children().front().get_var_name();
            int var_type=i->get_children().back().get_node_type();
            if(member_name=="parents")
            {
                int vector_addr=-1;
                if(var_type==__id) vector_addr=call_identifier(local_scope,i->get_children().back());
                else if(var_type==__vector) vector_addr=hash_generation(local_scope,i->get_children().back());
                else
                {
                    error_interrupt(__class_inheritence_need_vector,i->get_children().back().get_node_line());
                    return -1;
                }
                if(vector_addr<0)
                    return -1;
                if(nasal_gc.get_scalar(vector_addr).get_type()!=scalar_vector)
                {
                    error_interrupt(__class_inheritence_need_vector,i->get_children().back().get_node_line());
                    return -1;
                }
                for(int i=0;i<nasal_gc.get_scalar(vector_addr).get_vector().get_size();++i)
                    if(nasal_gc.get_scalar(vector_addr).get_vector().get_elem(i)<0)
                        return -1;
                nasal_gc.get_scalar(vector_addr).get_vector().generate_new_hash();
                nasal_gc.get_scalar(addr).get_hash().hash_push("parents",vector_addr);
            }
            else
            {
                if(var_type==__number)        nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,number_generation(i->get_children().back()));
                else if(var_type==__string)   nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,string_generation(i->get_children().back()));
                else if(var_type==__id)       nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,call_identifier(local_scope,i->get_children().back()));
                else if(var_type==__vector)   nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,vector_generation(local_scope,i->get_children().back()));
                else if(var_type==__hash)     nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,hash_generation(local_scope,i->get_children().back()));
                else if(var_type==__function) nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,function_generation(local_scope,i->get_children().back()));
                else if(var_type==__add_operator  || var_type==__sub_operator || var_type==__mul_operator  || var_type==__div_operator || var_type==__link_operator
                || var_type==__cmp_equal || var_type==__cmp_less || var_type==__cmp_more || var_type==__cmp_not_equal || var_type==__cmp_less_or_equal || var_type==__cmp_more_or_equal
                || var_type==__and_operator || var_type==__or_operator || var_type==__ques_mark
                || var_type==__equal || var_type==__add_equal || var_type==__sub_equal || var_type==__div_equal || var_type==__mul_equal || var_type==__link_equal)
                    nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,calculation(local_scope,i->get_children().back()));
            }
        }
    }
    int last_hash_addr=-1;
    for(;call_node!=node.get_children().end();++call_node)
    {
        if(nasal_gc.get_scalar(addr).get_type()==scalar_hash)
        {
            int tmp_hash_addr=last_hash_addr;
            last_hash_addr=addr;
            nasal_gc.reference_add(last_hash_addr);
            if(tmp_hash_addr>=0)
                nasal_gc.reference_delete(tmp_hash_addr);
        }
        // call vector/special call hash/subvec
        // the special type of calling hash like a["name"] is also generated as calling vector
        if(call_node->get_node_type()==__call_vector)
            addr=bracket_call_vector(local_scope,call_node,addr);
        // call hash identifier.identifier
        else if(call_node->get_node_type()==__call_hash)
            addr=dot_call_hash(local_scope,call_node,addr);
        // call function identifier(...)
        else if(call_node->get_node_type()==__call_function)
            addr=call_function(local_scope,call_node,addr,last_hash_addr);
        
        if(addr<0) break;
    }
    return addr;
}
int nasal_runtime::function_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    // function
    //     parameters
    //     block
    //     calls...
    int addr=nasal_gc.gc_alloc();
    int scope_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(scope_addr).set_type(scalar_closure);
    nasal_gc.get_scalar(scope_addr).get_closure().set_local_scope(local_scope);
    nasal_gc.get_scalar(addr).set_type(scalar_function);
    nasal_gc.get_scalar(addr).get_function().set_local_scope(scope_addr);
    nasal_gc.get_scalar(addr).get_function().set_closure_update_state(false);
    
    std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();
    nasal_gc.get_scalar(addr).get_function().set_paramemter_list(*i);

    ++i;
    nasal_gc.get_scalar(addr).get_function().set_statement_block(*i);

    ++i;
    int last_hash_addr=-1;
    for(std::list<abstract_syntax_tree>::iterator call_node=i;call_node!=node.get_children().end();++call_node)
    {
        if(nasal_gc.get_scalar(addr).get_type()==scalar_hash)
        {
            int tmp_hash_addr=last_hash_addr;
            last_hash_addr=addr;
            nasal_gc.reference_add(last_hash_addr);
            if(tmp_hash_addr>=0)
                nasal_gc.reference_delete(tmp_hash_addr);
        }
        // call vector/special call hash/subvec
        // the special type of calling hash like a["name"] is also generated as calling vector
        if(call_node->get_node_type()==__call_vector)
            addr=bracket_call_vector(local_scope,call_node,addr);
        // call hash identifier.identifier
        else if(call_node->get_node_type()==__call_hash)
            addr=dot_call_hash(local_scope,call_node,addr);
        // call function identifier(...)
        else if(call_node->get_node_type()==__call_function)
            addr=call_function(local_scope,call_node,addr,last_hash_addr);
        
        if(addr<0)
            break;
    }
    return addr;
}
void nasal_runtime::update_closure(std::vector<int>& update_list,int closure_addr)
{
    // update_closure
    // each new function will be updated only once, after updating closure,functions' closure_updated flag will be set true
    int size=update_list.size();
    for(int i=0;i<size;++i)
        if(!nasal_gc.get_scalar(update_list[i]).get_function().get_closure_update_state())
        {
            nasal_gc.get_scalar(update_list[i]).get_function().set_local_scope(closure_addr);
            nasal_gc.reference_add(closure_addr);
            nasal_gc.get_scalar(update_list[i]).get_function().set_closure_update_state(true);
        }
    return;
}
bool nasal_runtime::check_condition(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    bool ret=false;
    int checked_addr=calculation(local_scope,node);
    if(checked_addr<0)
        return false;
    int checked_type=nasal_gc.get_scalar(checked_addr).get_type();
    if(checked_type==scalar_number)
        ret=(nasal_gc.get_scalar(checked_addr).get_number().get_number()!=0);
    else if(checked_type==scalar_string)
    {
        std::string tmp_str=nasal_gc.get_scalar(checked_addr).get_string().get_string();
        if(!tmp_str.length())
            ret=false;
        else if(check_numerable_string(tmp_str))
            ret=(trans_string_to_number(tmp_str)!=0);
        else
            ret=true;// not numerable string like 'hello' is also true
    }
    else
    {
        error_interrupt(__error_value_type,node.get_node_line());
        return false;
    }
    nasal_gc.reference_delete(checked_addr);
    return ret;
}
int nasal_runtime::calculation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    // calculation will return a value that points to a new area in memory
    int node_type=node.get_node_type();
    if(node_type==__nil)
    {
        int ret=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(ret).set_type(scalar_nil);
        return ret;
    }
    else if(node_type==__number)
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
            // note: sub operator may mean that this is unary calculation
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
            if(check_numerable_string(tmp_str))
                num_1=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_children().front().get_node_line());
                return -1;
            }
            num_2=nasal_gc.get_scalar(addr_2).get_number().get_number();
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
        else if(type_1==scalar_nil && type_2==scalar_nil)
            nasal_gc.get_scalar(ret_addr).get_number().set_number((double(node_type==__cmp_equal)));
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
                case __cmp_less:         nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<num_2)));break;
                case __cmp_less_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<=num_2)));break;
                case __cmp_more:         nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>num_2)));break;
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
                case __cmp_less:         nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<num_2)));break;
                case __cmp_less_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<=num_2)));break;
                case __cmp_more:         nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>num_2)));break;
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
                case __cmp_less:         nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<num_2)));break;
                case __cmp_less_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1<=num_2)));break;
                case __cmp_more:         nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>num_2)));break;
                case __cmp_more_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(num_1>=num_2)));break;
            }
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            std::string str_1=nasal_gc.get_scalar(addr_1).get_string().get_string();
            std::string str_2=nasal_gc.get_scalar(addr_2).get_string().get_string();
            switch(node_type)
            {
                case __cmp_less:         nasal_gc.get_scalar(ret_addr).get_number().set_number((double(str_1<str_2)));break;
                case __cmp_less_or_equal:nasal_gc.get_scalar(ret_addr).get_number().set_number((double(str_1<=str_2)));break;
                case __cmp_more:         nasal_gc.get_scalar(ret_addr).get_number().set_number((double(str_1>str_2)));break;
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
        	std::vector<abstract_syntax_tree*> assignment_ast;
        	for(std::list<abstract_syntax_tree>::iterator i=node.get_children().front().get_children().begin();i!=node.get_children().front().get_children().end();++i)
				assignment_ast.push_back(&(*i));
			if(assignment_ast.empty())
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
				if(data_addrs.size()!=assignment_ast.size())
				{
					error_interrupt(__multi_assign_incorrect_value_number,node.get_children().back().get_node_line());
					return -1;
				}
                // multi assignment doesn't have returned value
                // because multi assignment cannot be used in calculation expression
				for(int i=0;i<assignment_ast.size();++i)
                {
                    int tmp_addr=assignment(local_scope,*assignment_ast[i],data_addrs[i]);
                    if(tmp_addr>=0)
                        nasal_gc.reference_delete(tmp_addr);
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
				else if(nasal_gc.get_scalar(data_addr).get_vector().get_size()!=assignment_ast.size())
				{
					error_interrupt(__multi_assign_incorrect_value_number,node.get_children().back().get_node_line());
					return -1;
				}
				for(int i=0;i<assignment_ast.size();++i)
				{
                    int new_data_addr=-1;
                    int vector_member_addr=nasal_gc.get_scalar(data_addr).get_vector().get_elem(i);
                    if(vector_member_addr<0)
                    {
                        error_interrupt(__invalid_vector_member,node.get_children().back().get_node_line());
                        return -1;
                    }
                    int tmp_addr=assignment(local_scope,*assignment_ast[i],vector_member_addr);
                    if(tmp_addr>=0)
                        nasal_gc.reference_delete(tmp_addr);
                }
                nasal_gc.reference_delete(data_addr);
			}
            return -1;
		}
        else
        {
        	int ret_addr=-1;
        	abstract_syntax_tree* assignment_ast=&node.get_children().front();
        	int data_addr=calculation(local_scope,node.get_children().back());
            if(data_addr<0)
                return -1;
        	ret_addr=assignment(local_scope,*assignment_ast,data_addr);
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
        double num_1=0;
        double num_2=0;
        if(type_1==scalar_number && type_2==scalar_number)
        {
            num_1=nasal_gc.get_scalar(id_addr).get_number().get_number();
            num_2=nasal_gc.get_scalar(value_addr).get_number().get_number();
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            num_1=nasal_gc.get_scalar(id_addr).get_number().get_number();
            std::string tmp_str=nasal_gc.get_scalar(value_addr).get_string().get_string();
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
            std::string tmp_str=nasal_gc.get_scalar(id_addr).get_string().get_string();
            if(check_numerable_string(tmp_str))
                num_1=trans_string_to_number(tmp_str);
            else
            {
                error_interrupt(__not_numerable_str,node.get_children().front().get_node_line());
                return -1;
            }
            num_2=nasal_gc.get_scalar(value_addr).get_number().get_number();
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
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
        }
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        if(nasal_gc.get_scalar(id_addr).get_type()==scalar_string)
        {
            nasal_gc.get_scalar(id_addr).get_string().set_clear();
            nasal_gc.get_scalar(id_addr).set_type(scalar_number);
        }
        switch(node_type)
        {
            case __add_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1+num_2);break;
            case __sub_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1-num_2);break;
            case __mul_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1*num_2);break;
            case __div_equal:nasal_gc.get_scalar(id_addr).get_number().set_number(num_1/num_2);break;
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
        std::string str_1,str_2;
        if(type_1==scalar_number && type_2==scalar_number)
        {
            str_1=trans_number_to_string(nasal_gc.get_scalar(id_addr).get_number().get_number());
            str_2=trans_number_to_string(nasal_gc.get_scalar(value_addr).get_number().get_number());
        }
        else if(type_1==scalar_number && type_2==scalar_string)
        {
            str_1=trans_number_to_string(nasal_gc.get_scalar(id_addr).get_number().get_number());
            str_2=nasal_gc.get_scalar(value_addr).get_string().get_string();
        }
        else if(type_1==scalar_string && type_2==scalar_number)
        {
            str_1=nasal_gc.get_scalar(id_addr).get_string().get_string();
            str_2=trans_number_to_string(nasal_gc.get_scalar(value_addr).get_number().get_number());
        }
        else if(type_1==scalar_string && type_2==scalar_string)
        {
            str_1=nasal_gc.get_scalar(id_addr).get_string().get_string();
            str_2=nasal_gc.get_scalar(value_addr).get_string().get_string();
        }
        else
        {
            error_interrupt(__error_value_type,node.get_node_line());
            return -1;
        }
        if(nasal_gc.get_scalar(id_addr).get_type()==scalar_number)
        {
            nasal_gc.get_scalar(id_addr).get_number().set_clear();
            nasal_gc.get_scalar(id_addr).set_type(scalar_string);
        }
        nasal_gc.get_scalar(id_addr).get_string().set_string(str_1+str_2);
        nasal_gc.reference_delete(value_addr);
        return id_addr;
    }
    else if(node_type==__and_operator)
    {
        // and will return the last value that is not null
        // if and gets a null,and returns 0
        // vector,hash,function is not allowed here
        int ret_addr=-1;
        int val_addr=calculation(local_scope,node.get_children().front());
        if(val_addr<0) return -1;
        int val_type=nasal_gc.get_scalar(val_addr).get_type();
        if(val_type==scalar_number)
        {
            double num=nasal_gc.get_scalar(val_addr).get_number().get_number();
            if(num==0)
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(val_addr);
                return ret_addr;
            }
            else
                nasal_gc.reference_delete(val_addr);
        }
        else if(val_type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(val_addr).get_string().get_string();
            if(str.length()==0 || (check_numerable_string(str) && trans_string_to_number(str)==0))
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(val_addr);
                return ret_addr;
            }
            else
                nasal_gc.reference_delete(val_addr);
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().front().get_node_line());
            return -1;
        }
        val_addr=calculation(local_scope,node.get_children().back());
        if(val_addr<0) return -1;
        val_type=nasal_gc.get_scalar(val_addr).get_type();
        if(val_type==scalar_number)
        {
            double num=nasal_gc.get_scalar(val_addr).get_number().get_number();
            if(num==0)
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(val_addr);
                return ret_addr;
            }
            else ret_addr=val_addr;
        }
        else if(val_type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(val_addr).get_string().get_string();
            if(str.length()==0 || (check_numerable_string(str) && trans_string_to_number(str)==0))
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(val_addr);
                return ret_addr;
            }
            else ret_addr=val_addr;
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().back().get_node_line());
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
        int val_addr=calculation(local_scope,node.get_children().front());
        if(val_addr<0) return -1;
        int val_type=nasal_gc.get_scalar(val_addr).get_type();
        if(val_type==scalar_number)
        {
            double num=nasal_gc.get_scalar(val_addr).get_number().get_number();
            if(num!=0) return val_addr;
            else nasal_gc.reference_delete(val_addr);
        }
        else if(val_type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(val_addr).get_string().get_string();
            if(str.length()!=0 && (!check_numerable_string(str) || !trans_string_to_number(str)==0))
                return val_addr;
            else nasal_gc.reference_delete(val_addr);
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().front().get_node_line());
            return -1;
        }
        val_addr=calculation(local_scope,node.get_children().back());
        if(val_addr<0) return -1;
        val_type=nasal_gc.get_scalar(val_addr).get_type();
        if(val_type==scalar_number)
        {
            double num=nasal_gc.get_scalar(val_addr).get_number().get_number();
            if(num==0)
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(val_addr);
                return ret_addr;
            }
            else return val_addr;
        }
        else if(val_type==scalar_string)
        {
            std::string str=nasal_gc.get_scalar(val_addr).get_string().get_string();
            if(str.length()==0 || (check_numerable_string(str) && trans_string_to_number(str)==0))
            {
                ret_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
                nasal_gc.get_scalar(ret_addr).get_number().set_number(0);
                nasal_gc.reference_delete(val_addr);
                return ret_addr;
            }
            else return val_addr;
        }
        else
        {
            error_interrupt(__error_value_type,node.get_children().back().get_node_line());
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
        bool checked_result=check_condition(local_scope,node.get_children().front());
        ret_addr=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
        nasal_gc.get_scalar(ret_addr).get_number().set_number((double)checked_result);
        return ret_addr;
    }
    else if(node_type==__ques_mark)
    {
    	// ?:
    	// this will return the first element if the condition is true
    	// this will return the second element if the condition is null(0,nil,"0",'0',"",'',"0x0","0o0")
        std::list<abstract_syntax_tree>::iterator ptr=node.get_children().begin();
        if(check_condition(local_scope,*ptr))
        {
            ++ptr;
            return calculation(local_scope,*ptr);
        }
        else
        {
            ++ptr;++ptr;
            return calculation(local_scope,*ptr);
        }
		return -1;
    }
    return -1;
}
int nasal_runtime::assignment(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node,int used_data_addr)
{
    int* assigned_addr=NULL;
    std::string tmp_id_name=node.get_var_name();
    if(global_scope.find(tmp_id_name)!=global_scope.end())
        assigned_addr=&(global_scope[tmp_id_name]);
    for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
        if(iter->find(tmp_id_name)!=iter->end())
            assigned_addr=&((*iter)[tmp_id_name]);
    if(!assigned_addr)
        return -1;
    std::vector<int> tmp_subvec_addr;
    for(std::list<abstract_syntax_tree>::iterator iter=node.get_children().begin();iter!=node.get_children().end();++iter)
    {
        // call vector/special call hash/subvec
        // the special type of calling hash like a["name"] is also generated as calling vector
        if(iter->get_node_type()==__call_vector)
        {
            // check the scalar type of called identifier here
            int called_type=-1;
            if(!assigned_addr && tmp_subvec_addr.size())
                called_type=nasal_gc.get_scalar(tmp_subvec_addr.back()).get_type();
            else if(assigned_addr)
                called_type=nasal_gc.get_scalar(*assigned_addr).get_type();
            if(iter->get_children().size()==1 && called_type!=scalar_vector && called_type!=scalar_hash)
            {
                error_interrupt(__error_value_type_when_calling_vector,iter->get_node_line());
                return -1;
            }
            if(iter->get_children().size()>1 && called_type!=scalar_vector)
            {
                error_interrupt(__error_value_type_when_calling_vector,iter->get_node_line());
                return -1;
            }
            if(iter->get_children().size()==1)
            {
                if(iter->get_children().front().get_node_type()==__sub_vector)
                {
                    if(called_type==scalar_hash)
                    {
                        error_interrupt(__not_callable_vector,iter->get_node_line());
                        return -1;
                    }
                    int num1_addr=-1;
                    int num2_addr=-1;
                    // identifier[num1:];
                    if(iter->get_children().front().get_children().size()==1)
                    {
                        num1_addr=calculation(local_scope,iter->get_children().front().get_children().front());
                        if(num1_addr<0)
                            return -1;
                    }
                    // identifier[num1:num2];
                    else
                    {
                        num1_addr=calculation(local_scope,iter->get_children().front().get_children().front());
                        num2_addr=calculation(local_scope,iter->get_children().front().get_children().back());
                        if(num1_addr<0 || num2_addr<0)
                            return -1;
                    }
                    if(nasal_gc.get_scalar(num1_addr).get_type()!=scalar_number
                        && nasal_gc.get_scalar(num1_addr).get_type()!=scalar_string)
                    {
                        error_interrupt(__error_value_type_when_calling_vector,iter->get_children().front().get_children().front().get_node_line());
                        return -1;
                    }
                    if(num2_addr>=0
                        && (nasal_gc.get_scalar(num2_addr).get_type()!=scalar_number
                        && nasal_gc.get_scalar(num2_addr).get_type()!=scalar_string
                        && nasal_gc.get_scalar(num2_addr).get_type()!=scalar_nil))
                    {
                        error_interrupt(__error_value_type_when_calling_vector,iter->get_children().front().get_children().back().get_node_line());
                        return -1;
                    }
                    if(nasal_gc.get_scalar(num1_addr).get_type()==scalar_string)
                    {
                        if(check_numerable_string(nasal_gc.get_scalar(num1_addr).get_string().get_string()))
                        {
                            double tmp_num=trans_string_to_number(nasal_gc.get_scalar(num1_addr).get_string().get_string());
                            if(tmp_num<0)
                            {
                                error_interrupt(__special_call_vector_negative_value,iter->get_children().front().get_children().front().get_node_line());
                                return -1;
                            }
                            nasal_gc.reference_delete(num1_addr);
                            num1_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(num1_addr).set_type(scalar_number);
                            nasal_gc.get_scalar(num1_addr).get_number().set_number(tmp_num);
                        }
                        else
                        {
                            error_interrupt(__not_numerable_str,iter->get_children().front().get_children().front().get_node_line());
                            return -1;
                        }
                    }
                    if(num2_addr>=0 && nasal_gc.get_scalar(num2_addr).get_type()==scalar_string)
                    {
                        if(check_numerable_string(nasal_gc.get_scalar(num2_addr).get_string().get_string()))
                        {
                            double tmp_num=trans_string_to_number(nasal_gc.get_scalar(num2_addr).get_string().get_string());
                            if(tmp_num<0)
                            {
                                error_interrupt(__special_call_vector_negative_value,iter->get_children().front().get_children().back().get_node_line());
                                return -1;
                            }
                            nasal_gc.reference_delete(num2_addr);
                            num2_addr=nasal_gc.gc_alloc();
                            nasal_gc.get_scalar(num2_addr).set_type(scalar_number);
                            nasal_gc.get_scalar(num2_addr).get_number().set_number(tmp_num);
                        }
                        else
                        {
                            error_interrupt(__not_numerable_str,iter->get_children().front().get_children().back().get_node_line());
                            return -1;
                        }
                    }
                    if(nasal_gc.get_scalar(num1_addr).get_number().get_number()>2147483647)
                    {
                        error_interrupt(__special_call_vector_too_large_value,iter->get_children().front().get_children().front().get_node_line());
                        return -1;
                    }
                    if(num2_addr>=0 && nasal_gc.get_scalar(num2_addr).get_number().get_number()>2147483647)
                    {
                        error_interrupt(__special_call_vector_too_large_value,iter->get_children().front().get_children().back().get_node_line());
                        return -1;
                    }
                    int begin_num=(int)nasal_gc.get_scalar(num1_addr).get_number().get_number();
                    int end_num=0;
                    if(num2_addr<0 || nasal_gc.get_scalar(num2_addr).get_type()==scalar_nil)
                    {
                        if(!assigned_addr && tmp_subvec_addr.size())
                            end_num=nasal_gc.get_scalar(tmp_subvec_addr.back()).get_vector().get_size();
                        else if(assigned_addr)
                            end_num=nasal_gc.get_scalar(*assigned_addr).get_vector().get_size();
                    }
                    else
                        end_num=(int)nasal_gc.get_scalar(num2_addr).get_number().get_number();
                    if(num1_addr>=0)
                        nasal_gc.reference_delete(num1_addr);
                    if(num2_addr>=0)
                        nasal_gc.reference_delete(num2_addr);
                    std::vector<int> subvec_result;
                    for(int i=begin_num;i<end_num;++i)
                    {
                        // addr used here
                        int tmp_data_addr=-1;
                        if(!assigned_addr && tmp_subvec_addr.size())
                            tmp_data_addr=nasal_gc.get_scalar(tmp_subvec_addr.back()).get_vector().get_elem(i);
                        else if(assigned_addr)
                            tmp_data_addr=nasal_gc.get_scalar(*assigned_addr).get_vector().get_elem(i);
                        int new_addr=-1;
                        if(tmp_data_addr<0)
                        {
                            error_interrupt(__invalid_vector_member,iter->get_children().front().get_children().front().get_node_line());
                            return -1;
                        }
                        switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
                        {
                            case scalar_nil:
                                new_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                                break;
                            case scalar_number:
                                new_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                                nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
                                break;
                            case scalar_string:
                                new_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                                nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
                                break;
                            case scalar_function:
                                new_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                                nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
                                break;
                            case scalar_vector:
                            case scalar_hash:
                                new_addr=tmp_data_addr;
                                nasal_gc.reference_add(new_addr);
                                break;
                        }
                        subvec_result.push_back(new_addr);
                    }
                    int tmp_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(tmp_addr).set_type(scalar_vector);
                    for(int i=0;i<subvec_result.size();++i)
                        nasal_gc.get_scalar(tmp_addr).get_vector().vec_push(subvec_result[i]);
                    tmp_subvec_addr.push_back(tmp_addr);
                    assigned_addr=NULL;
                }// end sub-vector
                else
                {
                    // normal vector/hash calling
                    int tmp_data_addr=calculation(local_scope,iter->get_children().front());
                    if(tmp_data_addr<0)
                        return -1;
                    if(nasal_gc.get_scalar(tmp_data_addr).get_type()!=scalar_number && nasal_gc.get_scalar(tmp_data_addr).get_type()!=scalar_string)
                    {
                        error_interrupt(__error_value_type_when_calling_vector,iter->get_children().front().get_node_line());
                        return -1;
                    }
                    if(called_type==scalar_vector)
                    {
                        double place_num=0;
                        if(nasal_gc.get_scalar(tmp_data_addr).get_type()==scalar_string)
                        {
                            if(check_numerable_string(nasal_gc.get_scalar(tmp_data_addr).get_string().get_string()))
                                place_num=(int)trans_string_to_number(nasal_gc.get_scalar(tmp_data_addr).get_string().get_string());
                            else
                            {
                                error_interrupt(__not_numerable_str,iter->get_children().front().get_node_line());
                                return -1;
                            }
                        }
                        else if(nasal_gc.get_scalar(tmp_data_addr).get_type()==scalar_number)
                            place_num=nasal_gc.get_scalar(tmp_data_addr).get_number().get_number();
                        if(place_num>2147483647 || place_num<-2147483648)
                        {
                            error_interrupt(__normal_call_vector_too_large_value,iter->get_children().front().get_node_line());
                            return -1;
                        }
                        if(!assigned_addr && tmp_subvec_addr.size())
                            assigned_addr=nasal_gc.get_scalar(tmp_subvec_addr.back()).get_vector().get_elem_addr((int)place_num);
                        else if(assigned_addr)
                            assigned_addr=nasal_gc.get_scalar(*assigned_addr).get_vector().get_elem_addr((int)place_num);
                        if(!assigned_addr)
                        {
                            error_interrupt(__invalid_vector_member,iter->get_children().front().get_node_line());
                            return -1;
                        }
                    }
                    else if(called_type==scalar_hash)
                    {
                        if(nasal_gc.get_scalar(tmp_data_addr).get_type()!=scalar_string)
                        {
                            error_interrupt(__error_value_type_when_calling_hash,iter->get_children().front().get_node_line());
                            return -1;
                        }
                        // only subvec's addr is stored in tmp_subvec_addr
                        // so hash doesn't need to do this
                        assigned_addr=nasal_gc.get_scalar(*assigned_addr).get_hash().get_hash_member_addr(nasal_gc.get_scalar(tmp_data_addr).get_string().get_string());
                        if(!assigned_addr)
                        {
                            error_interrupt(__invalid_hash_member,iter->get_children().front().get_node_line());
                            return -1;
                        }
                    }
                    nasal_gc.reference_delete(tmp_data_addr);
                }
            }
            else if(iter->get_children().size()>1)
            {
                int generated_subvec_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(generated_subvec_addr).set_type(scalar_vector);
                for(std::list<abstract_syntax_tree>::iterator i=iter->get_children().begin();i!=iter->get_children().end();++i)
                {
                    if(i->get_node_type()==__sub_vector)
                    {
                        if(called_type==scalar_hash)
                        {
                            error_interrupt(__not_callable_vector,i->get_node_line());
                            return -1;
                        }
                        int num1_addr=-1;
                        int num2_addr=-1;
                        // identifier[num1:];
                        if(i->get_children().size()==1)
                        {
                            num1_addr=calculation(local_scope,i->get_children().front());
                            if(num1_addr<0)
                                return -1;
                        }
                        // identifier[num1:num2];
                        else
                        {
                            num1_addr=calculation(local_scope,i->get_children().front());
                            num2_addr=calculation(local_scope,i->get_children().back());
                            if(num1_addr<0 || num2_addr<0)
                                return -1;
                        }
                        if(nasal_gc.get_scalar(num1_addr).get_type()!=scalar_number
                            && nasal_gc.get_scalar(num1_addr).get_type()!=scalar_string)
                        {
                            error_interrupt(__error_value_type_when_calling_vector,i->get_children().front().get_node_line());
                            return -1;
                        }
                        if(num2_addr>=0
                            && (nasal_gc.get_scalar(num2_addr).get_type()!=scalar_number
                            && nasal_gc.get_scalar(num2_addr).get_type()!=scalar_string
                            && nasal_gc.get_scalar(num2_addr).get_type()!=scalar_nil))
                        {
                            error_interrupt(__error_value_type_when_calling_vector,i->get_children().back().get_node_line());
                            return -1;
                        }
                        if(nasal_gc.get_scalar(num1_addr).get_type()==scalar_string)
                        {
                            if(check_numerable_string(nasal_gc.get_scalar(num1_addr).get_string().get_string()))
                            {
                                double tmp_num=trans_string_to_number(nasal_gc.get_scalar(num1_addr).get_string().get_string());
                                if(tmp_num<0)
                                {
                                    error_interrupt(__special_call_vector_negative_value,i->get_children().front().get_node_line());
                                    return -1;
                                }
                                nasal_gc.reference_delete(num1_addr);
                                num1_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(num1_addr).set_type(scalar_number);
                                nasal_gc.get_scalar(num1_addr).get_number().set_number(tmp_num);
                            }
                            else
                            {
                                error_interrupt(__not_numerable_str,i->get_children().front().get_node_line());
                                return -1;
                            }
                        }
                        if(num2_addr>=0 && nasal_gc.get_scalar(num2_addr).get_type()==scalar_string)
                        {
                            if(check_numerable_string(nasal_gc.get_scalar(num2_addr).get_string().get_string()))
                            {
                                double tmp_num=trans_string_to_number(nasal_gc.get_scalar(num2_addr).get_string().get_string());
                                if(tmp_num<0)
                                {
                                    error_interrupt(__special_call_vector_negative_value,i->get_children().back().get_node_line());
                                    return -1;
                                }
                                nasal_gc.reference_delete(num2_addr);
                                num2_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(num2_addr).set_type(scalar_number);
                                nasal_gc.get_scalar(num2_addr).get_number().set_number(tmp_num);
                            }
                            else
                            {
                                error_interrupt(__not_numerable_str,i->get_children().back().get_node_line());
                                return -1;
                            }
                        }
                        if(nasal_gc.get_scalar(num1_addr).get_number().get_number()>2147483647)
                        {
                            error_interrupt(__special_call_vector_too_large_value,i->get_children().front().get_node_line());
                            return -1;
                        }
                        if(num2_addr>=0 && nasal_gc.get_scalar(num2_addr).get_number().get_number()>2147483647)
                        {
                            error_interrupt(__special_call_vector_too_large_value,i->get_children().back().get_node_line());
                            return -1;
                        }
                        int begin_num=(int)nasal_gc.get_scalar(num1_addr).get_number().get_number();
                        int end_num=0;
                        if(num2_addr<0 || nasal_gc.get_scalar(num2_addr).get_type()==scalar_nil)
                        {
                            if(!assigned_addr && tmp_subvec_addr.size())
                                end_num=nasal_gc.get_scalar(tmp_subvec_addr.back()).get_vector().get_size();
                            else if(assigned_addr)
                                end_num=nasal_gc.get_scalar(*assigned_addr).get_vector().get_size();
                        }
                        else
                            end_num=(int)nasal_gc.get_scalar(num2_addr).get_number().get_number();
                        if(num1_addr>=0)
                            nasal_gc.reference_delete(num1_addr);
                        if(num2_addr>=0)
                            nasal_gc.reference_delete(num2_addr);
                        std::vector<int> subvec_result;
                        for(int j=begin_num;j<end_num;++j)
                        {
                            // addr used here
                            int tmp_data_addr=-1;
                            if(!assigned_addr && tmp_subvec_addr.size())
                                tmp_data_addr=nasal_gc.get_scalar(tmp_subvec_addr.back()).get_vector().get_elem(j);
                            else if(assigned_addr)
                                tmp_data_addr=nasal_gc.get_scalar(*assigned_addr).get_vector().get_elem(j);
                            
                            if(tmp_data_addr<0)
                            {
                                error_interrupt(__invalid_vector_member,i->get_children().front().get_node_line());
                                return -1;
                            }
                            int new_addr=-1;
                            switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
                            {
                                case scalar_nil:
                                    new_addr=nasal_gc.gc_alloc();
                                    nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                                    break;
                                case scalar_number:
                                    new_addr=nasal_gc.gc_alloc();
                                    nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                                    nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
                                    break;
                                case scalar_string:
                                    new_addr=nasal_gc.gc_alloc();
                                    nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                                    nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
                                    break;
                                case scalar_function:
                                    new_addr=nasal_gc.gc_alloc();
                                    nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                                    nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
                                    break;
                                case scalar_vector:
                                case scalar_hash:
                                    new_addr=tmp_data_addr;
                                    nasal_gc.reference_add(new_addr);
                                    break;
                            }
                            subvec_result.push_back(new_addr);
                        }
                        for(int j=0;j<subvec_result.size();++j)
                            nasal_gc.get_scalar(generated_subvec_addr).get_vector().vec_push(subvec_result[j]);
                    }// end sub-vector
                    else
                    {
                        // normal vector calling
                        int data_addr=calculation(local_scope,*i);
                        if(data_addr<0) return -1;
                        if(nasal_gc.get_scalar(data_addr).get_type()!=scalar_number && nasal_gc.get_scalar(data_addr).get_type()!=scalar_string)
                        {
                            error_interrupt(__error_value_type_when_calling_vector,i->get_node_line());
                            return -1;
                        }
                        double place_num=0;
                        if(nasal_gc.get_scalar(data_addr).get_type()==scalar_string)
                        {
                            if(check_numerable_string(nasal_gc.get_scalar(data_addr).get_string().get_string()))
                                place_num=(int)trans_string_to_number(nasal_gc.get_scalar(data_addr).get_string().get_string());
                            else
                            {
                                error_interrupt(__not_numerable_str,i->get_node_line());
                                return -1;
                            }
                        }
                        else if(nasal_gc.get_scalar(data_addr).get_type()==scalar_number)
                            place_num=nasal_gc.get_scalar(data_addr).get_number().get_number();
                        if(place_num>2147483647 || place_num<-2147483648)
                        {
                            error_interrupt(__normal_call_vector_too_large_value,i->get_node_line());
                            return -1;
                        }
                        int tmp_data_addr=-1;
                        if(!assigned_addr && tmp_subvec_addr.size())
                            tmp_data_addr=nasal_gc.get_scalar(tmp_subvec_addr.back()).get_vector().get_elem((int)place_num);
                        else if(assigned_addr)
                            tmp_data_addr=nasal_gc.get_scalar(*assigned_addr).get_vector().get_elem((int)place_num);
                        if(tmp_data_addr<0)
                        {
                            error_interrupt(__invalid_vector_member,i->get_node_line());
                            return -1;
                        }
                        int new_addr=-1;
                        switch(nasal_gc.get_scalar(tmp_data_addr).get_type())
                        {
                            case scalar_nil:
                                new_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                                break;
                            case scalar_number:
                                new_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                                nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_number());
                                break;
                            case scalar_string:
                                new_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                                nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_string());
                                break;
                            case scalar_function:
                                new_addr=nasal_gc.gc_alloc();
                                nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                                nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp_data_addr).get_function());
                                break;
                            case scalar_vector:
                            case scalar_hash:
                                new_addr=tmp_data_addr;
                                nasal_gc.reference_add(new_addr);
                                break;
                        }
                        nasal_gc.get_scalar(generated_subvec_addr).get_vector().vec_push(new_addr);
                        nasal_gc.reference_delete(data_addr);
                    }
                }
                tmp_subvec_addr.push_back(generated_subvec_addr);
                assigned_addr=NULL;
            }
        }// end call vector
        // call hash identifier.identifier
        else if(iter->get_node_type()==__call_hash)
        {
            if(nasal_gc.get_scalar(*assigned_addr).get_type()!=scalar_hash)
            {
                error_interrupt(__not_callable_hash,iter->get_node_line());
                return -1;
            }
            assigned_addr=nasal_gc.get_scalar(*assigned_addr).get_hash().get_hash_member_addr(iter->get_var_name());
            if(!assigned_addr)
            {
                error_interrupt(__invalid_hash_member,iter->get_node_line());
                return -1;
            }
        }// end call hash
        // call function identifier(...)
        else if(iter->get_node_type()==__call_function)
        {
            error_interrupt(__function_returned_value_be_assigned,iter->get_node_line());
            return -1;
        }
    }
    if(assigned_addr)
    {
        // delete the value that assigned_addr points to and update it with a new one
        nasal_gc.reference_delete(*assigned_addr);
        switch(nasal_gc.get_scalar(used_data_addr).get_type())
        {
            case scalar_nil:
                *assigned_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(*assigned_addr).set_type(scalar_nil);
                break;
            case scalar_number:
                *assigned_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(*assigned_addr).set_type(scalar_number);
                nasal_gc.get_scalar(*assigned_addr).get_number().deep_copy(nasal_gc.get_scalar(used_data_addr).get_number());
                break;
            case scalar_string:
                *assigned_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(*assigned_addr).set_type(scalar_string);
                nasal_gc.get_scalar(*assigned_addr).get_string().deep_copy(nasal_gc.get_scalar(used_data_addr).get_string());
                break;
            case scalar_function:
                *assigned_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(*assigned_addr).set_type(scalar_function);
                nasal_gc.get_scalar(*assigned_addr).get_function().deep_copy(nasal_gc.get_scalar(used_data_addr).get_function());
                break;
            case scalar_vector:
            case scalar_hash:
                *assigned_addr=used_data_addr;
                nasal_gc.reference_add(used_data_addr);
                break;
        }
    }
    else
    {
        error_interrupt(__subvec_value_be_assigned,node.get_children().back().get_node_line());
        return -1;
    }
    /*
        assigned_addr=find_address();
        while(children.size())
        {
            assigned_addr=new_addr();
        }
        *assigned_addr->refcnt--;
        *assigned_addr=new_value_addr;
    */
    // data_addr is only a parameter here,and it's refcnt has not been changed when using it here
    nasal_gc.reference_add(*assigned_addr);
    for(int i=0;i<tmp_subvec_addr.size();++i)
        nasal_gc.reference_delete(tmp_subvec_addr[i]);
    return *assigned_addr;
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
    if(addr>0 && !nasal_gc.get_reference(addr))
    {
        std::cout<<">> [Runtime] line "<<node.get_node_line()<<": \'"<<tmp_id_name<<"\' has 0 reference ,address: ";
        prt_hex(addr);
        std::cout<<std::endl;
        return -1;
    }
    if(addr<0)
    {
        // built in function call only needs local_scope and function name
        // because all the built in functions are wraped in lib functions that can be found in global scope
        // you cannot use built in functions without wraping it in a function that written by yourself
        addr=builtin_function(local_scope,tmp_id_name);
        if(addr>=0) return addr;
    }
    if(addr<0)
    {
        error_interrupt(__undefined_identifier,node.get_node_line());
        return -1;
    }
    int last_hash_addr=-1;
    // after calculation or assignment/definition,reference counter of this address will -1
    // to avoid nasal_gc collecting this address,reference counter+1 here
    nasal_gc.reference_add(addr);
    for(std::list<abstract_syntax_tree>::iterator iter=node.get_children().begin();iter!=node.get_children().end();++iter)
    {
        if(nasal_gc.get_scalar(addr).get_type()==scalar_hash)
        {
            int tmp_hash_addr=last_hash_addr;
            last_hash_addr=addr;
            nasal_gc.reference_add(last_hash_addr);
            if(tmp_hash_addr>=0)
                nasal_gc.reference_delete(tmp_hash_addr);
        }
        // call vector/special call hash/subvec
        // the special type of calling hash like a["name"] is also generated as calling vector
        if(iter->get_node_type()==__call_vector)
            addr=bracket_call_vector(local_scope,iter,addr);
        // call hash identifier.identifier
        else if(iter->get_node_type()==__call_hash)
            addr=dot_call_hash(local_scope,iter,addr);
        // call function identifier(...)
        else if(iter->get_node_type()==__call_function)
            addr=call_function(local_scope,iter,addr,last_hash_addr);
        if(addr<0) break;
    }
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
                if(new_addr<0) return;
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
            if(data_addr<0) return;
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
        if(now_scope.find(id_name)!=now_scope.end())
            nasal_gc.reference_delete(now_scope[id_name]);
        now_scope[id_name]=new_data_addr;
    }
    return;
}
int nasal_runtime::loop_expr(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    int loop_type=node.get_node_type();
    int state=__state_no_operation;
    if(loop_type==__while)
    {
        while(check_condition(local_scope,node.get_children().front()))
        {
            state=block_proc(local_scope,node.get_children().back());
            if(state==__state_break || state==__state_return || state==__state_error)
                break;
        }
    }
    else if(loop_type==__foreach)
    {
        // foreach(i;vector)
        // foreach gives elements in the vector to the identifier 'i' and the 'i' will take part in calculation
        std::list<abstract_syntax_tree>::iterator iter=node.get_children().begin();
        abstract_syntax_tree& assignment_ast=*iter;// definition/assignment
        if(iter->get_node_type()==__definition)
        {
            std::string identifier_name=iter->get_children().front().get_var_name();
            int new_identifier_addr=nasal_gc.gc_alloc();
            local_scope.back()[identifier_name]=new_identifier_addr;
            nasal_gc.get_scalar(new_identifier_addr).set_type(scalar_nil);
            assignment_ast=iter->get_children().front();
        }
        else
            assignment_ast=*iter;
        ++iter;
        int vec_addr=calculation(local_scope,*iter);
        if(vec_addr<0)
            return __state_error;
        if(nasal_gc.get_scalar(vec_addr).get_type()!=scalar_vector)
        {
            error_interrupt(__forindex_foreach_not_vector,iter->get_node_line());
            return __state_error;
        }
        ++iter;
        for(int i=0;i<nasal_gc.get_scalar(vec_addr).get_vector().get_size();++i)
        {
            int now_step_elem_addr=nasal_gc.get_scalar(vec_addr).get_vector().get_elem(i);
            int tmp_val=assignment(local_scope,assignment_ast,now_step_elem_addr);
            if(tmp_val>=0)
                nasal_gc.reference_delete(tmp_val);
            state=block_proc(local_scope,*iter);
            if(state==__state_break || state==__state_return || state==__state_error)
                break;
        }
        nasal_gc.reference_delete(vec_addr);
    }
    else if(loop_type==__forindex)
    {
        // forindex(i;vector)
        // forindex gives the index of elements in the vector to identifier 'i'
        // and the 'i' will take part in the calculation
        std::list<abstract_syntax_tree>::iterator iter=node.get_children().begin();
        abstract_syntax_tree& assignment_ast=*iter;// definition/assignment
        if(iter->get_node_type()==__definition)
        {
            std::string identifier_name=iter->get_children().front().get_var_name();
            int new_identifier_addr=nasal_gc.gc_alloc();
            local_scope.back()[identifier_name]=new_identifier_addr;
            nasal_gc.get_scalar(new_identifier_addr).set_type(scalar_number);
            nasal_gc.get_scalar(new_identifier_addr).get_number().set_number(0);
            assignment_ast=iter->get_children().front();
        }
        else
            assignment_ast=*iter;
        ++iter;
        int vec_addr=calculation(local_scope,*iter);
        if(vec_addr<0)
            return __state_error;
        if(nasal_gc.get_scalar(vec_addr).get_type()!=scalar_vector)
        {
            error_interrupt(__forindex_foreach_not_vector,iter->get_node_line());
            return __state_error;
        }
        ++iter;
        for(int i=0;i<nasal_gc.get_scalar(vec_addr).get_vector().get_size();++i)
        {
            int tmp_addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(tmp_addr).set_type(scalar_number);
            nasal_gc.get_scalar(tmp_addr).get_number().set_number((double)i);
            int tmp_val=assignment(local_scope,assignment_ast,tmp_addr);
            if(tmp_val>=0)
                nasal_gc.reference_delete(tmp_val);
            nasal_gc.reference_delete(tmp_addr);
            state=block_proc(local_scope,*iter);
            if(state==__state_break || state==__state_return || state==__state_error)
                break;
        }
        nasal_gc.reference_delete(vec_addr);
    }
    else if(loop_type==__for)
    {
        std::list<abstract_syntax_tree>::iterator iter=node.get_children().begin();
        if(iter->get_node_type()!=__null_type)
            definition(local_scope,local_scope.back(),*iter);
        // definition before loop
        ++iter; std::list<abstract_syntax_tree>::iterator condition_iterator=iter;
        // conditional
        ++iter; std::list<abstract_syntax_tree>::iterator step_iterator=iter;
        // step length
        ++iter; std::list<abstract_syntax_tree>::iterator block_proc_iterator=iter;
        // run block
        while(check_condition(local_scope,*condition_iterator))
        {
            state=block_proc(local_scope,*block_proc_iterator);
            if(state==__state_break || state==__state_return || state==__state_error)
                break;
            // step update here
            if(step_iterator->get_node_type()!=__null_type)
            {
                int assign_addr=calculation(local_scope,*step_iterator);
                if(assign_addr<0)
                {
                    state=__state_error;
                    break;
                }
                nasal_gc.reference_delete(assign_addr);
            }
        }
    }
    for(std::map<std::string,int>::iterator i=local_scope.back().begin();i!=local_scope.back().end();++i)
        nasal_gc.reference_delete(i->second);
    local_scope.pop_back();
    return state;
}
int nasal_runtime::conditional(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int ret_state=__state_no_operation;
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
    {
        int condition_node_type=i->get_node_type();
        if(condition_node_type==__if || condition_node_type==__elsif)
        {
            if(check_condition(local_scope,i->get_children().front()))
            {
                int state=block_proc(local_scope,i->get_children().back());
                ret_state=state;
                break;
            }
        }
        else
        {
            int state=block_proc(local_scope,i->get_children().front());
            ret_state=state;
            break;
        }
    }
    for(std::map<std::string,int>::iterator i=local_scope.back().begin();i!=local_scope.back().end();++i)
        nasal_gc.reference_delete(i->second);
    local_scope.pop_back();
    return ret_state;
}
int nasal_runtime::block_proc(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int state=__state_no_operation;
    int closure_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(closure_addr).set_type(scalar_closure);
    std::vector<int> assignedfunc_addrs;
    for(std::list<abstract_syntax_tree>::iterator iter=node.get_children().begin();iter!=node.get_children().end();++iter)
    {
        // use local value node_type to avoid calling function too many times.
        int node_type=iter->get_node_type();
        if(node_type==__number || node_type==__string)
            ;
        else if(node_type==__id)
        {
            int addr=this->call_identifier(local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__vector)
        {
            int addr=this->vector_generation(local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__hash)
        {
            int addr=this->hash_generation(local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__function)
        {
            int addr=this->function_generation(local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__add_operator  || node_type==__sub_operator || node_type==__mul_operator  || node_type==__div_operator || node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more || node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark ||
                node_type==__equal || node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
        {
            int addr=this->calculation(local_scope,*iter);
            if(addr>=0)
            {
                if(nasal_gc.get_scalar(addr).get_type()==scalar_function) assignedfunc_addrs.push_back(addr);
                nasal_gc.reference_delete(addr);
            }
        }
        else if(node_type==__definition)
            this->definition(local_scope,local_scope.back(),*iter);
        else if(node_type==__conditional)
            state=this->conditional(local_scope,*iter);
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            state=this->loop_expr(local_scope,*iter);
        else if(node_type==__return)
        {
            if(!iter->get_children().size())
            {
                function_returned_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(function_returned_addr).set_type(scalar_nil);
            }
            else function_returned_addr=calculation(local_scope,iter->get_children().front());
            state=__state_return;
        }
        if(runtime_error_exit_mark>=0)
        {
            state=__state_error;
            break;
        }
        if(state==__state_break || state==__state_continue || state==__state_return || state==__state_error)
            break;
    }
    // update_closure
    nasal_gc.get_scalar(closure_addr).get_closure().set_local_scope(local_scope);
    update_closure(assignedfunc_addrs,closure_addr);
    nasal_gc.reference_delete(closure_addr);
    return state;
}
int nasal_runtime::func_proc(
    std::list<std::map<std::string,int> >& parameters_assist_scope,// scope that used to generate parameters
    std::list<std::map<std::string,int> >& local_scope,// running scope,often gets the scope that calls it
    abstract_syntax_tree& parameter_list,              // parameter list format of nasal function
    abstract_syntax_tree& func_root,                   // main runnning block of nasal function
    abstract_syntax_tree& input_parameters,            // input parameters when calling this nasal function
    int called_hash_addr                               // if called a hash before calling this nasal function,this address will be given to 'me'
)
{
    // local scope should not used as reference
    // because this will change the elements in scalar_function value itself
    // if you call the same function in the function,the scope will be in a mess
    function_returned_addr=-1;
    std::map<std::string,int> new_scope;
    // new_scope will be used in parameters initialling process
    // if push back this scope to local_scope
    // there may be an error if an identifier has the same name as one identifier in the local_scope before

    if(called_hash_addr>=0)
    {
        new_scope["me"]=called_hash_addr;
        nasal_gc.reference_add(called_hash_addr);
    }
    // loading parameters
    std::vector<std::string> para_name_list;
    int dynamic_args=-1;
    for(std::list<abstract_syntax_tree>::iterator iter=parameter_list.get_children().begin();iter!=parameter_list.get_children().end();++iter)
    {
        if(iter->get_node_type()==__id)
        {
            para_name_list.push_back(iter->get_var_name());
            new_scope[para_name_list.back()]=-1;
        }
        else if(iter->get_node_type()==__default_parameter)
        {
            para_name_list.push_back(iter->get_children().front().get_var_name());
            int default_val_addr=calculation(parameters_assist_scope,iter->get_children().back());
            if(default_val_addr<0)
                return -1;
            new_scope[para_name_list.back()]=default_val_addr;
        }
        else if(iter->get_node_type()==__dynamic_id)
        {
            dynamic_args=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(dynamic_args).set_type(scalar_vector);
            new_scope[iter->get_var_name()]=dynamic_args;
            break;
        }
    }
    if(!input_parameters.get_children().empty() && input_parameters.get_children().front().get_node_type()!=__special_parameter)
    {
        int tmp_ptr=0;
        for(std::list<abstract_syntax_tree>::iterator iter=input_parameters.get_children().begin();iter!=input_parameters.get_children().end();++iter)
        {
            if(tmp_ptr>=para_name_list.size())
            {
                if(dynamic_args>=0)
                {
                    int val_addr=calculation(parameters_assist_scope,*iter);
                    if(val_addr<0)
                        return -1;
                    nasal_gc.get_scalar(dynamic_args).get_vector().vec_push(val_addr);
                }
                else
                    break;
            }
            else
            {
                int val_addr=calculation(parameters_assist_scope,*iter);
                if(val_addr<0)
                    return -1;
                if(new_scope[para_name_list[tmp_ptr]]<0)
                    new_scope[para_name_list[tmp_ptr]]=val_addr;
                else
                {
                    nasal_gc.reference_delete(new_scope[para_name_list[tmp_ptr]]);
                    new_scope[para_name_list[tmp_ptr]]=val_addr;
                }
            }
            ++tmp_ptr;
        }
    }
    else// special parameter f(a:1,b:2)
    {
        for(std::list<abstract_syntax_tree>::iterator iter=input_parameters.get_children().begin();iter!=input_parameters.get_children().end();++iter)
        {
            std::string tmp_para_name=iter->get_children().front().get_var_name();
            if(new_scope.find(tmp_para_name)!=new_scope.end())
            {
                int val_addr=calculation(parameters_assist_scope,iter->get_children().back());
                if(val_addr<0)
                    return -1;
                if(new_scope[tmp_para_name]<0)
                    new_scope[tmp_para_name]=val_addr;
                else
                {
                    nasal_gc.reference_delete(new_scope[tmp_para_name]);
                    new_scope[tmp_para_name]=val_addr;
                }
            }
        }
    }
    for(int i=0;i<para_name_list.size();++i)
        if(new_scope[para_name_list[i]]<0)
        {
            error_interrupt(__call_function_lack_para,input_parameters.get_node_line());
            return -1;
        }
    local_scope.push_back(new_scope);
    // process
    int state=__state_no_operation;
    int closure_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(closure_addr).set_type(scalar_closure);
    std::vector<int> assignedfunc_addrs;
    for(std::list<abstract_syntax_tree>::iterator iter=func_root.get_children().begin();iter!=func_root.get_children().end();++iter)
    {
        // use local value node_type to avoid calling function too many times.
        int node_type=iter->get_node_type();
        if(node_type==__number || node_type==__string)
            ;
        else if(node_type==__id)
        {
            int addr=this->call_identifier(local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__vector)
        {
            int addr=this->vector_generation(local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        } 
        else if(node_type==__hash)
        {
            int addr=this->hash_generation(local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__function)
        {
            int addr=this->function_generation(local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__add_operator  || node_type==__sub_operator || node_type==__mul_operator  || node_type==__div_operator || node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more || node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark ||
                node_type==__equal || node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
        {
            int addr=this->calculation(local_scope,*iter);
            if(addr>=0)
            {
                if(nasal_gc.get_scalar(addr).get_type()==scalar_function) assignedfunc_addrs.push_back(addr);
                nasal_gc.reference_delete(addr);
            }
        }
        else if(node_type==__definition)
            this->definition(local_scope,local_scope.back(),*iter);
        else if(node_type==__conditional)
            state=this->conditional(local_scope,*iter);
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            state=this->loop_expr(local_scope,*iter);
        else if(node_type==__return)
        {
            if(!iter->get_children().size())
            {
                function_returned_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(function_returned_addr).set_type(scalar_nil);
            }
            else function_returned_addr=calculation(local_scope,iter->get_children().front());
            state=__state_return;
        }
        if(runtime_error_exit_mark>=0)
            break;
        if(state==__state_return)
            break;
        else if(state==__state_break)
        {
            error_interrupt(__break_not_used_in_loop,iter->get_node_line());
            return -1;
        }
        else if(state==__state_continue)
        {
            error_interrupt(__continue_not_used_in_loop,iter->get_node_line());
            return -1;
        }
        else if(state==__state_error)
            return -1;
        else if(state==__state_no_operation)
            ;
    }
    if(state!=__state_return)
    {
        function_returned_addr=nasal_gc.gc_alloc();
        nasal_gc.get_scalar(function_returned_addr).set_type(scalar_nil);
    }
    // update closure
    nasal_gc.get_scalar(closure_addr).get_closure().set_local_scope(local_scope);
    update_closure(assignedfunc_addrs,closure_addr);
    nasal_gc.reference_delete(closure_addr);
    for(std::map<std::string,int>::iterator i=local_scope.back().begin();i!=local_scope.back().end();++i)
        nasal_gc.reference_delete(i->second);
    local_scope.pop_back();
    return function_returned_addr;
}
int nasal_runtime::builtin_function(std::list<std::map<std::string,int> >& local_scope,std::string func_name)
{
    int ret_addr=-1;
    if(func_name=="nasal_call_builtin_push_back")
        ret_addr=append(local_scope);
    else if(func_name=="nasal_call_builtin_set_size")
        ret_addr=setsize(local_scope);
    else if(func_name=="nasal_call_builtin_subvec")
        ret_addr=subvec(local_scope);
    else if(func_name=="nasal_call_builtin_c_std_puts")
        ret_addr=print(local_scope);
    else if(func_name=="nasal_call_builtin_scalar_type")
        ret_addr=system_type(local_scope);
    else if(func_name=="nasal_call_builtin_sin")
        ret_addr=nas_trigfunc(local_scope,0);
    else if(func_name=="nasal_call_builtin_cos")
        ret_addr=nas_trigfunc(local_scope,1);
    else if(func_name=="nasal_call_builtin_tan")
        ret_addr=nas_trigfunc(local_scope,2);
    else if(func_name=="nasal_call_builtin_exp")
        ret_addr=nas_exp(local_scope);
    // "nasal_call_builtin_contains",
    // "nasal_call_builtin_delete",
    // "nasal_call_builtin_trans_int",
    // "nasal_call_builtin_trans_num",
    // "nasal_call_builtin_get_keys",
    // "nasal_call_builtin_pop_back",
    // "nasal_call_builtin_sizeof",
    // "nasal_call_builtin_str_cmp_equal",
    // "nasal_call_builtin_cmp",
    // "nasal_call_builtin_cpp_sort",
    // "nasal_call_builtin_substr",
    // "nasal_call_builtin_sprintf",
    // "nasal_call_builtin_find_first_occur",
    // "nasal_call_builtin_split",
    // "nasal_call_builtin_rand",
    // "nasal_call_builtin_get_id",

    // "nasal_call_builtin_bitcalc",
    // "nasal_call_builtin_sbitcalc",
    // "nasal_call_builtin_setbit",
    // "nasal_call_builtin_null_string_gen",

    // "nasal_call_builtin_cpp_math_ln",
    // "nasal_call_builtin_cpp_math_sqrt",
    // "nasal_call_builtin_cpp_atan2",
    return ret_addr;
}

void nasal_runtime::main_proc(abstract_syntax_tree& root)
{
    time_t begin_time,end_time;
    begin_time=std::time(NULL);
    // initializing global scope and nasal_gc
    // runtime_error_exit_mark is set to -1,if runtime_error_exit_mark >=0,this means an error occurred
    global_scope.clear();
    main_local_scope.clear();
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
        int state=__state_no_operation;
        if(node_type==__number || node_type==__string)
            ;
        else if(node_type==__id)
        {
            int addr=this->call_identifier(main_local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__vector)
        {
            int addr=this->vector_generation(main_local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__hash)
        {
            int addr=this->hash_generation(main_local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__function)
        {
            int addr=this->function_generation(main_local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
        }
        else if(node_type==__add_operator  || node_type==__sub_operator || node_type==__mul_operator  || node_type==__div_operator || node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more || node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark ||
                node_type==__equal || node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
        {
            int addr=this->calculation(main_local_scope,*iter);
            if(addr>=0) nasal_gc.reference_delete(addr);
            // new functions in global scope must set its closure_update_state to true
            // or its local scope maybe changed after running other expressions
            if(nasal_gc.get_scalar(addr).get_type()==scalar_function && !nasal_gc.get_scalar(addr).get_function().get_closure_update_state())
                nasal_gc.get_scalar(addr).get_function().set_closure_update_state(true);
        }
        else if(node_type==__definition)
            this->definition(main_local_scope,global_scope,*iter);
        else if(node_type==__conditional)
            state=this->conditional(main_local_scope,*iter);
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            state=this->loop_expr(main_local_scope,*iter);
        if(runtime_error_exit_mark>=0)
            break;
        if(state==__state_no_operation)
            ;
        else if(state==__state_break)
        {
            error_interrupt(__break_not_used_in_loop,iter->get_node_line());
            break;
        }
        else if(state==__state_continue)
        {
            error_interrupt(__continue_not_used_in_loop,iter->get_node_line());
            break;
        }
        else if(state==__state_return)
        {
            error_interrupt(__return_not_used_in_function,iter->get_node_line());
            break;
        }
        else if(state==__state_error)
            break;
    }
    end_time=std::time(NULL);
    std::cout<<std::endl<<">> [Runtime] process exited after "<<end_time-begin_time<<" s ."<<std::endl;
    global_scope.clear();
    main_local_scope.clear();
    nasal_gc.gc_init();
    return;
}

#endif
