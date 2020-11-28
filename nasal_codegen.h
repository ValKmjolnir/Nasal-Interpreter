#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

enum op_code
{
    op_nop,
    op_load,
    op_pushnum,op_pushone,op_pushzero,
    op_pushnil,
    op_pushstr,
    op_newvec,op_newhash,op_newfunc,
    op_vecappend,op_hashappend,
    op_para,op_defaultpara,op_dynamicpara,op_funcbegin,
    op_unot,op_usub,
    op_add,op_sub,op_mul,op_div,op_lnk,
    op_eq,op_neq,op_less,op_leq,op_grt,op_geq,
    op_pop,
    op_jmp,op_jmptrue,op_jmpfalse,
    op_gccall,op_gccallvec,op_gccallhash,op_gccallfunc,op_builtincall,
    op_slice,op_slice2,
    op_memcall,op_memcallvec,op_memcallhash,
    op_return,
    op_end
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
    {op_vecappend,   "vapp  "},
    {op_hashappend,  "happ  "},
    {op_para,        "para  "},
    {op_defaultpara, "deflt "},
    {op_dynamicpara, "dyn   "},
    {op_funcbegin,   "fbegin"},
    {op_unot,        "not   "},
    {op_usub,        "usub  "},
    {op_add,         "add   "},
    {op_sub,         "sub   "},
    {op_mul,         "mult  "},
    {op_div,         "div   "},
    {op_lnk,         "link  "},
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
    {op_gccall,      "calli "},
    {op_gccallvec,   "callv "},
    {op_gccallhash,  "callh "},
    {op_gccallfunc,  "callf "},
    {op_builtincall, "callb "},
    {op_slice,       "slice "},
    {op_slice2,      "slice2"},
    {op_memcall,     "mcalli"},
    {op_memcallvec,  "mcallv"},
    {op_memcallhash, "mcallh"},
    {op_return,      "ret   "},
    {op_end,         "end   "},
    {-1,             NULL},
};

struct opcode
{
    unsigned char op;
    unsigned int index;
    opcode(){op=op_nop;index=0;}
};

// unfinished
// now it can output ast but it is not byte code yet
// please wait...
class nasal_codegen
{
private:
    std::map<double,int> number_table;
    std::map<std::string,int> string_table;
    std::vector<opcode> exec_code;
    int error;
    void nil_gen();
    void number_gen(nasal_ast&);
    void string_gen(nasal_ast&);
    void vector_gen(nasal_ast&);
    void hash_gen(nasal_ast&);
    void function_gen(nasal_ast&);
    void call_gen(nasal_ast&);
    void call_id(nasal_ast&);
    void definition_gen(nasal_ast&);
    void multi_assignment_gen(nasal_ast&);
    void conditional_gen(nasal_ast&);
    void loop_gen(nasal_ast&);
    void calculation_gen(nasal_ast&);
    void block_gen(nasal_ast&);
    void return_gen(nasal_ast&);
public:
    nasal_codegen();
    void main_progress(nasal_ast&);
    void print_byte_code();
};

nasal_codegen::nasal_codegen()
{
    error=0;
    number_table.clear();
    string_table.clear();
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
    int size=number_table.size();
    double num=ast.get_num();
    opcode op;
    if(number_table.find(num)==number_table.end())
        number_table[num]=size;
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
    int size=string_table.size();
    std::string str=ast.get_str();
    opcode op;
    if(string_table.find(str)==string_table.end())
        string_table[str]=size;
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
        tmp.op=op_vecappend;
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
        string_gen(ast.get_children()[i].get_children()[0]);
        calculation_gen(ast.get_children()[i].get_children()[1]);
        op.op=op_hashappend;
        op.index=0;
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
            int size=string_table.size();
            std::string str=tmp.get_str();
            if(string_table.find(str)==string_table.end())
                string_table[str]=size;
            opcode tmp;
            tmp.op=op_para;
            tmp.index=string_table[str];
            exec_code.push_back(tmp);
        }
        else if(tmp.get_type()==ast_default_arg)
        {
            calculation_gen(tmp.get_children()[1]);
            int size=string_table.size();
            std::string str=tmp.get_children()[0].get_str();
            if(string_table.find(str)==string_table.end())
                string_table[str]=size;
            opcode tmp;
            tmp.op=op_defaultpara;
            tmp.index=string_table[str];
            exec_code.push_back(tmp);
        }
        else if(tmp.get_type()==ast_dynamic_id)
        {
            int size=string_table.size();
            std::string str=tmp.get_str();
            if(string_table.find(str)==string_table.end())
                string_table[str]=size;
            opcode tmp;
            tmp.op=op_dynamicpara;
            tmp.index=string_table[str];
            exec_code.push_back(tmp);
        }
    }
    
    op.op=op_funcbegin;
    op.index=0;
    exec_code.push_back(op);

    nasal_ast& block=ast.get_children()[1];
    block_gen(block);
    if(!block.get_children().size() || block.get_children().back().get_type()!=ast_return)
    {
        opcode tmp;
        tmp.op=op_return;
        tmp.index=0;
        exec_code.push_back(tmp);
    }
    return;
}

void nasal_codegen::call_gen(nasal_ast& ast)
{
    if(ast.get_children()[0].get_type()==ast_identifier)
    {
        int size=string_table.size();
        std::string str=ast.get_children()[0].get_str();
        if(string_table.find(str)==string_table.end())
            string_table[str]=size;
        opcode op;
        op.op=op_gccall;
        op.index=string_table[str];
        exec_code.push_back(op);
    }
    else
        calculation_gen(ast.get_children()[0]);
    int child_size=ast.get_children().size();
    for(int i=0;i<child_size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()==ast_call_hash)
        {
            opcode op;
            op.op=op_gccallhash;
            int size=string_table.size();
            std::string str=tmp.get_str();
            if(string_table.find(str)==string_table.end())
                string_table[str]=size;
            op.index=string_table[str];
            exec_code.push_back(op);
        }
        ;// unfinished
    }
    return;
}

void nasal_codegen::call_id(nasal_ast& ast)
{
    int size=string_table.size();
    std::string str=ast.get_str();
    if(string_table.find(str)==string_table.end())
        string_table[str]=size;
    opcode op;
    op.op=op_gccall;
    op.index=string_table[str];
    exec_code.push_back(op);
    return;
}

void nasal_codegen::definition_gen(nasal_ast& ast)
{
    if(ast.get_children()[0].get_type()==ast_identifier)
    {
        calculation_gen(ast.get_children()[1]);
        int size=string_table.size();
        std::string str=ast.get_children()[0].get_str();
        if(string_table.find(str)==string_table.end())
            string_table[str]=size;
        opcode op;
        op.op=op_load;
        op.index=string_table[str];
        exec_code.push_back(op);
    }
    return;
}

void nasal_codegen::multi_assignment_gen(nasal_ast& ast)
{
    return;
}

void nasal_codegen::conditional_gen(nasal_ast& ast)
{
    return;
}

void nasal_codegen::loop_gen(nasal_ast& ast)
{
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
        case ast_add_equal:
        case ast_sub_equal:
        case ast_mult_equal:
        case ast_div_equal:
        case ast_link_equal:break;
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
        case ast_trinocular:break;
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
            case ast_trinocular:calculation_gen(tmp);break;
            case ast_break:break;
            case ast_continue:break;
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
            case ast_trinocular:calculation_gen(tmp);break;
        }
    }
    return;
}

void nasal_codegen::print_byte_code()
{
    int size=exec_code.size();
    for(int i=0;i<size;++i)
    {
        int type=exec_code[i].op;
        for(int j=0;code_table[j].name;++j)
            if(code_table[j].type==type)
            {
                std::cout<<code_table[j].name<<' ';
                break;
            }
        std::cout<<exec_code[i].index<<'\n';
    }
    return;
}

#endif