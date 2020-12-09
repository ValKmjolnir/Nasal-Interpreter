#ifndef __NASAL_BYTECODE_VM_H__
#define __NASAL_BYTECODE_VM_H__

class nasal_bytecode_vm
{
private:
    int ptr;
    int global_scope_addr;
    nasal_virtual_machine vm;
    std::vector<opcode> exec_code;
    std::stack<int> value_stack;
    std::vector<std::string> string_table;
    std::vector<double> number_table;
    std::map<int,void (nasal_bytecode_vm::*)()> opr_table;

    void opr_nop();
    void opr_load();
    void opr_pushnum();
    void opr_pushone();
    void opr_pushzero();
    void opr_pushnil();
    void opr_pushstr();
public:
    nasal_bytecode_vm();
    ~nasal_bytecode_vm();
    void set_string_table(std::vector<std::string>&);
    void set_number_table(std::vector<double>&);
    void set_exec_code(std::vector<opcode>&);
    void run();
};

nasal_bytecode_vm::nasal_bytecode_vm()
{
    struct
    {
        int op;
        void (nasal_bytecode_vm::*ptr)();
    }function_table[]=
    {
        {op_nop,nasal_bytecode_vm::opr_nop},
        {op_load,nasal_bytecode_vm::opr_load},
        {op_pushnum,nasal_bytecode_vm::opr_pushnum},
        {op_pushone,nasal_bytecode_vm::opr_pushone},
        {op_pushzero,nasal_bytecode_vm::opr_pushzero},
        {op_pushnil,nasal_bytecode_vm::opr_pushnil},
        {op_pushstr,nasal_bytecode_vm::opr_pushstr},
        {op_newvec,      NULL},
        {op_newhash,     NULL},
        {op_newfunc,     NULL},
        {op_vecapp,      NULL},
        {op_hashapp,     NULL},
        {op_para,        NULL},
        {op_defpara,     NULL},
        {op_dynpara,     NULL},
        {op_entry,       NULL},
        {op_unot,        NULL},
        {op_usub,        NULL},
        {op_add,         NULL},
        {op_sub,         NULL},
        {op_mul,         NULL},
        {op_div,         NULL},
        {op_lnk,         NULL},
        {op_addeq,       NULL},
        {op_subeq,       NULL},
        {op_muleq,       NULL},
        {op_diveq,       NULL},
        {op_lnkeq,       NULL},
        {op_meq,         NULL},
        {op_eq,          NULL},
        {op_neq,         NULL},
        {op_less,        NULL},
        {op_leq,         NULL},
        {op_grt,         NULL},
        {op_geq,         NULL},
        {op_pop,         NULL},
        {op_jmp,         NULL},
        {op_jmptrue,     NULL},
        {op_jmpfalse,    NULL},
        {op_forindex,    NULL},
        {op_foreach,     NULL},
        {op_call,        NULL},
        {op_callv,       NULL},
        {op_callvi,      NULL},
        {op_callh,       NULL},
        {op_callf,       NULL},
        {op_builtincall, NULL},
        {op_slicebegin,  NULL},
        {op_sliceend,    NULL},
        {op_slice,       NULL},
        {op_slice2,      NULL},
        {op_mcall,       NULL},
        {op_mcallv,      NULL},
        {op_mcallh,      NULL},
        {op_return,      NULL},
        {-1,NULL}
    };
    for(int i=0;function_table[i].ptr;++i)
        opr_table[function_table[i].op]=function_table[i].ptr;
    return;
}
nasal_bytecode_vm::~nasal_bytecode_vm()
{
    opr_table.clear();
    return;
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
    global_scope_addr=vm.gc_alloc(vm_closure);
    int size=exec_code.size();
    for(ptr=0;ptr<size;++ptr)
        (this->*opr_table[exec_code[ptr].op])();
    vm.del_reference(global_scope_addr);
    return;
}
#endif