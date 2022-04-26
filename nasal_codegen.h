#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

enum op_code
{
    op_exit,    // stop the virtual machine
    op_intg,    // global scope size
    op_intl,    // local scope size
    op_loadg,   // load global value
    op_loadl,   // load local value
    op_loadu,   // load upvalue
    op_pnum,    // push constant number to the stack
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
    op_ret      // return
};

struct
{
    int type;
    const char* name;
}code_table[]=
{
    {op_exit,    "exit  "},
    {op_intg,    "intg  "},
    {op_intl,    "intl  "},
    {op_loadg,   "loadg "},
    {op_loadl,   "loadl "},
    {op_loadu,   "loadu "},
    {op_pnum,    "pnum  "},
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
    uint8_t op;  // opcode
    uint16_t fidx;// source code file index
    uint32_t num; // imm num
    uint32_t line;// line of source code
    opcode(uint8_t o=op_exit,uint16_t f=0,uint32_t n=0,uint32_t l=0):
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
    uint16_t fileindex;
    nasal_err& nerr;
    const std::string* file;
    std::stack<uint32_t> in_iterloop;
    std::unordered_map<double,int>      num_table;
    std::unordered_map<std::string,int> str_table;
    std::vector<double>                 num_res;
    std::vector<std::string>            str_res;
    std::vector<opcode>                 code;
    std::list<std::vector<int>>         continue_ptr;
    std::list<std::vector<int>>         break_ptr;
    // global : max 4095 values
    std::unordered_map<std::string,int> global;
    // local  : max 32768 upvalues 65536 values
    std::list<std::unordered_map<std::string,int>> local;

    // func end stack, reserved for code print
    std::stack<uint32_t> fbstk;
    std::stack<uint32_t> festk;
    
    void die(std::string,const uint32_t);
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
    nasal_codegen(nasal_err& e):fileindex(0),nerr(e),file(nullptr){}
    void compile(const nasal_parse&,const nasal_import&);
    void print_op(uint32_t);
    void print();
    const std::vector<std::string>& get_strs() const {return str_res;}
    const std::vector<double>&      get_nums() const {return num_res;}
    const std::vector<opcode>&      get_code() const {return code;}
};

void nasal_codegen::die(std::string info,const uint32_t line)
{
    nerr.load(file[fileindex]);
    nerr.err("code",line,info);
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
    else if(node.type()==ast_iter)
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
        if(global.count(name))
            return;
        int index=global.size();
        global[name]=index;
        return;
    }
    if(local.back().count(name))
        return;
    int index=local.back().size();
    local.back()[name]=index;
}

int nasal_codegen::local_find(const std::string& name)
{
    if(local.empty())
        return -1;
    return local.back().count(name)?local.back()[name]:-1;
}

int nasal_codegen::global_find(const std::string& name)
{
    return global.count(name)?global[name]:-1;
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
        if(iter->count(name))
            index=((i<<16)|(*iter)[name]);
    return index;
}

void nasal_codegen::gen(uint8_t op,uint32_t num,uint32_t line)
{
    code.push_back({op,fileindex,num,line});
}

void nasal_codegen::num_gen(const nasal_ast& ast)
{
    double num=ast.num();
    regist_number(num);
    gen(op_pnum,num_table[num],ast.line());
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
    local.push_back({{"me",0}});

    // generate parameter list
    for(auto& tmp:ast[0].child())
    {
        const std::string& str=tmp.str();
        if(str=="me")
            die("\"me\" should not be a parameter",tmp.line());
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
    gen(op_jmp,0,ast.line());

    const nasal_ast& block=ast[1];
    // search symbols first, must use after loading parameters
    // or the location of symbols will change and cause fatal error
    find_symbol(block);
    in_iterloop.push(0);
    block_gen(block);
    in_iterloop.pop();
    code[local_label].num=local.back().size();
    if(local.back().size()>=nasal_gc::stack_depth)
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
    else if(ast[0].type()==ast_pair)
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
    if(ast.size()==1) // foreach and forindex use call-id ast to get mcall
    {
        mcall_id(ast[0]);
        return;
    }
    calc_gen(ast[0]);
    for(size_t i=1;i<ast.size()-1;++i)
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
    if(ast[1].type()==ast_multi_scalar) // (var a,b,c)=(c,b,a);
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
    else // (var a,b,c)=[0,1,2];
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
                gen(op_meq,1,ast[0][i].line());
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
                gen(op_meq,1,ast[0][i].line());
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
            gen(op_jf,0,tmp.line());
            block_gen(tmp[1]);
            // without 'else' the last condition doesn't need to jmp
            if(&tmp!=&ast.child().back())
            {
                jmp_label.push_back(code.size());
                gen(op_jmp,0,tmp.line());
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
    gen(op_jf,0,ast[0].line());

    block_gen(ast[1]);
    gen(op_jmp,loop_ptr,ast[1].line());
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
        case ast_addeq:case ast_subeq:
        case ast_multeq:case ast_diveq:case ast_lnkeq:
            calc_gen(ast[0]);
            if(op_addeq<=code.back().op && code.back().op<=op_lnkeq)
                code.back().num=1;
            else if(op_addeqc<=code.back().op && code.back().op<=op_lnkeqc)
                code.back().num|=0x80000000;
            else
                gen(op_pop,0,ast[0].line());
            break;
        case ast_nil:case ast_num:case ast_str:break;
        case ast_vec:case ast_hash:case ast_func:
        case ast_call:
        case ast_neg:case ast_not:
        case ast_add:case ast_sub:
        case ast_mult:case ast_div:
        case ast_link:
        case ast_cmpeq:case ast_neq:
        case ast_leq:case ast_less:
        case ast_geq:case ast_grt:
        case ast_trino:
            calc_gen(ast[0]);
            if(code.back().op==op_meq)
                code.back().num=1;
            else
                gen(op_pop,0,ast[0].line());
            break;
        case ast_equal:
            if(ast[0][0].type()==ast_id)
            {
                calc_gen(ast[0][1]);
                mcall_id(ast[0][0]);
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
                calc_gen(ast[0]);
                if(code.back().op==op_meq)
                    code.back().num=1;
                else
                    gen(op_pop,0,ast[0].line());
            }
            break;
    }
    int jmp_place=code.size();
    if(ast[1].type()==ast_null)
        gen(op_pnum,num_table[1],ast[1].line());
    else
        calc_gen(ast[1]);
    int label_exit=code.size();
    gen(op_jf,0,ast[1].line());

    block_gen(ast[3]);
    int continue_place=code.size();
    switch(ast[2].type())
    {
        case ast_null:break;
        case ast_def:def_gen(ast[2]);break;
        case ast_multi_assign:multi_assign_gen(ast[2]);break;
        case ast_addeq:case ast_subeq:
        case ast_multeq:case ast_diveq:case ast_lnkeq:
            calc_gen(ast[2]);
            if(op_addeq<=code.back().op && code.back().op<=op_lnkeq)
                code.back().num=1;
            else if(op_addeqc<=code.back().op && code.back().op<=op_lnkeqc)
                code.back().num|=0x80000000;
            else
                gen(op_pop,0,ast[2].line());
            break;
        case ast_nil:case ast_num:case ast_str:break;
        case ast_vec:case ast_hash:case ast_func:
        case ast_call:
        case ast_neg:case ast_not:
        case ast_add:case ast_sub:case ast_mult:
        case ast_div:case ast_link:
        case ast_cmpeq:case ast_neq:case ast_leq:
        case ast_less:case ast_geq:case ast_grt:
        case ast_trino:
            calc_gen(ast[2]);
            if(code.back().op==op_meq)
                code.back().num=1;
            else
                gen(op_pop,0,ast[2].line());
            break;
        case ast_equal:
            if(ast[2][0].type()==ast_id)
            {
                calc_gen(ast[2][1]);
                mcall_id(ast[2][0]);
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
                calc_gen(ast[2]);
                if(code.back().op==op_meq)
                    code.back().num=1;
                else
                    gen(op_pop,0,ast[2].line());
            }
            break;
    }
    gen(op_jmp,jmp_place,ast[2].line());
    code[label_exit].num=code.size();

    load_continue_break(continue_place,code.size());
}
void nasal_codegen::forindex_gen(const nasal_ast& ast)
{
    calc_gen(ast[1]);
    gen(op_cnt,0,ast[1].line());
    int ptr=code.size();
    gen(op_findex,0,ast.line());
    if(ast[0].type()==ast_iter) // define a new iterator
    {
        const std::string& str=ast[0][0].str();
        local.empty()?
            gen(op_loadg,global_find(str),ast[0][0].line()):
            gen(op_loadl,local_find(str),ast[0][0].line());
    }
    else // use exist variable as the iterator
    {
        mcall(ast[0]);
        if(code.back().op==op_mcallg)
            code.back().op=op_loadg;
        else if(code.back().op==op_mcalll)
            code.back().op=op_loadl;
        else if(code.back().op==op_mupval)
            code.back().op=op_loadu;
        else
            gen(op_meq,1,ast[0].line());
    }
    ++in_iterloop.top();
    block_gen(ast[2]);
    --in_iterloop.top();
    gen(op_jmp,ptr,ast.line());
    code[ptr].num=code.size();
    load_continue_break(code.size()-1,code.size());
    gen(op_pop,0,ast[1].line());// pop vector
    gen(op_pop,0,ast.line());// pop iterator
}
void nasal_codegen::foreach_gen(const nasal_ast& ast)
{
    calc_gen(ast[1]);
    gen(op_cnt,0,ast.line());
    int ptr=code.size();
    gen(op_feach,0,ast.line());
    if(ast[0].type()==ast_iter) // define a new iterator
    {
        const std::string& str=ast[0][0].str();
        local.empty()?
            gen(op_loadg,global_find(str),ast[0][0].line()):
            gen(op_loadl,local_find(str),ast[0][0].line());
    }
    else // use exist variable as the iterator
    {
        mcall(ast[0]);
        if(code.back().op==op_mcallg)
            code.back().op=op_loadg;
        else if(code.back().op==op_mcalll)
            code.back().op=op_loadl;
        else if(code.back().op==op_mupval)
            code.back().op=op_loadu;
        else
            gen(op_meq,1,ast[0].line());
    }
    ++in_iterloop.top();
    block_gen(ast[2]);
    --in_iterloop.top();
    gen(op_jmp,ptr,ast.line());
    code[ptr].num=code.size();
    load_continue_break(code.size()-1,code.size());
    gen(op_pop,0,ast[1].line());// pop vector
    gen(op_pop,0,ast.line());// pop iterator
}

void nasal_codegen::or_gen(const nasal_ast& ast)
{
    calc_gen(ast[0]);
    int l1=code.size();
    gen(op_jt,0,ast[0].line());

    gen(op_pop,0,ast[0].line());
    calc_gen(ast[1]);
    int l2=code.size();
    gen(op_jt,0,ast[1].line());

    gen(op_pop,0,ast[1].line());
    gen(op_pnil,0,ast[1].line());

    code[l1].num=code[l2].num=code.size();
}

void nasal_codegen::and_gen(const nasal_ast& ast)
{
    calc_gen(ast[0]);
    gen(op_jt,code.size()+2,ast[0].line());

    int lfalse=code.size();
    gen(op_jmp,0,ast[0].line());
    gen(op_pop,0,ast[1].line());// jt jumps here

    calc_gen(ast[1]);
    gen(op_jt,code.size()+3,ast[1].line());

    code[lfalse].num=code.size();
    gen(op_pop,0,ast[1].line());
    gen(op_pnil,0,ast[1].line());
    //jt jumps here
}

void nasal_codegen::trino_gen(const nasal_ast& ast)
{
    calc_gen(ast[0]);
    int lfalse=code.size();
    gen(op_jf,0,ast[0].line());
    calc_gen(ast[1]);
    int lexit=code.size();
    gen(op_jmp,0,ast[1].line());
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
            case ast_null:case ast_nil:case ast_num:case ast_str:break;
            case ast_file:fileindex=tmp.num();break; // special node type in main block
            case ast_def:def_gen(tmp);break;
            case ast_multi_assign:multi_assign_gen(tmp);break;
            case ast_conditional:conditional_gen(tmp);break;
            case ast_continue:
                continue_ptr.front().push_back(code.size());
                gen(op_jmp,0,tmp.line());
                break;
            case ast_break:
                break_ptr.front().push_back(code.size());
                gen(op_jmp,0,tmp.line());
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
                    if(code.back().op==op_meq)
                        code.back().num=1;
                    else
                        gen(op_pop,0,tmp.line());
                }
                break;
            case ast_addeq:case ast_subeq:
            case ast_multeq:case ast_diveq:case ast_lnkeq:
                calc_gen(tmp);
                if(op_addeq<=code.back().op && code.back().op<=op_lnkeq)
                    code.back().num=1;
                else if(op_addeqc<=code.back().op && code.back().op<=op_lnkeqc)
                    code.back().num|=0x80000000;
                else
                    gen(op_pop,0,tmp.line());
                break;
            case ast_id:
            case ast_vec:
            case ast_hash:
            case ast_func:
            case ast_call:
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
            case ast_trino:
                calc_gen(tmp);
                if(code.back().op==op_meq)
                    code.back().num=1;
                else
                    gen(op_pop,0,tmp.line());
                break;
            case ast_ret:ret_gen(tmp);break;
        }
}

void nasal_codegen::ret_gen(const nasal_ast& ast)
{
    for(uint32_t i=0;i<in_iterloop.top();++i)
    {
        gen(op_pop,0,ast.line());
        gen(op_pop,0,ast.line());
    }
    if(ast.size())
        calc_gen(ast[0]);
    else
        gen(op_pnil,0,ast.line());
    gen(op_ret,0,ast.line());
}

void nasal_codegen::compile(const nasal_parse& parse,const nasal_import& import)
{
    fileindex=0;
    file=import.get_file().data();
    in_iterloop.push(0);

    regist_number(0);
    regist_number(1);

    find_symbol(parse.ast()); // search symbols first
    gen(op_intg,global.size(),0);
    block_gen(parse.ast()); // generate main block
    gen(op_exit,0,0);
    if(global.size()>=nasal_gc::stack_depth)
        die("too many global variants: "+std::to_string(global.size())+".",0);
    nerr.chkerr();
}

void nasal_codegen::print_op(uint32_t index)
{
    // print opcode index,opcode name,opcode immediate number
    const opcode& c=code[index];
    if(!festk.empty() && index==festk.top())
    {
        printf("<0x%x>;\n",fbstk.top());
        if(code[index].op!=op_newf) // avoid two empty lines
            printf("\n");
        fbstk.pop();
        festk.pop();
    }
    if(c.op==op_newf)
    {
        printf("\nfunc <0x%x>:\n",index);
        for(uint32_t i=index;i<code.size();++i)
            if(code[i].op==op_jmp)
            {
                fbstk.push(index);
                festk.push(code[i].num);
                break;
            }
    }
    printf("  0x%.8x:       %.2x %.2x %.2x %.2x %.2x        %s  ",
        index,
        c.op,
        uint8_t((c.num>>24)&0xff),
        uint8_t((c.num>>16)&0xff),
        uint8_t((c.num>>8)&0xff),
        uint8_t(c.num&0xff),
        code_table[c.op].name
    );
    // print detail info
    switch(c.op)
    {
        case op_addeq: case op_subeq:  case op_muleq: case op_diveq:
        case op_lnkeq: case op_meq:
            printf("0x%x sp-%u\n",c.num,c.num);break;
        case op_addeqc:case op_subeqc: case op_muleqc:case op_diveqc:
            printf("0x%x (",c.num&0x7fffffff);
            std::cout<<num_res[c.num&0x7fffffff]<<") sp-"<<(c.num>>31)<<"\n";
            break;
        case op_lnkeqc:
            printf("0x%x (\"%s\") sp-%u\n",c.num&0x7fffffff,rawstr(str_res[c.num&0x7fffffff]).c_str(),c.num>>31);break;
        case op_addc:  case op_subc:   case op_mulc:  case op_divc:
        case op_lessc: case op_leqc:   case op_grtc:  case op_geqc:
        case op_pnum:
            printf("0x%x (",c.num);std::cout<<num_res[c.num]<<")\n";break;
        case op_callvi:case op_newv:   case op_callfv:
        case op_intg:  case op_intl:
        case op_newf:  case op_jmp:    case op_jt:    case op_jf:
        case op_callg: case op_mcallg: case op_loadg:
        case op_calll: case op_mcalll: case op_loadl:
            printf("0x%x\n",c.num);break;
        case op_callb:
            printf("0x%x <%s@0x%lx>\n",c.num,builtin[c.num].name,(uint64_t)builtin[c.num].func);break;
        case op_upval:case op_mupval:  case op_loadu:
            printf("0x%x[0x%x]\n",(c.num>>16)&0xffff,c.num&0xffff);break;
        case op_happ:  case op_pstr:
        case op_lnkc:
        case op_callh: case op_mcallh:
        case op_para:  case op_defpara:case op_dynpara:
            printf("0x%x (\"%s\")\n",c.num,rawstr(str_res[c.num]).c_str());break;
        default:printf("\n");break;
    }
}

void nasal_codegen::print()
{
    for(auto& num:num_res)
        std::cout<<"  .number "<<num<<'\n';
    for(auto& str:str_res)
        std::cout<<"  .symbol \""<<rawstr(str)<<"\"\n";
    std::cout<<"\n";
    for(uint32_t i=0;i<code.size();++i)
        print_op(i);
}

#endif