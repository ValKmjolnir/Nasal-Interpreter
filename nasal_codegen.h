#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

enum op_code
{
    op_nop,
    op_load,// load value in value_stack to identifier
    op_pushnum,
    op_pushone,
    op_pushzero,
    op_pushnil,
    op_pushstr,
    op_newvec,op_newhash,op_newfunc,
    op_vecapp,op_hashapp,
    op_para,op_defpara,op_dynpara,op_entry,
    op_unot,op_usub,
    op_add,op_sub,op_mul,op_div,op_lnk,
    op_addeq,op_subeq,op_muleq,op_diveq,op_lnkeq,op_meq,
    op_eq,op_neq,op_less,op_leq,op_grt,op_geq,
    op_pop,
    op_jmp,        // jump with no condition
    op_jmptrue,    // used in operator and/or,jmp when condition is true and DO NOT POP
    op_jmpfalse,   // used in conditional/loop,jmp when condition is false and POP STACK
    op_counter,    // add counter for forindex/foreach
    op_cntpop,     // pop counter
    op_forindex,   // index counter on the top of forindex_stack plus 1
    op_foreach,    // index counter on the top of forindex_stack plus 1 and get the value in vector
    op_call,       // call identifier
    op_callv,      // call vec[index]
    op_callvi,     // call vec[immediate] (used in multi-assign/multi-define)
    op_callh,      // call hash.label
    op_callf,      // call function(parameters)
    op_builtincall,// call builtin-function
    op_slicebegin, // begin of slice like: vec[1,2,3:6,0,-1]
    op_sliceend,   // end of slice
    op_slice,      // slice like vec[1]
    op_slice2,     // slice like vec[nil:10]
    op_mcall,      // get memory of identifier
    op_mcallv,     // get memory of vec[index]
    op_mcallh,     // get memory of hash.label
    op_return      // return
};

struct
{
    int type;
    const char* name;
}code_table[]=
{
    {op_nop,         "nop   "},
    {op_load,        "load  "},
    {op_pushnum,     "pnum  "},
    {op_pushone,     "pone  "},
    {op_pushzero,    "pzero "},
    {op_pushnil,     "pnil  "},
    {op_pushstr,     "pstr  "},
    {op_newvec,      "newv  "},
    {op_newhash,     "newh  "},
    {op_newfunc,     "newf  "},
    {op_vecapp,      "vapp  "},
    {op_hashapp,     "happ  "},
    {op_para,        "para  "},
    {op_defpara,     "deflt "},
    {op_dynpara,     "dyn   "},
    {op_entry,       "entry "},
    {op_unot,        "not   "},
    {op_usub,        "usub  "},
    {op_add,         "add   "},
    {op_sub,         "sub   "},
    {op_mul,         "mult  "},
    {op_div,         "div   "},
    {op_lnk,         "link  "},
    {op_addeq,       "addeq "},
    {op_subeq,       "subeq "},
    {op_muleq,       "muleq "},
    {op_diveq,       "diveq "},
    {op_lnkeq,       "linkeq"},
    {op_meq,         "memeq "},
    {op_eq,          "eq    "},
    {op_neq,         "neq   "},
    {op_less,        "less  "},
    {op_leq,         "leq   "},
    {op_grt,         "grt   "},
    {op_geq,         "geq   "},
    {op_pop,         "pop   "},
    {op_jmp,         "jmp   "},
    {op_jmptrue,     "jt    "},
    {op_jmpfalse,    "jf    "},
    {op_counter,     "cnt   "},
    {op_cntpop,      "cntpop"},
    {op_forindex,    "findx "},
    {op_foreach,     "feach "},
    {op_call,        "call  "},
    {op_callv,       "callv "},
    {op_callvi,      "callvi"},
    {op_callh,       "callh "},
    {op_callf,       "callf "},
    {op_builtincall, "callb "},
    {op_slicebegin,  "slcbeg"},
    {op_sliceend,    "slcend"},
    {op_slice,       "slc   "},
    {op_slice2,      "slc2  "},
    {op_mcall,       "mcall "},
    {op_mcallv,      "mcallv"},
    {op_mcallh,      "mcallh"},
    {op_return,      "ret   "},
    {-1,             NULL},
};

struct opcode
{
    unsigned char op;
    unsigned int index;
    opcode(unsigned char _op=op_nop,unsigned int _index=0)
    {
        op=_op;
        index=_index;
        return;
    }
    opcode& operator=(const opcode& tmp)
    {
        op=tmp.op;
        index=tmp.index;
        return *this;
    }
};

class nasal_codegen
{
private:
    std::map<double,int>         number_table;
    std::map<std::string,int>    string_table;
    std::vector<double>          number_result_table;
    std::vector<std::string>     string_result_table;
    std::vector<opcode>          exec_code;
    std::list<std::vector<int> > continue_ptr;
    std::list<std::vector<int> > break_ptr;
    int error;
    void regist_number(double);
    void regist_string(std::string);
    void gen(unsigned char,unsigned int);
    void pop_gen();
    void nil_gen();
    void number_gen(nasal_ast&);
    void string_gen(nasal_ast&);
    void vector_gen(nasal_ast&);
    void hash_gen(nasal_ast&);
    void function_gen(nasal_ast&);
    void call_gen(nasal_ast&);
    void call_id(nasal_ast&);
    void call_hash(nasal_ast&);
    void call_vec(nasal_ast&);
    void call_func(nasal_ast&);
    void mem_call(nasal_ast&);
    void mem_call_id(nasal_ast&);
    void mem_call_vec(nasal_ast&);
    void mem_call_hash(nasal_ast&);
    void multi_def(nasal_ast&);
    void single_def(nasal_ast&);
    void definition_gen(nasal_ast&);
    void multi_assignment_gen(nasal_ast&);
    void conditional_gen(nasal_ast&);
    void loop_gen(nasal_ast&);
    void load_continue_break(int,int);
    void while_gen(nasal_ast&);
    void for_gen(nasal_ast&);
    void forindex_gen(nasal_ast&);
    void foreach_gen(nasal_ast&);
    void or_gen(nasal_ast&);
    void and_gen(nasal_ast&);
    void trino_gen(nasal_ast&);
    void calculation_gen(nasal_ast&);
    void block_gen(nasal_ast&);
    void return_gen(nasal_ast&);
public:
    nasal_codegen();
    void main_progress(nasal_ast&);
    void print_op(int);
    void print_byte_code();
    std::vector<std::string>& get_string_table();
    std::vector<double>& get_number_table();
    std::vector<opcode>& get_exec_code();
};

nasal_codegen::nasal_codegen()
{
    error=0;
    return;
}

void nasal_codegen::regist_number(double num)
{
    int size=number_table.size();
    if(number_table.find(num)==number_table.end())
        number_table[num]=size;
    return;
}

void nasal_codegen::regist_string(std::string str)
{
    int size=string_table.size();
    if(string_table.find(str)==string_table.end())
        string_table[str]=size;
    return;
}

void nasal_codegen::gen(unsigned char op,unsigned int index)
{
    opcode opr(op,index);
    exec_code.push_back(opr);
    return;
}

void nasal_codegen::pop_gen()
{
    opcode op(op_pop,0);
    exec_code.push_back(op);
    return;
}

void nasal_codegen::nil_gen()
{
    opcode op(op_pushnil,0);
    exec_code.push_back(op);
    return;
}

void nasal_codegen::number_gen(nasal_ast& ast)
{
    double num=ast.get_num();
    regist_number(num);
    if(num==1)      gen(op_pushone,0);
    else if(num==0) gen(op_pushzero,0);
    else            gen(op_pushnum,number_table[num]);
    return;
}

void nasal_codegen::string_gen(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    gen(op_pushstr,string_table[str]);
    return;
}

void nasal_codegen::vector_gen(nasal_ast& ast)
{
    int size=ast.get_children().size();
    gen(op_newvec,0);
    for(int i=0;i<size;++i)
    {
        calculation_gen(ast.get_children()[i]);
        gen(op_vecapp,0);
    }
    return;
}

void nasal_codegen::hash_gen(nasal_ast& ast)
{
    int size=ast.get_children().size();
    gen(op_newhash,0);
    for(int i=0;i<size;++i)
    {
        calculation_gen(ast.get_children()[i].get_children()[1]);
        std::string str=ast.get_children()[i].get_children()[0].get_str();
        regist_string(str);
        gen(op_hashapp,string_table[str]);
    }
    return;
}

void nasal_codegen::function_gen(nasal_ast& ast)
{
    gen(op_newfunc,0);

    nasal_ast& ref_arg=ast.get_children()[0];
    int arg_size=ref_arg.get_children().size();
    for(int i=0;i<arg_size;++i)
    {
        nasal_ast& tmp=ref_arg.get_children()[i];
        if(tmp.get_type()==ast_id)
        {
            std::string str=tmp.get_str();
            regist_string(str);
            gen(op_para,string_table[str]);
        }
        else if(tmp.get_type()==ast_default_arg)
        {
            calculation_gen(tmp.get_children()[1]);
            std::string str=tmp.get_children()[0].get_str();
            regist_string(str);
            gen(op_defpara,string_table[str]);
        }
        else if(tmp.get_type()==ast_dynamic_id)
        {
            std::string str=tmp.get_str();
            regist_string(str);
            gen(op_dynpara,string_table[str]);
        }
    }
    gen(op_entry,exec_code.size()+2);
    int ptr=exec_code.size();
    gen(op_jmp,0);
    nasal_ast& block=ast.get_children()[1];
    block_gen(block);
    if(!block.get_children().size() || block.get_children().back().get_type()!=ast_return)
    {
        nil_gen();
        gen(op_return,0);
    }
    exec_code[ptr].index=exec_code.size();
    return;
}

void nasal_codegen::call_gen(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[0]);
    if(exec_code.back().op==op_builtincall)
        return;
    int child_size=ast.get_children().size();
    for(int i=1;i<child_size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        switch(tmp.get_type())
        {
            case ast_callh:call_hash(tmp);break;
            case ast_callv: call_vec(tmp); break;
            case ast_callf:call_func(tmp);break;
        }
    }
    return;
}

void nasal_codegen::call_id(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    for(int i=0;builtin_func_table[i].func_pointer;++i)
        if(builtin_func_table[i].func_name==str)
        {
            gen(op_builtincall,string_table[str]);
            return;
        }
    gen(op_call,string_table[str]);
    return;
}

void nasal_codegen::call_hash(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    gen(op_callh,string_table[str]);
    return;
}

void nasal_codegen::call_vec(nasal_ast& ast)
{
    if(ast.get_children().size()==1 && ast.get_children()[0].get_type()!=ast_subvec)
    {
        calculation_gen(ast.get_children()[0]);
        gen(op_callv,0);
        return;
    }
    gen(op_slicebegin,0);
    int size=ast.get_children().size();
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()!=ast_subvec)
        {
            calculation_gen(tmp);
            gen(op_slice,0);
        }
        else
        {
            calculation_gen(tmp.get_children()[0]);
            calculation_gen(tmp.get_children()[1]);
            gen(op_slice2,0);
        }
    }
    gen(op_sliceend,0);
    return;
}

void nasal_codegen::call_func(nasal_ast& ast)
{
    if(!ast.get_children().size())
        gen(op_newvec,0);
    else if(ast.get_children()[0].get_type()==ast_hashmember)
        hash_gen(ast);
    else
        vector_gen(ast);
    gen(op_callf,0);
    return;
}

void nasal_codegen::mem_call(nasal_ast& ast)
{
    if(ast.get_type()==ast_id)
        mem_call_id(ast);
    else
        mem_call_id(ast.get_children()[0]);
    int child_size=ast.get_children().size();
    for(int i=1;i<child_size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()==ast_callh)
            mem_call_hash(tmp);
        else if(tmp.get_type()==ast_callv)
            mem_call_vec(tmp);
    }
    return;
}

void nasal_codegen::mem_call_id(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    gen(op_mcall,string_table[str]);
    return;
}

void nasal_codegen::mem_call_vec(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[0]);
    gen(op_mcallv,0);
    return;
}

void nasal_codegen::mem_call_hash(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    gen(op_mcallh,string_table[str]);
    return;
}

void nasal_codegen::single_def(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[1]);
    std::string str=ast.get_children()[0].get_str();
    regist_string(str);
    gen(op_load,string_table[str]);
    return;
}
void nasal_codegen::multi_def(nasal_ast& ast)
{
    int size=ast.get_children()[0].get_children().size();
    if(ast.get_children()[1].get_type()==ast_multi_scalar)
    {
        for(int i=0;i<size;++i)
        {
            calculation_gen(ast.get_children()[1].get_children()[i]);
            std::string str=ast.get_children()[0].get_children()[i].get_str();
            regist_string(str);
            gen(op_load,string_table[str]);
        }
    }
    else
    {
        calculation_gen(ast.get_children()[1]);
        for(int i=0;i<size;++i)
        {
            gen(op_callvi,i);
            std::string str=ast.get_children()[0].get_children()[i].get_str();
            regist_string(str);
            gen(op_load,string_table[str]);
        }
        pop_gen();
    }
    return;
}

void nasal_codegen::definition_gen(nasal_ast& ast)
{
    if(ast.get_children()[0].get_type()==ast_id)
        single_def(ast);
    else
        multi_def(ast);
    return;
}

void nasal_codegen::multi_assignment_gen(nasal_ast& ast)
{
    int size=ast.get_children()[0].get_children().size();
    if(ast.get_children()[1].get_type()==ast_multi_scalar)
    {
        for(int i=size-1;i>=0;--i)
            calculation_gen(ast.get_children()[1].get_children()[i]);
        for(int i=0;i<size;++i)
        {
            mem_call(ast.get_children()[0].get_children()[i]);
            gen(op_meq,0);
            pop_gen();
        }
    }
    else
    {
        calculation_gen(ast.get_children()[1]);
        for(int i=0;i<size;++i)
        {
            gen(op_callvi,i);
            mem_call(ast.get_children()[0].get_children()[i]);
            gen(op_meq,0);
            pop_gen();
        }
        pop_gen();
    }
    return;
}

void nasal_codegen::conditional_gen(nasal_ast& ast)
{
    int size=ast.get_children().size();
    std::vector<int> jmp_label;
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()==ast_if || tmp.get_type()==ast_elsif)
        {
            calculation_gen(tmp.get_children()[0]);
            int ptr=exec_code.size();
            gen(op_jmpfalse,0);
            block_gen(tmp.get_children()[1]);
            jmp_label.push_back(exec_code.size());
            gen(op_jmp,0);
            exec_code[ptr].index=exec_code.size();
        }
        else
        {
            block_gen(tmp.get_children()[0]);
            break;
        }
    }
    for(std::vector<int>::iterator j=jmp_label.begin();j!=jmp_label.end();++j)
        exec_code[*j].index=exec_code.size();
    return;
}

void nasal_codegen::loop_gen(nasal_ast& ast)
{
    std::vector<int> new_level;
    continue_ptr.push_front(new_level);
    break_ptr.push_front(new_level);
    switch(ast.get_type())
    {
        case ast_while:    while_gen(ast);    break;
        case ast_for:      for_gen(ast);      break;
        case ast_forindex: forindex_gen(ast); break;
        case ast_foreach:  foreach_gen(ast);  break;
    }
    return;
}

void nasal_codegen::load_continue_break(int continue_place,int break_place)
{
    for(int i=0;i<continue_ptr.front().size();++i)
        exec_code[continue_ptr.front()[i]].index=continue_place;
    for(int i=0;i<break_ptr.front().size();++i)
        exec_code[break_ptr.front()[i]].index=break_place;
    continue_ptr.pop_front();
    break_ptr.pop_front();
    return;
}

void nasal_codegen::while_gen(nasal_ast& ast)
{
    int loop_ptr=exec_code.size();
    calculation_gen(ast.get_children()[0]);
    int condition_ptr=exec_code.size();
    gen(op_jmpfalse,0);

    block_gen(ast.get_children()[1]);
    gen(op_jmp,loop_ptr);
    exec_code[condition_ptr].index=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    return;
}

void nasal_codegen::for_gen(nasal_ast& ast)
{
    opcode op;
    switch(ast.get_children()[0].get_type())
    {
        case ast_null:break;
        case ast_definition:definition_gen(ast.get_children()[0]);break;
        case ast_multi_assign:multi_assignment_gen(ast.get_children()[0]);break;
        case ast_nil:case ast_num:case ast_str:case ast_func:break;
        case ast_vec:case ast_hash:
        case ast_call:
        case ast_equal:case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:case ast_lnkeq:
        case ast_neg:case ast_not:
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
        case ast_cmpeq:case ast_neq:case ast_leq:case ast_less:case ast_geq:case ast_grt:
        case ast_trino:calculation_gen(ast.get_children()[0]);pop_gen();break;
    }
    int jmp_place=exec_code.size();
    if(ast.get_children()[1].get_type()==ast_null)
        gen(op_pushone,0);
    else
        calculation_gen(ast.get_children()[1]);
    int label_exit=exec_code.size();
    gen(op_jmpfalse,0);

    block_gen(ast.get_children()[3]);
    int continue_place=exec_code.size();
    switch(ast.get_children()[2].get_type())
    {
        case ast_null:break;
        case ast_definition:definition_gen(ast.get_children()[2]);break;
        case ast_multi_assign:multi_assignment_gen(ast.get_children()[2]);break;
        case ast_nil:case ast_num:case ast_str:case ast_func:break;
        case ast_vec:case ast_hash:
        case ast_call:
        case ast_equal:case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:case ast_lnkeq:
        case ast_neg:case ast_not:
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
        case ast_cmpeq:case ast_neq:case ast_leq:case ast_less:case ast_geq:case ast_grt:
        case ast_trino:calculation_gen(ast.get_children()[2]);pop_gen();break;
    }
    gen(op_jmp,jmp_place);
    exec_code[label_exit].index=exec_code.size();

    load_continue_break(continue_place,exec_code.size());
    return;
}
void nasal_codegen::forindex_gen(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[1]);
    gen(op_counter,0);
    int ptr=exec_code.size();
    gen(op_forindex,0);
    if(ast.get_children()[0].get_type()==ast_new_iter)
    {
        std::string str=ast.get_children()[0].get_children()[0].get_str();
        regist_string(str);
        gen(op_load,string_table[str]);
    }
    else
    {
        mem_call(ast.get_children()[0]);
        gen(op_meq,0);
        pop_gen();
    }

    block_gen(ast.get_children()[2]);
    gen(op_jmp,ptr);
    exec_code[ptr].index=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    pop_gen();// pop vector
    gen(op_cntpop,0);
    return;
}
void nasal_codegen::foreach_gen(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[1]);
    gen(op_counter,0);
    int ptr=exec_code.size();
    gen(op_foreach,0);
    if(ast.get_children()[0].get_type()==ast_new_iter)
    {
        std::string str=ast.get_children()[0].get_children()[0].get_str();
        regist_string(str);
        gen(op_load,string_table[str]);
    }
    else
    {
        mem_call(ast.get_children()[0]);
        gen(op_meq,0);
        pop_gen();
    }

    block_gen(ast.get_children()[2]);
    gen(op_jmp,ptr);
    exec_code[ptr].index=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    pop_gen();// pop vector
    gen(op_cntpop,0);
    return;
}

void nasal_codegen::or_gen(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[0]);
    int l1=exec_code.size();
    gen(op_jmptrue,0);

    pop_gen();
    calculation_gen(ast.get_children()[1]);
    int l2=exec_code.size();
    gen(op_jmptrue,0);

    pop_gen();
    nil_gen();

    exec_code[l1].index=exec_code[l2].index=exec_code.size();
    return;
}

void nasal_codegen::and_gen(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[0]);
    gen(op_jmptrue,exec_code.size()+2);

    int lfalse=exec_code.size();
    gen(op_jmp,0);
    pop_gen();// jt jumps here

    calculation_gen(ast.get_children()[1]);
    gen(op_jmptrue,exec_code.size()+3);

    exec_code[lfalse].index=exec_code.size();
    pop_gen();
    nil_gen();
    //jt jumps here
    return;
}

void nasal_codegen::trino_gen(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[0]);
    int lfalse=exec_code.size();
    gen(op_jmpfalse,0);
    calculation_gen(ast.get_children()[1]);
    int lexit=exec_code.size();
    gen(op_jmp,0);
    exec_code[lfalse].index=exec_code.size();
    calculation_gen(ast.get_children()[2]);
    exec_code[lexit].index=exec_code.size();
    return;
}

void nasal_codegen::calculation_gen(nasal_ast& ast)
{
    switch(ast.get_type())
    {
        case ast_nil:  nil_gen();         break;
        case ast_num:  number_gen(ast);   break;
        case ast_str:  string_gen(ast);   break;
        case ast_id:   call_id(ast);      break;
        case ast_vec:  vector_gen(ast);   break;
        case ast_hash: hash_gen(ast);     break;
        case ast_func: function_gen(ast); break;
        case ast_call: call_gen(ast);     break;
        case ast_equal:
            calculation_gen(ast.get_children()[1]);
            mem_call(ast.get_children()[0]);
            gen(op_meq,0);
            break;
        // ast_addeq(22)~ast_lnkeq(26) op_addeq(23)~op_lnkeq(27)
        case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:case ast_lnkeq:
            calculation_gen(ast.get_children()[1]);
            mem_call(ast.get_children()[0]);
            gen(ast.get_type()-ast_addeq+op_addeq,0);
            break;
        case ast_or:or_gen(ast);break;
        case ast_and:and_gen(ast);break;
        // ast_add(33)~ast_link(37) op_add(18)~op_lnk(22)
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            gen(ast.get_type()-ast_add+op_add,0);
            break;
        // ast_cmpeq(27)~ast_geq(32) op_eq(29)~op_geq(34)
        case ast_cmpeq:case ast_neq:case ast_less:case ast_leq:case ast_grt:case ast_geq:
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            gen(ast.get_type()-ast_cmpeq+op_eq,0);
            break;
        case ast_trino:trino_gen(ast);break;
        case ast_neg:
            calculation_gen(ast.get_children()[0]);
            gen(op_usub,0);
            break;
        case ast_not:
            calculation_gen(ast.get_children()[0]);
            gen(op_unot,0);
            break;
    }
    return;
}

void nasal_codegen::block_gen(nasal_ast& ast)
{
    int size=ast.get_children().size();
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        switch(tmp.get_type())
        {
            case ast_null:case ast_nil:case ast_num:case ast_str:case ast_func:break;
            case ast_definition:definition_gen(tmp);break;
            case ast_multi_assign:multi_assignment_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_continue:
                continue_ptr.front().push_back(exec_code.size());
                gen(op_jmp,0);
                break;
            case ast_break:
                break_ptr.front().push_back(exec_code.size());
                gen(op_jmp,0);
                break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:loop_gen(tmp);break;
            case ast_id:
            case ast_vec:
            case ast_hash:
            case ast_call:
            case ast_equal:
            case ast_addeq:
            case ast_subeq:
            case ast_multeq:
            case ast_diveq:
            case ast_lnkeq:
            case ast_neg:
            case ast_not:
            case ast_add:
            case ast_sub:
            case ast_mult:
            case ast_div:
            case ast_link:
            case ast_cmpeq:
            case ast_neq:
            case ast_leq:
            case ast_less:
            case ast_geq:
            case ast_grt:
            case ast_or:
            case ast_and:
            case ast_trino:calculation_gen(tmp);pop_gen();break;
            case ast_return:return_gen(tmp);break;
        }
    }
    return;
}

void nasal_codegen::return_gen(nasal_ast& ast)
{
    if(ast.get_children().size())
        calculation_gen(ast.get_children()[0]);
    else
        nil_gen();
    gen(op_return,0);
    return;
}

void nasal_codegen::main_progress(nasal_ast& ast)
{
    error=0;
    number_table.clear();
    string_table.clear();
    exec_code.clear();

    int size=ast.get_children().size();
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        switch(tmp.get_type())
        {
            case ast_null:case ast_nil:case ast_num:case ast_str:case ast_func:break;
            case ast_definition:definition_gen(tmp);break;
            case ast_multi_assign:multi_assignment_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:loop_gen(tmp);break;
            case ast_id:
            case ast_vec:
            case ast_hash:
            case ast_call:
            case ast_equal:
            case ast_addeq:
            case ast_subeq:
            case ast_multeq:
            case ast_diveq:
            case ast_lnkeq:
            case ast_neg:
            case ast_not:
            case ast_add:
            case ast_sub:
            case ast_mult:
            case ast_div:
            case ast_link:
            case ast_cmpeq:
            case ast_neq:
            case ast_leq:
            case ast_less:
            case ast_geq:
            case ast_grt:
            case ast_or:
            case ast_and:
            case ast_trino:calculation_gen(tmp);pop_gen();break;
        }
    }
    gen(op_nop,0);
    number_result_table.resize(number_table.size());
    string_result_table.resize(string_table.size());
    for(std::map<double,int>::iterator i=number_table.begin();i!=number_table.end();++i)
        number_result_table[i->second]=i->first;
    for(std::map<std::string,int>::iterator i=string_table.begin();i!=string_table.end();++i)
        string_result_table[i->second]=i->first;
    return;
}

void nasal_codegen::print_op(int index)
{
    // print opcode ptr
    printf("0x%.8x: ",index);
    // print opcode name
    for(int i=0;code_table[i].name;++i)
        if(exec_code[index].op==code_table[i].type)
        {
            std::cout<<code_table[i].name<<" ";
            break;
        }
    // print opcode index
    printf("0x%.8x ",exec_code[index].index);
    // print detail info
    switch(exec_code[index].op)
    {
        case op_pushnum:std::cout<<'('<<number_result_table[exec_code[index].index]<<')';break;
        case op_hashapp:
        case op_call:
        case op_builtincall:
        case op_mcall:
        case op_pushstr:
        case op_callh:
        case op_mcallh:
        case op_para:
        case op_defpara:
        case op_dynpara:
        case op_load:std::cout<<'('<<string_result_table[exec_code[index].index]<<')';break;
    }
    std::cout<<'\n';
    return;
}

void nasal_codegen::print_byte_code()
{
    for(int i=0;i<number_result_table.size();++i)
        std::cout<<".number "<<number_result_table[i]<<'\n';
    for(int i=0;i<string_result_table.size();++i)
        std::cout<<".symbol "<<string_result_table[i]<<'\n';
    int size=exec_code.size();
    for(int i=0;i<size;++i)
        print_op(i);
    return;
}

std::vector<std::string>& nasal_codegen::get_string_table()
{
    return string_result_table;
}

std::vector<double>& nasal_codegen::get_number_table()
{
    return number_result_table;
}

std::vector<opcode>& nasal_codegen::get_exec_code()
{
    return exec_code;
}

#endif