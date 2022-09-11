#ifndef __NASAL_VM_H__
#define __NASAL_VM_H__

#include <iomanip>
#include <stack>
#include "nasal_codegen.h"

class nasal_vm
{
protected:
    /* registers and constants of nasal_vm */
    u32           pc;       // program counter
    nas_ref*      localr;   // local scope register
    nas_ref*      memr;     // used for mem_call
    nas_ref       funcr;    // function register
    nas_ref       upvalr;   // upvalue register
    nas_ref*      canary;   // avoid stackoverflow
    nas_ref*      top;      // stack top
    const f64*    num_table;// const numbers, ref from nasal_codegen
    const string* str_table;// const symbols, ref from nasal_codegen
    std::vector<u32> imm;   // immediate number
    
    /* garbage collector */
    nasal_gc      gc;
    /* main stack */
    nas_ref       stack[STACK_DEPTH];

    /* values used for debugger */
    const string* files;    // ref from nasal_import
    const opcode* bytecode; // ref from nasal_codegen

    void init(
        const std::vector<string>&,
        const std::vector<f64>&,
        const std::vector<opcode>&,
        const std::vector<string>&,
        const std::vector<string>&);
    /* debug functions */
    bool detail_info;
    void valinfo(nas_ref&);
    void traceback();
    void stackinfo(const u32);
    void reginfo();
    void gstate();
    void lstate();
    void ustate();
    void detail();
    [[noreturn]] void die(const string&);
    /* vm calculation functions*/
    bool condition(nas_ref);
    /* vm operands */
    void o_intg();
    void o_intl();
    void o_loadg();
    void o_loadl();
    void o_loadu();
    void o_pnum();
    void o_pnil();
    void o_pstr();
    void o_newv();
    void o_newh();
    void o_newf();
    void o_happ();
    void o_para();
    void o_deft();
    void o_dyn();
    void o_unot();
    void o_usub();
    void o_add();
    void o_sub();
    void o_mul();
    void o_div();
    void o_lnk();
    void o_addc();
    void o_subc();
    void o_mulc();
    void o_divc();
    void o_lnkc();
    void o_addeq();
    void o_subeq();
    void o_muleq();
    void o_diveq();
    void o_lnkeq();
    void o_addeqc();
    void o_subeqc();
    void o_muleqc();
    void o_diveqc();
    void o_lnkeqc();
    void o_meq();
    void o_eq();
    void o_neq();
    void o_less();
    void o_leq();
    void o_grt();
    void o_geq();
    void o_lessc();
    void o_leqc();
    void o_grtc();
    void o_geqc();
    void o_pop();
    void o_jmp();
    void o_jt();
    void o_jf();
    void o_cnt();
    void o_findex();
    void o_feach();
    void o_callg();
    void o_calll();
    void o_upval();
    void o_callv();
    void o_callvi();
    void o_callh();
    void o_callfv();
    void o_callfh();
    void o_callb();
    void o_slcbeg();
    void o_slcend();
    void o_slc();
    void o_slc2();
    void o_mcallg();
    void o_mcalll();
    void o_mupval();
    void o_mcallv();
    void o_mcallh();
    void o_ret();
public:
    nasal_vm():pc(0),localr(nullptr),memr(nullptr),funcr(nil),
               upvalr(nil),canary(nullptr),top(stack),
               num_table(nullptr),str_table(nullptr),
               gc(pc,localr,memr,funcr,upvalr,canary,top,stack),
               files(nullptr),bytecode(nullptr),detail_info(false){}
    void run(
        const nasal_codegen&,
        const nasal_import&,
        const std::vector<string>&,
        const bool);
};

void nasal_vm::init(
    const std::vector<string>& strs,
    const std::vector<f64>&    nums,
    const std::vector<opcode>& code,
    const std::vector<string>& filenames,
    const std::vector<string>& argv)
{
    gc.init(strs,argv);
    num_table=nums.data();
    str_table=strs.data();
    bytecode=code.data();
    files=filenames.data();

    /* set canary and program counter */
    pc=0;
    localr=memr=nullptr;
    funcr=upvalr=nil;
    canary=stack+STACK_DEPTH-1; // stack[STACK_DEPTH-1]
    top=stack;

    /* clear main stack */
    for(u32 i=0;i<STACK_DEPTH;++i)
        stack[i]=nil;
}
void nasal_vm::valinfo(nas_ref& val)
{
    const nas_val* p=val.val.gcobj;
    std::cout<<"\t";
    switch(val.type)
    {
        case vm_none: std::cout<<"| null |";break;
        case vm_ret:  std::cout<<"| pc   | 0x"<<std::hex
                               <<val.ret()<<std::dec;break;
        case vm_addr: std::cout<<"| addr | 0x"<<std::hex
                               <<(u64)val.addr()<<std::dec;break;
        case vm_cnt:  std::cout<<"| cnt  | "<<val.cnt();break;
        case vm_nil:  std::cout<<"| nil  |";break;
        case vm_num:  std::cout<<"| num  | "<<val.num();break;
        case vm_str:  std::cout<<"| str  | <0x"<<std::hex<<(u64)p
                               <<"> "<<rawstr(val.str(),16)<<std::dec;break;
        case vm_func: std::cout<<"| func | <0x"<<std::hex<<(u64)p
                               <<"> entry:0x"<<val.func().entry
                               <<std::dec;break;
        case vm_upval:std::cout<<"| upval| <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> ["<<val.upval().size
                               <<" val]";break;
        case vm_vec:  std::cout<<"| vec  | <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> ["<<val.vec().size()
                               <<" val]";break;
        case vm_hash: std::cout<<"| hash | <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> {"<<val.hash().size()
                               <<" val}";break;
        case vm_obj:  std::cout<<"| obj  | <0x"<<std::hex<<(u64)p
                               <<"> obj:0x"<<(u64)val.obj().ptr
                               <<std::dec;break;
        case vm_co:   std::cout<<"| co   | <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> coroutine";break;
        default:      std::cout<<"| err  | <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> unknown object";break;
    }
    std::cout<<"\n";
}
void nasal_vm::traceback()
{
    /* bytecode[0].num is the global size */
    nas_ref* bottom=gc.stack==stack?stack+bytecode[0].num:gc.stack;
    nas_ref* ctx_top=gc.stack==stack?top:gc.top;
    std::stack<u32> ret;
    for(nas_ref* i=bottom;i<=ctx_top;++i)
        if(i->type==vm_ret && i->ret()!=0)
            ret.push(i->ret());
    ret.push(pc); // store the position program crashed
    std::cout<<"trace back ("<<(gc.stack==stack?"main":"coroutine")<<")\n";
    for(u32 p=0,same=0,prev=0xffffffff;!ret.empty();prev=p,ret.pop())
    {
        if((p=ret.top())==prev)
        {
            ++same;
            continue;
        }
        if(same)
            std::cout
            <<"  0x"<<std::hex<<std::setw(8)<<std::setfill('0')
            <<prev<<std::dec<<":       "<<same<<" same call(s)\n";
        same=0;
        std::cout<<"  "<<codestream(bytecode[p],p,num_table,str_table,files)<<"\n";
    }
    // the first called place has no same calls
}
void nasal_vm::stackinfo(const u32 limit=10)
{
    /* bytecode[0].num is the global size */
    const u32 gsize=gc.stack==stack?bytecode[0].num:0;
    nas_ref*  t=top;
    nas_ref*  bottom=gc.stack+gsize;
    std::cout<<"vm stack (0x"<<std::hex<<(u64)bottom<<std::dec
             <<" <sp+"<<gsize<<">, limit "<<limit<<", total "
             <<(t<bottom? 0:(i64)(t-bottom+1))<<")\n";
    for(u32 i=0;i<limit && t>=bottom;++i,--t)
    {
        std::cout<<"  0x"<<std::hex
                 <<std::setw(8)<<std::setfill('0')
                 <<(u64)(t-gc.stack)<<std::dec;
        valinfo(t[0]);
    }
}
void nasal_vm::reginfo()
{
    std::cout<<"registers ("<<(gc.cort?"coroutine":"main")<<")\n"<<std::hex
             <<"  [ pc     ]    | pc   | 0x"<<pc<<"\n"
             <<"  [ global ]    | addr | 0x"<<(u64)stack<<"\n"
             <<"  [ localr ]    | addr | 0x"<<(u64)localr<<"\n"
             <<"  [ memr   ]    | addr | 0x"<<(u64)memr<<"\n"
             <<"  [ canary ]    | addr | 0x"<<(u64)canary<<"\n"
             <<"  [ top    ]    | addr | 0x"<<(u64)top<<"\n"
             <<std::dec;
    std::cout<<"  [ funcr  ]";valinfo(funcr);
    std::cout<<"  [ upvalr ]";valinfo(upvalr);
}
void nasal_vm::gstate()
{
    if(!bytecode[0].num || stack[0].type==vm_none) // bytecode[0].op is op_intg
        return;
    std::cout<<"global (0x"<<std::hex<<(u64)stack<<" <sp+0>)\n"<<std::dec;
    for(u32 i=0;i<bytecode[0].num;++i)
    {
        std::cout<<"  0x"<<std::hex<<std::setw(8)
                 <<std::setfill('0')<<i<<std::dec;
        valinfo(stack[i]);
    }
}
void nasal_vm::lstate()
{
    if(!localr || !funcr.func().lsize)
        return;
    const u32 lsize=funcr.func().lsize;
    std::cout<<"local (0x"<<std::hex<<(u64)localr
             <<" <sp+"<<(u64)(localr-gc.stack)<<">)\n"<<std::dec;
    for(u32 i=0;i<lsize;++i)
    {
        std::cout<<"  0x"<<std::hex<<std::setw(8)
                 <<std::setfill('0')<<i<<std::dec;
        valinfo(localr[i]);
    }
}
void nasal_vm::ustate()
{
    if(funcr.type==vm_nil || funcr.func().upval.empty())
        return;
    std::cout<<"upvalue\n";
    auto& upval=funcr.func().upval;
    for(u32 i=0;i<upval.size();++i)
    {
        std::cout<<"  -> upval["<<i<<"]:\n";
        auto& uv=upval[i].upval();
        for(u32 j=0;j<uv.size;++j)
        {
            std::cout<<"     0x"<<std::hex<<std::setw(8)
                     <<std::setfill('0')<<j<<std::dec;
            valinfo(uv[j]);
        }
    }
}
void nasal_vm::detail()
{
    reginfo();
    gstate();
    lstate();
    ustate();
}

[[noreturn]]
void nasal_vm::die(const string& str)
{
    std::cout<<"[vm] error: "<<str<<"\n";
    traceback();
    stackinfo();
    if(detail_info)
        detail();
    std::exit(1);
}
inline bool nasal_vm::condition(nas_ref val)
{
    if(val.type==vm_num)
        return val.num();
    else if(val.type==vm_str)
    {
        const f64 num=str2num(val.str().c_str());
        return std::isnan(num)?!val.str().empty():num;
    }
    return false;
}
inline void nasal_vm::o_intg()
{
    // global values store on stack
    top+=imm[pc];
    --top;// point to the top
}
inline void nasal_vm::o_intl()
{
    top[0].func().local.resize(imm[pc],nil);
    top[0].func().lsize=imm[pc];
}
inline void nasal_vm::o_loadg()
{
    stack[imm[pc]]=(top--)[0];
}
inline void nasal_vm::o_loadl()
{
    localr[imm[pc]]=(top--)[0];
}
inline void nasal_vm::o_loadu()
{
    funcr.func().upval[(imm[pc]>>16)&0xffff]
         .upval()[imm[pc]&0xffff]=(top--)[0];
}
inline void nasal_vm::o_pnum()
{
    (++top)[0]={vm_num,num_table[imm[pc]]};
}
inline void nasal_vm::o_pnil()
{
    (++top)[0]=nil;
}
inline void nasal_vm::o_pstr()
{
    (++top)[0]=gc.strs[imm[pc]];
}
inline void nasal_vm::o_newv()
{
    nas_ref newv=gc.alloc(vm_vec);
    auto& vec=newv.vec().elems;
    vec.resize(imm[pc]);
    // use top-=imm[pc]-1 here will cause error if imm[pc] is 0
    top=top-imm[pc]+1;
    for(u32 i=0;i<imm[pc];++i)
        vec[i]=top[i];
    top[0]=newv;
}
inline void nasal_vm::o_newh()
{
    (++top)[0]=gc.alloc(vm_hash);
}
inline void nasal_vm::o_newf()
{
    (++top)[0]=gc.alloc(vm_func);
    nas_func& func=top[0].func();
    func.entry=imm[pc];
    func.psize=1;

    /* this means you create a new function in local scope */
    if(localr)
    {
        func.upval=funcr.func().upval;
        nas_ref upval=(upvalr.type==vm_nil)?gc.alloc(vm_upval):upvalr;
        upval.upval().size=funcr.func().lsize;
        upval.upval().stk=localr;
        func.upval.push_back(upval);
        upvalr=upval;
    }
}
inline void nasal_vm::o_happ()
{
    top[-1].hash().elems[str_table[imm[pc]]]=top[0];
    --top;
}
inline void nasal_vm::o_para()
{
    nas_func& func=top[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[imm[pc]]=func.psize;
    func.local[func.psize++]={vm_none};
}
inline void nasal_vm::o_deft()
{
    nas_ref val=top[0];
    nas_func& func=(--top)[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[imm[pc]]=func.psize;
    func.local[func.psize++]=val;
}
inline void nasal_vm::o_dyn()
{
    top[0].func().dpara=imm[pc];
}
inline void nasal_vm::o_unot()
{
    nas_ref val=top[0];
    switch(val.type)
    {
        case vm_nil:top[0]=one;break;
        case vm_num:top[0]=val.num()?zero:one;break;
        case vm_str:
        {
            const f64 num=str2num(val.str().c_str());
            if(std::isnan(num))
                top[0]={vm_num,(f64)val.str().empty()};
            else
                top[0]=num?zero:one;
        }break;
        default:die("incorrect value type");break;
    }
}
inline void nasal_vm::o_usub()
{
    top[0]={vm_num,-top[0].tonum()};
}

#define op_calc(type)\
    top[-1]={vm_num,top[-1].tonum() type top[0].tonum()};\
    --top;

inline void nasal_vm::o_add(){op_calc(+);}
inline void nasal_vm::o_sub(){op_calc(-);}
inline void nasal_vm::o_mul(){op_calc(*);}
inline void nasal_vm::o_div(){op_calc(/);}
inline void nasal_vm::o_lnk()
{
    top[-1]=gc.newstr(top[-1].tostr()+top[0].tostr());
    --top;
}

#define op_calc_const(type)\
    top[0]={vm_num,top[0].tonum() type num_table[imm[pc]]};

inline void nasal_vm::o_addc(){op_calc_const(+);}
inline void nasal_vm::o_subc(){op_calc_const(-);}
inline void nasal_vm::o_mulc(){op_calc_const(*);}
inline void nasal_vm::o_divc(){op_calc_const(/);}
inline void nasal_vm::o_lnkc()
{
    top[0]=gc.newstr(top[0].tostr()+str_table[imm[pc]]);
}

#define op_calc_eq(type)\
    top[-1]=memr[0]={vm_num,memr[0].tonum() type top[-1].tonum()};\
    memr=nullptr;\
    top-=imm[pc]+1;

inline void nasal_vm::o_addeq(){op_calc_eq(+);}
inline void nasal_vm::o_subeq(){op_calc_eq(-);}
inline void nasal_vm::o_muleq(){op_calc_eq(*);}
inline void nasal_vm::o_diveq(){op_calc_eq(/);}
inline void nasal_vm::o_lnkeq()
{
    top[-1]=memr[0]=gc.newstr(memr[0].tostr()+top[-1].tostr());
    memr=nullptr;
    top-=imm[pc]+1;
}

#define op_calc_eq_const(type)\
    top[0]=memr[0]={vm_num,memr[0].tonum() type num_table[imm[pc]&0x7fffffff]};\
    memr=nullptr;\
    top-=(imm[pc]>>31);

inline void nasal_vm::o_addeqc(){op_calc_eq_const(+);}
inline void nasal_vm::o_subeqc(){op_calc_eq_const(-);}
inline void nasal_vm::o_muleqc(){op_calc_eq_const(*);}
inline void nasal_vm::o_diveqc(){op_calc_eq_const(/);}
inline void nasal_vm::o_lnkeqc()
{
    top[0]=memr[0]=gc.newstr(memr[0].tostr()+str_table[imm[pc]&0x7fffffff]);
    memr=nullptr;
    top-=(imm[pc]>>31);
}

inline void nasal_vm::o_meq()
{
    // pop old memr[0] and replace it
    // the reason why we should get memr and push the old value on stack
    // is that when lnkeq/lnkeqc is called, there will be
    // a new gc object vm_str which is returned by gc::alloc
    // this may cause gc, so we should temporarily put it on stack
    memr[0]=top[-1];
    memr=nullptr;
    top-=imm[pc]+1;
}
inline void nasal_vm::o_eq()
{
    nas_ref val2=top[0];
    nas_ref val1=(--top)[0];
    if(val1.type==vm_nil && val2.type==vm_nil)
        top[0]=one;
    else if(val1.type==vm_str && val2.type==vm_str)
        top[0]=(val1.str()==val2.str())?one:zero;
    else if((val1.type==vm_num || val2.type==vm_num)
        && val1.type!=vm_nil && val2.type!=vm_nil)
        top[0]=(val1.tonum()==val2.tonum())?one:zero;
    else
        top[0]=(val1==val2)?one:zero;
}
inline void nasal_vm::o_neq()
{
    nas_ref val2=top[0];
    nas_ref val1=(--top)[0];
    if(val1.type==vm_nil && val2.type==vm_nil)
        top[0]=zero;
    else if(val1.type==vm_str && val2.type==vm_str)
        top[0]=(val1.str()!=val2.str())?one:zero;
    else if((val1.type==vm_num || val2.type==vm_num)
        && val1.type!=vm_nil && val2.type!=vm_nil)
        top[0]=(val1.tonum()!=val2.tonum())?one:zero;
    else
        top[0]=(val1!=val2)?one:zero;
}

#define op_cmp(type)\
    --top;\
    top[0]=(top[0].tonum() type top[1].tonum())?one:zero;

inline void nasal_vm::o_less(){op_cmp(<);}
inline void nasal_vm::o_leq(){op_cmp(<=);}
inline void nasal_vm::o_grt(){op_cmp(>);}
inline void nasal_vm::o_geq(){op_cmp(>=);}

#define op_cmp_const(type)\
    top[0]=(top[0].tonum() type num_table[imm[pc]])?one:zero;

inline void nasal_vm::o_lessc(){op_cmp_const(<);}
inline void nasal_vm::o_leqc(){op_cmp_const(<=);}
inline void nasal_vm::o_grtc(){op_cmp_const(>);}
inline void nasal_vm::o_geqc(){op_cmp_const(>=);}

inline void nasal_vm::o_pop()
{
    --top;
}
inline void nasal_vm::o_jmp()
{
    pc=imm[pc]-1;
}
inline void nasal_vm::o_jt()
{
    if(condition(top[0]))
        pc=imm[pc]-1;
}
inline void nasal_vm::o_jf()
{
    if(!condition(top[0]))
        pc=imm[pc]-1;
    --top;
}
inline void nasal_vm::o_cnt()
{
    if(top[0].type!=vm_vec)
        die("must use vector in forindex/foreach");
    (++top)[0]={vm_cnt,(i64)-1};
}
inline void nasal_vm::o_findex()
{
    if((usize)(++top[0].cnt())>=top[-1].vec().size())
    {
        pc=imm[pc]-1;
        return;
    }
    top[1]={vm_num,(f64)top[0].cnt()};
    ++top;
}
inline void nasal_vm::o_feach()
{
    std::vector<nas_ref>& ref=top[-1].vec().elems;
    if((usize)(++top[0].cnt())>=ref.size())
    {
        pc=imm[pc]-1;
        return;
    }
    top[1]=ref[top[0].cnt()];
    ++top;
}
inline void nasal_vm::o_callg()
{
    (++top)[0]=stack[imm[pc]];
}
inline void nasal_vm::o_calll()
{
    (++top)[0]=localr[imm[pc]];
}
inline void nasal_vm::o_upval()
{
    (++top)[0]=funcr.func().upval[(imm[pc]>>16)&0xffff]
                    .upval()[imm[pc]&0xffff];
}
inline void nasal_vm::o_callv()
{
    nas_ref val=top[0];
    nas_ref vec=(--top)[0];
    if(vec.type==vm_vec)
    {
        top[0]=vec.vec().get_val(val.tonum());
        if(top[0].type==vm_none)
            die("index out of range:"+std::to_string(val.tonum()));
    }
    else if(vec.type==vm_hash)
    {
        if(val.type!=vm_str)
            die("must use string as the key");
        top[0]=vec.hash().get_val(val.str());
        if(top[0].type==vm_none)
            die("cannot find member \""+val.str()+"\"");
        if(top[0].type==vm_func)
            top[0].func().local[0]=val;// 'me'
    }
    else if(vec.type==vm_str)
    {
        string& str=vec.str();
        i32 num=val.tonum();
        i32 len=str.length();
        if(num<-len || num>=len)
            die("index out of range:"+std::to_string(val.tonum()));
        top[0]={vm_num,f64((u8)str[num>=0? num:num+len])};
    }
    else
        die("must call a vector/hash/string");
}
inline void nasal_vm::o_callvi()
{
    nas_ref val=top[0];
    if(val.type!=vm_vec)
        die("must use a vector");

    // cannot use operator[],because this may cause overflow
    (++top)[0]=val.vec().get_val(imm[pc]);
    if(top[0].type==vm_none)
        die("index out of range:"+std::to_string(imm[pc]));
}
inline void nasal_vm::o_callh()
{
    nas_ref val=top[0];
    if(val.type!=vm_hash)
        die("must call a hash");

    top[0]=val.hash().get_val(str_table[imm[pc]]);
    if(top[0].type==vm_none)
        die("member \""+str_table[imm[pc]]+"\" does not exist");

    if(top[0].type==vm_func)
        top[0].func().local[0]=val;// 'me'
}
inline void nasal_vm::o_callfv()
{
    u32 argc=imm[pc]; // arguments counter
    nas_ref* local=top-argc+1; // arguments begin address
    if(local[-1].type!=vm_func)
        die("must call a function");
    
    auto& func=local[-1].func();
    nas_ref tmp=local[-1];
    local[-1]=funcr;
    funcr=tmp;
    // top-argc+lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if(top-argc+func.lsize+3>=canary)
        die("stack overflow");
    // parameter size is func->psize-1, 1 is reserved for "me"
    u32 psize=func.psize-1;
    if(argc<psize && func.local[argc+1].type==vm_none)
        die("lack argument(s)");

    nas_ref dynamic=nil;
    top=local+func.lsize;
    if(func.dpara>=0)// load dynamic arguments
    {
        dynamic=gc.alloc(vm_vec);
        for(u32 i=psize;i<argc;++i)
            dynamic.vec().elems.push_back(local[i]);
    }
#ifdef _MSC_VER
    u32 min_size=(std::min)(psize,argc);
#else
    u32 min_size=std::min(psize,argc);
#endif
    for(u32 i=min_size;i>=1;--i)// load arguments
        local[i]=local[i-1];
    local[0]=func.local[0];// load "me"
    // load local scope & default arguments
    for(u32 i=min_size+1;i<func.lsize;++i)
        local[i]=func.local[i];
    if(func.dpara>=0)
        local[psize+1]=dynamic;

    top[0]=upvalr;
    (++top)[0]={vm_addr,localr};
    (++top)[0]={vm_ret,pc};
    pc=func.entry-1;
    localr=local;
    upvalr=nil;
}
inline void nasal_vm::o_callfh()
{
    auto& hash=top[0].hash().elems;
    if(top[-1].type!=vm_func)
        die("must call a function");

    auto& func=top[-1].func();
    nas_ref tmp=top[-1];
    top[-1]=funcr;
    funcr=tmp;
    // top -1(hash) +lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if(top+func.lsize+2>=canary)
        die("stack overflow");
    if(func.dpara>=0)
        die("special call cannot use dynamic argument");

    nas_ref* local=top;
    top+=func.lsize;
    for(u32 i=0;i<func.lsize;++i)
        local[i]=func.local[i];
    
    for(auto& i:func.keys)
    {
        const string& key=str_table[i.first];
        if(hash.count(key))
            local[i.second]=hash[key];
        else if(local[i.second].type==vm_none)
            die("lack argument(s): \""+key+"\"");
    }

    top[0]=upvalr;
    (++top)[0]={vm_addr,localr};
    (++top)[0]={vm_ret,pc}; // rewrite top with vm_ret
    pc=func.entry-1;
    localr=local;
    upvalr=nil;
}
inline void nasal_vm::o_callb()
{
    // reserve place for builtin function return,
    // in fact this code is changed because of coroutine
    (++top)[0]=nil;
    // this ++top should not be used like: (++top)[0] here
    // because if running a builtin function about coroutine
    // (top) will be set to another context.top, instead of main_context.top
    top[0]=(*builtin[imm[pc]].func)(localr,gc);
    if(top[0].type==vm_none)
        die("native function error");
}
inline void nasal_vm::o_slcbeg()
{
    // +--------------+
    // | slice_vector | <-- top[0]
    // +--------------+
    // | resource_vec | <-- top[-1]
    // +--------------+
    (++top)[0]=gc.alloc(vm_vec);
    if(top[-1].type!=vm_vec)
        die("must slice a vector");
}
inline void nasal_vm::o_slcend()
{
    top[-1]=top[0];
    --top;
}
inline void nasal_vm::o_slc()
{
    nas_ref val=(top--)[0];
    nas_ref res=top[-1].vec().get_val(val.tonum());
    if(res.type==vm_none)
        die("index out of range:"+std::to_string(val.tonum()));
    top[0].vec().elems.push_back(res);
}
inline void nasal_vm::o_slc2()
{
    nas_ref val2=(top--)[0];
    nas_ref val1=(top--)[0];
    std::vector<nas_ref>& ref=top[-1].vec().elems;
    std::vector<nas_ref>& aim=top[0].vec().elems;

    u8 type1=val1.type,type2=val2.type;
    i32 num1=val1.tonum();
    i32 num2=val2.tonum();
    i32 size=ref.size();
    if(type1==vm_nil && type2==vm_nil)
    {
        num1=0;
        num2=size-1;
    }
    else if(type1==vm_nil && type2!=vm_nil)
        num1=num2<0? -size:0;
    else if(type1!=vm_nil && type2==vm_nil)
        num2=num1<0? -1:size-1;

    if(num1>num2)
        die("begin index must be less than or equal to end index");
    else if(num1<-size || num1>=size)
        die("begin index out of range: "+std::to_string(num1));
    else if(num2<-size || num2>=size)
        die("end index out of range: "+std::to_string(num2));
    else
        for(i32 i=num1;i<=num2;++i)
            aim.push_back(i>=0?ref[i]:ref[i+size]);
}
inline void nasal_vm::o_mcallg()
{
    memr=stack+imm[pc];
    (++top)[0]=memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}
inline void nasal_vm::o_mcalll()
{
    memr=localr+imm[pc];
    (++top)[0]=memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}
inline void nasal_vm::o_mupval()
{
    memr=&(funcr.func().upval[(imm[pc]>>16)&0xffff].upval()[imm[pc]&0xffff]);
    (++top)[0]=memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}
inline void nasal_vm::o_mcallv()
{
    nas_ref val=top[0];     // index
    nas_ref vec=(--top)[0]; // mcall vector, reserved on stack to avoid gc
    if(vec.type==vm_vec)
    {
        memr=vec.vec().get_mem(val.tonum());
        if(!memr)
            die("index out of range:"+std::to_string(val.tonum()));
    }
    else if(vec.type==vm_hash) // do mcallh but use the mcallv way
    {
        if(val.type!=vm_str)
            die("must use string as the key");
        nas_hash& ref=vec.hash();
        string& str=val.str();
        memr=ref.get_mem(str);
        if(!memr)
        {
            ref.elems[str]=nil;
            memr=ref.get_mem(str);
        }
    }
    else
        die("cannot get memory space in this type");
}
inline void nasal_vm::o_mcallh()
{
    nas_ref hash=top[0]; // mcall hash, reserved on stack to avoid gc
    if(hash.type!=vm_hash)
        die("must call a hash");
    nas_hash& ref=hash.hash();
    const string& str=str_table[imm[pc]];
    memr=ref.get_mem(str);
    if(!memr) // create a new key
    {
        ref.elems[str]=nil;
        memr=ref.get_mem(str);
    }
}
inline void nasal_vm::o_ret()
{
/*  +-------------+
*   | return value| <- top[0]
*   +-------------+
*   | old pc      | <- top[-1]
*   +-------------+
*   | old localr  | <- top[-2]
*   +-------------+
*   | old upvalr  | <- top[-3]
*   +-------------+
*   | local scope |
*   | ...         |
*   +-------------+ <- local pointer stored in localr
*   | old funcr   | <- old function stored in funcr
*   +-------------+
*/
    nas_ref  ret  =top[0];
    nas_ref* local=localr;
    nas_ref  func =funcr;
    nas_ref  up   =upvalr;

    pc    =top[-1].ret();
    localr=top[-2].addr();
    upvalr=top[-3];

    top=local-1;
    funcr=top[0];
    top[0]=ret; // rewrite func with returned value

    if(up.type==vm_upval) // synchronize upvalue
    {
        auto& upval=up.upval();
        auto size=func.func().lsize;
        upval.onstk=false;
        for(u32 i=0;i<size;++i)
            upval.elems.push_back(local[i]);
    }
    // cannot use gc.cort to judge,
    // because there maybe another function call inside
    if(!pc)
        gc.ctxreserve();
}
void nasal_vm::run(
    const nasal_codegen& gen,
    const nasal_import& linker,
    const std::vector<string>& argv,
    const bool detail)
{
    detail_info=detail;
    init(gen.strs(),gen.nums(),gen.codes(),linker.filelist(),argv);
#ifndef _MSC_VER
    const void* oprs[]=
    {
        &&vmexit, &&intg,   &&intl,   &&loadg,
        &&loadl,  &&loadu,  &&pnum,   &&pnil,
        &&pstr,   &&newv,   &&newh,   &&newf,
        &&happ,   &&para,   &&deft,   &&dyn,
        &&unot,   &&usub,   &&add,    &&sub,
        &&mul,    &&div,    &&lnk,    &&addc,
        &&subc,   &&mulc,   &&divc,   &&lnkc,
        &&addeq,  &&subeq,  &&muleq,  &&diveq,
        &&lnkeq,  &&addeqc, &&subeqc, &&muleqc,
        &&diveqc, &&lnkeqc, &&meq,    &&eq,
        &&neq,    &&less,   &&leq,    &&grt,
        &&geq,    &&lessc,  &&leqc,   &&grtc,
        &&geqc,   &&pop,    &&jmp,    &&jt,
        &&jf,     &&cnt,    &&findex, &&feach,
        &&callg,  &&calll,  &&upval,  &&callv,
        &&callvi, &&callh,  &&callfv, &&callfh,
        &&callb,  &&slcbeg, &&slcend, &&slc,
        &&slc2,   &&mcallg, &&mcalll, &&mupval,
        &&mcallv, &&mcallh, &&ret
    };
    std::vector<const void*> code;
    for(auto& i:gen.codes())
    {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    // goto the first operand
    goto *code[pc];
#else
    typedef void (nasal_vm::*nafunc)();
    const nafunc oprs[]=
    {
        nullptr,             &nasal_vm::o_intg,
        &nasal_vm::o_intl,   &nasal_vm::o_loadg,
        &nasal_vm::o_loadl,  &nasal_vm::o_loadu,
        &nasal_vm::o_pnum,   &nasal_vm::o_pnil,
        &nasal_vm::o_pstr,   &nasal_vm::o_newv,
        &nasal_vm::o_newh,   &nasal_vm::o_newf,
        &nasal_vm::o_happ,   &nasal_vm::o_para,
        &nasal_vm::o_deft,   &nasal_vm::o_dyn,
        &nasal_vm::o_unot,   &nasal_vm::o_usub,
        &nasal_vm::o_add,    &nasal_vm::o_sub,
        &nasal_vm::o_mul,    &nasal_vm::o_div,
        &nasal_vm::o_lnk,    &nasal_vm::o_addc,
        &nasal_vm::o_subc,   &nasal_vm::o_mulc,
        &nasal_vm::o_divc,   &nasal_vm::o_lnkc,
        &nasal_vm::o_addeq,  &nasal_vm::o_subeq,
        &nasal_vm::o_muleq,  &nasal_vm::o_diveq,
        &nasal_vm::o_lnkeq,  &nasal_vm::o_addeqc,
        &nasal_vm::o_subeqc, &nasal_vm::o_muleqc,
        &nasal_vm::o_diveqc, &nasal_vm::o_lnkeqc,
        &nasal_vm::o_meq,    &nasal_vm::o_eq,
        &nasal_vm::o_neq,    &nasal_vm::o_less,
        &nasal_vm::o_leq,    &nasal_vm::o_grt,
        &nasal_vm::o_geq,    &nasal_vm::o_lessc,
        &nasal_vm::o_leqc,   &nasal_vm::o_grtc,
        &nasal_vm::o_geqc,   &nasal_vm::o_pop,
        &nasal_vm::o_jmp,    &nasal_vm::o_jt,
        &nasal_vm::o_jf,     &nasal_vm::o_cnt,
        &nasal_vm::o_findex, &nasal_vm::o_feach,
        &nasal_vm::o_callg,  &nasal_vm::o_calll,
        &nasal_vm::o_upval,  &nasal_vm::o_callv,
        &nasal_vm::o_callvi, &nasal_vm::o_callh,
        &nasal_vm::o_callfv, &nasal_vm::o_callfh,
        &nasal_vm::o_callb,  &nasal_vm::o_slcbeg,
        &nasal_vm::o_slcend, &nasal_vm::o_slc,
        &nasal_vm::o_slc2,   &nasal_vm::o_mcallg,
        &nasal_vm::o_mcalll, &nasal_vm::o_mupval,
        &nasal_vm::o_mcallv, &nasal_vm::o_mcallh,
        &nasal_vm::o_ret
    };
    std::vector<nafunc> code;
    for(auto& i:gen.codes())
    {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    while(code[pc]){
        (this->*code[pc])();
        if(top>=canary)
            break;
        ++pc;
    }
#endif

vmexit:
    if(top>=canary)
        die("stack overflow");
    if(detail)
        gc.info();
    gc.clear();
    imm.clear();
    return;

#ifndef _MSC_VER
// may cause stackoverflow
#define exec_check(op) {\
        op();\
        if(top<canary)\
            goto *code[++pc];\
        goto vmexit;\
    }
// do not cause stackoverflow
#define exec_nodie(op) {op();goto *code[++pc];}

intg:   exec_nodie(o_intg  ); // +imm[pc] (detected at codegen)
intl:   exec_nodie(o_intl  ); // -0
loadg:  exec_nodie(o_loadg ); // -1
loadl:  exec_nodie(o_loadl ); // -1
loadu:  exec_nodie(o_loadu ); // -1
pnum:   exec_check(o_pnum  ); // +1
pnil:   exec_check(o_pnil  ); // +1
pstr:   exec_check(o_pstr  ); // +1
newv:   exec_check(o_newv  ); // +1-imm[pc]
newh:   exec_check(o_newh  ); // +1
newf:   exec_check(o_newf  ); // +1
happ:   exec_nodie(o_happ  ); // -1
para:   exec_nodie(o_para  ); // -0
deft:   exec_nodie(o_deft  ); // -1
dyn:    exec_nodie(o_dyn   ); // -0
unot:   exec_nodie(o_unot  ); // -0
usub:   exec_nodie(o_usub  ); // -0
add:    exec_nodie(o_add   ); // -1
sub:    exec_nodie(o_sub   ); // -1
mul:    exec_nodie(o_mul   ); // -1
div:    exec_nodie(o_div   ); // -1
lnk:    exec_nodie(o_lnk   ); // -1
addc:   exec_nodie(o_addc  ); // -0
subc:   exec_nodie(o_subc  ); // -0
mulc:   exec_nodie(o_mulc  ); // -0
divc:   exec_nodie(o_divc  ); // -0
lnkc:   exec_nodie(o_lnkc  ); // -0
addeq:  exec_nodie(o_addeq ); // -1
subeq:  exec_nodie(o_subeq ); // -1
muleq:  exec_nodie(o_muleq ); // -1
diveq:  exec_nodie(o_diveq ); // -1
lnkeq:  exec_nodie(o_lnkeq ); // -1
addeqc: exec_nodie(o_addeqc); // -0
subeqc: exec_nodie(o_subeqc); // -0
muleqc: exec_nodie(o_muleqc); // -0
diveqc: exec_nodie(o_diveqc); // -0
lnkeqc: exec_nodie(o_lnkeqc); // -0
meq:    exec_nodie(o_meq   ); // -1
eq:     exec_nodie(o_eq    ); // -1
neq:    exec_nodie(o_neq   ); // -1
less:   exec_nodie(o_less  ); // -1
leq:    exec_nodie(o_leq   ); // -1
grt:    exec_nodie(o_grt   ); // -1
geq:    exec_nodie(o_geq   ); // -1
lessc:  exec_nodie(o_lessc ); // -0
leqc:   exec_nodie(o_leqc  ); // -0
grtc:   exec_nodie(o_grtc  ); // -0
geqc:   exec_nodie(o_geqc  ); // -0
pop:    exec_nodie(o_pop   ); // -1
jmp:    exec_nodie(o_jmp   ); // -0
jt:     exec_nodie(o_jt    ); // -0
jf:     exec_nodie(o_jf    ); // -1
cnt:    exec_nodie(o_cnt   ); // -0
findex: exec_check(o_findex); // +1
feach:  exec_check(o_feach ); // +1
callg:  exec_check(o_callg ); // +1
calll:  exec_check(o_calll ); // +1
upval:  exec_check(o_upval ); // +1
callv:  exec_nodie(o_callv ); // -0
callvi: exec_nodie(o_callvi); // -0
callh:  exec_nodie(o_callh ); // -0
callfv: exec_nodie(o_callfv); // check in the function
callfh: exec_nodie(o_callfh); // check in the function
callb:  exec_nodie(o_callb ); // -0
slcbeg: exec_check(o_slcbeg); // +1
slcend: exec_nodie(o_slcend); // -1
slc:    exec_nodie(o_slc   ); // -1
slc2:   exec_nodie(o_slc2  ); // -2
mcallg: exec_check(o_mcallg); // +1
mcalll: exec_check(o_mcalll); // +1
mupval: exec_check(o_mupval); // +1
mcallv: exec_nodie(o_mcallv); // -0
mcallh: exec_nodie(o_mcallh); // -0
ret:    exec_nodie(o_ret   ); // -2
#endif
}
#endif