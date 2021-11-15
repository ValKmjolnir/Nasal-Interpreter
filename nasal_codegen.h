#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

enum op_code
{
    op_nop,     // do nothing
    op_intg,    // global scope size
    op_intl,    // local scope size
    op_loadg,   // load global value
    op_loadl,   // load local value
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
    op_callg,   // get value in global scope
    op_calll,   // get value in local scope
    op_upval,   // get upvalue in closure
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
    op_ret,     // return
    op_exit     // stop the virtual machine
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
    {op_lnk,     "lnk   "},
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
    {op_exit,    "exit  "},
    {-1,         nullptr },
};

struct opcode
{
    uint16_t op;  // opcode
    uint16_t fidx;// source code file index
    uint32_t num; // imm num
    uint32_t line;// line of source code
    opcode(uint8_t o=op_nop,uint16_t f=0,uint32_t n=0,uint32_t l=0):
        op(o),fidx(f),num(n),line(l){}
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
    const std::string* file;
    std::unordered_map<double,int>      num_table;
    std::unordered_map<std::string,int> str_table;
    std::vector<double>                 num_res;
    std::vector<std::string>            str_res;
    std::vector<opcode>                 code;
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
    uint32_t                  err(){return error;}
    void                      compile(const nasal_parse&,const nasal_import&);
    void                      print_op(uint32_t);
    void                      print();
    const std::vector<std::string>& get_strs() const {return str_res;}
    const std::vector<double>&      get_nums() const {return num_res;}
    const std::vector<opcode>&      get_code() const {return code;}
};

void nasal_codegen::die(const std::string info,const int line)
{
    ++error;
    std::cout<<"[code] <"<<file[fileindex]<<"> line "<<line<<": "<<info<<"\n";
}

void nasal_codegen::regist_number(const double num)
{
    int size=num_table.size();
    if(!num_table.count(num))
    {
        num_table[num]=size;
        num_res.push_back(num);
    }
}

void nasal_codegen::regist_string(const std::string& str)
{
    int size=str_table.size();
    if(!str_table.count(str))
    {
        str_table[str]=size;
        str_res.push_back(str);
    }
}

void nasal_codegen::find_symbol(const nasal_ast& node)
{
    // symbol definition checked here
    // if find a function, return
    if(node.type()==ast_func)
        return;
    // find definition, check
    else if(node.type()==ast_def)
    {
        if(node[0].type()==ast_multi_id)
            for(auto& i:node[0].child())
                add_sym(i.str());
        else
            add_sym(node[0].str());
        find_symbol(node[1]);
    }
    // find iterator(foreach, forindex), check
    else if(node.type()==ast_new_iter)
        add_sym(node[0].str());
    // check children
    else
        for(auto& i:node.child())
            find_symbol(i);
}

void nasal_codegen::add_sym(const std::string& name)
{
    if(local.empty())
    {
        for(auto& sym:global)
            if(sym==name)
                return;
        global.push_back(name);
        return;
    }
    for(auto& sym:local.back())
        if(sym==name)
            return;
    local.back().push_back(name);
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
    for(uint32_t i=0;i<size-1;++i,++iter)
        for(uint32_t j=0;j<iter->size();++j)
            if((*iter)[j]==name)
                index=((i<<16)|j);
    return index;
}

void nasal_codegen::gen(uint8_t op,uint32_t num,uint32_t line)
{
    code.push_back({op,fileindex,num,line});
}

void nasal_codegen::num_gen(const nasal_ast& ast)
{
    double num=ast.num();
    if(num==0)gen(op_pzero,0,ast.line());
    else if(num==1)gen(op_pone,0,ast.line());
    else
    {
        regist_number(num);
        gen(op_pnum,num_table[num],ast.line());
    }
}

void nasal_codegen::str_gen(const nasal_ast& ast)
{
    regist_string(ast.str());
    gen(op_pstr,str_table[ast.str()],ast.line());
}

void nasal_codegen::vec_gen(const nasal_ast& ast)
{
    for(auto& node:ast.child())
        calc_gen(node);
    gen(op_newv,ast.size(),ast.line());
}

void nasal_codegen::hash_gen(const nasal_ast& ast)
{
    gen(op_newh,0,ast.line());
    for(auto& node:ast.child())
    {
        calc_gen(node[1]);
        const std::string& str=node[0].str();
        regist_string(str);
        gen(op_happ,str_table[str],node.line());
    }
}

void nasal_codegen::func_gen(const nasal_ast& ast)
{
    int newfunc_label=code.size();
    gen(op_newf,0,ast.line());
    int local_label=code.size();
    gen(op_intl,0,ast.line());
    
    // add special keyword 'me' into symbol table
    // this symbol is only used in local scope(function's scope)
    // this keyword is set to nil as default value
    // after calling a hash, this keyword is set to this hash
    // this symbol's index will be 0
    local.push_back({"me"});

    // generate parameter list
    for(auto& tmp:ast[0].child())
    {
        const std::string& str=tmp.str();
        regist_string(str);
        switch(tmp.type())
        {
            case ast_id:gen(op_para,str_table[str],tmp.line());break;
            case ast_default:
                calc_gen(tmp[0]);
                gen(op_defpara,str_table[str],tmp.line());
                break;
            case ast_dynamic:gen(op_dynpara,str_table[str],tmp.line());break;
        }
        add_sym(str);
    }

    code[newfunc_label].num=code.size()+1;
    int jmp_ptr=code.size();
    gen(op_jmp,0,0);

    const nasal_ast& block=ast[1];
    // search symbols first, must use after loading parameters
    // or the location of symbols will change and cause fatal error
    find_symbol(block);
    block_gen(block);
    code[local_label].num=local.back().size();
    if(local.back().size()>65536)
        die("too many local variants: "+std::to_string(local.back().size())+".",block.line());
    local.pop_back();

    if(!block.size() || block.child().back().type()!=ast_ret)
    {
        gen(op_pnil,0,block.line());
        gen(op_ret,0,block.line());
    }
    code[jmp_ptr].num=code.size();
}

void nasal_codegen::call_gen(const nasal_ast& ast)
{
    calc_gen(ast[0]);
    if(code.empty())
        std::cout<<"warning\n";
    if(code.back().op==op_callb)
        return;
    int child_size=ast.size();
    for(int i=1;i<child_size;++i)
    {
        const nasal_ast& tmp=ast[i];
        switch(tmp.type())
        {
            case ast_callh:call_hash(tmp);break;
            case ast_callv:call_vec(tmp); break;
            case ast_callf:call_func(tmp);break;
        }
    }
}

void nasal_codegen::call_id(const nasal_ast& ast)
{
    const std::string& str=ast.str();
    for(uint32_t i=0;builtin[i].name;++i)
        if(builtin[i].name==str)
        {
            gen(op_callb,i,ast.line());
            if(local.empty())
                die("builtin functions work in a local scope.",ast.line());
            return;
        }
    int index;
    if((index=local_find(str))>=0)
    {
        gen(op_calll,index,ast.line());
        return;
    }
    if((index=upvalue_find(str))>=0)
    {
        gen(op_upval,index,ast.line());
        return;
    }
    if((index=global_find(str))>=0)
    {
        gen(op_callg,index,ast.line());
        return;
    }
    die("undefined symbol \""+str+"\".",ast.line());
}

void nasal_codegen::call_hash(const nasal_ast& ast)
{
    regist_string(ast.str());
    gen(op_callh,str_table[ast.str()],ast.line());
}

void nasal_codegen::call_vec(const nasal_ast& ast)
{
    // maybe this place can use callv-const if ast's first child is ast_num
    if(ast.size()==1 && ast[0].type()!=ast_subvec)
    {
        calc_gen(ast[0]);
        gen(op_callv,0,ast[0].line());
        return;
    }
    gen(op_slcbegin,0,ast.line());
    for(auto& tmp:ast.child())
    {
        if(tmp.type()!=ast_subvec)
        {
            calc_gen(tmp);
            gen(op_slc,0,tmp.line());
        }
        else
        {
            calc_gen(tmp[0]);
            calc_gen(tmp[1]);
            gen(op_slc2,0,tmp.line());
        }
    }
    gen(op_slcend,0,ast.line());
}

void nasal_codegen::call_func(const nasal_ast& ast)
{
    if(!ast.size())
        gen(op_callfv,0,ast.line());
    else if(ast[0].type()==ast_hashmember)
    {
        hash_gen(ast);
        gen(op_callfh,0,ast.line());
    }
    else
    {
        for(auto& node:ast.child())
            calc_gen(node);
        gen(op_callfv,ast.size(),ast.line());
    }
}

void nasal_codegen::mcall(const nasal_ast& ast)
{
    if(ast.type()==ast_id)
    {
        mcall_id(ast);
        return;
    }
    calc_gen(ast[0]);
    for(int i=1;i<ast.size()-1;++i)
    {
        const nasal_ast& tmp=ast[i];
        switch(tmp.type())
        {
            case ast_callh:call_hash(tmp);break;
            case ast_callv:call_vec(tmp); break;
            case ast_callf:call_func(tmp);break;
        }
    }
    const nasal_ast& tmp=ast.child().back();
    if(tmp.type()==ast_callh)
        mcall_hash(tmp);
    else if(tmp.type()==ast_callv)
        mcall_vec(tmp);
}

void nasal_codegen::mcall_id(const nasal_ast& ast)
{
    const std::string& str=ast.str();
    for(uint32_t i=0;builtin[i].name;++i)
        if(builtin[i].name==str)
        {
            die("cannot change builtin function.",ast.line());
            return;
        }
    int index;
    if((index=local_find(str))>=0)
    {
        gen(op_mcalll,index,ast.line());
        return;
    }
    if((index=upvalue_find(str))>=0)
    {
        gen(op_mupval,index,ast.line());
        return;
    }
    if((index=global_find(str))>=0)
    {
        gen(op_mcallg,index,ast.line());
        return;
    }
    die("undefined symbol \""+str+"\".",ast.line());
}

void nasal_codegen::mcall_vec(const nasal_ast& ast)
{
    calc_gen(ast[0]);
    gen(op_mcallv,0,ast.line());
}

void nasal_codegen::mcall_hash(const nasal_ast& ast)
{
    regist_string(ast.str());
    gen(op_mcallh,str_table[ast.str()],ast.line());
}

void nasal_codegen::single_def(const nasal_ast& ast)
{
    const std::string& str=ast[0].str();
    calc_gen(ast[1]);
    local.empty()?
        gen(op_loadg,global_find(str),ast.line()):
        gen(op_loadl,local_find(str),ast.line());
}
void nasal_codegen::multi_def(const nasal_ast& ast)
{
    auto& ids=ast[0].child();
    int size=ids.size();
    if(ast[1].type()==ast_multi_scalar)
    {
        auto& vals=ast[1].child();
        for(int i=0;i<size;++i)
        {
            calc_gen(vals[i]);
            const std::string& str=ids[i].str();
            local.empty()?
                gen(op_loadg,global_find(str),ids[i].line()):
                gen(op_loadl,local_find(str),ids[i].line());
        }
    }
    else
    {
        calc_gen(ast[1]);
        for(int i=0;i<size;++i)
        {
            gen(op_callvi,i,ast[1].line());
            const std::string& str=ids[i].str();
            local.empty()?
                gen(op_loadg,global_find(str),ids[i].line()):
                gen(op_loadl,local_find(str),ids[i].line());
        }
        gen(op_pop,0,ast.line());
    }
}

void nasal_codegen::def_gen(const nasal_ast& ast)
{
    ast[0].type()==ast_id?
        single_def(ast):
        multi_def(ast);
}

void nasal_codegen::multi_assign_gen(const nasal_ast& ast)
{
    int size=ast[0].size();
    if(ast[1].type()==ast_multi_scalar)
    {
        for(int i=size-1;i>=0;--i)
            calc_gen(ast[1][i]);
        for(int i=0;i<size;++i)
        {
            mcall(ast[0][i]);
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            if(code.back().op==op_mcalll)
                code.back().op=op_loadl;
            else if(code.back().op==op_mupval)
                code.back().op=op_loadu;
            else if(code.back().op==op_mcallg)
                code.back().op=op_loadg;
            else
            {
                gen(op_meq,0,ast[0][i].line());
                gen(op_pop,0,ast[0][i].line());
            }
        }
    }
    else
    {
        calc_gen(ast[1]);
        for(int i=0;i<size;++i)
        {
            gen(op_callvi,i,ast[1].line());
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            mcall(ast[0][i]);
            if(code.back().op==op_mcalll)
                code.back().op=op_loadl;
            else if(code.back().op==op_mupval)
                code.back().op=op_loadu;
            else if(code.back().op==op_mcallg)
                code.back().op=op_loadg;
            else
            {
                gen(op_meq,0,ast[0][i].line());
                gen(op_pop,0,ast[0][i].line());
            }
        }
        gen(op_pop,0,ast.line());
    }
}

void nasal_codegen::conditional_gen(const nasal_ast& ast)
{
    std::vector<int> jmp_label;
    for(auto& tmp:ast.child())
    {
        if(tmp.type()==ast_if || tmp.type()==ast_elsif)
        {
            calc_gen(tmp[0]);
            int ptr=code.size();
            gen(op_jf,0,0);
            block_gen(tmp[1]);
            // without 'else' the last condition doesn't need to jmp
            if(&tmp!=&ast.child().back())
            {
                jmp_label.push_back(code.size());
                gen(op_jmp,0,0);
            }
            code[ptr].num=code.size();
        }
        else
        {
            block_gen(tmp[0]);
            break;
        }
    }
    for(auto i:jmp_label)
        code[i].num=code.size();
}

void nasal_codegen::loop_gen(const nasal_ast& ast)
{
    continue_ptr.push_front(std::vector<int>());
    break_ptr.push_front(std::vector<int>());
    switch(ast.type())
    {
        case ast_while:   while_gen(ast);   break;
        case ast_for:     for_gen(ast);     break;
        case ast_forindex:forindex_gen(ast);break;
        case ast_foreach: foreach_gen(ast); break;
    }
}

void nasal_codegen::load_continue_break(int continue_place,int break_place)
{
    for(auto i:continue_ptr.front())
        code[i].num=continue_place;
    for(auto i:break_ptr.front())
        code[i].num=break_place;
    continue_ptr.pop_front();
    break_ptr.pop_front();
}

void nasal_codegen::while_gen(const nasal_ast& ast)
{
    int loop_ptr=code.size();
    calc_gen(ast[0]);
    int condition_ptr=code.size();
    gen(op_jf,0,0);

    block_gen(ast[1]);
    gen(op_jmp,loop_ptr,0);
    code[condition_ptr].num=code.size();
    load_continue_break(code.size()-1,code.size());
}

void nasal_codegen::for_gen(const nasal_ast& ast)
{
    switch(ast[0].type())
    {
        case ast_null:break;
        case ast_def:def_gen(ast[0]);break;
        case ast_multi_assign:multi_assign_gen(ast[0]);break;
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
            calc_gen(ast[0]);
            gen(op_pop,0,0);
            break;
    }
    int jmp_place=code.size();
    if(ast[1].type()==ast_null)
        gen(op_pone,0,ast[1].line());
    else
        calc_gen(ast[1]);
    int label_exit=code.size();
    gen(op_jf,0,0);

    block_gen(ast[3]);
    int continue_place=code.size();
    switch(ast[2].type())
    {
        case ast_null:break;
        case ast_def:def_gen(ast[2]);break;
        case ast_multi_assign:multi_assign_gen(ast[2]);break;
        case ast_nil:case ast_num:case ast_str:case ast_func:break;
        case ast_vec:case ast_hash:
        case ast_call:
        case ast_equal:case ast_addeq:
        case ast_subeq:case ast_multeq:
        case ast_diveq:case ast_lnkeq:
        case ast_neg:case ast_not:
        case ast_add:case ast_sub:case ast_mult:
        case ast_div:case ast_link:
        case ast_cmpeq:case ast_neq:case ast_leq:
        case ast_less:case ast_geq:case ast_grt:
        case ast_trino:
            calc_gen(ast[2]);
            gen(op_pop,0,0);
            break;
    }
    gen(op_jmp,jmp_place,0);
    code[label_exit].num=code.size();

    load_continue_break(continue_place,code.size());
}
void nasal_codegen::forindex_gen(const nasal_ast& ast)
{
    calc_gen(ast[1]);
    gen(op_cnt,0,ast[1].line());
    int ptr=code.size();
    gen(op_findex,0,ast.line());
    if(ast[0].type()==ast_new_iter)
    {
        const std::string& str=ast[0][0].str();
        local.empty()?
            gen(op_loadg,global_find(str),ast[0][0].line()):
            gen(op_loadl,local_find(str),ast[0][0].line());
    }
    else
    {
        mcall(ast[0]);
        gen(op_meq,0,ast[0].line());
        gen(op_pop,0,0);
    }
    ++in_forindex;
    block_gen(ast[2]);
    --in_forindex;
    gen(op_jmp,ptr,0);
    code[ptr].num=code.size();
    load_continue_break(code.size()-1,code.size());
    gen(op_pop,0,0);// pop vector
    gen(op_cntpop,0,0);
}
void nasal_codegen::foreach_gen(const nasal_ast& ast)
{
    calc_gen(ast[1]);
    gen(op_cnt,0,ast.line());
    int ptr=code.size();
    gen(op_feach,0,ast.line());
    if(ast[0].type()==ast_new_iter)
    {
        const std::string& str=ast[0][0].str();
        local.empty()?
            gen(op_loadg,global_find(str),ast[0][0].line()):
            gen(op_loadl,local_find(str),ast[0][0].line());
    }
    else
    {
        mcall(ast[0]);
        gen(op_meq,0,ast[0].line());
        gen(op_pop,0,0);
    }
    ++in_foreach;
    block_gen(ast[2]);
    --in_foreach;
    gen(op_jmp,ptr,0);
    code[ptr].num=code.size();
    load_continue_break(code.size()-1,code.size());
    gen(op_pop,0,0);// pop vector
    gen(op_cntpop,0,0);
}

void nasal_codegen::or_gen(const nasal_ast& ast)
{
    calc_gen(ast[0]);
    int l1=code.size();
    gen(op_jt,0,0);

    gen(op_pop,0,0);
    calc_gen(ast[1]);
    int l2=code.size();
    gen(op_jt,0,0);

    gen(op_pop,0,0);
    gen(op_pnil,0,0);

    code[l1].num=code[l2].num=code.size();
}

void nasal_codegen::and_gen(const nasal_ast& ast)
{
    calc_gen(ast[0]);
    gen(op_jt,code.size()+2,0);

    int lfalse=code.size();
    gen(op_jmp,0,0);
    gen(op_pop,0,0);// jt jumps here

    calc_gen(ast[1]);
    gen(op_jt,code.size()+3,0);

    code[lfalse].num=code.size();
    gen(op_pop,0,0);
    gen(op_pnil,0,0);
    //jt jumps here
}

void nasal_codegen::trino_gen(const nasal_ast& ast)
{
    calc_gen(ast[0]);
    int lfalse=code.size();
    gen(op_jf,0,0);
    calc_gen(ast[1]);
    int lexit=code.size();
    gen(op_jmp,0,0);
    code[lfalse].num=code.size();
    calc_gen(ast[2]);
    code[lexit].num=code.size();
}

void nasal_codegen::calc_gen(const nasal_ast& ast)
{
    switch(ast.type())
    {
        case ast_nil:  gen(op_pnil,0,ast.line());break;
        case ast_num:  num_gen(ast);  break;
        case ast_str:  str_gen(ast);  break;
        case ast_id:   call_id(ast);  break;
        case ast_vec:  vec_gen(ast);  break;
        case ast_hash: hash_gen(ast); break;
        case ast_func: func_gen(ast); break;
        case ast_call: call_gen(ast); break;
        case ast_equal:
            calc_gen(ast[1]);
            mcall(ast[0]);
            gen(op_meq,0,ast.line());
            break;
        // ast_addeq(22)~ast_lnkeq(26) op_addeq(23)~op_lnkeq(27)
        case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:
            if(ast[1].type()!=ast_num)
                calc_gen(ast[1]);
            mcall(ast[0]);
            if(ast[1].type()!=ast_num)
                gen(ast.type()-ast_addeq+op_addeq,0,ast.line());
            else
            {
                regist_number(ast[1].num());
                gen(ast.type()-ast_addeq+op_addeqc,num_table[ast[1].num()],ast.line());
            }
            break;
        case ast_lnkeq:
            if(ast[1].type()!=ast_str)
                calc_gen(ast[1]);
            else
                regist_string(ast[1].str());
            mcall(ast[0]);
            if(ast[1].type()!=ast_str)
                gen(op_lnkeq,0,ast.line());
            else
                gen(op_lnkeqc,str_table[ast[1].str()],ast.line());
            break;
        case ast_or:or_gen(ast);break;
        case ast_and:and_gen(ast);break;
        // ast_add(33)~ast_link(37) op_add(18)~op_lnk(22)
        case ast_add:case ast_sub:case ast_mult:case ast_div:
            calc_gen(ast[0]);
            if(ast[1].type()!=ast_num)
            {
                calc_gen(ast[1]);
                gen(ast.type()-ast_add+op_add,0,ast.line());
            }
            else
            {
                regist_number(ast[1].num());
                gen(ast.type()-ast_add+op_addc,num_table[ast[1].num()],ast.line());
            }
            break;
        case ast_link:
            calc_gen(ast[0]);
            if(ast[1].type()!=ast_str)
            {
                calc_gen(ast[1]);
                gen(op_lnk,0,ast.line());
            }
            else
            {
                regist_string(ast[1].str());
                gen(op_lnkc,str_table[ast[1].str()],ast.line());
            }
            break;
        // ast_cmpeq(27)~ast_geq(32) op_eq(29)~op_geq(34)
        case ast_cmpeq:case ast_neq:
            calc_gen(ast[0]);
            calc_gen(ast[1]);
            gen(ast.type()-ast_cmpeq+op_eq,0,ast.line());
            break;
        case ast_less:case ast_leq:case ast_grt:case ast_geq:
            calc_gen(ast[0]);
            if(ast[1].type()!=ast_num)
            {
                calc_gen(ast[1]);
                gen(ast.type()-ast_less+op_less,0,ast.line());
            }
            else
            {
                regist_number(ast[1].num());
                gen(ast.type()-ast_less+op_lessc,num_table[ast[1].num()],ast.line());
            }
            break;
        case ast_trino:trino_gen(ast);break;
        case ast_neg:
            calc_gen(ast[0]);
            gen(op_usub,0,ast.line());
            break;
        case ast_not:
            calc_gen(ast[0]);
            gen(op_unot,0,ast.line());
            break;
        case ast_def:
            single_def(ast);
            call_id(ast[0]);
            break;
    }
}

void nasal_codegen::block_gen(const nasal_ast& ast)
{
    for(auto& tmp:ast.child())
        switch(tmp.type())
        {
            case ast_null:case ast_nil:case ast_num:case ast_str:case ast_func:break;
            case ast_def:def_gen(tmp);break;
            case ast_multi_assign:multi_assign_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_continue:
                continue_ptr.front().push_back(code.size());
                gen(op_jmp,0,0);
                break;
            case ast_break:
                break_ptr.front().push_back(code.size());
                gen(op_jmp,0,0);
                break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:loop_gen(tmp);break;
            case ast_equal:
                if(tmp[0].type()==ast_id)
                {
                    calc_gen(tmp[1]);
                    mcall_id(tmp[0]);
                    // only the first mcall_id can use load
                    if(code.back().op==op_mcalll)
                        code.back().op=op_loadl;
                    else if(code.back().op==op_mupval)
                        code.back().op=op_loadu;
                    else
                        code.back().op=op_loadg;
                }
                else
                {
                    calc_gen(tmp);
                    gen(op_pop,0,tmp.line());
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
}

void nasal_codegen::ret_gen(const nasal_ast& ast)
{
    for(uint32_t i=0;i<in_foreach+in_forindex;++i)
    {
        gen(op_pop,0,0);
        gen(op_cntpop,0,0);
    }
    if(ast.size())
        calc_gen(ast[0]);
    else
        gen(op_pnil,0,ast.line());
    gen(op_ret,0,ast.line());
}

void nasal_codegen::compile(const nasal_parse& parse,const nasal_import& import)
{
    error=in_foreach=in_forindex=0;
    fileindex=0;
    file=import.get_file().data();

    // search symbols first
    find_symbol(parse.ast());
    gen(op_intg,global.size(),0);
    for(auto& tmp:parse.ast().child())
    {
        switch(tmp.type())
        {
            case ast_null:case ast_nil:case ast_num:case ast_str:case ast_func:break;
            case ast_file:fileindex=tmp.num();break;
            case ast_def:def_gen(tmp);break;
            case ast_multi_assign:multi_assign_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_while:
            case ast_for:
            case ast_forindex:
            case ast_foreach:loop_gen(tmp);break;
            case ast_equal:
                if(tmp[0].type()==ast_id)
                {
                    calc_gen(tmp[1]);
                    mcall_id(tmp[0]);
                    // only the first mcall_id can use load
                    if(code.back().op==op_mcalll)
                        code.back().op=op_loadl;
                    else if(code.back().op==op_mupval)
                        code.back().op=op_loadu;
                    else
                        code.back().op=op_loadg;
                }
                else
                {
                    calc_gen(tmp);
                    gen(op_pop,0,tmp.line());
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
            case ast_trino:calc_gen(tmp);gen(op_pop,0,tmp.line());break;
        }
    }
    gen(op_exit,0,0);
    if(global.size()>=STACK_MAX_DEPTH)
        die("too many global variants: "+std::to_string(global.size())+".",0);
}

void nasal_codegen::print_op(uint32_t index)
{
    // print opcode index,opcode name,opcode immediate number
    const opcode& c=code[index];
    printf("0x%.8x: %s ",index,code_table[c.op].name);
    // print detail info
    switch(c.op)
    {
        case op_addc:  case op_subc:   case op_mulc:  case op_divc:
        case op_addeqc:case op_subeqc: case op_muleqc:case op_diveqc:
        case op_lessc: case op_leqc:   case op_grtc:  case op_geqc:
        case op_pnum:
            printf("0x%x (",c.num);std::cout<<num_res[c.num]<<")\n";break;
        case op_callvi:case op_newv:   case op_callfv:
        case op_intg:  case op_intl:
        case op_newf:  case op_jmp:    case op_jt:    case op_jf:
            printf("0x%x\n",c.num);break;
        case op_callb:
            printf("0x%x <%s>\n",c.num,builtin[c.num].name);break;
        case op_callg: case op_mcallg: case op_loadg:
        case op_calll: case op_mcalll: case op_loadl:
            printf("0x%x\n",c.num);break;
        case op_upval:case op_mupval:  case op_loadu:
            printf("0x%x[0x%x]\n",(c.num>>16)&0xffff,c.num&0xffff);break;
        case op_happ:  case op_pstr:
        case op_lnkc:  case op_lnkeqc:
        case op_callh: case op_mcallh:
        case op_para:  case op_defpara:case op_dynpara:
            printf("0x%x (\"%s\")\n",c.num,rawstr(str_res[c.num]).c_str());break;
        default:printf("\n");break;
    }
}

void nasal_codegen::print()
{
    for(auto& num:num_res)
        std::cout<<".number "<<num<<'\n';
    for(auto& str:str_res)
        std::cout<<".symbol \""<<rawstr(str)<<"\"\n";
    for(uint32_t i=0;i<code.size();++i)
        print_op(i);
}

#endif