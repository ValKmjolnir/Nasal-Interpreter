#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

enum op_code
{
    op_nop,
    op_intg,
    op_intl,
    op_loadg,
    op_loadl,
    op_pnum,
    op_pone,
    op_pzero,
    op_pnil,
    op_pstr,
    op_newv,
    op_newh,
    op_newf,
    op_vapp,
    op_happ,
    op_para,
    op_defpara,
    op_dynpara,
    op_unot,
    op_usub,
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_lnk,
    op_addeq,
    op_subeq,
    op_muleq,
    op_diveq,
    op_lnkeq,
    op_meq,
    op_eq,
    op_neq,
    op_less,
    op_leq,
    op_grt,
    op_geq,
    op_pop,
    op_jmp,      // jump with no condition
    op_jt,       // used in operator and/or,jmp when condition is true and DO NOT POP
    op_jf,       // used in conditional/loop,jmp when condition is false and POP STACK
    op_cnt,      // add counter for forindex/foreach
    op_cntpop,   // pop counter
    op_findex,   // index counter on the top of forindex_stack plus 1
    op_feach,    // index counter on the top of forindex_stack plus 1 and get the value in vector
    op_callg,    // call value in global scope
    op_calll,    // call value in local scope
    op_callv,    // call vec[index]
    op_callvi,   // call vec[immediate] (used in multi-assign/multi-define)
    op_callh,    // call hash.label
    op_callf,    // call function(parameters)
    op_callb,    // call builtin-function
    op_slcbegin, // begin of slice like: vec[1,2,3:6,0,-1]
    op_slcend,   // end of slice
    op_slc,      // slice like vec[1]
    op_slc2,     // slice like vec[nil:10]
    op_mcallg,   // get memory space of value in global scope
    op_mcalll,   // get memory space of value in local scope
    op_mcallv,   // get memory space of vec[index]
    op_mcallh,   // get memory space of hash.label
    op_ret       // return
};

struct
{
    int type;
    const char* name;
}code_table[]=
{
    {op_nop,      "nop   "},
    {op_intg,     "intg  "},
    {op_intl,     "intl  "},
    {op_loadg,    "loadg "},
    {op_loadl,    "loadl "},
    {op_pnum,     "pnum  "},
    {op_pone,     "pone  "},
    {op_pzero,    "pzero "},
    {op_pnil,     "pnil  "},
    {op_pstr,     "pstr  "},
    {op_newv,     "newv  "},
    {op_newh,     "newh  "},
    {op_newf,     "newf  "},
    {op_vapp,     "vapp  "},
    {op_happ,     "happ  "},
    {op_para,     "para  "},
    {op_defpara,  "def   "},
    {op_dynpara,  "dyn   "},
    {op_unot,     "not   "},
    {op_usub,     "usub  "},
    {op_add,      "add   "},
    {op_sub,      "sub   "},
    {op_mul,      "mult  "},
    {op_div,      "div   "},
    {op_lnk,      "link  "},
    {op_addeq,    "addeq "},
    {op_subeq,    "subeq "},
    {op_muleq,    "muleq "},
    {op_diveq,    "diveq "},
    {op_lnkeq,    "lnkeq "},
    {op_meq,      "meq   "},
    {op_eq,       "eq    "},
    {op_neq,      "neq   "},
    {op_less,     "less  "},
    {op_leq,      "leq   "},
    {op_grt,      "grt   "},
    {op_geq,      "geq   "},
    {op_pop,      "pop   "},
    {op_jmp,      "jmp   "},
    {op_jt,       "jt    "},
    {op_jf,       "jf    "},
    {op_cnt,      "cnt   "},
    {op_cntpop,   "cntpop"},
    {op_findex,   "findx "},
    {op_feach,    "feach "},
    {op_callg,    "callg "},
    {op_calll,    "calll "},
    {op_callv,    "callv "},
    {op_callvi,   "callvi"},
    {op_callh,    "callh "},
    {op_callf,    "callf "},
    {op_callb,    "callb "},
    {op_slcbegin, "slcbeg"},
    {op_slcend,   "slcend"},
    {op_slc,      "slc   "},
    {op_slc2,     "slc2  "},
    {op_mcallg,   "mcallg"},
    {op_mcalll,   "mcalll"},
    {op_mcallv,   "mcallv"},
    {op_mcallh,   "mcallh"},
    {op_ret,      "ret   "},
    {-1,          NULL    },
};

struct opcode
{
    unsigned char op;
    unsigned int num;
    opcode(unsigned char _op=op_nop,unsigned int _num=0)
    {
        op=_op;
        num=_num;
        return;
    }
    opcode& operator=(const opcode& tmp)
    {
        op=tmp.op;
        num=tmp.num;
        return *this;
    }
};

class nasal_codegen
{
private:
    int error;
    int in_forindex;
    int in_foreach;
    std::unordered_map<double,int>       number_table;
    std::unordered_map<std::string,int>  string_table;
    std::vector<double>                  num_res_table;
    std::vector<std::string>             str_res_table;
    std::vector<opcode>                  exec_code;
    std::list<std::vector<int> >         continue_ptr;
    std::list<std::vector<int> >         break_ptr;
    std::vector<std::string>             global;
    std::list<std::vector<std::string> > local;
    
    void die(std::string,int);
    void regist_number(double);
    void regist_string(std::string&);
    void add_sym(std::string);
    int  local_find(std::string&);
    int  global_find(std::string&);
    void gen(unsigned char,unsigned int);
    void pop_gen();
    void nil_gen();
    void num_gen(nasal_ast&);
    void str_gen(nasal_ast&);
    void vec_gen(nasal_ast&);
    void hash_gen(nasal_ast&);
    void func_gen(nasal_ast&);
    void call_gen(nasal_ast&);
    void call_id(nasal_ast&);
    void call_hash(nasal_ast&);
    void call_vec(nasal_ast&);
    void call_func(nasal_ast&);
    void mcall(nasal_ast&);
    void mcall_id(nasal_ast&);
    void mcall_vec(nasal_ast&);
    void mcall_hash(nasal_ast&);
    void multi_def(nasal_ast&);
    void single_def(nasal_ast&);
    void def_gen(nasal_ast&);
    void multi_assign_gen(nasal_ast&);
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
    void calc_gen(nasal_ast&);
    void block_gen(nasal_ast&);
    void ret_gen(nasal_ast&);
public:
    nasal_codegen();
    int                       get_error();
    void                      main_progress(nasal_ast&);
    void                      print_op(int);
    void                      print_byte_code();
    std::vector<std::string>& get_str_table();
    std::vector<double>&      get_num_table();
    std::vector<opcode>&      get_exec_code();
};

nasal_codegen::nasal_codegen()
{
    error=0;
    in_foreach=0;
    in_forindex=0;
    return;
}

void nasal_codegen::die(std::string info,int line)
{
    ++error;
    std::cout<<">> [code] line "<<line<<": "<<info<<"\n";
    return;
}

void nasal_codegen::regist_number(double num)
{
    int size=number_table.size();
    if(!number_table.count(num))
        number_table[num]=size;
    return;
}

void nasal_codegen::regist_string(std::string& str)
{
    int size=string_table.size();
    if(!string_table.count(str))
        string_table[str]=size;
    return;
}

void nasal_codegen::add_sym(std::string name)
{
    if(local.empty())
    {
        for(int i=0;i<global.size();++i)
            if(global[i]==name)
                return;
        global.push_back(name);
    }
    else
    {
        for(int i=0;i<local.back().size();++i)
            if(local.back()[i]==name)
                return;
        local.back().push_back(name);
    }
    return;
}

int nasal_codegen::local_find(std::string& name)
{
    int index=-1;
    int cnt=0;
    for(auto i=local.begin();i!=local.end();++i)
    {
        for(int j=0;j<i->size();++j)
            if((*i)[j]==name)
                index=cnt+j;
        cnt+=i->size();
    }
    return index;
}

int nasal_codegen::global_find(std::string& name)
{
    for(int i=0;i<global.size();++i)
        if(global[i]==name)
            return i;
    return -1;
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
    opcode op(op_pnil,0);
    exec_code.push_back(op);
    return;
}

void nasal_codegen::num_gen(nasal_ast& ast)
{
    double num=ast.get_num();
    if(num==1)      gen(op_pone,0);
    else if(num==0) gen(op_pzero,0);
    else
    {
        regist_number(num);
        gen(op_pnum,number_table[num]);
    }
    return;
}

void nasal_codegen::str_gen(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    gen(op_pstr,string_table[str]);
    return;
}

void nasal_codegen::vec_gen(nasal_ast& ast)
{
    int size=ast.get_children().size();
    gen(op_newv,0);
    for(int i=0;i<size;++i)
        calc_gen(ast.get_children()[i]);
    gen(op_vapp,size);
    return;
}

void nasal_codegen::hash_gen(nasal_ast& ast)
{
    int size=ast.get_children().size();
    gen(op_newh,0);
    for(int i=0;i<size;++i)
    {
        calc_gen(ast.get_children()[i].get_children()[1]);
        std::string str=ast.get_children()[i].get_children()[0].get_str();
        regist_string(str);
        gen(op_happ,string_table[str]);
    }
    return;
}

void nasal_codegen::func_gen(nasal_ast& ast)
{
    int newfunc_label=exec_code.size();
    gen(op_newf,0);
    int local_label=exec_code.size();
    gen(op_intl,0);
    std::vector<std::string> new_scope;
    local.push_back(new_scope);

    // add special keyword 'me' into symbol table
    // this symbol is only used in local scope(function's scope)
    // this keyword is set to nil as default value
    // after calling a hash, this keyword is set to this hash
    // this symbol's index will be 0
    if(local.size()==1)
        add_sym("me");

    nasal_ast& ref_arg=ast.get_children()[0];
    int arg_size=ref_arg.get_children().size();
    for(int i=0;i<arg_size;++i)
    {
        nasal_ast& tmp=ref_arg.get_children()[i];
        if(tmp.get_type()==ast_id)
        {
            std::string str=tmp.get_str();
            regist_string(str);
            add_sym(str);
            gen(op_para,string_table[str]);
        }
        else if(tmp.get_type()==ast_default_arg)
        {
            calc_gen(tmp.get_children()[1]);
            std::string str=tmp.get_children()[0].get_str();
            regist_string(str);
            add_sym(str);
            gen(op_defpara,string_table[str]);
        }
        else if(tmp.get_type()==ast_dynamic_id)
        {
            std::string str=tmp.get_str();
            regist_string(str);
            add_sym(str);
            gen(op_dynpara,string_table[str]);
        }
    }
    exec_code[newfunc_label].num=exec_code.size()+1;
    int jmp_ptr=exec_code.size();
    gen(op_jmp,0);
    nasal_ast& block=ast.get_children()[1];

    block_gen(block);
    for(auto i=local.begin();i!=local.end();++i)
        exec_code[local_label].num+=i->size();
    local.pop_back();

    if(!block.get_children().size() || block.get_children().back().get_type()!=ast_return)
    {
        nil_gen();
        gen(op_ret,0);
    }
    exec_code[jmp_ptr].num=exec_code.size();
    return;
}

void nasal_codegen::call_gen(nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    if(exec_code.back().op==op_callb)
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
    for(int i=0;builtin_func[i].func;++i)
        if(builtin_func[i].name==str)
        {
            gen(op_callb,i);
            return;
        }
    int index=local_find(str);
    if(index>=0)
    {
        gen(op_calll,index);
        return;
    }
    index=global_find(str);
    if(index>=0)
    {
        gen(op_callg,index);
        return;
    }
    die("cannot find symbol named \""+str+"\".",ast.get_line());
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
        calc_gen(ast.get_children()[0]);
        gen(op_callv,0);
        return;
    }
    gen(op_slcbegin,0);
    int size=ast.get_children().size();
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()!=ast_subvec)
        {
            calc_gen(tmp);
            gen(op_slc,0);
        }
        else
        {
            calc_gen(tmp.get_children()[0]);
            calc_gen(tmp.get_children()[1]);
            gen(op_slc2,0);
        }
    }
    gen(op_slcend,0);
    return;
}

void nasal_codegen::call_func(nasal_ast& ast)
{
    if(!ast.get_children().size())
        gen(op_newv,0);
    else if(ast.get_children()[0].get_type()==ast_hashmember)
        hash_gen(ast);
    else
        vec_gen(ast);
    gen(op_callf,0);
    return;
}

void nasal_codegen::mcall(nasal_ast& ast)
{
    if(ast.get_type()==ast_id)
        mcall_id(ast);
    else
        mcall_id(ast.get_children()[0]);
    int child_size=ast.get_children().size();
    for(int i=1;i<child_size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        if(tmp.get_type()==ast_callh)
            mcall_hash(tmp);
        else if(tmp.get_type()==ast_callv)
            mcall_vec(tmp);
    }
    return;
}

void nasal_codegen::mcall_id(nasal_ast& ast)
{
    std::string str=ast.get_str();
    int index=local_find(str);
    if(index>=0)
    {
        gen(op_mcalll,index);
        return;
    }
    index=global_find(str);
    if(index>=0)
    {
        gen(op_mcallg,index);
        return;
    }
    die("cannot find symbol named \""+str+"\".",ast.get_line());
    return;
}

void nasal_codegen::mcall_vec(nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    gen(op_mcallv,0);
    return;
}

void nasal_codegen::mcall_hash(nasal_ast& ast)
{
    std::string str=ast.get_str();
    regist_string(str);
    gen(op_mcallh,string_table[str]);
    return;
}

void nasal_codegen::single_def(nasal_ast& ast)
{
    std::string str=ast.get_children()[0].get_str();
    add_sym(str);
    calc_gen(ast.get_children()[1]);
    if(local.empty())
        gen(op_loadg,global_find(str));
    else
        gen(op_loadl,local_find(str));
    return;
}
void nasal_codegen::multi_def(nasal_ast& ast)
{
    int size=ast.get_children()[0].get_children().size();
    if(ast.get_children()[1].get_type()==ast_multi_scalar)
    {
        for(int i=0;i<size;++i)
        {
            std::string str=ast.get_children()[0].get_children()[i].get_str();
            add_sym(str);
            calc_gen(ast.get_children()[1].get_children()[i]);
            if(local.empty())
                gen(op_loadg,global_find(str));
            else
                gen(op_loadl,local_find(str));
        }
    }
    else
    {
        calc_gen(ast.get_children()[1]);
        for(int i=0;i<size;++i)
        {
            gen(op_callvi,i);
            std::string str=ast.get_children()[0].get_children()[i].get_str();
            add_sym(str);
            if(local.empty())
                gen(op_loadg,global_find(str));
            else
                gen(op_loadl,local_find(str));
        }
        pop_gen();
    }
    return;
}

void nasal_codegen::def_gen(nasal_ast& ast)
{
    if(ast.get_children()[0].get_type()==ast_id)
        single_def(ast);
    else
        multi_def(ast);
    return;
}

void nasal_codegen::multi_assign_gen(nasal_ast& ast)
{
    int size=ast.get_children()[0].get_children().size();
    if(ast.get_children()[1].get_type()==ast_multi_scalar)
    {
        for(int i=size-1;i>=0;--i)
            calc_gen(ast.get_children()[1].get_children()[i]);
        for(int i=0;i<size;++i)
        {
            mcall(ast.get_children()[0].get_children()[i]);
            gen(op_meq,0);
            pop_gen();
        }
    }
    else
    {
        calc_gen(ast.get_children()[1]);
        for(int i=0;i<size;++i)
        {
            gen(op_callvi,i);
            mcall(ast.get_children()[0].get_children()[i]);
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
            calc_gen(tmp.get_children()[0]);
            int ptr=exec_code.size();
            gen(op_jf,0);
            block_gen(tmp.get_children()[1]);
            jmp_label.push_back(exec_code.size());
            // without 'else' the last condition doesn't need to jmp
            if(i!=size-1)
                gen(op_jmp,0);
            exec_code[ptr].num=exec_code.size();
        }
        else
        {
            block_gen(tmp.get_children()[0]);
            break;
        }
    }
    for(std::vector<int>::iterator i=jmp_label.begin();i!=jmp_label.end();++i)
        exec_code[*i].num=exec_code.size();
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
        case ast_forindex: ++in_forindex;forindex_gen(ast);--in_forindex; break;
        case ast_foreach:  ++in_foreach; foreach_gen(ast); --in_foreach;  break;
    }
    return;
}

void nasal_codegen::load_continue_break(int continue_place,int break_place)
{
    for(int i=0;i<continue_ptr.front().size();++i)
        exec_code[continue_ptr.front()[i]].num=continue_place;
    for(int i=0;i<break_ptr.front().size();++i)
        exec_code[break_ptr.front()[i]].num=break_place;
    continue_ptr.pop_front();
    break_ptr.pop_front();
    return;
}

void nasal_codegen::while_gen(nasal_ast& ast)
{
    int loop_ptr=exec_code.size();
    calc_gen(ast.get_children()[0]);
    int condition_ptr=exec_code.size();
    gen(op_jf,0);

    block_gen(ast.get_children()[1]);
    gen(op_jmp,loop_ptr);
    exec_code[condition_ptr].num=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    return;
}

void nasal_codegen::for_gen(nasal_ast& ast)
{
    opcode op;
    switch(ast.get_children()[0].get_type())
    {
        case ast_null:break;
        case ast_definition:def_gen(ast.get_children()[0]);break;
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
        case ast_trino:calc_gen(ast.get_children()[0]);pop_gen();break;
    }
    int jmp_place=exec_code.size();
    if(ast.get_children()[1].get_type()==ast_null)
        gen(op_pone,0);
    else
        calc_gen(ast.get_children()[1]);
    int label_exit=exec_code.size();
    gen(op_jf,0);

    block_gen(ast.get_children()[3]);
    int continue_place=exec_code.size();
    switch(ast.get_children()[2].get_type())
    {
        case ast_null:break;
        case ast_definition:def_gen(ast.get_children()[2]);break;
        case ast_multi_assign:multi_assign_gen(ast.get_children()[2]);break;
        case ast_nil:case ast_num:case ast_str:case ast_func:break;
        case ast_vec:case ast_hash:
        case ast_call:
        case ast_equal:case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:case ast_lnkeq:
        case ast_neg:case ast_not:
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
        case ast_cmpeq:case ast_neq:case ast_leq:case ast_less:case ast_geq:case ast_grt:
        case ast_trino:calc_gen(ast.get_children()[2]);pop_gen();break;
    }
    gen(op_jmp,jmp_place);
    exec_code[label_exit].num=exec_code.size();

    load_continue_break(continue_place,exec_code.size());
    return;
}
void nasal_codegen::forindex_gen(nasal_ast& ast)
{
    calc_gen(ast.get_children()[1]);
    gen(op_cnt,0);
    int ptr=exec_code.size();
    gen(op_findex,0);
    if(ast.get_children()[0].get_type()==ast_new_iter)
    {
        std::string str=ast.get_children()[0].get_children()[0].get_str();
        add_sym(str);
        if(local.empty())
            gen(op_loadg,global_find(str));
        else
            gen(op_loadl,local_find(str));
    }
    else
    {
        mcall(ast.get_children()[0]);
        gen(op_meq,0);
        pop_gen();
    }

    block_gen(ast.get_children()[2]);
    gen(op_jmp,ptr);
    exec_code[ptr].num=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    pop_gen();// pop vector
    gen(op_cntpop,0);
    return;
}
void nasal_codegen::foreach_gen(nasal_ast& ast)
{
    calc_gen(ast.get_children()[1]);
    gen(op_cnt,0);
    int ptr=exec_code.size();
    gen(op_feach,0);
    if(ast.get_children()[0].get_type()==ast_new_iter)
    {
        std::string str=ast.get_children()[0].get_children()[0].get_str();
        add_sym(str);
        if(local.empty())
            gen(op_loadg,global_find(str));
        else
            gen(op_loadl,local_find(str));
    }
    else
    {
        mcall(ast.get_children()[0]);
        gen(op_meq,0);
        pop_gen();
    }

    block_gen(ast.get_children()[2]);
    gen(op_jmp,ptr);
    exec_code[ptr].num=exec_code.size();
    load_continue_break(exec_code.size()-1,exec_code.size());
    pop_gen();// pop vector
    gen(op_cntpop,0);
    return;
}

void nasal_codegen::or_gen(nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    int l1=exec_code.size();
    gen(op_jt,0);

    pop_gen();
    calc_gen(ast.get_children()[1]);
    int l2=exec_code.size();
    gen(op_jt,0);

    pop_gen();
    nil_gen();

    exec_code[l1].num=exec_code[l2].num=exec_code.size();
    return;
}

void nasal_codegen::and_gen(nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    gen(op_jt,exec_code.size()+2);

    int lfalse=exec_code.size();
    gen(op_jmp,0);
    pop_gen();// jt jumps here

    calc_gen(ast.get_children()[1]);
    gen(op_jt,exec_code.size()+3);

    exec_code[lfalse].num=exec_code.size();
    pop_gen();
    nil_gen();
    //jt jumps here
    return;
}

void nasal_codegen::trino_gen(nasal_ast& ast)
{
    calc_gen(ast.get_children()[0]);
    int lfalse=exec_code.size();
    gen(op_jf,0);
    calc_gen(ast.get_children()[1]);
    int lexit=exec_code.size();
    gen(op_jmp,0);
    exec_code[lfalse].num=exec_code.size();
    calc_gen(ast.get_children()[2]);
    exec_code[lexit].num=exec_code.size();
    return;
}

void nasal_codegen::calc_gen(nasal_ast& ast)
{
    switch(ast.get_type())
    {
        case ast_nil:  nil_gen();    break;
        case ast_num:  num_gen(ast); break;
        case ast_str:  str_gen(ast); break;
        case ast_id:   call_id(ast); break;
        case ast_vec:  vec_gen(ast); break;
        case ast_hash: hash_gen(ast);break;
        case ast_func: func_gen(ast);break;
        case ast_call: call_gen(ast);break;
        case ast_equal:
            calc_gen(ast.get_children()[1]);
            mcall(ast.get_children()[0]);
            gen(op_meq,0);
            break;
        // ast_addeq(22)~ast_lnkeq(26) op_addeq(23)~op_lnkeq(27)
        case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:case ast_lnkeq:
            calc_gen(ast.get_children()[1]);
            mcall(ast.get_children()[0]);
            gen(ast.get_type()-ast_addeq+op_addeq,0);
            break;
        case ast_or:or_gen(ast);break;
        case ast_and:and_gen(ast);break;
        // ast_add(33)~ast_link(37) op_add(18)~op_lnk(22)
        case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
            calc_gen(ast.get_children()[0]);
            calc_gen(ast.get_children()[1]);
            gen(ast.get_type()-ast_add+op_add,0);
            break;
        // ast_cmpeq(27)~ast_geq(32) op_eq(29)~op_geq(34)
        case ast_cmpeq:case ast_neq:case ast_less:case ast_leq:case ast_grt:case ast_geq:
            calc_gen(ast.get_children()[0]);
            calc_gen(ast.get_children()[1]);
            gen(ast.get_type()-ast_cmpeq+op_eq,0);
            break;
        case ast_trino:trino_gen(ast);break;
        case ast_neg:
            calc_gen(ast.get_children()[0]);
            gen(op_usub,0);
            break;
        case ast_not:
            calc_gen(ast.get_children()[0]);
            gen(op_unot,0);
            break;
        case ast_definition:
            single_def(ast);
            call_id(ast.get_children()[0]);
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
            case ast_definition:def_gen(tmp);break;
            case ast_multi_assign:multi_assign_gen(tmp);break;
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
            case ast_trino:calc_gen(tmp);pop_gen();break;
            case ast_return:ret_gen(tmp);break;
        }
    }
    return;
}

void nasal_codegen::ret_gen(nasal_ast& ast)
{
    for(int i=0;i<in_foreach;++i)
    {
        gen(op_pop,0);
        gen(op_cntpop,0);
    }
    for(int i=0;i<in_forindex;++i)
    {
        gen(op_pop,0);
        gen(op_cntpop,0);
    }
    if(ast.get_children().size())
        calc_gen(ast.get_children()[0]);
    else
        nil_gen();
    gen(op_ret,0);
    return;
}

void nasal_codegen::main_progress(nasal_ast& ast)
{
    error=0;
    number_table.clear();
    string_table.clear();
    exec_code.clear();

    global.clear();
    local.clear();
    gen(op_intg,0);
    int size=ast.get_children().size();
    for(int i=0;i<size;++i)
    {
        nasal_ast& tmp=ast.get_children()[i];
        switch(tmp.get_type())
        {
            case ast_null:case ast_nil:case ast_num:case ast_str:case ast_func:break;
            case ast_definition:def_gen(tmp);break;
            case ast_multi_assign:multi_assign_gen(tmp);break;
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
            case ast_trino:calc_gen(tmp);pop_gen();break;
        }
    }
    gen(op_nop,0);
    exec_code[0].num=global.size();
    num_res_table.resize(number_table.size());
    str_res_table.resize(string_table.size());
    for(auto i=number_table.begin();i!=number_table.end();++i)
        num_res_table[i->second]=i->first;
    for(auto i=string_table.begin();i!=string_table.end();++i)
        str_res_table[i->second]=i->first;
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
    printf("0x%.8x ",exec_code[index].num);
    // print detail info
    switch(exec_code[index].op)
    {
        case op_pnum:std::cout<<'('<<num_res_table[exec_code[index].num]<<')';break;
        case op_callb:std::cout<<'('<<builtin_func[exec_code[index].num].name<<')';break;
        case op_happ:
        case op_pstr:
        case op_callh:
        case op_mcallh:
        case op_para:
        case op_defpara:
        case op_dynpara:std::cout<<'('<<str_res_table[exec_code[index].num]<<')';break;
    }
    std::cout<<'\n';
    return;
}

void nasal_codegen::print_byte_code()
{
    for(int i=0;i<num_res_table.size();++i)
        std::cout<<".number "<<num_res_table[i]<<'\n';
    for(int i=0;i<str_res_table.size();++i)
        std::cout<<".symbol "<<str_res_table[i]<<'\n';
    int size=exec_code.size();
    for(int i=0;i<size;++i)
        print_op(i);
    return;
}

std::vector<std::string>& nasal_codegen::get_str_table()
{
    return str_res_table;
}

std::vector<double>& nasal_codegen::get_num_table()
{
    return num_res_table;
}

std::vector<opcode>& nasal_codegen::get_exec_code()
{
    return exec_code;
}

int nasal_codegen::get_error()
{
    return error;
}

#endif