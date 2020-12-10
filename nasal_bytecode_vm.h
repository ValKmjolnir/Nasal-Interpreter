#ifndef __NASAL_BYTECODE_VM_H__
#define __NASAL_BYTECODE_VM_H__

class nasal_bytecode_vm
{
private:
    int error;
    int ptr;
    int global_scope_addr;
    nasal_virtual_machine vm;
    std::vector<opcode> exec_code;
    std::stack<int> value_stack;
    std::stack<int> local_scope_stack;
    std::stack<int> slice_stack;
    std::stack<int> call_stack;
    std::vector<std::string> string_table;
    std::vector<double> number_table;
    std::vector<void (nasal_bytecode_vm::*)()> opr_table;
    std::map<std::string,int (*)(int x,nasal_virtual_machine& vm)> builtin_func_hashmap;
    void die(std::string);
    bool check_condition(int);
    void opr_nop();
    void opr_load();
    void opr_pushnum();
    void opr_pushone();
    void opr_pushzero();
    void opr_pushnil();
    void opr_pushstr();
    void opr_newvec();
    void opr_newhash();
    void opr_newfunc();
    void opr_vecapp();
    void opr_hashapp();
    void opr_para();
    void opr_defpara();
    void opr_dynpara();
    void opr_entry();
    void opr_unot();
    void opr_usub();
    void opr_add();
    void opr_sub();
    void opr_mul();
    void opr_div();
    void opr_lnk();
    void opr_addeq();
    void opr_subeq();
    void opr_muleq();
    void opr_diveq();
    void opr_lnkeq();
    void opr_meq();
    void opr_eq();
    void opr_neq();
    void opr_less();
    void opr_leq();
    void opr_grt();
    void opr_geq();
    void opr_pop();
    void opr_jmp();
    void opr_jmptrue();
    void opr_jmpfalse();
    void opr_forindex();
    void opr_foreach();
    void opr_call();
    void opr_callv();
    void opr_callvi();
    void opr_callh();
    void opr_callf();
    void opr_builtincall();
    void opr_slicebegin();
    void opr_sliceend();
    void opr_slice();
    void opr_slice2();
    void opr_mcall();
    void opr_mcallv();
    void opr_mcallh();
    void opr_return();
public:
    nasal_bytecode_vm();
    ~nasal_bytecode_vm();
    void clear();
    void set_string_table(std::vector<std::string>&);
    void set_number_table(std::vector<double>&);
    void set_exec_code(std::vector<opcode>&);
    void run();
};

nasal_bytecode_vm::nasal_bytecode_vm()
{
    local_scope_stack.push(-1);

    struct
    {
        int op;
        void (nasal_bytecode_vm::*ptr)();
    }function_table[]=
    {
        {op_nop,         nasal_bytecode_vm::opr_nop},
        {op_load,        nasal_bytecode_vm::opr_load},
        {op_pushnum,     nasal_bytecode_vm::opr_pushnum},
        {op_pushone,     nasal_bytecode_vm::opr_pushone},
        {op_pushzero,    nasal_bytecode_vm::opr_pushzero},
        {op_pushnil,     nasal_bytecode_vm::opr_pushnil},
        {op_pushstr,     nasal_bytecode_vm::opr_pushstr},
        {op_newvec,      nasal_bytecode_vm::opr_newvec},
        {op_newhash,     nasal_bytecode_vm::opr_newhash},
        {op_newfunc,     nasal_bytecode_vm::opr_newfunc},
        {op_vecapp,      nasal_bytecode_vm::opr_vecapp},
        {op_hashapp,     nasal_bytecode_vm::opr_hashapp},
        {op_para,        nasal_bytecode_vm::opr_para},
        {op_defpara,     nasal_bytecode_vm::opr_defpara},
        {op_dynpara,     nasal_bytecode_vm::opr_dynpara},
        {op_entry,       nasal_bytecode_vm::opr_entry},
        {op_unot,        nasal_bytecode_vm::opr_unot},
        {op_usub,        nasal_bytecode_vm::opr_usub},
        {op_add,         nasal_bytecode_vm::opr_add},
        {op_sub,         nasal_bytecode_vm::opr_sub},
        {op_mul,         nasal_bytecode_vm::opr_mul},
        {op_div,         nasal_bytecode_vm::opr_div},
        {op_lnk,         nasal_bytecode_vm::opr_lnk},
        {op_addeq,       nasal_bytecode_vm::opr_addeq},
        {op_subeq,       nasal_bytecode_vm::opr_subeq},
        {op_muleq,       nasal_bytecode_vm::opr_muleq},
        {op_diveq,       nasal_bytecode_vm::opr_diveq},
        {op_lnkeq,       nasal_bytecode_vm::opr_lnkeq},
        {op_meq,         nasal_bytecode_vm::opr_meq},
        {op_eq,          nasal_bytecode_vm::opr_eq},
        {op_neq,         nasal_bytecode_vm::opr_neq},
        {op_less,        nasal_bytecode_vm::opr_less},
        {op_leq,         nasal_bytecode_vm::opr_leq},
        {op_grt,         nasal_bytecode_vm::opr_grt},
        {op_geq,         nasal_bytecode_vm::opr_geq},
        {op_pop,         nasal_bytecode_vm::opr_pop},
        {op_jmp,         nasal_bytecode_vm::opr_jmp},
        {op_jmptrue,     nasal_bytecode_vm::opr_jmptrue},
        {op_jmpfalse,    nasal_bytecode_vm::opr_jmpfalse},
        {op_forindex,    nasal_bytecode_vm::opr_forindex},
        {op_foreach,     nasal_bytecode_vm::opr_foreach},
        {op_call,        nasal_bytecode_vm::opr_call},
        {op_callv,       nasal_bytecode_vm::opr_callv},
        {op_callvi,      nasal_bytecode_vm::opr_callvi},
        {op_callh,       nasal_bytecode_vm::opr_callh},
        {op_callf,       nasal_bytecode_vm::opr_callf},
        {op_builtincall, nasal_bytecode_vm::opr_builtincall},
        {op_slicebegin,  nasal_bytecode_vm::opr_slicebegin},
        {op_sliceend,    nasal_bytecode_vm::opr_sliceend},
        {op_slice,       nasal_bytecode_vm::opr_slice},
        {op_slice2,      nasal_bytecode_vm::opr_slice2},
        {op_mcall,       nasal_bytecode_vm::opr_mcall},
        {op_mcallv,      nasal_bytecode_vm::opr_mcallv},
        {op_mcallh,      nasal_bytecode_vm::opr_mcallh},
        {op_return,      nasal_bytecode_vm::opr_return},
        {-1,NULL}
    };
    for(int i=0;function_table[i].ptr;++i)
        opr_table.push_back(NULL);
    for(int i=0;function_table[i].ptr;++i)
        opr_table[function_table[i].op]=function_table[i].ptr;
    for(int i=0;builtin_func_table[i].func_pointer;++i)
        builtin_func_hashmap[builtin_func_table[i].func_name]=builtin_func_table[i].func_pointer;
    return;
}
nasal_bytecode_vm::~nasal_bytecode_vm()
{
    opr_table.clear();
    return;
}
void nasal_bytecode_vm::clear()
{
    vm.clear();
    global_scope_addr=-1;
    while(!value_stack.empty())value_stack.pop();
    while(local_scope_stack.top()!=-1)local_scope_stack.pop();
    while(!slice_stack.empty())slice_stack.pop();
    while(!call_stack.empty())call_stack.pop();
    return;
}
void nasal_bytecode_vm::die(std::string str)
{
    ++error;
    std::cout<<">> [vm] "<<str<<'\n';
    return;
}
bool nasal_bytecode_vm::check_condition(int value_addr)
{
    if(value_addr<0)
        return false;
    int type=vm.gc_get(value_addr).get_type();
    if(type==vm_nil || type==vm_vector || type==vm_hash || type==vm_function)
        return false;
    else if(type==vm_string)
    {
        std::string str=vm.gc_get(value_addr).get_string();
        double number=trans_string_to_number(str);
        if(std::isnan(number))
            return str.length()!=0;
        return (number!=0);
    }
    else if(type==vm_number)
        return (vm.gc_get(value_addr).get_number()!=0);
    return false;
}
void nasal_bytecode_vm::opr_nop()
{
    return;
}
void nasal_bytecode_vm::opr_load()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    vm.gc_get(global_scope_addr).get_closure().add_new_value(string_table[exec_code[ptr].index],val_addr);
    return;
}
void nasal_bytecode_vm::opr_pushnum()
{
    int val_addr=vm.gc_alloc(vm_number);
    vm.gc_get(val_addr).set_number(number_table[exec_code[ptr].index]);
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_pushone()
{
    int val_addr=vm.gc_alloc(vm_number);
    vm.gc_get(val_addr).set_number(1);
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_pushzero()
{
    int val_addr=vm.gc_alloc(vm_number);
    vm.gc_get(val_addr).set_number(0);
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_pushnil()
{
    int val_addr=vm.gc_alloc(vm_nil);
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_pushstr()
{
    int val_addr=vm.gc_alloc(vm_string);
    vm.gc_get(val_addr).set_string(string_table[exec_code[ptr].index]);
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_newvec()
{
    int val_addr=vm.gc_alloc(vm_vector);
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_newhash()
{
    int val_addr=vm.gc_alloc(vm_hash);
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_newfunc()
{
    int val_addr=vm.gc_alloc(vm_function);
    if(local_scope_stack.top()>=0)
        vm.gc_get(val_addr).get_func().set_closure_addr(local_scope_stack.top());
    else
    {
        int tmp_closure=vm.gc_alloc(vm_closure);
        vm.gc_get(val_addr).get_func().set_closure_addr(tmp_closure);
        vm.del_reference(tmp_closure);
    }
    // unfinished
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_vecapp()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    vm.gc_get(value_stack.top()).get_vector().add_elem(val_addr);
    return;
}
void nasal_bytecode_vm::opr_hashapp()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    int str_addr=value_stack.top();
    value_stack.pop();
    int hash_addr=value_stack.top();
    vm.gc_get(hash_addr).get_hash().add_elem(vm.gc_get(str_addr).get_string(),val_addr);
    vm.del_reference(str_addr);
    return;
}
void nasal_bytecode_vm::opr_para(){return;}
void nasal_bytecode_vm::opr_defpara(){return;}
void nasal_bytecode_vm::opr_dynpara(){return;}
void nasal_bytecode_vm::opr_entry()
{
    vm.gc_get(value_stack.top()).get_func().set_entry(exec_code[ptr].index);
    return;
}
void nasal_bytecode_vm::opr_unot()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    nasal_scalar& ref=vm.gc_get(val_addr);
    int type=ref.get_type();
    int new_value_address=-1;
    if(type==vm_nil)
    {
        new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number(1);
    }
    else if(type==vm_number)
    {
        double number=(ref.get_number()==0);
        new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number(number);
    }
    else if(type==vm_string)
    {
        double number=trans_string_to_number(ref.get_string());
        if(std::isnan(number))
        {
            new_value_address=vm.gc_alloc(vm_number);
            vm.gc_get(new_value_address).set_number(!(ref.get_string()).length());
        }
        else
        {
            new_value_address=vm.gc_alloc(vm_number);
            vm.gc_get(new_value_address).set_number((double)(number==0));
        }
    }
    else
        die("unot: incorrect value type");
    value_stack.push(new_value_address);
    vm.del_reference(val_addr);
    return;
}
void nasal_bytecode_vm::opr_usub()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    nasal_scalar& ref=vm.gc_get(val_addr);
    int type=ref.get_type();
    double num=(1/0.0)+(-1/0.0);
    if(type==vm_number)
        num=ref.get_number();
    else if(type==vm_string)
        num=trans_string_to_number(ref.get_string());
    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number(-num);
    value_stack.push(new_value_address);
    vm.del_reference(val_addr);
    return;
}
void nasal_bytecode_vm::opr_add()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    double a_num=(1/0.0)+(-1/0.0);
    double b_num=(1/0.0)+(-1/0.0);
    if(a_ref_type==vm_number)
        a_num=a_ref.get_number();
    else if(a_ref_type==vm_string)
        a_num=trans_string_to_number(a_ref.get_string());
    if(b_ref_type==vm_number)
        b_num=b_ref.get_number();
    else if(b_ref_type==vm_string)
        b_num=trans_string_to_number(b_ref.get_string());

    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number(a_num+b_num);
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_sub()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    double a_num=(1/0.0)+(-1/0.0);
    double b_num=(1/0.0)+(-1/0.0);
    if(a_ref_type==vm_number)
        a_num=a_ref.get_number();
    else if(a_ref_type==vm_string)
        a_num=trans_string_to_number(a_ref.get_string());
    if(b_ref_type==vm_number)
        b_num=b_ref.get_number();
    else if(b_ref_type==vm_string)
        b_num=trans_string_to_number(b_ref.get_string());

    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number(a_num-b_num);
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_mul()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    double a_num=(1/0.0)+(-1/0.0);
    double b_num=(1/0.0)+(-1/0.0);
    if(a_ref_type==vm_number)
        a_num=a_ref.get_number();
    else if(a_ref_type==vm_string)
        a_num=trans_string_to_number(a_ref.get_string());
    if(b_ref_type==vm_number)
        b_num=b_ref.get_number();
    else if(b_ref_type==vm_string)
        b_num=trans_string_to_number(b_ref.get_string());

    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number(a_num*b_num);
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_div()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    double a_num=(1/0.0)+(-1/0.0);
    double b_num=(1/0.0)+(-1/0.0);
    if(a_ref_type==vm_number)
        a_num=a_ref.get_number();
    else if(a_ref_type==vm_string)
        a_num=trans_string_to_number(a_ref.get_string());
    if(b_ref_type==vm_number)
        b_num=b_ref.get_number();
    else if(b_ref_type==vm_string)
        b_num=trans_string_to_number(b_ref.get_string());

    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number(a_num/b_num);
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_lnk()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    if((a_ref_type!=vm_number && a_ref_type!=vm_string)||(b_ref_type!=vm_number && b_ref_type!=vm_string))
    {
        die("lnk: error value type");
        return;
    }
    std::string a_str=(a_ref_type==vm_number)? trans_number_to_string(a_ref.get_number()):a_ref.get_string();
    std::string b_str=(b_ref_type==vm_number)? trans_number_to_string(b_ref.get_number()):b_ref.get_string();
    int new_value_address=vm.gc_alloc(vm_string);
    vm.gc_get(new_value_address).set_string(a_str+b_str);
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_addeq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr=value_stack.top();
    // unfinished
    return;
}
void nasal_bytecode_vm::opr_subeq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr=value_stack.top();
    // unfinished
    return;
}
void nasal_bytecode_vm::opr_muleq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr=value_stack.top();
    // unfinished
    return;
}
void nasal_bytecode_vm::opr_diveq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr=value_stack.top();
    // unfinished
    return;
}
void nasal_bytecode_vm::opr_lnkeq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr=value_stack.top();
    // unfinished
    return;
}
void nasal_bytecode_vm::opr_meq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr=value_stack.top();
    vm.add_reference(val_addr);
    vm.mem_change(mem_addr,val_addr);
    return;
}
void nasal_bytecode_vm::opr_eq()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    if(val_addr1==val_addr2)
    {
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number(1);
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    if(a_ref_type==vm_nil && b_ref_type==vm_nil)
    {
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number(1);
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    else if((a_ref_type==vm_number || a_ref_type==vm_string) && (b_ref_type==vm_number || b_ref_type==vm_string))
    {
        if(a_ref_type==vm_string && b_ref_type==vm_string)
        {
            std::string astr=a_ref.get_string();
            std::string bstr=b_ref.get_string();
            int new_value_address=vm.gc_alloc(vm_number);
            vm.gc_get(new_value_address).set_number((double)(astr==bstr));
            value_stack.push(new_value_address);
            vm.del_reference(val_addr1);
            vm.del_reference(val_addr2);
            return;
        }
        double a_num;
        double b_num;
        if(a_ref_type==vm_number)
            a_num=a_ref.get_number();
        else
            a_num=trans_string_to_number(a_ref.get_string());
        if(b_ref_type==vm_number)
            b_num=b_ref.get_number();
        else
            b_num=trans_string_to_number(b_ref.get_string());
        
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number((double)(a_num==b_num));
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    else
    {
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number(0);
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    return;
}
void nasal_bytecode_vm::opr_neq()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    if(val_addr1==val_addr2)
    {
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number(0);
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    if(a_ref_type==vm_nil && b_ref_type==vm_nil)
    {
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number(0);
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    else if((a_ref_type==vm_number || a_ref_type==vm_string) && (b_ref_type==vm_number || b_ref_type==vm_string))
    {
        if(a_ref_type==vm_string && b_ref_type==vm_string)
        {
            std::string astr=a_ref.get_string();
            std::string bstr=b_ref.get_string();
            int new_value_address=vm.gc_alloc(vm_number);
            vm.gc_get(new_value_address).set_number((double)(astr!=bstr));
            value_stack.push(new_value_address);
            vm.del_reference(val_addr1);
            vm.del_reference(val_addr2);
            return;
        }
        double a_num;
        double b_num;
        if(a_ref_type==vm_number)
            a_num=a_ref.get_number();
        else
            a_num=trans_string_to_number(a_ref.get_string());
        if(b_ref_type==vm_number)
            b_num=b_ref.get_number();
        else
            b_num=trans_string_to_number(b_ref.get_string());
        
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number((double)(a_num!=b_num));
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    else
    {
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number(1);
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_less()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    if(a_ref_type==vm_string && b_ref_type==vm_string)
    {
        std::string a_str=a_ref.get_string();
        std::string b_str=b_ref.get_string();
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number((double)(a_str<b_str));
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    double a_num=(1/0.0)+(-1/0.0);
    double b_num=(1/0.0)+(-1/0.0);
    if(a_ref_type==vm_number)
        a_num=a_ref.get_number();
    else if(a_ref_type==vm_string)
        a_num=trans_string_to_number(a_ref.get_string());
    if(b_ref_type==vm_number)
        b_num=b_ref.get_number();
    else if(b_ref_type==vm_string)
        b_num=trans_string_to_number(b_ref.get_string());

    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number((double)(a_num<b_num));
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_leq()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    if(a_ref_type==vm_string && b_ref_type==vm_string)
    {
        std::string a_str=a_ref.get_string();
        std::string b_str=b_ref.get_string();
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number((double)(a_str<=b_str));
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    double a_num=(1/0.0)+(-1/0.0);
    double b_num=(1/0.0)+(-1/0.0);
    if(a_ref_type==vm_number)
        a_num=a_ref.get_number();
    else if(a_ref_type==vm_string)
        a_num=trans_string_to_number(a_ref.get_string());
    if(b_ref_type==vm_number)
        b_num=b_ref.get_number();
    else if(b_ref_type==vm_string)
        b_num=trans_string_to_number(b_ref.get_string());

    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number((double)(a_num<=b_num));
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_grt()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    if(a_ref_type==vm_string && b_ref_type==vm_string)
    {
        std::string a_str=a_ref.get_string();
        std::string b_str=b_ref.get_string();
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number((double)(a_str>b_str));
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    double a_num=(1/0.0)+(-1/0.0);
    double b_num=(1/0.0)+(-1/0.0);
    if(a_ref_type==vm_number)
        a_num=a_ref.get_number();
    else if(a_ref_type==vm_string)
        a_num=trans_string_to_number(a_ref.get_string());
    if(b_ref_type==vm_number)
        b_num=b_ref.get_number();
    else if(b_ref_type==vm_string)
        b_num=trans_string_to_number(b_ref.get_string());

    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number((double)(a_num>b_num));
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_geq()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    if(a_ref_type==vm_string && b_ref_type==vm_string)
    {
        std::string a_str=a_ref.get_string();
        std::string b_str=b_ref.get_string();
        int new_value_address=vm.gc_alloc(vm_number);
        vm.gc_get(new_value_address).set_number((double)(a_str>=b_str));
        value_stack.push(new_value_address);
        vm.del_reference(val_addr1);
        vm.del_reference(val_addr2);
        return;
    }
    double a_num=(1/0.0)+(-1/0.0);
    double b_num=(1/0.0)+(-1/0.0);
    if(a_ref_type==vm_number)
        a_num=a_ref.get_number();
    else if(a_ref_type==vm_string)
        a_num=trans_string_to_number(a_ref.get_string());
    if(b_ref_type==vm_number)
        b_num=b_ref.get_number();
    else if(b_ref_type==vm_string)
        b_num=trans_string_to_number(b_ref.get_string());

    int new_value_address=vm.gc_alloc(vm_number);
    vm.gc_get(new_value_address).set_number((double)(a_num>=b_num));
    value_stack.push(new_value_address);
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_pop()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    vm.del_reference(val_addr);
    return;
}
void nasal_bytecode_vm::opr_jmp()
{
    ptr=exec_code[ptr].index-1;
    return;
}
void nasal_bytecode_vm::opr_jmptrue()
{
    if(check_condition(value_stack.top()))
        ptr=exec_code[ptr].index-1;
    return;
}
void nasal_bytecode_vm::opr_jmpfalse()
{
    if(!check_condition(value_stack.top()))
        ptr=exec_code[ptr].index-1;
    return;
}
void nasal_bytecode_vm::opr_forindex(){return;}
void nasal_bytecode_vm::opr_foreach(){return;}
void nasal_bytecode_vm::opr_call()
{
    int val_addr=-1;
    if(local_scope_stack.top()>=0)
        vm.gc_get(local_scope_stack.top()).get_closure().get_value_address(string_table[exec_code[ptr].index]);
    else
        vm.gc_get(global_scope_addr).get_closure().get_value_address(string_table[exec_code[ptr].index]);
    if(val_addr<0)
        die("cannot find symbol named \""+string_table[exec_code[ptr].index]+"\"");
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_callv(){return;}
void nasal_bytecode_vm::opr_callvi()
{
    int val_addr=value_stack.top();
    if(vm.gc_get(val_addr).get_type()!=vm_vector)
    {
        die("multi-definition/multi-assignment must use a vector");
        return;
    }
    int res=vm.gc_get(val_addr).get_vector().get_value_address(exec_code[ptr].index);
    if(res<0)
    {
        die("index out of range");
        return;
    }
    vm.add_reference(res);
    value_stack.push(res);
    return;
}
void nasal_bytecode_vm::opr_callh()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    if(vm.gc_get(val_addr).get_type()!=vm_hash)
    {
        die("must call a hash");
        return;
    }
    int res=vm.gc_get(val_addr).get_hash().get_value_address(string_table[exec_code[ptr].index]);
    vm.add_reference(res);
    vm.del_reference(val_addr);
    value_stack.push(res);
    return;
}
void nasal_bytecode_vm::opr_callf()
{
    int para_addr=value_stack.top();
    value_stack.pop();
    int func_addr=value_stack.top();
    if(vm.gc_get(func_addr).get_type()!=vm_function)
    {
        die("called a value that is not a function");
        return;
    }
    // unfinished
    call_stack.push(ptr);
    ptr=vm.gc_get(func_addr).get_func().get_entry()-1;
    return;
}
void nasal_bytecode_vm::opr_builtincall()
{
    int ret_value_addr=-1;
    std::string val_name=string_table[exec_code[ptr].index];
    if(builtin_func_hashmap.find(val_name)!=builtin_func_hashmap.end())
    {
        ret_value_addr=(*builtin_func_hashmap[val_name])(local_scope_stack.top(),vm);
        error+=builtin_die_state;
    }
    value_stack.push(ret_value_addr);
    return;
}
void nasal_bytecode_vm::opr_slicebegin()
{
    int val_addr=vm.gc_alloc(vm_vector);
    slice_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_sliceend()
{
    int val_addr=slice_stack.top();
    slice_stack.pop();
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_slice()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    int type=vm.gc_get(val_addr).get_type();
    // unfinished
    return;
}
void nasal_bytecode_vm::opr_slice2(){return;}
void nasal_bytecode_vm::opr_mcall()
{
    int mem_addr=-1;
    if(local_scope_stack.top()>=0)
        vm.gc_get(local_scope_stack.top()).get_closure().get_mem_address(string_table[exec_code[ptr].index]);
    else
        vm.gc_get(global_scope_addr).get_closure().get_mem_address(string_table[exec_code[ptr].index]);
    if(mem_addr<0)
        die("cannot find symbol named \""+string_table[exec_code[ptr].index]+"\"");
    value_stack.push(mem_addr);
    return;
}
void nasal_bytecode_vm::opr_mcallv()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    int vec_addr=vm.mem_get(value_stack.top());
    value_stack.pop();
    // unfinished
    return;
}
void nasal_bytecode_vm::opr_mcallh()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    int hash_addr=vm.mem_get(value_stack.top());
    value_stack.pop();
    if(vm.gc_get(hash_addr).get_type()!=vm_hash)
        die("must call a hash");
    else
        value_stack.push(vm.gc_get(hash_addr).get_hash().get_mem_address(vm.gc_get(val_addr).get_string()));
    return;
}
void nasal_bytecode_vm::opr_return()
{
    int closure_addr=local_scope_stack.top();
    local_scope_stack.pop();
    vm.gc_get(closure_addr).get_closure().del_scope();
    vm.del_reference(closure_addr);
    ptr=call_stack.top();
    call_stack.pop();
    return;
}
void nasal_bytecode_vm::set_string_table(std::vector<std::string>& table)
{
    string_table=table;
    return;
}
void nasal_bytecode_vm::set_number_table(std::vector<double>& table)
{
    number_table=table;
    return;
}
void nasal_bytecode_vm::set_exec_code(std::vector<opcode>& exec)
{
    int size=exec.size();
    for(int i=0;i<size;++i)
    {
        opcode tmp;
        tmp=exec[i];
        exec_code.push_back(tmp);
    }
    return;
}
void nasal_bytecode_vm::run()
{
    error=0;
    global_scope_addr=vm.gc_alloc(vm_closure);
    int size=exec_code.size();
    for(ptr=0;ptr<size;++ptr)
    {
        (this->*opr_table[exec_code[ptr].op])();
        if(error)
            break;
    }
    vm.del_reference(global_scope_addr);
    clear();
    return;
}
#endif