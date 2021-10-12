#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

enum op_code
{
    op_nop,     // do nothing and end the vm main loop
    op_intg,    // global scope size
    op_intl,    // local scope size
    op_loadg,   // load global symbol value
    op_loadl,   // load local symbol value
    op_loadu,   // load upvalue
    op_pnum,    // push constant number to the stack
    op_pone,    // push 1 to the stack
    op_pzero,   // push 0 to the stack
    op_pnil,    // push constant nil to the stack
    op_pstr,    // push constant string to the stack
    op_newv,    // push new vector with initial values from stack
    op_newh,    // push new hash to the stack
    op_newf,    // push new function to the stack
    op_happ,    // hash append
    op_para,    // normal  parameter
    op_defpara, // default parameter
    op_dynpara, // dynamic parameter
    op_unot,    // !
    op_usub,    // -
    op_add,     // +
    op_sub,     // -
    op_mul,     // *
    op_div,     // /
    op_lnk,     // ~
    op_addc,    // + const
    op_subc,    // - const
    op_mulc,    // * const
    op_divc,    // / const
    op_lnkc,    // ~ const
    op_addeq,   // +=
    op_subeq,   // -=
    op_muleq,   // *=
    op_diveq,   // /=
    op_lnkeq,   // ~=
    op_addeqc,  // += const
    op_subeqc,  // -= const
    op_muleqc,  // *= const
    op_diveqc,  // /= const
    op_lnkeqc,  // ~= const
    op_meq,     // =
    op_eq,      // ==
    op_neq,     // !=
    op_less,    // <
    op_leq,     // <=
    op_grt,     // >
    op_geq,     // >=
    op_lessc,   // < const
    op_leqc,    // <= const
    op_grtc,    // > const
    op_geqc,    // >= const
    op_pop,     // pop a value from stack
    op_jmp,     // jump with no condition
    op_jt,      // used in operator and/or,jmp when condition is true and DO NOT POP
    op_jf,      // used in conditional/loop,jmp when condition is false and POP STACK
    op_cnt,     // add counter for forindex/foreach
    op_cntpop,  // pop counter
    op_findex,  // index counter on the top of forindex_stack plus 1
    op_feach,   // index counter on the top of forindex_stack plus 1 and get the value in vector
    op_callg,   // call value in global scope
    op_calll,   // call value in local scope
    op_upval,   // call upvalue in closure
    op_callv,   // call vec[index]
    op_callvi,  // call vec[immediate] (used in multi-assign/multi-define)
    op_callh,   // call hash.label
    op_callfv,  // call function(vector as parameters)
    op_callfh,  // call function(hash as parameters)
    op_callb,   // call builtin-function
    op_slcbegin,// begin of slice like: vec[1,2,3:6,0,-1]
    op_slcend,  // end of slice
    op_slc,     // slice like vec[1]
    op_slc2,    // slice like vec[nil:10]
    op_mcallg,  // get memory space of value in global scope
    op_mcalll,  // get memory space of value in local scope
    op_mupval,  // get memory space of value in closure
    op_mcallv,  // get memory space of vec[index]
    op_mcallh,  // get memory space of hash.label
    op_ret      // return
};

struct
{
    int type;
    const char* name;
}code_table[]=
{
    {op_nop,     "nop   "},
    {op_intg,    "intg  "},
    {op_intl,    "intl  "},
    {op_loadg,   "loadg "},
    {op_loadl,   "loadl "},
    {op_loadu,   "loadu "},
    {op_pnum,    "pnum  "},
    {op_pone,    "pone  "},
    {op_pzero,   "pzero "},
    {op_pnil,    "pnil  "},
    {op_pstr,    "pstr  "},
    {op_newv,    "newv  "},
    {op_newh,    "newh  "},
    {op_newf,    "newf  "},
    {op_happ,    "happ  "},
    {op_para,    "para  "},
    {op_defpara, "def   "},
    {op_dynpara, "dyn   "},
    {op_unot,    "not   "},
    {op_usub,    "usub  "},
    {op_add,     "add   "},
    {op_sub,     "sub   "},
    {op_mul,     "mult  "},
    {op_div,     "div   "},
    {op_lnk,     "link  "},
    {op_addc,    "addc  "},
    {op_subc,    "subc  "},
    {op_mulc,    "multc "},
    {op_divc,    "divc  "},
    {op_lnkc,    "lnkc  "},
    {op_addeq,   "addeq "},
    {op_subeq,   "subeq "},
    {op_muleq,   "muleq "},
    {op_diveq,   "diveq "},
    {op_lnkeq,   "lnkeq "},
    {op_addeqc,  "addeqc"},
    {op_subeqc,  "subeqc"},
    {op_muleqc,  "muleqc"},
    {op_diveqc,  "diveqc"},
    {op_lnkeqc,  "lnkeqc"},
    {op_meq,     "meq   "},
    {op_eq,      "eq    "},
    {op_neq,     "neq   "},
    {op_less,    "less  "},
    {op_leq,     "leq   "},
    {op_grt,     "grt   "},
    {op_geq,     "geq   "},
    {op_lessc,   "lessc "},
    {op_leqc,    "leqc  "},
    {op_grtc,    "grtc  "},
    {op_geqc,    "geqc  "},
    {op_pop,     "pop   "},
    {op_jmp,     "jmp   "},
    {op_jt,      "jt    "},
    {op_jf,      "jf    "},
    {op_cnt,     "cnt   "},
    {op_cntpop,  "cntpop"},
    {op_findex,  "findx "},
    {op_feach,   "feach "},
    {op_callg,   "callg "},
    {op_calll,   "calll "},
    {op_upval,   "upval "},
    {op_callv,   "callv "},
    {op_callvi,  "callvi"},
    {op_callh,   "callh "},
    {op_callfv,  "callfv"},
    {op_callfh,  "callfh"},
    {op_callb,   "callb "},
    {op_slcbegin,"slcbeg"},
    {op_slcend,  "slcend"},
    {op_slc,     "slc   "},
    {op_slc2,    "slc2  "},
    {op_mcallg,  "mcallg"},
    {op_mcalll,  "mcalll"},
    {op_mupval,  "mupval"},
    {op_mcallv,  "mcallv"},
    {op_mcallh,  "mcallh"},
    {op_ret,     "ret   "},
    {-1,         nullptr },
};

struct opcode
{
    uint16_t op;  // opcode
    uint16_t fidx;// source code file index
    uint32_t num; // imm num
    uint32_t line;// line of source code
    opcode(uint8_t _op=op_nop,uint16_t _fidx=0,uint32_t _num=0,uint32_t _line=0)
    {
        op=_op;
        fidx=_fidx;
        num=_num;
        line=_line;
        return;
    }
    opcode& operator=(const opcode& tmp)
    {
        op=tmp.op;
        fidx=tmp.fidx;
        num=tmp.num;
        line=tmp.line;
        return *this;
    }
};

class nasal_codegen
{
private:
    uint32_t error;
    uint16_t fileindex;
    uint32_t in_forindex;
    uint32_t in_foreach;
    std::unordered_map<double,int>      number_table;
    std::unordered_map<std::string,int> string_table;
    std::vector<double>                 num_res_table;
    std::vector<std::string>            str_res_table;
    std::vector<std::string>            file_name;
    std::vector<opcode>                 exec_code;
    std::list<std::vector<int>>         continue_ptr;
    std::list<std::vector<int>>         break_ptr;
    std::vector<std::string>            global;
    std::list<std::vector<std::string>> local;
    
    void die(const std::string,const int);
    void regist_number(const double);
    void regist_string(const std::string&);
    void find_symbol(const nasal_ast&);
    void add_sym(const std::string&);
    int  local_find(const std::string&);
    int  global_find(const std::string&);
    int  upvalue_find(const std::string&);
    void gen(uint8_t,uint32_t,uint32_t);
    void num_gen(const nasal_ast&);
    void str_gen(const nasal_ast&);
    void vec_gen(const nasal_ast&);
    void hash_gen(const nasal_ast&);
    void func_gen(const nasal_ast&);
    void call_gen(const nasal_ast&);
    void call_id(const nasal_ast&);
    void call_hash(const nasal_ast&);
    void call_vec(const nasal_ast&);
    void call_func(const nasal_ast&);
    void mcall(const nasal_ast&);
    void mcall_id(const nasal_ast&);
    void mcall_vec(const nasal_ast&);
    void mcall_hash(const nasal_ast&);
    void multi_def(const nasal_ast&);
    void single_def(const nasal_ast&);
    void def_gen(const nasal_ast&);
    void multi_assign_gen(const nasal_ast&);
    void conditional_gen(const nasal_ast&);
    void loop_gen(const nasal_ast&);
    void load_continue_break(int,int);
    void while_gen(const nasal_ast&);
    void for_gen(const nasal_ast&);
    void forindex_gen(const nasal_ast&);
    void foreach_gen(const nasal_ast&);
    void or_gen(const nasal_ast&);
    void and_gen(const nasal_ast&);
    void trino_gen(const nasal_ast&);
    void calc_gen(const nasal_ast&);
    void block_gen(const nasal_ast&);
    void ret_gen(const nasal_ast&);
public:
    uint32_t                  get_error(){return error;}
    void                      main_progress(const nasal_ast&,const std::vector<std::string>&);
    void                      print_op(int);
    void                      print_byte_code();
    std::vector<std::string>& get_str_table(){return str_res_table;}
    std::vector<double>&      get_num_table(){return num_res_table;}
    std::vector<opcode>&      get_exec_code(){return exec_code;}
};

void nasal_codegen::die(const std::string info,const int line)
{
    ++error;
    std::cout<<"[code] <"<<file_name[fileindex]<<"> line "<<line<<": "<<info<<"\n";
    return;
}

void nasal_codegen::regist_number(const double num)
{
    int size=number_table.size();
    if(!number_table.count(num))
    {
        number_table[num]=size;
        num_res_table.push_back(num);
    }
    return;
}

void nasal_codegen::regist_string(const std::string& str)
{
    int size=string_table.size();
    if(!string_table.count(str))
    {
        string_table[str]=size;
        str_res_table.push_back(str);
    }
    return;
}

void nasal_codegen::find_symbol(const nasal_ast& node)
{
    // symbol definition checked here
    // if find a function, return
    if(node.get_type()==ast_func)
        return;
    // find definition, check
    else if(node.get_type()==ast_def)
    {
        if(node.get_children()[0].get_type()==ast_multi_id)
            for(auto& i:node.get_children()[0].get_children())
                add_sym(i.get_str());
        else
            add_sym(node.get_children()[0].get_str());
        find_symbol(node.get_children()[1]);
    }
    // find iterator(foreach, forindex), check
    else if(node.get_type()==ast_new_iter)
        add_sym(node.get_children()[0].get_str());
    // check children
    else
        for(auto& i:node.get_children())
            find_symbol(i);
    return;
}

void nasal_codegen::add_sym(const std::string& name)
{
    if(local.empty())
    {
        for(auto& sym:global)
            if(sym==name)
                return;
        global.push_back(name);
    }
    else
    {
        for(auto& sym:local.back())
            if(sym==name)
                return;
        local.back().push_back(name);
    }
    return;
}

int nasal_codegen::local_find(const std::string& name)
{
    if(local.empty())
        return -1;
    for(uint32_t i=0;i<local.back().size();++i)
        if(local.back()[i]==name)
            return i;
    return -1;
}

int nasal_codegen::global_find(const std::string& name)
{
    for(int i=0;i<global.size();++i)
        if(global[i]==name)
            return i;
    return -1;
}

int nasal_codegen::upvalue_find(const std::string& name)
{
    // 32768 level 65536 upvalues
    int index=-1;
    size_t size=local.size();
    if(size<=1)
        return -1;
    auto iter=local.begin();
    for(uint32_t i=0;i<size-1;++i)
    {
        for(uint32_t j=0;j<iter->size();++j)
            if((*iter)[j]==name)
                index=((i<<16)|j);
        ++iter;
    }
    return index;
}

void nasal_codegen::gen(uint8_t op,uint32_t num,uint32_t line)
{
    exec_code.push_back({op,fileindex,num,line});
    return;
}

void nasal_codegen::num_gen(const nasal_ast& ast)
{
    double num=ast.get_num();
    if(num==0)gen(op_pzero,0,ast.get_line());
    else if(num==1)gen(op_pone,0,ast.get_line());
    else
    {
        regist_number(num);
        gen(op_pnum,number_table[num],ast.get_line());
    }
    return;
}

void nasal_codegen::str_gen(const nasal_ast& ast)
{
    const std::string& str=ast.get_str();
    regist_string(str);
    gen(op_pstr,string_table[str],ast.get_line());
    return;
}

void nasal_codegen::vec_gen(const nasal_ast& ast)
{
    for(auto& node:ast.get_children())
        calc_gen(node);
    gen(op_newv,ast.get_children().size(),ast.get_line());
    return;
}

void nasal_codegen::hash_gen(const nasal_ast& ast)
{
    gen(op_newh,0,ast.get_line());
    for(auto& node:ast.get_children())
    {
        calc_gen(node.get_children()[1]);
        const std::string& str=node.get_children()[0].get_str();
        regist_string(str);
        gen(op_happ,string_table[str],node.get_line());
    }
    return;
}

void nasal_codegen::func_gen(const nasal_ast& ast)
{
    int newfunc_label=exec_code.size();
    gen(op_newf,0,ast.get_line());
    int local_label=exec_code.size();
    gen(op_intl,0,ast.get_line());
    
    // add special keyword 'me' into symbol table
    // this symbol is only used in local scope(function's scope)
    // this keyword is set to nil as default value
    // after calling a hash, this keyword is set to this hash
    // this symbol's index will be 0
    local.push_back({"me"});

    // generate parameter list
    for(auto& tmp:ast.get_children()[0].get_children())
    {
        if(tmp.get_type()==ast_id)
        {
            const std::string& str=tmp.get_str();
            regist_string(str);
            add_sym(str);
            gen(op_para,string_table[str],tmp.get_line());
        }
        else if(tmp.get_type()==ast_default_arg)
        {
            calc_gen(tmp.get_children()[0]);
            const std::string& str=tmp.get_str();
            regist_string(str);
            add_sym(str);
            gen(op_defpara,string_table[str],tmp.get_line());
        }
        else if(tmp.get_type()==ast_dynamic_id)
        {
            const std::string& str=tmp.get_str();
            regist_string(str);
            add_sym(str);
            gen(op_dynpara,string_table[str],tmp.get_line());
        }
    }
    // default dynamic parameter name is arg.
    // if there are too many arguments,unused arguments will be collected to a nasal_vec
    // if(!ast.get_children().size() || ast.get_children().back().get_type()!=ast_dynamic_id)
    // {
    //     const std::string str="arg";
    //     regist_string(str);
    //     add_sym(str);
    //     gen(op_dynpara,string_table[str],ast.get_line());
    // }
    exec_code[newfunc_label].num=exec_code.size()+1;
    int jmp_ptr=exec_code.size();
    gen(op_jmp,0,0);

    const nasal_ast& block=ast.get_children()[1];
    // search symbols first, must use after loading parameters
    // or the location of symbols will change and cause fatal error
    find_symbol(block);
    block_gen(block);
    exec_code[local_label].num=local.back().size();
    local.pop_back();

    if(!block.get_children().size() || block.get_children().back().get_type()!=ast_ret)
    {
        gen(op_pnil,0,block.get_line());
        gen(op_ret,0,block.get_line());
    }
    exec_code[jmp_ptr].num=exec_code.size();
    return;
}

void nasal_codegen::call_gen(const nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    if(exec_code.empty())
        std::cout<<"warning\n";
    if(exec_code.back().op==op_callb)
        return;
    int child_size=ast.get_children().size();
    for(int i=1;i<child_size;++i)
    {
        const nasal_ast& tmp=ast.get_children()[i];
        switch(tmp.get_type())
        {
            case ast_callh:call_hash(tmp);break;
            case ast_callv:call_vec(tmp); break;
            case ast_callf:call_func(tmp);break;
        }
    }
    return;
}

void nasal_codegen::call_id(const nasal_ast& ast)
{
    const std::string& str=ast.get_str();
    for(uint32_t i=0;builtin_func[i].name;++i)
        if(builtin_func[i].name==str)
        {
            gen(op_callb,i,ast.get_line());
            if(local.empty())
                die("builtin functions work in a local scope.",ast.get_line());
            return;
        }
    int index=local_find(str);
    if(index>=0)
    {
        gen(op_calll,index,ast.get_line());
        return;
    }
    index=upvalue_find(str);
    if(index>=0)
    {
        gen(op_upval,index,ast.get_line());
        return;
    }
    index=global_find(str);
    if(index>=0)
    {
        gen(op_callg,index,ast.get_line());
        return;
    }
    die("undefined symbol \""+str+"\".",ast.get_line());
    return;
}

void nasal_codegen::call_hash(const nasal_ast& ast)
{
    const std::string& str=ast.get_str();
    regist_string(str);
    gen(op_callh,string_table[str],ast.get_line());
    return;
}

void nasal_codegen::call_vec(const nasal_ast& ast)
{
    // maybe this place can use callv-const if ast's first child is ast_num
    if(ast.get_children().size()==1 && ast.get_children()[0].get_type()!=ast_subvec)
    {
        calc_gen(ast.get_children()[0]);
        gen(op_callv,0,ast.get_children()[0].get_line());
        return;
    }
    gen(op_slcbegin,0,ast.get_line());
    for(auto& tmp:ast.get_children())
    {
        if(tmp.get_type()!=ast_subvec)
        {
            calc_gen(tmp);
            gen(op_slc,0,tmp.get_line());
        }
        else
        {
            calc_gen(tmp.get_children()[0]);
            calc_gen(tmp.get_children()[1]);
            gen(op_slc2,0,tmp.get_line());
        }
    }
    gen(op_slcend,0,ast.get_line());
    return;
}

void nasal_codegen::call_func(const nasal_ast& ast)
{
    if(!ast.get_children().size())
        gen(op_callfv,0,ast.get_line());
    else if(ast.get_children()[0].get_type()==ast_hashmember)
    {
        hash_gen(ast);
        gen(op_callfh,0,ast.get_line());
    }
    else
    {
        for(auto& node:ast.get_children())
            calc_gen(node);
        gen(op_callfv,ast.get_children().size(),ast.get_line());
    }
    return;
}

void nasal_codegen::mcall(const nasal_ast& ast)
{
    if(ast.get_type()==ast_id)
    {
        mcall_id(ast);
        return;
    }
    calc_gen(ast.get_children()[0]);
    for(int i=1;i<ast.get_children().size()-1;++i)
    {
        const nasal_ast& tmp=ast.get_children()[i];
        switch(tmp.get_type())
        {
            case ast_callh:call_hash(tmp);break;
            case ast_callv:call_vec(tmp); break;
            case ast_callf:call_func(tmp);break;
        }
    }
    const nasal_ast& tmp=ast.get_children().back();
    if(tmp.get_type()==ast_callh)
        mcall_hash(tmp);
    else if(tmp.get_type()==ast_callv)
        mcall_vec(tmp);
    return;
}

void nasal_codegen::mcall_id(const nasal_ast& ast)
{
    const std::string& str=ast.get_str();
    for(int i=0;builtin_func[i].name;++i)
        if(builtin_func[i].name==str)
        {
            die("cannot change builtin function.",ast.get_line());
            return;
        }
    int index=local_find(str);
    if(index>=0)
    {
        gen(op_mcalll,index,ast.get_line());
        return;
    }
    index=upvalue_find(str);
    if(index>=0)
    {
        gen(op_mupval,index,ast.get_line());
        return;
    }
    index=global_find(str);
    if(index>=0)
    {
        gen(op_mcallg,index,ast.get_line());
        return;
    }
    die("undefined symbol \""+str+"\".",ast.get_line());
    return;
}

void nasal_codegen::mcall_vec(const nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    gen(op_mcallv,0,ast.get_line());
    return;
}

void nasal_codegen::mcall_hash(const nasal_ast& ast)
{
    const std::string& str=ast.get_str();
    regist_string(str);
    gen(op_mcallh,string_table[str],ast.get_line());
    return;
}

void nasal_codegen::single_def(const nasal_ast& ast)
{
    const std::string& str=ast.get_children()[0].get_str();
    calc_gen(ast.get_children()[1]);
    local.empty()?gen(op_loadg,global_find(str),ast.get_line()):gen(op_loadl,local_find(str),ast.get_line());
    return;
}
void nasal_codegen::multi_def(const nasal_ast& ast)
{
    auto& ids=ast.get_children()[0].get_children();
    int size=ids.size();
    if(ast.get_children()[1].get_type()==ast_multi_scalar)
    {
        auto& vals=ast.get_children()[1].get_children();
        for(int i=0;i<size;++i)
        {
            calc_gen(vals[i]);
            const std::string& str=ids[i].get_str();
            local.empty()?gen(op_loadg,global_find(str),ids[i].get_line()):gen(op_loadl,local_find(str),ids[i].get_line());
        }
    }
    else
    {
        calc_gen(ast.get_children()[1]);
        for(int i=0;i<size;++i)
        {
            gen(op_callvi,i,ast.get_children()[1].get_line());
            const std::string& str=ids[i].get_str();
            local.empty()?gen(op_loadg,global_find(str),ids[i].get_line()):gen(op_loadl,local_find(str),ids[i].get_line());
        }
        gen(op_pop,0,ast.get_line());
    }
    return;
}

void nasal_codegen::def_gen(const nasal_ast& ast)
{
    if(ast.get_children()[0].get_type()==ast_id)
        single_def(ast);
    else
        multi_def(ast);
    return;
}

void nasal_codegen::multi_assign_gen(const nasal_ast& ast)
{
    int size=ast.get_children()[0].get_children().size();
    if(ast.get_children()[1].get_type()==ast_multi_scalar)
    {
        for(int i=size-1;i>=0;--i)
            calc_gen(ast.get_children()[1].get_children()[i]);
        for(int i=0;i<size;++i)
        {
            mcall(ast.get_children()[0].get_children()[i]);
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            if(exec_code.back().op==op_mcalll)
                exec_code.back().op=op_loadl;
            else if(exec_code.back().op==op_mupval)
                exec_code.back().op=op_loadu;
            else if(exec_code.back().op==op_mcallg)
                exec_code.back().op=op_loadg;
            else
            {
                gen(op_meq,0,ast.get_children()[0].get_children()[i].get_line());
                gen(op_pop,0,ast.get_children()[0].get_children()[i].get_line());
            }
        }
    }
    else
    {
        calc_gen(ast.get_children()[1]);
        for(int i=0;i<size;++i)
        {
            gen(op_callvi,i,ast.get_children()[1].get_line());
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            mcall(ast.get_children()[0].get_children()[i]);
            if(exec_code.back().op==op_mcalll)
                exec_code.back().op=op_loadl;
            else if(exec_code.back().op==op_mupval)
                exec_code.back().op=op_loadu;
            else if(exec_code.back().op==op_mcallg)
                exec_code.back().op=op_loadg;
            else
            {
                gen(op_meq,0,ast.get_children()[0].get_children()[i].get_line());
                gen(op_pop,0,ast.get_children()[0].get_children()[i].get_line());
            }
        }
        gen(op_pop,0,ast.get_line());
    }
    return;
}

void nasal_codegen::conditional_gen(const nasal_ast& ast)
{
    std::vector<int> jmp_label;
    for(auto& tmp:ast.get_children())
    {
        if(tmp.get_type()==ast_if || tmp.get_type()==ast_elsif)
        {
            calc_gen(tmp.get_children()[0]);
            int ptr=exec_code.size();
            gen(op_jf,0,0);
            block_gen(tmp.get_children()[1]);
            // without 'else' the last condition doesn't need to jmp
            if(&tmp!=&ast.get_children().back())
            {
                jmp_label.push_back(exec_code.size());
                gen(op_jmp,0,0);
            }
            exec_code[ptr].num=exec_code.size();
        }
        else
        {
            block_gen(tmp.get_children()[0]);
            break;
        }
    }
    for(auto i:jmp_label)
        exec_code[i].num=exec_code.size();
    return;
}

void nasal_codegen::loop_gen(const nasal_ast& ast)
{
    continue_ptr.push_front(std::vector<int>());
    break_ptr.push_front(std::vector<int>());
    switch(ast.get_type())
    {
        case ast_while:    while_gen(ast);    break;
        case ast_for:      for_gen(ast);      break;
        case ast_forindex: ++in_forindex;forindex_gen(ast);--in_forindex; break;
        case ast_foreach:  ++in_foreach; foreach_gen(ast); --in_foreach;  break;
    }
    return;
}

void nasal_codegen::load_continue_break(int continue_place,int break_place)
{
    for(auto i:continue_ptr.front())
        exec_code[i].num=continue_place;
    for(auto i:break_ptr.front())
        exec_code[i].num=break_place;
    continue_ptr.pop_front();
    break_ptr.pop_front();
    return;
}

void nasal_codegen::while_gen(const nasal_ast& ast)
{
    int loop_ptr=exec_code.size();
    calc_gen(ast.get_children()[0]);
    int condition_ptr=exec_code.size();
    gen(op_jf,0,0);

    block_gen(ast.get_children()[1]);
    gen(op_jmp,loop_ptr,0);
    exec_code[condition_ptr].num=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    return;
}

void nasal_codegen::for_gen(const nasal_ast& ast)
{
    switch(ast.get_children()[0].get_type())
    {
        case ast_null:break;
        case ast_def:def_gen(ast.get_children()[0]);break;
        case ast_multi_assign:multi_assign_gen(ast.get_children()[0]);break;
        case ast_nil:
        case ast_num:
        case ast_str:
        case ast_func:break;
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
        case ast_trino:
            calc_gen(ast.get_children()[0]);
            gen(op_pop,0,0);
            break;
    }
    int jmp_place=exec_code.size();
    if(ast.get_children()[1].get_type()==ast_null)
        gen(op_pone,0,ast.get_children()[1].get_line());
    else
        calc_gen(ast.get_children()[1]);
    int label_exit=exec_code.size();
    gen(op_jf,0,0);

    block_gen(ast.get_children()[3]);
    int continue_place=exec_code.size();
    switch(ast.get_children()[2].get_type())
    {
        case ast_null:break;
        case ast_def:def_gen(ast.get_children()[2]);break;
        case ast_multi_assign:multi_assign_gen(ast.get_children()[2]);break;
        case ast_nil:case ast_num:case ast_str:case ast_func:break;
        case ast_vec:case ast_hash:
        case ast_call:
        case ast_equal:case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:case ast_lnkeq:
        case ast_neg:case ast_not:
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
        case ast_cmpeq:case ast_neq:case ast_leq:case ast_less:case ast_geq:case ast_grt:
        case ast_trino:
            calc_gen(ast.get_children()[2]);
            gen(op_pop,0,0);
            break;
    }
    gen(op_jmp,jmp_place,0);
    exec_code[label_exit].num=exec_code.size();

    load_continue_break(continue_place,exec_code.size());
    return;
}
void nasal_codegen::forindex_gen(const nasal_ast& ast)
{
    calc_gen(ast.get_children()[1]);
    gen(op_cnt,0,ast.get_children()[1].get_line());
    int ptr=exec_code.size();
    gen(op_findex,0,ast.get_line());
    if(ast.get_children()[0].get_type()==ast_new_iter)
    {
        const std::string& str=ast.get_children()[0].get_children()[0].get_str();
        local.empty()?
            gen(op_loadg,global_find(str),ast.get_children()[0].get_children()[0].get_line())
            :gen(op_loadl,local_find(str),ast.get_children()[0].get_children()[0].get_line());
    }
    else
    {
        mcall(ast.get_children()[0]);
        gen(op_meq,0,ast.get_children()[0].get_line());
        gen(op_pop,0,0);
    }

    block_gen(ast.get_children()[2]);
    gen(op_jmp,ptr,0);
    exec_code[ptr].num=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    gen(op_pop,0,0);// pop vector
    gen(op_cntpop,0,0);
    return;
}
void nasal_codegen::foreach_gen(const nasal_ast& ast)
{
    calc_gen(ast.get_children()[1]);
    gen(op_cnt,0,ast.get_line());
    int ptr=exec_code.size();
    gen(op_feach,0,ast.get_line());
    if(ast.get_children()[0].get_type()==ast_new_iter)
    {
        const std::string& str=ast.get_children()[0].get_children()[0].get_str();
        local.empty()?
            gen(op_loadg,global_find(str),ast.get_children()[0].get_children()[0].get_line())
            :gen(op_loadl,local_find(str),ast.get_children()[0].get_children()[0].get_line());
    }
    else
    {
        mcall(ast.get_children()[0]);
        gen(op_meq,0,ast.get_children()[0].get_line());
        gen(op_pop,0,0);
    }

    block_gen(ast.get_children()[2]);
    gen(op_jmp,ptr,0);
    exec_code[ptr].num=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    gen(op_pop,0,0);// pop vector
    gen(op_cntpop,0,0);
    return;
}

void nasal_codegen::or_gen(const nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    int l1=exec_code.size();
    gen(op_jt,0,0);

    gen(op_pop,0,0);
    calc_gen(ast.get_children()[1]);
    int l2=exec_code.size();
    gen(op_jt,0,0);

    gen(op_pop,0,0);
    gen(op_pnil,0,0);

    exec_code[l1].num=exec_code[l2].num=exec_code.size();
    return;
}

void nasal_codegen::and_gen(const nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    gen(op_jt,exec_code.size()+2,0);

    int lfalse=exec_code.size();
    gen(op_jmp,0,0);
    gen(op_pop,0,0);// jt jumps here

    calc_gen(ast.get_children()[1]);
    gen(op_jt,exec_code.size()+3,0);

    exec_code[lfalse].num=exec_code.size();
    gen(op_pop,0,0);
    gen(op_pnil,0,0);
    //jt jumps here
    return;
}

void nasal_codegen::trino_gen(const nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    int lfalse=exec_code.size();
    gen(op_jf,0,0);
    calc_gen(ast.get_children()[1]);
    int lexit=exec_code.size();
    gen(op_jmp,0,0);
    exec_code[lfalse].num=exec_code.size();
    calc_gen(ast.get_children()[2]);
    exec_code[lexit].num=exec_code.size();
    return;
}

void nasal_codegen::calc_gen(const nasal_ast& ast)
{
    switch(ast.get_type())
    {
        case ast_nil:  gen(op_pnil,0,ast.get_line());break;
        case ast_num:  num_gen(ast);   break;
        case ast_str:  str_gen(ast);   break;
        case ast_id:   call_id(ast);   break;
        case ast_vec:  vec_gen(ast);   break;
        case ast_hash: hash_gen(ast);  break;
        case ast_func: func_gen(ast);  break;
        case ast_call: call_gen(ast);  break;
        case ast_equal:
            calc_gen(ast.get_children()[1]);
            mcall(ast.get_children()[0]);
            gen(op_meq,0,ast.get_line());
            break;
        // ast_addeq(22)~ast_lnkeq(26) op_addeq(23)~op_lnkeq(27)
        case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:
            if(ast.get_children()[1].get_type()!=ast_num)
                calc_gen(ast.get_children()[1]);
            mcall(ast.get_children()[0]);
            if(ast.get_children()[1].get_type()!=ast_num)
                gen(ast.get_type()-ast_addeq+op_addeq,0,ast.get_line());
            else
            {
                regist_number(ast.get_children()[1].get_num());
                gen(ast.get_type()-ast_addeq+op_addeqc,number_table[ast.get_children()[1].get_num()],ast.get_line());
            }
            break;
        case ast_lnkeq:
            if(ast.get_children()[1].get_type()!=ast_str)
                calc_gen(ast.get_children()[1]);
            else
                regist_string(ast.get_children()[1].get_str());
            mcall(ast.get_children()[0]);
            if(ast.get_children()[1].get_type()!=ast_str)
                gen(op_lnkeq,0,ast.get_line());
            else
                gen(op_lnkeqc,string_table[ast.get_children()[1].get_str()],ast.get_line());
            break;
        case ast_or:or_gen(ast);break;
        case ast_and:and_gen(ast);break;
        // ast_add(33)~ast_link(37) op_add(18)~op_lnk(22)
        case ast_add:case ast_sub:case ast_mult:case ast_div:
            calc_gen(ast.get_children()[0]);
            if(ast.get_children()[1].get_type()!=ast_num)
            {
                calc_gen(ast.get_children()[1]);
                gen(ast.get_type()-ast_add+op_add,0,ast.get_line());
            }
            else
            {
                regist_number(ast.get_children()[1].get_num());
                gen(ast.get_type()-ast_add+op_addc,number_table[ast.get_children()[1].get_num()],ast.get_line());
            }
            break;
        case ast_link:
            calc_gen(ast.get_children()[0]);
            if(ast.get_children()[1].get_type()!=ast_str)
            {
                calc_gen(ast.get_children()[1]);
                gen(op_lnk,0,ast.get_line());
            }
            else
            {
                regist_string(ast.get_children()[1].get_str());
                gen(op_lnkc,string_table[ast.get_children()[1].get_str()],ast.get_line());
            }
            break;
        // ast_cmpeq(27)~ast_geq(32) op_eq(29)~op_geq(34)
        case ast_cmpeq:case ast_neq:
            calc_gen(ast.get_children()[0]);
            calc_gen(ast.get_children()[1]);
            gen(ast.get_type()-ast_cmpeq+op_eq,0,ast.get_line());
            break;
        case ast_less:case ast_leq:case ast_grt:case ast_geq:
            calc_gen(ast.get_children()[0]);
            if(ast.get_children()[1].get_type()!=ast_num)
            {
                calc_gen(ast.get_children()[1]);
                gen(ast.get_type()-ast_less+op_less,0,ast.get_line());
            }
            else
            {
                regist_number(ast.get_children()[1].get_num());
                gen(ast.get_type()-ast_less+op_lessc,number_table[ast.get_children()[1].get_num()],ast.get_line());
            }
            break;
        case ast_trino:trino_gen(ast);break;
        case ast_neg:
            calc_gen(ast.get_children()[0]);
            gen(op_usub,0,ast.get_line());
            break;
        case ast_not:
            calc_gen(ast.get_children()[0]);
            gen(op_unot,0,ast.get_line());
            break;
        case ast_def:
            single_def(ast);
            call_id(ast.get_children()[0]);
            break;
    }
    return;
}

void nasal_codegen::block_gen(const nasal_ast& ast)
{
    for(auto& tmp:ast.get_children())
        switch(tmp.get_type())
        {
            case ast_null:case ast_nil:case ast_num:case ast_str:case ast_func:break;
            case ast_def:def_gen(tmp);break;
            case ast_multi_assign:multi_assign_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_continue:
                continue_ptr.front().push_back(exec_code.size());
                gen(op_jmp,0,0);
                break;
            case ast_break:
                break_ptr.front().push_back(exec_code.size());
                gen(op_jmp,0,0);
                break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:loop_gen(tmp);break;
            case ast_equal:
                if(tmp.get_children()[0].get_type()==ast_id)
                {
                    calc_gen(tmp.get_children()[1]);
                    mcall_id(tmp.get_children()[0]);
                    // only the first mcall_id can use load
                    if(exec_code.back().op==op_mcalll)
                        exec_code.back().op=op_loadl;
                    else if(exec_code.back().op==op_mupval)
                        exec_code.back().op=op_loadu;
                    else
                        exec_code.back().op=op_loadg;
                }
                else
                {
                    calc_gen(tmp);
                    gen(op_pop,0,tmp.get_line());
                }
                break;
            case ast_id:
            case ast_vec:
            case ast_hash:
            case ast_call:
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
            case ast_trino:calc_gen(tmp);gen(op_pop,0,0);break;
            case ast_ret:ret_gen(tmp);break;
        }
    return;
}

void nasal_codegen::ret_gen(const nasal_ast& ast)
{
    for(uint32_t i=0;i<in_foreach;++i)
    {
        gen(op_pop,0,0);
        gen(op_cntpop,0,0);
    }
    for(uint32_t i=0;i<in_forindex;++i)
    {
        gen(op_pop,0,0);
        gen(op_cntpop,0,0);
    }
    if(ast.get_children().size())
        calc_gen(ast.get_children()[0]);
    else
        gen(op_pnil,0,ast.get_line());
    gen(op_ret,0,ast.get_line());
    return;
}

void nasal_codegen::main_progress(const nasal_ast& ast,const std::vector<std::string>& files)
{
    error=0;
    in_foreach=0;
    in_forindex=0;
    fileindex=0;

    number_table.clear();
    string_table.clear();
    num_res_table.clear();
    str_res_table.clear();
    file_name=files;
    exec_code.clear();

    global.clear();
    local.clear();

    // search symbols first
    find_symbol(ast);
    gen(op_intg,global.size(),0);
    for(auto& tmp:ast.get_children())
    {
        switch(tmp.get_type())
        {
            case ast_null:case ast_nil:case ast_num:case ast_str:case ast_func:break;
            case ast_file:fileindex=tmp.get_num();break;
            case ast_def:def_gen(tmp);break;
            case ast_multi_assign:multi_assign_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:loop_gen(tmp);break;
            case ast_equal:
                if(tmp.get_children()[0].get_type()==ast_id)
                {
                    calc_gen(tmp.get_children()[1]);
                    mcall_id(tmp.get_children()[0]);
                    // only the first mcall_id can use load
                    if(exec_code.back().op==op_mcalll)
                        exec_code.back().op=op_loadl;
                    else if(exec_code.back().op==op_mupval)
                        exec_code.back().op=op_loadu;
                    else
                        exec_code.back().op=op_loadg;
                }
                else
                {
                    calc_gen(tmp);
                    gen(op_pop,0,tmp.get_line());
                }
                break;
            case ast_id:
            case ast_vec:
            case ast_hash:
            case ast_call:
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
            case ast_trino:calc_gen(tmp);gen(op_pop,0,tmp.get_line());break;
        }
    }
    gen(op_nop,0,0);
    if(global.size()>=STACK_MAX_DEPTH)
        die("too many global variants: "+std::to_string(global.size())+".",0);
    return;
}

void nasal_codegen::print_op(int index)
{
    // print opcode index,opcode name,opcode immediate number
    printf("0x%.8x: %s 0x%.8x",index,code_table[exec_code[index].op].name,exec_code[index].num);
    // print detail info
    switch(exec_code[index].op)
    {
        case op_addc:case op_subc:case op_mulc:case op_divc:
        case op_addeqc:case op_subeqc:case op_muleqc:case op_diveqc:
        case op_lessc:case op_leqc:case op_grtc:case op_geqc:
        case op_pnum:printf(" (%lf)\n",num_res_table[exec_code[index].num]);break;
        case op_callb:printf(" (%s)\n",builtin_func[exec_code[index].num].name);break;
        case op_happ:
        case op_pstr:
        case op_lnkc:case op_lnkeqc:
        case op_callh:
        case op_mcallh:
        case op_para:
        case op_defpara:
        case op_dynpara:
            printf(" (");
            raw_string(str_res_table[exec_code[index].num]);
            printf(")\n");
            break;
        default:printf("\n");break;
    }
    return;
}

void nasal_codegen::print_byte_code()
{
    if(num_res_table.size())
        std::cout<<".number"<<std::endl;
    for(auto& num:num_res_table)
        std::cout<<'\t'<<num<<'\n';
    if(str_res_table.size())
        std::cout<<".symbol"<<std::endl;
    for(auto& str:str_res_table)
    {
        std::cout<<'\t';
        raw_string(str);
        std::cout<<std::endl;
    }
    for(int i=0;i<exec_code.size();++i)
        print_op(i);
    return;
}

#endif