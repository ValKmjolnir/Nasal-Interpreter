#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

enum op_code
{
    op_nop,
    op_load,
    op_loadv,
    op_pushnum,
    op_pushone,
    op_pushzero,
    op_pushnil,
    op_pushstr,
    op_newvec,op_newhash,op_newfunc,
    op_vecapp,op_hashapp,
    op_newscope,op_delscope,
    op_para,op_defpara,op_dynpara,op_entry,
    op_unot,op_usub,
    op_add,op_sub,op_mul,op_div,op_lnk,
    op_eq,op_neq,op_less,op_leq,op_grt,op_geq,
    op_pop,
    op_jmp,op_jmptrue,op_jmpfalse,
    op_call,op_callv,op_callh,op_callf,op_builtincall,
    op_slicebegin,op_sliceend,
    op_slice,op_slice2,
    op_mcall,op_mcallv,op_mcallh,
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
    {op_loadv,       "loadv "},
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
    {op_newscope,    "nscp  "},
    {op_delscope,    "dscp  "},
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
    {op_call,        "call  "},
    {op_callv,       "callv "},
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
    std::vector<double> number_result_table;
    std::vector<std::string> string_result_table;
    std::vector<opcode> exec_code;
    int error;
    void regist_number(double);
    void regist_string(std::string);
    void nil_gen();
    void number_gen(nasal_ast&);
    void string_gen(nasal_ast&);
    void vector_gen(nasal_ast&);
    void hash_gen(nasal_ast&);
    void function_gen(nasal_ast&);
    void call_gen(nasal_ast&);
    void call_id(nasal_ast&);
    void multi_def(nasal_ast&);
    void single_def(nasal_ast&);
    void definition_gen(nasal_ast&);
    void multi_assignment_gen(nasal_ast&);
    void conditional_gen(nasal_ast&);
    void loop_gen(nasal_ast&);
    void calculation_gen(nasal_ast&);
    int block_gen(nasal_ast&);
    void return_gen(nasal_ast&);
public:
    nasal_codegen();
    void main_progress(nasal_ast&);
    void print_op(int);
    void print_byte_code();
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
        string_gen(ast.get_children()[i].get_children()[0]);
        calculation_gen(ast.get_children()[i].get_children()[1]);
        op.op=op_hashapp;
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
    op.index=0;
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
    if(ast.get_children()[0].get_type()==ast_identifier)
    {
        std::string str=ast.get_children()[0].get_str();
        regist_string(str);
        opcode op;
        op.op=op_call;
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
            op.op=op_callh;
            std::string str=tmp.get_str();
            regist_string(str);
            op.index=string_table[str];
            exec_code.push_back(op);
        }
        ;// unfinished
    }
    return;
}

void nasal_codegen::call_id(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    opcode op;
    op.op=op_call;
    op.index=string_table[str];
    exec_code.push_back(op);
    return;
}

void nasal_codegen::single_def(nasal_ast& ast)
{
    calculation_gen(ast.get_children()[1]);
    std::string str=ast.get_children()[0].get_str();
    regist_string(str);
    opcode op;
    op.op=op_load;
    op.index=string_table[str];
    exec_code.push_back(op);
    return;
}
void nasal_codegen::multi_def(nasal_ast& ast)
{
    for(int i=0;i<ast.get_children()[0].get_children().size();++i);
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
    return;
}

void nasal_codegen::conditional_gen(nasal_ast& ast)
{
    opcode op;
    op.op=op_newscope;
    op.index=0;
    exec_code.push_back(op);
    // unfinished
    op.op=op_delscope;
    op.index=0;
    exec_code.push_back(op);
    return;
}

void nasal_codegen::loop_gen(nasal_ast& ast)
{
    opcode op;
    op.op=op_newscope;
    op.index=0;
    exec_code.push_back(op);
    // unfinished
    op.op=op_delscope;
    op.index=0;
    exec_code.push_back(op);
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
        case ast_or:
        case ast_and:break;
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

int nasal_codegen::block_gen(nasal_ast& ast)
{
    int begin_size=exec_code.size();
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
            case ast_or:
            case ast_and:
            case ast_trinocular:calculation_gen(tmp);break;
            case ast_break:break;
            case ast_continue:break;
            case ast_return:return_gen(tmp);break;
        }
    }
    return exec_code.size()-begin_size;
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
            case ast_or:
            case ast_and:
            case ast_trinocular:calculation_gen(tmp);break;
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
    std::string numinfo="";
    int num=index;
    for(int i=0;i<8;++i)
    {
        int tmp=num&0x0f;
        numinfo=(char)(tmp>9? 'a'+tmp-10:'0'+tmp)+numinfo;
        num>>=4;
    }
    std::cout<<"0x"<<numinfo<<": ";
    for(int i=0;code_table[i].name;++i)
        if(exec_code[index].op==code_table[i].type)
        {
            std::cout<<code_table[i].name<<" ";
            break;
        }
    numinfo="";
    num=exec_code[index].index;
    for(int i=0;i<8;++i)
    {
        int tmp=num&0x0f;
        numinfo=(char)(tmp>9? 'a'+tmp-10:'0'+tmp)+numinfo;
        num>>=4;
    }
    std::cout<<"0x"<<numinfo;
    switch(exec_code[index].op)
    {
        case op_pushnum:std::cout<<'('<<number_result_table[exec_code[index].index]<<')';break;
        case op_call:
        case op_mcall:
        case op_pushstr:
        case op_callh:
        case op_mcallh:
        case op_para:
        case op_defpara:
        case op_dynpara:
        case op_load:
        case op_loadv:std::cout<<'('<<string_result_table[exec_code[index].index]<<')';break;
    }
    std::cout<<'\n';
    return;
}

void nasal_codegen::print_byte_code()
{
    int size=exec_code.size();
    for(int i=0;i<size;++i)
        print_op(i);
    return;
}

#endif