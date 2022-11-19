#pragma once

#include "nasal_err.h"
#include <iomanip>
#include <list>
#include <stack>
#include <unordered_map>

enum op_code_type:u8
{
    op_exit,   // stop the virtual machine
    op_intg,   // global scope size
    op_intl,   // local scope size
    op_loadg,  // load global value
    op_loadl,  // load local value
    op_loadu,  // load upvalue
    op_pnum,   // push constant number to the stack
    op_pnil,   // push constant nil to the stack
    op_pstr,   // push constant string to the stack
    op_newv,   // push new vector with initial values from stack
    op_newh,   // push new hash to the stack
    op_newf,   // push new function to the stack
    op_happ,   // hash append
    op_para,   // normal  parameter
    op_deft,   // default parameter
    op_dyn,    // dynamic parameter
    op_unot,   // !
    op_usub,   // -
    op_add,    // +
    op_sub,    // -
    op_mul,    // *
    op_div,    // /
    op_lnk,    // ~
    op_addc,   // + const
    op_subc,   // - const
    op_mulc,   // * const
    op_divc,   // / const
    op_lnkc,   // ~ const
    op_addeq,  // +=
    op_subeq,  // -=
    op_muleq,  // *=
    op_diveq,  // /=
    op_lnkeq,  // ~=
    op_addeqc, // += const
    op_subeqc, // -= const
    op_muleqc, // *= const
    op_diveqc, // /= const
    op_lnkeqc, // ~= const
    op_meq,    // =
    op_eq,     // ==
    op_neq,    // !=
    op_less,   // <
    op_leq,    // <=
    op_grt,    // >
    op_geq,    // >=
    op_lessc,  // < const
    op_leqc,   // <= const
    op_grtc,   // > const
    op_geqc,   // >= const
    op_pop,    // pop a value from stack
    op_jmp,    // jump with no condition
    op_jt,     // used in operator and/or,jmp when condition is true and DO NOT POP
    op_jf,     // used in conditional/loop,jmp when condition is false and POP STACK
    op_cnt,    // add counter for forindex/foreach
    op_findex, // index counter on the top of forindex_stack plus 1
    op_feach,  // index counter on the top of forindex_stack plus 1 and get the value in vector
    op_callg,  // get value in global scope
    op_calll,  // get value in local scope
    op_upval,  // get upvalue in closure
    op_callv,  // call vec[index]
    op_callvi, // call vec[immediate] (used in multi-assign/multi-define)
    op_callh,  // call hash.label
    op_callfv, // call function(vector as parameters)
    op_callfh, // call function(hash as parameters)
    op_callb,  // call builtin-function
    op_slcbeg, // begin of slice like: vec[1,2,3:6,0,-1]
    op_slcend, // end of slice
    op_slc,    // slice like vec[1]
    op_slc2,   // slice like vec[nil:10]
    op_mcallg, // get memory space of value in global scope
    op_mcalll, // get memory space of value in local scope
    op_mupval, // get memory space of value in closure
    op_mcallv, // get memory space of vec[index]
    op_mcallh, // get memory space of hash.label
    op_ret     // return
};

const char* opname[]=
{
    "exit  ","intg  ","intl  ","loadg ",
    "loadl ","loadu ","pnum  ","pnil  ",
    "pstr  ","newv  ","newh  ","newf  ",
    "happ  ","para  ","def   ","dyn   ",
    "not   ","usub  ","add   ","sub   ",
    "mult  ","div   ","lnk   ","addc  ",
    "subc  ","multc ","divc  ","lnkc  ",
    "addeq ","subeq ","muleq ","diveq ",
    "lnkeq ","addeqc","subeqc","muleqc",
    "diveqc","lnkeqc","meq   ","eq    ",
    "neq   ","less  ","leq   ","grt   ",
    "geq   ","lessc ","leqc  ","grtc  ",
    "geqc  ","pop   ","jmp   ","jt    ",
    "jf    ","cnt   ","findx ","feach ",
    "callg ","calll ","upval ","callv ",
    "callvi","callh ","callfv","callfh",
    "callb ","slcbeg","slcend","slc   ",
    "slc2  ","mcallg","mcalll","mupval",
    "mcallv","mcallh","ret   "
};

struct opcode
{
    u8  op;  // opcode
    u16 fidx;// source code file index
    u32 num; // imm num
    u32 line;// line of source code
    opcode(u8 o=op_exit,u16 f=0,u32 n=0,u32 l=0):
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

class codestream
{
private:
    opcode code;
    const u32 index;
    const f64* nums;
    const string* strs;
    const string* files;
public:
    codestream(const opcode& c,const u32 i,const f64* n,const string* s,const string* f=nullptr):
        code(c.op,c.fidx,c.num,c.line),index(i),nums(n),strs(s),files(f){}
    friend std::ostream& operator<<(std::ostream& out,const codestream& ins)
    {
        u8  op=ins.code.op;
        u32 num=ins.code.num;
        out<<std::hex<<"0x"
           <<std::setw(8)<<std::setfill('0')<<ins.index<<"      "
           <<std::setw(2)<<std::setfill('0')<<(u32)op<<" "
           <<std::setw(2)<<std::setfill('0')<<((num>>24)&0xff)<<" "
           <<std::setw(2)<<std::setfill('0')<<((num>>16)&0xff)<<" "
           <<std::setw(2)<<std::setfill('0')<<((num>>8)&0xff)<<" "
           <<std::setw(2)<<std::setfill('0')<<(num&0xff)<<"      "
           <<opname[op]<<"  "<<std::dec;
        switch(op)
        {
            case op_addeq: case op_subeq:  case op_muleq: case op_diveq:
            case op_lnkeq: case op_meq:
                out<<std::hex<<"0x"<<num<<std::dec
                   <<" sp-"<<num;break;
            case op_addeqc:case op_subeqc: case op_muleqc:case op_diveqc:
                out<<std::hex<<"0x"<<(num&0x7fffffff)<<std::dec
                   <<" ("<<ins.nums[num&0x7fffffff]<<") sp-"
                   <<(num>>31);break;
            case op_lnkeqc:
                out<<std::hex<<"0x"<<(num&0x7fffffff)<<std::dec<<" (\""
                   <<rawstr(ins.strs[num&0x7fffffff],16)<<"\") sp-"
                   <<(num>>31);break;
            case op_addc:  case op_subc:   case op_mulc:  case op_divc:
            case op_lessc: case op_leqc:   case op_grtc:  case op_geqc:
            case op_pnum:
                out<<std::hex<<"0x"<<num<<std::dec<<" ("
                   <<ins.nums[num]<<")";break;
            case op_callvi:case op_newv:   case op_callfv:
            case op_intg:  case op_intl:
            case op_findex:case op_feach:
            case op_newf:  case op_jmp:    case op_jt:    case op_jf:
            case op_callg: case op_mcallg: case op_loadg:
            case op_calll: case op_mcalll: case op_loadl:
                out<<std::hex<<"0x"<<num<<std::dec;break;
            case op_callb:
                out<<std::hex<<"0x"<<num<<" <"<<builtin[num].name
                   <<"@0x"<<(u64)builtin[num].func<<std::dec<<">";break;
            case op_upval: case op_mupval: case op_loadu:
                out<<std::hex<<"0x"<<((num>>16)&0xffff)
                   <<"[0x"<<(num&0xffff)<<"]"<<std::dec;break;
            case op_happ:  case op_pstr:
            case op_lnkc:
            case op_callh: case op_mcallh:
            case op_para:  case op_deft:   case op_dyn:
                out<<std::hex<<"0x"<<num<<std::dec
                   <<" (\""<<rawstr(ins.strs[num],16)<<"\")";break;
            default:
                if(ins.files)
                    out<<std::hex<<"0x"<<num<<std::dec;
                break;
        }
        if(ins.files)
            out<<" ("<<ins.files[ins.code.fidx]<<":"<<ins.code.line<<")";
        return out;
    }
};

class codegen
{
private:
    u16 fileindex;
    error& err;
    const string* file;
    std::stack<u32> in_iterloop;
    std::unordered_map<f64,u32> num_table;
    std::unordered_map<string,u32> str_table;
    std::vector<f64> num_res;
    std::vector<string> str_res;
    std::vector<opcode> code;
    std::list<std::vector<i32>> continue_ptr;
    std::list<std::vector<i32>> break_ptr;
    // global : max 4095 values
    std::unordered_map<string,i32> global;
    // local  : max 32768 upvalues 65536 values
    std::list<std::unordered_map<string,i32>> local;

    // func end stack, reserved for code print
    std::stack<u32> fbstk;
    std::stack<u32> festk;
    
    void die(const string&,const u32,const u32,const u32);
    void regist_num(const f64);
    void regist_str(const string&);
    void find_symbol(const ast&);
    void add_sym(const string&);
    i32  local_find(const string&);
    i32  global_find(const string&);
    i32  upvalue_find(const string&);
    void gen(u8,u32,u32);
    void num_gen(const ast&);
    void str_gen(const ast&);
    void vec_gen(const ast&);
    void hash_gen(const ast&);
    void func_gen(const ast&);
    void call_gen(const ast&);
    void call_id(const ast&);
    void call_hash(const ast&);
    void call_vec(const ast&);
    void call_func(const ast&);
    void mcall(const ast&);
    void mcall_id(const ast&);
    void mcall_vec(const ast&);
    void mcall_hash(const ast&);
    void multi_def(const ast&);
    void single_def(const ast&);
    void def_gen(const ast&);
    void multi_assign_gen(const ast&);
    void cond_gen(const ast&);
    void loop_gen(const ast&);
    void load_continue_break(i32,i32);
    void while_gen(const ast&);
    void for_gen(const ast&);
    void forindex_gen(const ast&);
    void foreach_gen(const ast&);
    void or_gen(const ast&);
    void and_gen(const ast&);
    void trino_gen(const ast&);
    void calc_gen(const ast&);
    void block_gen(const ast&);
    void ret_gen(const ast&);

    void singleop(const u32);
public:
    codegen(error& e):fileindex(0),err(e),file(nullptr){}
    const error& compile(const parse&,const linker&);
    void print();
    const std::vector<string>& strs()  const {return str_res;}
    const std::vector<f64>&    nums()  const {return num_res;}
    const std::vector<opcode>& codes() const {return code;}
};

void codegen::die(const string& info,const u32 line,const u32 col,const u32 len=1)
{
    err.load(file[fileindex]);
    err.err("code",line,col,len,info);
}

void codegen::regist_num(const f64 num)
{
    if(!num_table.count(num))
    {
        u32 size=num_table.size();
        num_table[num]=size;
        num_res.push_back(num);
    }
}

void codegen::regist_str(const string& str)
{
    if(!str_table.count(str))
    {
        u32 size=str_table.size();
        str_table[str]=size;
        str_res.push_back(str);
    }
}

void codegen::find_symbol(const ast& node)
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

void codegen::add_sym(const string& name)
{
    if(local.empty())
    {
        if(global.count(name))
            return;
        i32 index=global.size();
        global[name]=index;
        return;
    }
    if(local.back().count(name))
        return;
    i32 index=local.back().size();
    local.back()[name]=index;
}

i32 codegen::local_find(const string& name)
{
    if(local.empty())
        return -1;
    return local.back().count(name)?local.back()[name]:-1;
}

i32 codegen::global_find(const string& name)
{
    return global.count(name)?global[name]:-1;
}

i32 codegen::upvalue_find(const string& name)
{
    // 32768 level 65536 upvalues
    i32 index=-1;
    usize size=local.size();
    if(size<=1)
        return -1;
    auto iter=local.begin();
    for(u32 i=0;i<size-1;++i,++iter)
        if(iter->count(name))
            index=((i<<16)|(*iter)[name]);
    return index;
}

void codegen::gen(u8 op,u32 num,u32 line)
{
    code.push_back({op,fileindex,num,line});
}

void codegen::num_gen(const ast& node)
{
    f64 num=node.num();
    regist_num(num);
    gen(op_pnum,num_table[num],node.line());
}

void codegen::str_gen(const ast& node)
{
    regist_str(node.str());
    gen(op_pstr,str_table[node.str()],node.line());
}

void codegen::vec_gen(const ast& node)
{
    for(auto& child:node.child())
        calc_gen(child);
    gen(op_newv,node.size(),node.line());
}

void codegen::hash_gen(const ast& node)
{
    gen(op_newh,0,node.line());
    for(auto& child:node.child())
    {
        calc_gen(child[1]);
        const string& str=child[0].str();
        regist_str(str);
        gen(op_happ,str_table[str],child.line());
    }
}

void codegen::func_gen(const ast& node)
{
    // parameter list format check
    bool checked_default=false;
    bool checked_dynamic=false;
    std::unordered_map<string,bool> argname;
    for(auto& tmp:node[0].child()){
        if(tmp.type()==ast_default){
            checked_default=true;
        }else if(tmp.type()==ast_dynamic){
            checked_dynamic=true;
        }
        // check default parameter and dynamic parameter
        if(checked_default && tmp.type()!=ast_default){
            die("must use default parameters here",tmp.line(),tmp.col(),tmp.str().length());
        }
        if(checked_dynamic && &tmp!=&node[0].child().back()){
            die("dynamic parameter must be the last one",tmp.line(),tmp.col(),tmp.str().length());
        }
        // check redefinition
        string name=tmp.str();
        if(argname.count(name)){
            die("redefinition of parameter: "+name,tmp.line(),tmp.col(),name.length());
        }else{
            argname[name]=true;
        }
    }

    usize newf=code.size();
    gen(op_newf,0,node.line());
    usize lsize=code.size();
    gen(op_intl,0,node.line());
    
    // add special keyword 'me' into symbol table
    // this symbol is only used in local scope(function's scope)
    // this keyword is set to nil as default value
    // after calling a hash, this keyword is set to this hash
    // this symbol's index will be 0
    local.push_back({{"me",0}});

    // generate parameter list
    for(auto& tmp:node[0].child())
    {
        const string& str=tmp.str();
        if(str=="me")
            die("\"me\" should not be a parameter",tmp.line(),tmp.col(),tmp.str().length());
        regist_str(str);
        switch(tmp.type())
        {
            case ast_id:gen(op_para,str_table[str],tmp.line());break;
            case ast_default:
                calc_gen(tmp[0]);
                gen(op_deft,str_table[str],tmp.line());
                break;
            case ast_dynamic:gen(op_dyn,str_table[str],tmp.line());break;
        }
        add_sym(str);
    }

    code[newf].num=code.size()+1; // entry
    usize jmp_ptr=code.size();
    gen(op_jmp,0,node.line());

    const ast& block=node[1];
    // search symbols first, must use after loading parameters
    // or the location of symbols will change and cause fatal error
    find_symbol(block);
    in_iterloop.push(0);
    block_gen(block);
    in_iterloop.pop();
    code[lsize].num=local.back().size();
    if(local.back().size()>=STACK_DEPTH)
        die("too many local variants: "+std::to_string(local.back().size()),block.line(),0);
    local.pop_back();

    if(!block.size() || block.child().back().type()!=ast_ret)
    {
        gen(op_pnil,0,block.line());
        gen(op_ret,0,block.line());
    }
    code[jmp_ptr].num=code.size();
}

void codegen::call_gen(const ast& node)
{
    calc_gen(node[0]);
    if(code.back().op==op_callb)
        return;
    for(usize i=1;i<node.size();++i)
    {
        const ast& tmp=node[i];
        switch(tmp.type())
        {
            case ast_callh:call_hash(tmp);break;
            case ast_callv:call_vec(tmp); break;
            case ast_callf:call_func(tmp);break;
        }
    }
}

void codegen::call_id(const ast& node)
{
    const string& str=node.str();
    for(u32 i=0;builtin[i].name;++i)
        if(builtin[i].name==str)
        {
            gen(op_callb,i,node.line());
            if(local.empty())
                die("should warp native function in local scope",node.line(),node.col(),node.str().length());
            return;
        }
    i32 index;
    if((index=local_find(str))>=0)
    {
        gen(op_calll,index,node.line());
        return;
    }
    if((index=upvalue_find(str))>=0)
    {
        gen(op_upval,index,node.line());
        return;
    }
    if((index=global_find(str))>=0)
    {
        gen(op_callg,index,node.line());
        return;
    }
    die("undefined symbol \""+str+"\"",node.line(),node.col(),node.str().length());
}

void codegen::call_hash(const ast& node)
{
    regist_str(node.str());
    gen(op_callh,str_table[node.str()],node.line());
}

void codegen::call_vec(const ast& node)
{
    // maybe this place can use callv-const if ast's first child is ast_num
    if(node.size()==1 && node[0].type()!=ast_subvec)
    {
        calc_gen(node[0]);
        gen(op_callv,0,node[0].line());
        return;
    }
    gen(op_slcbeg,0,node.line());
    for(auto& tmp:node.child())
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
    gen(op_slcend,0,node.line());
}

void codegen::call_func(const ast& node)
{
    if(!node.size())
        gen(op_callfv,0,node.line());
    else if(node[0].type()==ast_pair)
    {
        hash_gen(node);
        gen(op_callfh,0,node.line());
    }
    else
    {
        for(auto& child:node.child())
            calc_gen(child);
        gen(op_callfv,node.size(),node.line());
    }
}

/* mcall should run after calc_gen because this operation may trigger gc.
*  to avoid gc incorrectly collecting values that include the memory space(which will cause SIGSEGV),
*  we must run ast[1] then we run ast[0] to get the memory space.
*  at this time the value including the memory space can must be found alive.
*  BUT in fact this method does not make much safety.
*  so we use another way to avoid gc-caused SIGSEGV: reserve m-called value on stack.
*  you could see the notes in `vm::opr_mcallv()`.
*/
void codegen::mcall(const ast& node)
{
    if(node.type()==ast_id)
    {
        mcall_id(node);
        return;
    }
    if(node.size()==1) // foreach and forindex use call-id ast to get mcall
    {
        mcall_id(node[0]);
        return;
    }
    calc_gen(node[0]);
    for(usize i=1;i<node.size()-1;++i)
    {
        const ast& tmp=node[i];
        switch(tmp.type())
        {
            case ast_callh:call_hash(tmp);break;
            case ast_callv:call_vec(tmp); break;
            case ast_callf:call_func(tmp);break;
        }
    }
    const ast& tmp=node.child().back();
    if(tmp.type()==ast_callh)
        mcall_hash(tmp);
    else if(tmp.type()==ast_callv)
        mcall_vec(tmp);
}

void codegen::mcall_id(const ast& node)
{
    const string& str=node.str();
    for(u32 i=0;builtin[i].name;++i)
        if(builtin[i].name==str)
        {
            die("cannot modify native function",node.line(),node.col(),node.str().length());
            return;
        }
    i32 index;
    if((index=local_find(str))>=0)
    {
        gen(op_mcalll,index,node.line());
        return;
    }
    if((index=upvalue_find(str))>=0)
    {
        gen(op_mupval,index,node.line());
        return;
    }
    if((index=global_find(str))>=0)
    {
        gen(op_mcallg,index,node.line());
        return;
    }
    die("undefined symbol \""+str+"\"",node.line(),node.col(),node.str().length());
}

void codegen::mcall_vec(const ast& node)
{
    calc_gen(node[0]);
    gen(op_mcallv,0,node.line());
}

void codegen::mcall_hash(const ast& node)
{
    regist_str(node.str());
    gen(op_mcallh,str_table[node.str()],node.line());
}

void codegen::single_def(const ast& node)
{
    const string& str=node[0].str();
    calc_gen(node[1]);
    local.empty()?
        gen(op_loadg,global_find(str),node.line()):
        gen(op_loadl,local_find(str),node.line());
}
void codegen::multi_def(const ast& node)
{
    auto& ids=node[0].child();
    usize size=ids.size();
    if(node[1].type()==ast_tuple) // (var a,b,c)=(c,b,a);
    {
        auto& vals=node[1].child();
        for(usize i=0;i<size;++i)
        {
            calc_gen(vals[i]);
            const string& str=ids[i].str();
            local.empty()?
                gen(op_loadg,global_find(str),ids[i].line()):
                gen(op_loadl,local_find(str),ids[i].line());
        }
    }
    else // (var a,b,c)=[0,1,2];
    {
        calc_gen(node[1]);
        for(usize i=0;i<size;++i)
        {
            gen(op_callvi,i,node[1].line());
            const string& str=ids[i].str();
            local.empty()?
                gen(op_loadg,global_find(str),ids[i].line()):
                gen(op_loadl,local_find(str),ids[i].line());
        }
        gen(op_pop,0,node.line());
    }
}

void codegen::def_gen(const ast& node)
{
    if(node[0].type()==ast_id && node[1].type()==ast_tuple){
        die("cannot accept too many values",node[1].line(),node[1].col(),1);
    }else if(node[0].type()==ast_multi_id && node[1].type()==ast_tuple && node[0].size()<node[1].size()){
        die("lack values in multi-definition",node[1].line(),node[1].col(),1);
    }else if(node[0].type()==ast_multi_id && node[1].type()==ast_tuple && node[0].size()>node[1].size()){
        die("too many values in multi-definition",node[1].line(),node[1].col(),1);
    }
    node[0].type()==ast_id?single_def(node):multi_def(node);
}

void codegen::multi_assign_gen(const ast& node)
{
    if(node[1].type()==ast_tuple && node[0].size()<node[1].size()){
        die("lack values in multi-assignment",node[1].line(),node[1].col(),1);
    }else if(node[1].type()==ast_tuple && node[0].size()>node[1].size()){
        die("too many values in multi-assignment",node[1].line(),node[1].col(),1);
    }
    i32 size=node[0].size();
    if(node[1].type()==ast_tuple)
    {
        for(i32 i=size-1;i>=0;--i)
            calc_gen(node[1][i]);
        for(i32 i=0;i<size;++i)
        {
            mcall(node[0][i]);
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            if(code.back().op==op_mcalll)
                code.back().op=op_loadl;
            else if(code.back().op==op_mupval)
                code.back().op=op_loadu;
            else if(code.back().op==op_mcallg)
                code.back().op=op_loadg;
            else
                gen(op_meq,1,node[0][i].line());
        }
    }
    else
    {
        calc_gen(node[1]);
        for(i32 i=0;i<size;++i)
        {
            gen(op_callvi,i,node[1].line());
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            mcall(node[0][i]);
            if(code.back().op==op_mcalll)
                code.back().op=op_loadl;
            else if(code.back().op==op_mupval)
                code.back().op=op_loadu;
            else if(code.back().op==op_mcallg)
                code.back().op=op_loadg;
            else
                gen(op_meq,1,node[0][i].line());
        }
        gen(op_pop,0,node.line());
    }
}

void codegen::cond_gen(const ast& node)
{
    std::vector<usize> jmp_label;
    for(auto& tmp:node.child())
    {
        if(tmp.type()==ast_if || tmp.type()==ast_elsif)
        {
            calc_gen(tmp[0]);
            usize ptr=code.size();
            gen(op_jf,0,tmp.line());
            block_gen(tmp[1]);
            // without 'else' the last condition doesn't need to jmp
            if(&tmp!=&node.child().back())
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

void codegen::loop_gen(const ast& node)
{
    continue_ptr.push_front(std::vector<i32>());
    break_ptr.push_front(std::vector<i32>());
    switch(node.type())
    {
        case ast_while:   while_gen(node);   break;
        case ast_for:     for_gen(node);     break;
        case ast_forindex:forindex_gen(node);break;
        case ast_foreach: foreach_gen(node); break;
    }
}

void codegen::load_continue_break(i32 continue_place,i32 break_place)
{
    for(auto i:continue_ptr.front())
        code[i].num=continue_place;
    for(auto i:break_ptr.front())
        code[i].num=break_place;
    continue_ptr.pop_front();
    break_ptr.pop_front();
}

void codegen::while_gen(const ast& node)
{
    usize loop_ptr=code.size();
    calc_gen(node[0]);
    usize condition_ptr=code.size();
    gen(op_jf,0,node[0].line());

    block_gen(node[1]);
    gen(op_jmp,loop_ptr,node[1].line());
    code[condition_ptr].num=code.size();
    load_continue_break(code.size()-1,code.size());
}

void codegen::for_gen(const ast& node)
{
    switch(node[0].type())
    {
        case ast_null:break;
        case ast_def:def_gen(node[0]);break;
        case ast_multi_assign:multi_assign_gen(node[0]);break;
        case ast_addeq:case ast_subeq:
        case ast_multeq:case ast_diveq:case ast_lnkeq:
            calc_gen(node[0]);
            if(op_addeq<=code.back().op && code.back().op<=op_lnkeq)
                code.back().num=1;
            else if(op_addeqc<=code.back().op && code.back().op<=op_lnkeqc)
                code.back().num|=0x80000000;
            else
                gen(op_pop,0,node[0].line());
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
            calc_gen(node[0]);
            if(code.back().op==op_meq)
                code.back().num=1;
            else
                gen(op_pop,0,node[0].line());
            break;
        case ast_equal:
            if(node[0][0].type()==ast_id)
            {
                calc_gen(node[0][1]);
                mcall_id(node[0][0]);
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
                calc_gen(node[0]);
                if(code.back().op==op_meq)
                    code.back().num=1;
                else
                    gen(op_pop,0,node[0].line());
            }
            break;
    }
    usize jmp_place=code.size();
    if(node[1].type()==ast_null)
        gen(op_pnum,num_table[1],node[1].line());
    else
        calc_gen(node[1]);
    usize label_exit=code.size();
    gen(op_jf,0,node[1].line());

    block_gen(node[3]);
    usize continue_place=code.size();
    switch(node[2].type())
    {
        case ast_null:break;
        case ast_def:def_gen(node[2]);break;
        case ast_multi_assign:multi_assign_gen(node[2]);break;
        case ast_addeq:case ast_subeq:
        case ast_multeq:case ast_diveq:case ast_lnkeq:
            calc_gen(node[2]);
            if(op_addeq<=code.back().op && code.back().op<=op_lnkeq)
                code.back().num=1;
            else if(op_addeqc<=code.back().op && code.back().op<=op_lnkeqc)
                code.back().num|=0x80000000;
            else
                gen(op_pop,0,node[2].line());
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
            calc_gen(node[2]);
            if(code.back().op==op_meq)
                code.back().num=1;
            else
                gen(op_pop,0,node[2].line());
            break;
        case ast_equal:
            if(node[2][0].type()==ast_id)
            {
                calc_gen(node[2][1]);
                mcall_id(node[2][0]);
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
                calc_gen(node[2]);
                if(code.back().op==op_meq)
                    code.back().num=1;
                else
                    gen(op_pop,0,node[2].line());
            }
            break;
    }
    gen(op_jmp,jmp_place,node[2].line());
    code[label_exit].num=code.size();

    load_continue_break(continue_place,code.size());
}
void codegen::forindex_gen(const ast& node)
{
    calc_gen(node[1]);
    gen(op_cnt,0,node[1].line());
    usize ptr=code.size();
    gen(op_findex,0,node.line());
    if(node[0].type()==ast_iter) // define a new iterator
    {
        const string& str=node[0][0].str();
        local.empty()?
            gen(op_loadg,global_find(str),node[0][0].line()):
            gen(op_loadl,local_find(str),node[0][0].line());
    }
    else // use exist variable as the iterator
    {
        mcall(node[0]);
        if(code.back().op==op_mcallg)
            code.back().op=op_loadg;
        else if(code.back().op==op_mcalll)
            code.back().op=op_loadl;
        else if(code.back().op==op_mupval)
            code.back().op=op_loadu;
        else
            gen(op_meq,1,node[0].line());
    }
    ++in_iterloop.top();
    block_gen(node[2]);
    --in_iterloop.top();
    gen(op_jmp,ptr,node.line());
    code[ptr].num=code.size();
    load_continue_break(code.size()-1,code.size());
    gen(op_pop,0,node[1].line());// pop vector
    gen(op_pop,0,node.line());// pop iterator
}
void codegen::foreach_gen(const ast& node)
{
    calc_gen(node[1]);
    gen(op_cnt,0,node.line());
    usize ptr=code.size();
    gen(op_feach,0,node.line());
    if(node[0].type()==ast_iter) // define a new iterator
    {
        const string& str=node[0][0].str();
        local.empty()?
            gen(op_loadg,global_find(str),node[0][0].line()):
            gen(op_loadl,local_find(str),node[0][0].line());
    }
    else // use exist variable as the iterator
    {
        mcall(node[0]);
        if(code.back().op==op_mcallg)
            code.back().op=op_loadg;
        else if(code.back().op==op_mcalll)
            code.back().op=op_loadl;
        else if(code.back().op==op_mupval)
            code.back().op=op_loadu;
        else
            gen(op_meq,1,node[0].line());
    }
    ++in_iterloop.top();
    block_gen(node[2]);
    --in_iterloop.top();
    gen(op_jmp,ptr,node.line());
    code[ptr].num=code.size();
    load_continue_break(code.size()-1,code.size());
    gen(op_pop,0,node[1].line());// pop vector
    gen(op_pop,0,node.line());// pop iterator
}

void codegen::or_gen(const ast& node)
{
    calc_gen(node[0]);
    usize l1=code.size();
    gen(op_jt,0,node[0].line());

    gen(op_pop,0,node[0].line());
    calc_gen(node[1]);
    usize l2=code.size();
    gen(op_jt,0,node[1].line());

    gen(op_pop,0,node[1].line());
    gen(op_pnil,0,node[1].line());

    code[l1].num=code[l2].num=code.size();
}

void codegen::and_gen(const ast& node)
{
    calc_gen(node[0]);
    gen(op_jt,code.size()+2,node[0].line());

    usize lfalse=code.size();
    gen(op_jmp,0,node[0].line());
    gen(op_pop,0,node[1].line());// jt jumps here

    calc_gen(node[1]);
    gen(op_jt,code.size()+3,node[1].line());

    code[lfalse].num=code.size();
    gen(op_pop,0,node[1].line());
    gen(op_pnil,0,node[1].line());
    //jt jumps here
}

void codegen::trino_gen(const ast& node)
{
    calc_gen(node[0]);
    usize lfalse=code.size();
    gen(op_jf,0,node[0].line());
    calc_gen(node[1]);
    usize lexit=code.size();
    gen(op_jmp,0,node[1].line());
    code[lfalse].num=code.size();
    calc_gen(node[2]);
    code[lexit].num=code.size();
}

void codegen::calc_gen(const ast& node)
{
    switch(node.type())
    {
        case ast_nil:  gen(op_pnil,0,node.line());break;
        case ast_num:  num_gen(node);  break;
        case ast_str:  str_gen(node);  break;
        case ast_id:   call_id(node);  break;
        case ast_vec:  vec_gen(node);  break;
        case ast_hash: hash_gen(node); break;
        case ast_func: func_gen(node); break;
        case ast_call: call_gen(node); break;
        case ast_equal:
            calc_gen(node[1]);
            mcall(node[0]);
            gen(op_meq,0,node.line());
            break;
        // ast_addeq(22)~ast_lnkeq(26) op_addeq(23)~op_lnkeq(27)
        case ast_addeq:case ast_subeq:case ast_multeq:case ast_diveq:
            if(node[1].type()!=ast_num)
                calc_gen(node[1]);
            mcall(node[0]);
            if(node[1].type()!=ast_num)
                gen(node.type()-ast_addeq+op_addeq,0,node.line());
            else
            {
                regist_num(node[1].num());
                gen(node.type()-ast_addeq+op_addeqc,num_table[node[1].num()],node.line());
            }
            break;
        case ast_lnkeq:
            if(node[1].type()!=ast_str)
                calc_gen(node[1]);
            else
                regist_str(node[1].str());
            mcall(node[0]);
            if(node[1].type()!=ast_str)
                gen(op_lnkeq,0,node.line());
            else
                gen(op_lnkeqc,str_table[node[1].str()],node.line());
            break;
        case ast_or:or_gen(node);break;
        case ast_and:and_gen(node);break;
        // ast_add(33)~ast_link(37) op_add(18)~op_lnk(22)
        case ast_add:case ast_sub:case ast_mult:case ast_div:
            calc_gen(node[0]);
            if(node[1].type()!=ast_num)
            {
                calc_gen(node[1]);
                gen(node.type()-ast_add+op_add,0,node.line());
            }
            else
            {
                regist_num(node[1].num());
                gen(node.type()-ast_add+op_addc,num_table[node[1].num()],node.line());
            }
            break;
        case ast_link:
            calc_gen(node[0]);
            if(node[1].type()!=ast_str)
            {
                calc_gen(node[1]);
                gen(op_lnk,0,node.line());
            }
            else
            {
                regist_str(node[1].str());
                gen(op_lnkc,str_table[node[1].str()],node.line());
            }
            break;
        // ast_cmpeq(27)~ast_geq(32) op_eq(29)~op_geq(34)
        case ast_cmpeq:case ast_neq:
            calc_gen(node[0]);
            calc_gen(node[1]);
            gen(node.type()-ast_cmpeq+op_eq,0,node.line());
            break;
        case ast_less:case ast_leq:case ast_grt:case ast_geq:
            calc_gen(node[0]);
            if(node[1].type()!=ast_num)
            {
                calc_gen(node[1]);
                gen(node.type()-ast_less+op_less,0,node.line());
            }
            else
            {
                regist_num(node[1].num());
                gen(node.type()-ast_less+op_lessc,num_table[node[1].num()],node.line());
            }
            break;
        case ast_trino:trino_gen(node);break;
        case ast_neg:
            calc_gen(node[0]);
            gen(op_usub,0,node.line());
            break;
        case ast_not:
            calc_gen(node[0]);
            gen(op_unot,0,node.line());
            break;
        case ast_def:
            single_def(node);
            call_id(node[0]);
            break;
    }
}

void codegen::block_gen(const ast& node)
{
    for(auto& tmp:node.child())
        switch(tmp.type())
        {
            case ast_null:case ast_nil:case ast_num:case ast_str:break;
            case ast_file:fileindex=tmp.num();break; // special node type in main block
            case ast_def:def_gen(tmp);break;
            case ast_multi_assign:multi_assign_gen(tmp);break;
            case ast_cond:cond_gen(tmp);break;
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

void codegen::ret_gen(const ast& node)
{
    for(u32 i=0;i<in_iterloop.top();++i)
    {
        gen(op_pop,0,node.line());
        gen(op_pop,0,node.line());
    }
    if(node.size())
        calc_gen(node[0]);
    else
        gen(op_pnil,0,node.line());
    gen(op_ret,0,node.line());
}

const error& codegen::compile(const parse& parse,const linker& import)
{
    fileindex=0;
    file=import.filelist().data();
    in_iterloop.push(0);
    find_symbol(parse.tree()); // search symbols first
    gen(op_intg,global.size(),0);
    block_gen(parse.tree()); // generate main block
    gen(op_exit,0,0);
    if(global.size()>=STACK_DEPTH){
        err.load(file[fileindex]);
        err.err("code","too many global variants: "+std::to_string(global.size()));
    }
    if(code.size()>0xffffffff){
        err.load(file[fileindex]);
        err.err("code","too large generated bytecode file: "+std::to_string(code.size()));
    }
    return err;
}

void codegen::singleop(const u32 index)
{
    // print opcode index,opcode name,opcode immediate number
    const opcode& c=code[index];
    if(!festk.empty() && index==festk.top())
    {
        std::cout<<std::hex<<"<0x"<<fbstk.top()<<std::dec<<">;\n";
        if(code[index].op!=op_newf) // avoid two empty lines
            std::cout<<"\n";
        fbstk.pop();
        festk.pop();
    }
    if(c.op==op_newf)
    {
        std::cout<<std::hex<<"\nfunc <0x"<<index<<std::dec<<">:\n";
        for(u32 i=index;i<code.size();++i)
            if(code[i].op==op_jmp)
            {
                fbstk.push(index);
                festk.push(code[i].num);
                break;
            }
    }
    std::cout<<"  "<<codestream(c,index,num_res.data(),str_res.data())<<"\n";
}

void codegen::print()
{
    for(auto& num:num_res)
        std::cout<<"  .number "<<num<<"\n";
    for(auto& str:str_res)
        std::cout<<"  .symbol \""<<rawstr(str)<<"\"\n";
    std::cout<<"\n";
    for(u32 i=0;i<code.size();++i)
        singleop(i);
}
