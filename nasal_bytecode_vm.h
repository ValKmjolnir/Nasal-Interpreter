#ifndef __NASAL_BYTECODE_VM_H__
#define __NASAL_BYTECODE_VM_H__

class nasal_bytecode_vm
{
private:
    bool main_loop_break_mark;
    int ptr;
    int global_scope_addr;
    // garbage collector and memory manager
    nasal_virtual_machine vm;
    // byte codes store here
    std::vector<opcode> exec_code;
    // main calculation stack
    std::stack<int> value_stack;
    // local scope for function block
    std::stack<int> local_scope_stack;
    // slice stack for vec[val,val,val:val]
    std::stack<int> slice_stack;
    // ptr stack stores address for function to return
    std::stack<int> call_stack;
    // iterator stack for forindex/foreach
    std::stack<int> counter_stack;
    // string table
    std::vector<std::string> string_table;
    // number table
    std::vector<double> number_table;
    // builtin function address table
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
    void opr_counter();
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
    void clear();
    void run(std::vector<std::string>&,std::vector<double>&,std::vector<opcode>&);
    void nas_switch_threading(std::vector<std::string>&,std::vector<double>&,std::vector<opcode>&);
};

nasal_bytecode_vm::nasal_bytecode_vm()
{
    local_scope_stack.push(-1);
    for(int i=0;builtin_func_table[i].func_pointer;++i)
        builtin_func_hashmap[builtin_func_table[i].func_name]=builtin_func_table[i].func_pointer;
    return;
}
void nasal_bytecode_vm::clear()
{
    vm.clear();
    global_scope_addr=-1;
    while(!value_stack.empty())value_stack.pop();
    while(!local_scope_stack.empty())local_scope_stack.pop();
    local_scope_stack.push(-1);
    while(!slice_stack.empty())slice_stack.pop();
    while(!call_stack.empty())call_stack.pop();
    while(!counter_stack.empty())counter_stack.pop();
    string_table.clear();
    number_table.clear();
    exec_code.clear();
    return;
}
void nasal_bytecode_vm::die(std::string str)
{
    std::string numinfo="";
    int num=ptr;
    for(int i=0;i<8;++i)
    {
        int tmp=num&0x0f;
        numinfo=(char)(tmp>9? 'a'+tmp-10:'0'+tmp)+numinfo;
        num>>=4;
    }
    std::cout<<">> [vm] 0x"<<numinfo<<": "<<str<<'\n';
    main_loop_break_mark=false;
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
    // nop is the end of this program
    main_loop_break_mark=false;
    return;
}
void nasal_bytecode_vm::opr_load()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    if(local_scope_stack.top()>=0)
        vm.gc_get(local_scope_stack.top()).get_closure().add_new_value(string_table[exec_code[ptr].index],val_addr);
    else
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
    value_stack.push(vm.gc_alloc(vm_nil));
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
    value_stack.push(vm.gc_alloc(vm_vector));
    return;
}
void nasal_bytecode_vm::opr_newhash()
{
    value_stack.push(vm.gc_alloc(vm_hash));
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
    int hash_addr=value_stack.top();
    vm.gc_get(hash_addr).get_hash().add_elem(string_table[exec_code[ptr].index],val_addr);
    return;
}
void nasal_bytecode_vm::opr_para()
{
    vm.gc_get(value_stack.top()).get_func().add_para(string_table[exec_code[ptr].index]);
    return;
}
void nasal_bytecode_vm::opr_defpara()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    vm.gc_get(value_stack.top()).get_func().add_para(string_table[exec_code[ptr].index],val_addr);
    return;
}
void nasal_bytecode_vm::opr_dynpara()
{
    vm.gc_get(value_stack.top()).get_func().add_para(string_table[exec_code[ptr].index],-1,true);
    return;
}
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
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=vm.mem_get(mem_addr);
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
    vm.add_reference(new_value_address);
    value_stack.push(new_value_address);
    vm.mem_change(mem_addr,new_value_address);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_subeq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=vm.mem_get(mem_addr);
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
    vm.add_reference(new_value_address);
    value_stack.push(new_value_address);
    vm.mem_change(mem_addr,new_value_address);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_muleq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=vm.mem_get(mem_addr);
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
    vm.add_reference(new_value_address);
    value_stack.push(new_value_address);
    vm.mem_change(mem_addr,new_value_address);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_diveq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=vm.mem_get(mem_addr);
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
    vm.add_reference(new_value_address);
    value_stack.push(new_value_address);
    vm.mem_change(mem_addr,new_value_address);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_lnkeq()
{
    int mem_addr=value_stack.top();
    value_stack.pop();
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=vm.mem_get(mem_addr);
    nasal_scalar& a_ref=vm.gc_get(val_addr1);
    nasal_scalar& b_ref=vm.gc_get(val_addr2);
    int a_ref_type=a_ref.get_type();
    int b_ref_type=b_ref.get_type();
    if((a_ref_type!=vm_number && a_ref_type!=vm_string)||(b_ref_type!=vm_number && b_ref_type!=vm_string))
    {
        die("lnkeq: error value type");
        return;
    }
    std::string a_str=(a_ref_type==vm_number)? trans_number_to_string(a_ref.get_number()):a_ref.get_string();
    std::string b_str=(b_ref_type==vm_number)? trans_number_to_string(b_ref.get_number()):b_ref.get_string();
    int new_value_address=vm.gc_alloc(vm_string);
    vm.gc_get(new_value_address).set_string(a_str+b_str);
    vm.add_reference(new_value_address);
    value_stack.push(new_value_address);
    vm.mem_change(mem_addr,new_value_address);
    vm.del_reference(val_addr2);
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
    vm.del_reference(value_stack.top());
    value_stack.pop();
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
void nasal_bytecode_vm::opr_counter()
{
    if(vm.gc_get(value_stack.top()).get_type()!=vm_vector)
    {
        die("cnt: must use vector in forindex/foreach");
        return;
    }
    counter_stack.push(-1);
    return;
}
void nasal_bytecode_vm::opr_forindex()
{
    nasal_vector& ref=vm.gc_get(value_stack.top()).get_vector();
    counter_stack.top()++;
    if(counter_stack.top()>=ref.size())
    {
        vm.del_reference(value_stack.top());
        value_stack.pop();
        counter_stack.pop();
        ptr=exec_code[ptr].index-1;
        return;
    }
    int res=vm.gc_alloc(vm_number);
    vm.gc_get(res).set_number((double)counter_stack.top());
    value_stack.push(res);
    return;
}
void nasal_bytecode_vm::opr_foreach()
{
    nasal_vector& ref=vm.gc_get(value_stack.top()).get_vector();
    counter_stack.top()++;
    if(counter_stack.top()>=ref.size())
    {
        vm.del_reference(value_stack.top());
        value_stack.pop();
        counter_stack.pop();
        ptr=exec_code[ptr].index-1;
        return;
    }
    int res=ref.get_value_address(counter_stack.top());
    vm.add_reference(res);
    value_stack.push(res);
    return;
}
void nasal_bytecode_vm::opr_call()
{
    int val_addr=-1;
    if(local_scope_stack.top()>=0)
        val_addr=vm.gc_get(local_scope_stack.top()).get_closure().get_value_address(string_table[exec_code[ptr].index]);
    if(val_addr<0)
        val_addr=vm.gc_get(global_scope_addr).get_closure().get_value_address(string_table[exec_code[ptr].index]);
    if(val_addr<0)
    {
        die("call: cannot find symbol named \""+string_table[exec_code[ptr].index]+"\"");
        return;
    }
    vm.add_reference(val_addr);
    value_stack.push(val_addr);
    return;
}
void nasal_bytecode_vm::opr_callv()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    int vec_addr=value_stack.top();
    value_stack.pop();
    int type=vm.gc_get(vec_addr).get_type();
    if(type==vm_vector)
    {
        int num;
        switch(vm.gc_get(val_addr).get_type())
        {
            case vm_number:num=(int)vm.gc_get(val_addr).get_number();break;
            case vm_string:num=(int)trans_string_to_number(vm.gc_get(val_addr).get_string());break;
            default:die("callv: error value type");break;
        }
        int res=vm.gc_get(vec_addr).get_vector().get_value_address(num);
        if(res<0)
        {
            die("callv: index out of range");
            return;
        }
        vm.add_reference(res);
        value_stack.push(res);
    }
    else if(type==vm_string)
    {
        std::string str=vm.gc_get(vec_addr).get_string();
        int num;
        switch(vm.gc_get(val_addr).get_type())
        {
            case vm_number:num=(int)vm.gc_get(val_addr).get_number();break;
            case vm_string:num=(int)trans_string_to_number(vm.gc_get(val_addr).get_string());break;
            default:die("callv: error value type");break;
        }
        int str_size=str.length();
        if(num<-str_size || num>=str_size)
        {
            die("callv: index out of range");
            return;
        }
        int res=vm.gc_alloc(vm_number);
        vm.gc_get(res).set_number((double)str[(num+str_size)%str_size]);
        value_stack.push(res);
    }
    else if(type==vm_hash)
    {
        if(vm.gc_get(val_addr).get_type()!=vm_string)
        {
            die("callv: must use string as the key");
            return;
        }
        int res=vm.gc_get(vec_addr).get_hash().get_value_address(vm.gc_get(val_addr).get_string());
        if(res<0)
        {
            die("callv: cannot find member \""+vm.gc_get(val_addr).get_string()+"\" of this hash");
            return;
        }
        if(vm.gc_get(res).get_type()==vm_function)
        {
            vm.gc_get(vm.gc_get(res).get_func().get_closure_addr()).get_closure().add_new_value("me",val_addr);
            vm.add_reference(val_addr);
        }
        vm.add_reference(res);
        value_stack.push(res);
    }
    vm.del_reference(val_addr);
    vm.del_reference(vec_addr);
    return;
}
void nasal_bytecode_vm::opr_callvi()
{
    int val_addr=value_stack.top();
    if(vm.gc_get(val_addr).get_type()!=vm_vector)
    {
        die("callvi: multi-definition/multi-assignment must use a vector");
        return;
    }
    int res=vm.gc_get(val_addr).get_vector().get_value_address(exec_code[ptr].index);
    if(res<0)
    {
        die("callvi: index out of range");
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
        die("callh: must call a hash");
        return;
    }
    int res=vm.gc_get(val_addr).get_hash().get_value_address(string_table[exec_code[ptr].index]);
    if(res<0)
    {
        die("callh: hash member \""+string_table[exec_code[ptr].index]+"\" does not exist");
        return;
    }
    vm.add_reference(res);
    if(vm.gc_get(res).get_type()==vm_function)
        vm.gc_get(vm.gc_get(res).get_func().get_closure_addr()).get_closure().add_new_value("me",val_addr);
    else
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
        die("callf: called a value that is not a function");
        return;
    }
    nasal_function& ref=vm.gc_get(func_addr).get_func();
    int closure=ref.get_closure_addr();
    nasal_closure& ref_closure=vm.gc_get(closure).get_closure();
    ref_closure.add_scope();
    local_scope_stack.push(closure);
    vm.add_reference(closure);
    if(vm.gc_get(para_addr).get_type()==vm_vector)
    {
        nasal_vector& ref_vec=vm.gc_get(para_addr).get_vector();
        std::vector<std::string>& ref_para=ref.get_para();
        std::vector<int>& ref_default=ref.get_default();
        int i=0;
        for(;i<ref_para.size();++i)
        {
            if(i>=ref_vec.size())
            {
                if(ref_default[i]<0)
                {
                    die("callf: lack argument(s)");
                    return;
                }
                ref_closure.add_new_value(ref_para[i],ref_default[i]);
                vm.add_reference(ref_default[i]);
            }
            else
            {
                int tmp=ref_vec.get_value_address(i);
                ref_closure.add_new_value(ref_para[i],tmp);
                vm.add_reference(tmp);
            }
        }
        if(ref.get_dynamic_para().length())
        {
            int vec_addr=vm.gc_alloc(vm_vector);
            for(;i<ref_vec.size();++i)
            {
                int tmp=ref_vec.get_value_address(i);
                vm.gc_get(vec_addr).get_vector().add_elem(tmp);
                vm.add_reference(tmp);
            }
            ref_closure.add_new_value(ref.get_dynamic_para(),vec_addr);
        }
    }
    else
    {
        nasal_hash& ref_hash=vm.gc_get(para_addr).get_hash();
        std::vector<std::string>& ref_para=ref.get_para();
        std::vector<int>& ref_default=ref.get_default();
        if(ref.get_dynamic_para().length())
        {
            die("callf: special call cannot use dynamic parameter");
            return;
        }
        for(int i=0;i<ref_para.size();++i)
        {
            int tmp=ref_hash.get_special_para(ref_para[i]);
            if(tmp<0)
                tmp=ref_default[i];
            if(tmp<0)
            {
                die("callf: lack argument(s)");
                return;
            }
            ref_closure.add_new_value(ref_para[i],tmp);
            vm.add_reference(tmp);
        }
    }
    vm.del_reference(para_addr);
    call_stack.push(ptr);
    ptr=ref.get_entry()-1;
    return;
}
void nasal_bytecode_vm::opr_builtincall()
{
    int ret_value_addr=-1;
    std::string val_name=string_table[exec_code[ptr].index];
    if(builtin_func_hashmap.find(val_name)!=builtin_func_hashmap.end())
    {
        ret_value_addr=(*builtin_func_hashmap[val_name])(local_scope_stack.top(),vm);
        main_loop_break_mark=!builtin_die_state;
    }
    value_stack.push(ret_value_addr);
    return;
}
void nasal_bytecode_vm::opr_slicebegin()
{
    slice_stack.push(vm.gc_alloc(vm_vector));
    if(vm.gc_get(value_stack.top()).get_type()!=vm_vector)
        die("slcbegin: must slice a vector");
    return;
}
void nasal_bytecode_vm::opr_sliceend()
{
    value_stack.push(slice_stack.top());
    slice_stack.pop();
    return;
}
void nasal_bytecode_vm::opr_slice()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    int type=vm.gc_get(val_addr).get_type();
    double num;
    switch(type)
    {
        case vm_number:num=vm.gc_get(val_addr).get_number();break;
        case vm_string:num=trans_string_to_number(vm.gc_get(val_addr).get_string());break;
        default:die("slc: error value type");break;
    }
    int res=vm.gc_get(value_stack.top()).get_vector().get_value_address((int)num);
    if(res<0)
    {
        die("slc: index out of range");
        return;
    }
    vm.add_reference(res);
    vm.gc_get(slice_stack.top()).get_vector().add_elem(res);
    vm.del_reference(val_addr);
    return;
}
void nasal_bytecode_vm::opr_slice2()
{
    int val_addr2=value_stack.top();
    value_stack.pop();
    int val_addr1=value_stack.top();
    value_stack.pop();
    nasal_vector& ref=vm.gc_get(value_stack.top()).get_vector();
    nasal_vector& aim=vm.gc_get(slice_stack.top()).get_vector();

    int type1=vm.gc_get(val_addr1).get_type();
    int num1;
    switch(type1)
    {
        case vm_nil:break;
        case vm_number:num1=(int)vm.gc_get(val_addr1).get_number();break;
        case vm_string:num1=(int)trans_string_to_number(vm.gc_get(val_addr1).get_string());break;
        default:die("slc2: error value type");break;
    }
    int type2=vm.gc_get(val_addr2).get_type();
    int num2;
    switch(type2)
    {
        case vm_nil:break;
        case vm_number:num2=(int)vm.gc_get(val_addr2).get_number();break;
        case vm_string:num2=(int)trans_string_to_number(vm.gc_get(val_addr2).get_string());break;
        default:die("slc2: error value type");break;
    }
    int ref_size=ref.size();
    if(type1==vm_nil && type2==vm_nil)
    {
        num1=0;
        num2=ref_size-1;
    }
    else if(type1==vm_nil && type2!=vm_nil)
        num1=num2<0? -ref_size:0;
    else if(type1!=vm_nil && type2==vm_nil)
        num2=num1<0? -1:ref_size-1;
    if(num1>=num2)
    {
        die("slc2: begin index must be less than end index");
        return;
    }
    if(num1<-ref_size || num1>=ref_size || num2<-ref_size || num2>=ref_size)
    {
        die("slc2: begin or end index out of range");
        return;
    }
    for(int i=num1;i<num2;++i)
    {
        int tmp=ref.get_value_address(i);
        vm.add_reference(tmp);
        aim.add_elem(tmp);
    }
    vm.del_reference(val_addr1);
    vm.del_reference(val_addr2);
    return;
}
void nasal_bytecode_vm::opr_mcall()
{
    int mem_addr=-1;
    if(local_scope_stack.top()>=0)
        mem_addr=vm.gc_get(local_scope_stack.top()).get_closure().get_mem_address(string_table[exec_code[ptr].index]);
    if(mem_addr<0)
        mem_addr=vm.gc_get(global_scope_addr).get_closure().get_mem_address(string_table[exec_code[ptr].index]);
    if(mem_addr<0)
        die("mcall: cannot find symbol named \""+string_table[exec_code[ptr].index]+"\"");
    value_stack.push(mem_addr);
    return;
}
void nasal_bytecode_vm::opr_mcallv()
{
    int val_addr=value_stack.top();
    value_stack.pop();
    int vec_addr=vm.mem_get(value_stack.top());
    value_stack.pop();
    int type=vm.gc_get(vec_addr).get_type();
    if(type==vm_string)
    {
        die("mcallv: cannot get memory space in a string");
        return;
    }
    if(type==vm_vector)
    {
        int num;
        switch(vm.gc_get(val_addr).get_type())
        {
            case vm_number:num=(int)vm.gc_get(val_addr).get_number();break;
            case vm_string:num=(int)trans_string_to_number(vm.gc_get(val_addr).get_string());break;
            default:die("mcallv: error value type");break;
        }
        int res=vm.gc_get(vec_addr).get_vector().get_mem_address(num);
        if(res<0)
        {
            die("mcallv: index out of range");
            return;
        }
        value_stack.push(res);
    }
    else if(type==vm_hash)
    {
        if(vm.gc_get(val_addr).get_type()!=vm_string)
        {
            die("mcallv: must use string as the key");
            return;
        }
        int res=vm.gc_get(vec_addr).get_hash().get_mem_address(vm.gc_get(val_addr).get_string());
        if(res<0)
        {
            die("mcallv: cannot find member \""+vm.gc_get(val_addr).get_string()+"\" of this hash");
            return;
        }
        value_stack.push(res);
    }
    vm.del_reference(val_addr);
    return;
}
void nasal_bytecode_vm::opr_mcallh()
{
    int mem_addr=-1;
    int hash_addr=vm.mem_get(value_stack.top());
    value_stack.pop();
    if(vm.gc_get(hash_addr).get_type()!=vm_hash)
    {
        die("mcallh: must call a hash");
        return;
    }
    mem_addr=vm.gc_get(hash_addr).get_hash().get_mem_address(string_table[exec_code[ptr].index]);
    if(mem_addr<0)
    {
        die("mcallh: cannot get memory space in this hash");
        return;
    }
    value_stack.push(mem_addr);
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
    int tmp=value_stack.top();
    value_stack.pop();
    // delete function
    vm.del_reference(value_stack.top());
    value_stack.pop();
    value_stack.push(tmp);
    return;
}
void nasal_bytecode_vm::run(std::vector<std::string>& strs,std::vector<double>& nums,std::vector<opcode>& exec)
{
    string_table=strs;
    number_table=nums;
    exec_code=exec;

    static void (nasal_bytecode_vm::*opr_table[])()=
    {
        nasal_bytecode_vm::opr_nop,
        nasal_bytecode_vm::opr_load,
        nasal_bytecode_vm::opr_pushnum,
        nasal_bytecode_vm::opr_pushone,
        nasal_bytecode_vm::opr_pushzero,
        nasal_bytecode_vm::opr_pushnil,
        nasal_bytecode_vm::opr_pushstr,
        nasal_bytecode_vm::opr_newvec,
        nasal_bytecode_vm::opr_newhash,
        nasal_bytecode_vm::opr_newfunc,
        nasal_bytecode_vm::opr_vecapp,
        nasal_bytecode_vm::opr_hashapp,
        nasal_bytecode_vm::opr_para,
        nasal_bytecode_vm::opr_defpara,
        nasal_bytecode_vm::opr_dynpara,
        nasal_bytecode_vm::opr_entry,
        nasal_bytecode_vm::opr_unot,
        nasal_bytecode_vm::opr_usub,
        nasal_bytecode_vm::opr_add,
        nasal_bytecode_vm::opr_sub,
        nasal_bytecode_vm::opr_mul,
        nasal_bytecode_vm::opr_div,
        nasal_bytecode_vm::opr_lnk,
        nasal_bytecode_vm::opr_addeq,
        nasal_bytecode_vm::opr_subeq,
        nasal_bytecode_vm::opr_muleq,
        nasal_bytecode_vm::opr_diveq,
        nasal_bytecode_vm::opr_lnkeq,
        nasal_bytecode_vm::opr_meq,
        nasal_bytecode_vm::opr_eq,
        nasal_bytecode_vm::opr_neq,
        nasal_bytecode_vm::opr_less,
        nasal_bytecode_vm::opr_leq,
        nasal_bytecode_vm::opr_grt,
        nasal_bytecode_vm::opr_geq,
        nasal_bytecode_vm::opr_pop,
        nasal_bytecode_vm::opr_jmp,
        nasal_bytecode_vm::opr_jmptrue,
        nasal_bytecode_vm::opr_jmpfalse,
        nasal_bytecode_vm::opr_counter,
        nasal_bytecode_vm::opr_forindex,
        nasal_bytecode_vm::opr_foreach,
        nasal_bytecode_vm::opr_call,
        nasal_bytecode_vm::opr_callv,
        nasal_bytecode_vm::opr_callvi,
        nasal_bytecode_vm::opr_callh,
        nasal_bytecode_vm::opr_callf,
        nasal_bytecode_vm::opr_builtincall,
        nasal_bytecode_vm::opr_slicebegin,
        nasal_bytecode_vm::opr_sliceend,
        nasal_bytecode_vm::opr_slice,
        nasal_bytecode_vm::opr_slice2,
        nasal_bytecode_vm::opr_mcall,
        nasal_bytecode_vm::opr_mcallv,
        nasal_bytecode_vm::opr_mcallh,
        nasal_bytecode_vm::opr_return
    };

    main_loop_break_mark=true;
    builtin_die_state=false;

    global_scope_addr=vm.gc_alloc(vm_closure);
    time_t begin_time=std::time(NULL);

    // main loop
    for(ptr=0;main_loop_break_mark;++ptr)
        (this->*opr_table[exec_code[ptr].op])();
    
    time_t end_time=std::time(NULL);
    time_t total_run_time=end_time-begin_time;
    if(total_run_time>=1)
        std::cout<<">> [vm] process exited after "<<total_run_time<<"s.\n";
    vm.del_reference(global_scope_addr);

    clear();
    return;
}
#endif