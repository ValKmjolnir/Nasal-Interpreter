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
    op_para,op_defaultpara,op_dynamicpara,
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
    void definition_gen(nasal_ast&);
    void multi_assignment_gen(nasal_ast&);
    void conditional_gen(nasal_ast&);
    void loop_gen(nasal_ast&);
    void calculation_gen(nasal_ast&);
public:
    nasal_codegen();
    void main_progress(nasal_ast&);
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
        op.op=op_vecappend;
        op.index=0;
        exec_code.push_back(op);
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


    return;
}

void nasal_codegen::definition_gen(nasal_ast& ast)
{
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
    return;
}

void nasal_codegen::main_progress(nasal_ast& ast)
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
        }
    }
    return;
}

#endif