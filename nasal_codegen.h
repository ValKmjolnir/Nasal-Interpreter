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
    op_jmp,
    op_jmptrue,
    op_jmpfalse,
    op_counter,    // add counter for forindex/foreach
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
    {op_less,        "l     "},
    {op_leq,         "leq   "},
    {op_grt,         "g     "},
    {op_geq,         "geq   "},
    {op_pop,         "pop   "},
    {op_jmp,         "jmp   "},
    {op_jmptrue,     "jt    "},
    {op_jmpfalse,    "jf    "},
    {op_counter,     "cnt   "},
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
    opcode()
    {
        op=op_nop;
        index=0;
        return;
    }
    opcode& operator=(opcode& tmp)
    {
        op=tmp.op;
        index=tmp.index;
        return *this;
    }
};

// unfinished
// now it can output ast but it is not byte code yet
// please wait...
class nasal_codegen
{
private:
    std::map<double,int> number_table;
    std::map<std::string,int> string_table;
    std::vector<double> number_result_table;
    std::vector<std::string> string_result_table;
    std::vector<opcode> exec_code;
    std::vector<int> continue_ptr;
    std::vector<int> break_ptr;
    int error;
    void regist_number(double);
    void regist_string(std::string);
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
    void load_continue_break();
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

void nasal_codegen::pop_gen()
{
    opcode op;
    op.op=op_pop;
    op.index=0;
    exec_code.push_back(op);
    return;
}

void nasal_codegen::nil_gen()
{
    opcode op;
    op.op=op_pushnil;
    exec_code.push_back(op);
    return;
}

void nasal_codegen::number_gen(nasal_ast& ast)
{
    double num=ast.get_num();
    regist_number(num);
    opcode op;
    if(num==1)
        op.op=op_pushone;
    else if(num==0)
        op.op=op_pushzero;
    else
    {
        op.op=op_pushnum;
        op.index=number_table[num];
    }
    exec_code.push_back(op);
    return;
}

void nasal_codegen::string_gen(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    opcode op;
    op.op=op_pushstr;
    op.index=string_table[str];
    exec_code.push_back(op);
    return;
}

void nasal_codegen::vector_gen(nasal_ast& ast)
{
    int size=ast.get_children().size();
    opcode op;
    op.op=op_newvec;
    op.index=0;
    exec_code.push_back(op);
    for(int i=0;i<size;++i)
    {
        calculation_gen(ast.get_children()[i]);
        opcode tmp;
        tmp.op=op_vecapp;
        tmp.index=0;
        exec_code.push_back(tmp);
    }
    return;
}

void nasal_codegen::hash_gen(nasal_ast& ast)
{
    int size=ast.get_children().size();
    opcode op;
    op.op=op_newhash;
    op.index=0;
    exec_code.push_back(op);
    for(int i=0;i<size;++i)
    {
        calculation_gen(ast.get_children()[i].get_children()[1]);
        op.op=op_hashapp;
        std::string str=ast.get_children()[i].get_children()[0].get_str();
        regist_string(str);
        op.index=string_table[str];
        exec_code.push_back(op);
    }
    return;
}

void nasal_codegen::function_gen(nasal_ast& ast)
{
    opcode op;
    op.op=op_newfunc;
    op.index=0;
    exec_code.push_back(op);

    nasal_ast& ref_arg=ast.get_children()[0];
    int arg_size=ref_arg.get_children().size();
    for(int i=0;i<arg_size;++i)
    {
        nasal_ast& tmp=ref_arg.get_children()[i];
        if(tmp.get_type()==ast_identifier)
        {
            std::string str=tmp.get_str();
            regist_string(str);
            opcode tmp;
            tmp.op=op_para;
            tmp.index=string_table[str];
            exec_code.push_back(tmp);
        }
        else if(tmp.get_type()==ast_default_arg)
        {
            calculation_gen(tmp.get_children()[1]);
            std::string str=tmp.get_children()[0].get_str();
            regist_string(str);
            opcode tmp;
            tmp.op=op_defpara;
            tmp.index=string_table[str];
            exec_code.push_back(tmp);
        }
        else if(tmp.get_type()==ast_dynamic_id)
        {
            std::string str=tmp.get_str();
            regist_string(str);
            opcode tmp;
            tmp.op=op_dynpara;
            tmp.index=string_table[str];
            exec_code.push_back(tmp);
        }
    }
    
    op.op=op_entry;
    op.index=exec_code.size()+2;
    exec_code.push_back(op);
    op.op=op_jmp;
    op.index=0;
    int ptr=exec_code.size();
    exec_code.push_back(op);

    nasal_ast& block=ast.get_children()[1];
    block_gen(block);
    if(!block.get_children().size() || block.get_children().back().get_type()!=ast_return)
    {
        op.op=op_pushnil;
        op.index=0;
        exec_code.push_back(op);
        op.op=op_return;
        op.index=0;
        exec_code.push_back(op);
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
        if(tmp.get_type()==ast_call_hash)
            call_hash(tmp);
        else if(tmp.get_type()==ast_call_vec)
            call_vec(tmp);
        else if(tmp.get_type()==ast_call_func)
            call_func(tmp);
    }
    return;
}

void nasal_codegen::call_id(nasal_ast& ast)
{
    opcode op;
    op.op=op_call;
    std::string str=ast.get_str();
    for(int i=0;builtin_func_table[i].func_pointer;++i)
        if(builtin_func_table[i].func_name==str)
        {
            op.op=op_builtincall;
            break;
        }
    regist_string(str);
    op.index=string_table[str];
    exec_code.push_back(op);
    return;
}

void nasal_codegen::call_hash(nasal_ast& ast)
{
    opcode op;
    op.op=op_callh;
    std::string str=ast.get_str();
    regist_string(str);
    op.index=string_table[str];
    exec_code.push_back(op);
    return;
}

void nasal_codegen::call_vec(nasal_ast& ast)
{
    if(ast.get_children().size()==1 && ast.get_children()[0].get_type()!=ast_subvec)
    {
        calculation_gen(ast.get_children()[0]);
        opcode op;
        op.op=op_callv;
        op.index=0;
        exec_code.push_back(op);
        return;
    }
    opcode op;
    op.op=op_slicebegin;
    op.index=0;
    exec_code.push_back(op);
    int size=ast.get_children().size();
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()!=ast_subvec)
        {
            calculation_gen(tmp);
            op.op=op_slice;
            op.index=0;
            exec_code.push_back(op);
        }
        else
        {
            calculation_gen(tmp.get_children()[0]);
            calculation_gen(tmp.get_children()[1]);
            op.op=op_slice2;
            op.index=0;
            exec_code.push_back(op);
        }
    }
    op.op=op_sliceend;
    op.index=0;
    exec_code.push_back(op);
    return;
}

void nasal_codegen::call_func(nasal_ast& ast)
{
    if(!ast.get_children().size())
    {
        opcode op;
        op.op=op_newvec;
        op.index=0;
        exec_code.push_back(op);

        op.op=op_callf;
        op.index=0;
        exec_code.push_back(op);
        return;
    }
    if(ast.get_children()[0].get_type()==ast_hashmember)
    {
        opcode op;
        op.op=op_newhash;
        op.index=0;
        exec_code.push_back(op);
        int size=ast.get_children().size();
        for(int i=0;i<size;++i)
        {
            nasal_ast& tmp=ast.get_children()[i];
            string_gen(tmp.get_children()[0]);
            calculation_gen(tmp.get_children()[1]);
            op.op=op_hashapp;
            op.index=0;
            exec_code.push_back(op);
        }
        op.op=op_callf;
        op.index=0;
        exec_code.push_back(op);
    }
    else
    {
        opcode op;
        op.op=op_newvec;
        op.index=0;
        exec_code.push_back(op);
        int size=ast.get_children().size();
        for(int i=0;i<size;++i)
        {
            nasal_ast& tmp=ast.get_children()[i];
            calculation_gen(tmp);
            op.op=op_vecapp;
            op.index=0;
            exec_code.push_back(op);
        }
        op.op=op_callf;
        op.index=0;
        exec_code.push_back(op);
    }
    return;
}

void nasal_codegen::mem_call(nasal_ast& ast)
{
    if(ast.get_type()==ast_identifier)
        mem_call_id(ast);
    else
        mem_call_id(ast.get_children()[0]);
    int child_size=ast.get_children().size();
    for(int i=1;i<child_size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()==ast_call_hash)
            mem_call_hash(tmp);
        else if(tmp.get_type()==ast_call_vec)
            mem_call_vec(tmp);
    }
    return;
}

void nasal_codegen::mem_call_id(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    opcode op;
    op.op=op_mcall;
    op.index=string_table[str];
    exec_code.push_back(op);
    return;
}

void nasal_codegen::mem_call_vec(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[0]);
    opcode op;
    op.op=op_mcallv;
    op.index=0;
    exec_code.push_back(op);
    return;
}

void nasal_codegen::mem_call_hash(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    opcode op;
    op.op=op_mcallh;
    op.index=string_table[str];
    exec_code.push_back(op);
    return;
}

void nasal_codegen::single_def(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[1]);
    opcode op;
    op.op=op_load;
    std::string str=ast.get_children()[0].get_str();
    regist_string(str);
    op.index=string_table[str];
    exec_code.push_back(op);
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
            opcode op;
            op.op=op_load;
            std::string str=ast.get_children()[0].get_children()[i].get_str();
            regist_string(str);
            op.index=string_table[str];
            exec_code.push_back(op);
        }
    }
    else
    {
        calculation_gen(ast.get_children()[1]);
        for(int i=0;i<size;++i)
        {
            opcode op;
            op.op=op_callvi;
            op.index=i;
            exec_code.push_back(op);
            op.op=op_load;
            std::string str=ast.get_children()[0].get_children()[i].get_str();
            regist_string(str);
            op.index=string_table[str];
            exec_code.push_back(op);
        }
    }
    return;
}

void nasal_codegen::definition_gen(nasal_ast& ast)
{
    if(ast.get_children()[0].get_type()==ast_identifier)
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
            opcode op;
            op.op=op_meq;
            exec_code.push_back(op);
            pop_gen();
        }
    }
    else
    {
        calculation_gen(ast.get_children()[1]);
        for(int i=0;i<size;++i)
        {
            opcode op;
            op.op=op_callvi;
            op.index=i;
            exec_code.push_back(op);
            mem_call(ast.get_children()[0].get_children()[i]);
            op.op=op_meq;
            exec_code.push_back(op);
            pop_gen();
        }
    }
    return;
}

void nasal_codegen::conditional_gen(nasal_ast& ast)
{
    opcode op;
    int size=ast.get_children().size();
    std::vector<int> jmp_label;
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()==ast_if || tmp.get_type()==ast_elsif)
        {
            calculation_gen(tmp.get_children()[0]);
            op.op=op_jmpfalse;
            int ptr=exec_code.size();
            exec_code.push_back(op);

            pop_gen();
            block_gen(tmp.get_children()[1]);

            op.op=op_jmp;
            jmp_label.push_back(exec_code.size());
            exec_code.push_back(op);
            exec_code[ptr].index=exec_code.size();
            pop_gen();
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
    switch(ast.get_type())
    {
        case ast_while:while_gen(ast);break;
        case ast_for:for_gen(ast);break;
        case ast_forindex:forindex_gen(ast);break;
        case ast_foreach:foreach_gen(ast);break;
    }
    return;
}

void nasal_codegen::load_continue_break()
{
    for(int i=0;i<continue_ptr.size();++i)
        exec_code[continue_ptr[i]].index=exec_code.size()-2;
    continue_ptr.clear();
    for(int i=0;i<break_ptr.size();++i)
        exec_code[break_ptr[i]].index=exec_code.size();
    break_ptr.clear();
    return;
}

void nasal_codegen::while_gen(nasal_ast& ast)
{
    opcode op;
    int loop_ptr=exec_code.size();
    calculation_gen(ast.get_children()[0]);
    op.op=op_jmpfalse;
    int condition_ptr=exec_code.size();
    exec_code.push_back(op);
    pop_gen();
    block_gen(ast.get_children()[1]);
    op.op=op_jmp;
    op.index=loop_ptr;
    exec_code.push_back(op);
    exec_code[condition_ptr].index=exec_code.size();
    pop_gen();
    load_continue_break();
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
        case ast_nil:case ast_number:case ast_string:case ast_function:break;
        case ast_vector:case ast_hash:
        case ast_call:
        case ast_equal:case ast_add_equal:case ast_sub_equal:case ast_mult_equal:case ast_div_equal:case ast_link_equal:
        case ast_unary_sub:case ast_unary_not:
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
        case ast_cmp_equal:case ast_cmp_not_equal:case ast_less_equal:case ast_less_than:case ast_greater_equal:case ast_greater_than:
        case ast_trinocular:calculation_gen(ast.get_children()[0]);pop_gen();break;
    }
    int jmp_place=exec_code.size();
    if(ast.get_children()[1].get_type()==ast_null)
    {
        op.op=op_pushone;
        op.index=0;
        exec_code.push_back(op);
    }
    else
        calculation_gen(ast.get_children()[1]);
    op.op=op_jmpfalse;
    int label_exit=exec_code.size();
    exec_code.push_back(op);
    pop_gen();
    block_gen(ast.get_children()[3]);
    switch(ast.get_children()[2].get_type())
    {
        case ast_null:break;
        case ast_definition:definition_gen(ast.get_children()[2]);break;
        case ast_multi_assign:multi_assignment_gen(ast.get_children()[2]);break;
        case ast_nil:case ast_number:case ast_string:case ast_function:break;
        case ast_vector:case ast_hash:
        case ast_call:
        case ast_equal:case ast_add_equal:case ast_sub_equal:case ast_mult_equal:case ast_div_equal:case ast_link_equal:
        case ast_unary_sub:case ast_unary_not:
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
        case ast_cmp_equal:case ast_cmp_not_equal:case ast_less_equal:case ast_less_than:case ast_greater_equal:case ast_greater_than:
        case ast_trinocular:calculation_gen(ast.get_children()[2]);pop_gen();break;
    }
    op.op=op_jmp;
    op.index=jmp_place;
    exec_code.push_back(op);
    exec_code[label_exit].index=exec_code.size();
    pop_gen();
    load_continue_break();
    return;
}
void nasal_codegen::forindex_gen(nasal_ast& ast)
{
    opcode op;
    calculation_gen(ast.get_children()[1]);

    op.op=op_counter;
    op.index=0;
    exec_code.push_back(op);

    op.op=op_forindex;
    op.index=0;
    int ptr=exec_code.size();
    exec_code.push_back(op);
    if(ast.get_children()[0].get_type()==ast_new_iter)
    {
        op.op=op_load;
        std::string str=ast.get_children()[0].get_children()[0].get_str();
        regist_string(str);
        op.index=string_table[str];
        exec_code.push_back(op);
    }
    else
    {
        mem_call(ast.get_children()[0]);
        op.op=op_meq;
        op.index=0;
        exec_code.push_back(op);
    }
    block_gen(ast.get_children()[2]);
    op.op=op_jmp;
    op.index=ptr;
    exec_code.push_back(op);
    exec_code[ptr].index=exec_code.size();
    load_continue_break();
    return;
}
void nasal_codegen::foreach_gen(nasal_ast& ast)
{
    opcode op;
    calculation_gen(ast.get_children()[1]);

    op.op=op_counter;
    op.index=0;
    exec_code.push_back(op);

    op.op=op_foreach;
    op.index=0;
    int ptr=exec_code.size();
    exec_code.push_back(op);
    if(ast.get_children()[0].get_type()==ast_new_iter)
    {
        op.op=op_load;
        std::string str=ast.get_children()[0].get_children()[0].get_str();
        regist_string(str);
        op.index=string_table[str];
        exec_code.push_back(op);
    }
    else
    {
        mem_call(ast.get_children()[0]);
        op.op=op_meq;
        op.index=0;
        exec_code.push_back(op);
    }
    block_gen(ast.get_children()[2]);
    op.op=op_jmp;
    op.index=ptr;
    exec_code.push_back(op);
    exec_code[ptr].index=exec_code.size();
    load_continue_break();
    return;
}

void nasal_codegen::or_gen(nasal_ast& ast)
{
    // a
    // jt l1
    // pop
    // b
    // jt l1
    // pop
    // pushnil
    opcode op;
    int l1,l2;
    calculation_gen(ast.get_children()[0]);
    op.op=op_jmptrue;
    op.index=0;
    l1=exec_code.size();
    exec_code.push_back(op);

    pop_gen();
    calculation_gen(ast.get_children()[1]);
    op.op=op_jmptrue;
    op.index=0;
    l2=exec_code.size();
    exec_code.push_back(op);

    pop_gen();
    nil_gen();

    exec_code[l1].index=exec_code[l2].index=exec_code.size();
    return;
}

void nasal_codegen::and_gen(nasal_ast& ast)
{
    // a
    // jt l1
    // jmp lfalse
    // l1:pop
    // b
    // jt l2
    // lfalse:pop
    // pushnil
    // l2:
    opcode op;
    calculation_gen(ast.get_children()[0]);
    op.op=op_jmptrue;
    op.index=exec_code.size()+2;
    exec_code.push_back(op);

    op.op=op_jmp;
    int ptr=exec_code.size();
    exec_code.push_back(op);

    pop_gen();
    calculation_gen(ast.get_children()[1]);
    op.op=op_jmptrue;
    op.index=exec_code.size()+3;
    exec_code.push_back(op);

    exec_code[ptr].index=exec_code.size();
    pop_gen();
    nil_gen();

    return;
}

void nasal_codegen::trino_gen(nasal_ast& ast)
{
    opcode op;
    calculation_gen(ast.get_children()[0]);
    op.op=op_jmpfalse;
    int ptr=exec_code.size();
    exec_code.push_back(op);
    pop_gen();
    calculation_gen(ast.get_children()[1]);
    op.op=op_jmp;
    int ptr_exit=exec_code.size();
    exec_code.push_back(op);
    exec_code[ptr].index=exec_code.size();
    pop_gen();
    calculation_gen(ast.get_children()[2]);
    exec_code[ptr_exit].index=exec_code.size();
    return;
}

void nasal_codegen::calculation_gen(nasal_ast& ast)
{
    opcode op;
    op.index=0;
    switch(ast.get_type())
    {
        case ast_nil:nil_gen();break;
        case ast_number:number_gen(ast);break;
        case ast_string:string_gen(ast);break;
        case ast_identifier:call_id(ast);break;
        case ast_vector:vector_gen(ast);break;
        case ast_hash:hash_gen(ast);break;
        case ast_function:function_gen(ast);break;
        case ast_call:call_gen(ast);break;
        case ast_equal:
            op.op=op_meq;
            calculation_gen(ast.get_children()[1]);
            mem_call(ast.get_children()[0]);
            exec_code.push_back(op);
            break;
        case ast_add_equal:
            op.op=op_addeq;
            calculation_gen(ast.get_children()[1]);
            mem_call(ast.get_children()[0]);
            exec_code.push_back(op);
            break;
        case ast_sub_equal:
            op.op=op_subeq;
            calculation_gen(ast.get_children()[1]);
            mem_call(ast.get_children()[0]);
            exec_code.push_back(op);
            break;
        case ast_mult_equal:
            op.op=op_muleq;
            calculation_gen(ast.get_children()[1]);
            mem_call(ast.get_children()[0]);
            exec_code.push_back(op);
            break;
        case ast_div_equal:
            op.op=op_diveq;
            calculation_gen(ast.get_children()[1]);
            mem_call(ast.get_children()[0]);
            exec_code.push_back(op);
            break;
        case ast_link_equal:
            op.op=op_lnkeq;
            calculation_gen(ast.get_children()[1]);
            mem_call(ast.get_children()[0]);
            exec_code.push_back(op);
            break;
        case ast_or:or_gen(ast);break;
        case ast_and:and_gen(ast);break;
        case ast_add:
            op.op=op_add;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_sub:
            op.op=op_sub;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_mult:
            op.op=op_mul;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_div:
            op.op=op_div;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_link:
            op.op=op_lnk;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_cmp_equal:
            op.op=op_eq;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_cmp_not_equal:
            op.op=op_neq;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_less_equal:
            op.op=op_leq;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_less_than:
            op.op=op_less;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_greater_equal:
            op.op=op_geq;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_greater_than:
            op.op=op_grt;
            calculation_gen(ast.get_children()[0]);
            calculation_gen(ast.get_children()[1]);
            exec_code.push_back(op);
            break;
        case ast_trinocular:trino_gen(ast);break;
        case ast_unary_sub:
            op.op=op_usub;
            calculation_gen(ast.get_children()[0]);
            exec_code.push_back(op);
            break;
        case ast_unary_not:
            op.op=op_unot;
            calculation_gen(ast.get_children()[0]);
            exec_code.push_back(op);
            break;
    }
    return;
}

void nasal_codegen::block_gen(nasal_ast& ast)
{
    opcode op;
    int size=ast.get_children().size();
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        switch(tmp.get_type())
        {
            case ast_null:
            case ast_nil:
            case ast_number:
            case ast_string:
            case ast_function:break;
            case ast_definition:definition_gen(tmp);break;
            case ast_multi_assign:multi_assignment_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_continue:
                op.op=op_jmp;
                op.index=0;
                continue_ptr.push_back(exec_code.size());
                exec_code.push_back(op);
                break;
            case ast_break:
                op.op=op_jmp;
                op.index=0;
                break_ptr.push_back(exec_code.size());
                exec_code.push_back(op);
                break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:loop_gen(tmp);break;
            case ast_identifier:
            case ast_vector:
            case ast_hash:
            case ast_call:
            case ast_equal:
            case ast_add_equal:
            case ast_sub_equal:
            case ast_mult_equal:
            case ast_div_equal:
            case ast_link_equal:
            case ast_unary_sub:
            case ast_unary_not:
            case ast_add:
            case ast_sub:
            case ast_mult:
            case ast_div:
            case ast_link:
            case ast_cmp_equal:
            case ast_cmp_not_equal:
            case ast_less_equal:
            case ast_less_than:
            case ast_greater_equal:
            case ast_greater_than:
            case ast_or:
            case ast_and:
            case ast_trinocular:calculation_gen(tmp);pop_gen();break;
            case ast_return:return_gen(tmp);break;
        }
    }
    return;
}

void nasal_codegen::return_gen(nasal_ast& ast)
{
    if(ast.get_children().size())
        calculation_gen(ast.get_children()[0]);
    opcode op;
    op.op=op_return;
    exec_code.push_back(op);
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
            case ast_null:
            case ast_nil:
            case ast_number:
            case ast_string:
            case ast_function:break;
            case ast_definition:definition_gen(tmp);break;
            case ast_multi_assign:multi_assignment_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:loop_gen(tmp);break;
            case ast_identifier:
            case ast_vector:
            case ast_hash:
            case ast_call:
            case ast_equal:
            case ast_add_equal:
            case ast_sub_equal:
            case ast_mult_equal:
            case ast_div_equal:
            case ast_link_equal:
            case ast_unary_sub:
            case ast_unary_not:
            case ast_add:
            case ast_sub:
            case ast_mult:
            case ast_div:
            case ast_link:
            case ast_cmp_equal:
            case ast_cmp_not_equal:
            case ast_less_equal:
            case ast_less_than:
            case ast_greater_equal:
            case ast_greater_than:
            case ast_or:
            case ast_and:
            case ast_trinocular:calculation_gen(tmp);pop_gen();break;
        }
    }
    opcode op;
    op.op=op_nop;
    op.index=0;
    exec_code.push_back(op);

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
    std::string numinfo="";
    int num=index;
    for(int i=0;i<8;++i)
    {
        int tmp=num&0x0f;
        numinfo=(char)(tmp>9? 'a'+tmp-10:'0'+tmp)+numinfo;
        num>>=4;
    }
    std::cout<<"0x"<<numinfo<<": ";
    // print opcode name
    for(int i=0;code_table[i].name;++i)
        if(exec_code[index].op==code_table[i].type)
        {
            std::cout<<code_table[i].name<<" ";
            break;
        }
    // print opcode index
    numinfo="";
    num=exec_code[index].index;
    for(int i=0;i<8;++i)
    {
        int tmp=num&0x0f;
        numinfo=(char)(tmp>9? 'a'+tmp-10:'0'+tmp)+numinfo;
        num>>=4;
    }
    std::cout<<"0x"<<numinfo<<"  ";
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