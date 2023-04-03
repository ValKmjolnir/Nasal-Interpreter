#pragma once

#include <iomanip>
#include <stack>

#include "nasal_import.h"
#include "nasal_gc.h"
#include "nasal_codegen.h"

class vm {
protected:

    /* registers and constants of vm */
    u32  pc;     // program counter
    var* localr; // local frame pointer
    var* memr;   // store address got by memory call
    var funcr;   // function register
    var upvalr;  // upvalue register
    var* canary; // avoid stack overflow, at the top of main stack
    var* top;    // stack top pointer

    /* constants */
    const f64* cnum;      // constant numbers
    const string* cstr;   // constant symbols and strings
    std::vector<u32> imm; // immediate number table
    
    /* garbage collector */
    gc ngc;

    /* main stack */
    var stack[STACK_DEPTH];

    /* values used for debugger */
    const string* files;    // file name list
    const opcode* bytecode; // bytecode buffer address

    /* vm initializing function */
    void init(
        const std::vector<string>&,
        const std::vector<f64>&,
        const std::vector<opcode>&,
        const std::vector<string>&,
        const std::vector<string>&);

    /* debug functions */
    bool verbose;
    void valinfo(var&);
    void traceback();
    void stackinfo(const u32);
    void reginfo();
    void gstate();
    void lstate();
    void ustate();
    void detail();
    void die(const string&);

    /* vm calculation functions*/
    bool cond(var&);

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
    void o_lnot();
    void o_usub();
    void o_bnot();
    void o_btor();
    void o_btxor();
    void o_btand();
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
    void o_bandeq();
    void o_boreq();
    void o_bxoreq();
    void o_addeqc();
    void o_subeqc();
    void o_muleqc();
    void o_diveqc();
    void o_lnkeqc();
    void o_addecp();
    void o_subecp();
    void o_mulecp();
    void o_divecp();
    void o_lnkecp();
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

    /* constructor of vm instance */
    vm():
        pc(0),localr(nullptr),memr(nullptr),funcr(nil),
        upvalr(nil),canary(nullptr),top(stack),
        cnum(nullptr),cstr(nullptr),
        ngc(pc,localr,memr,funcr,upvalr,canary,top,stack),
        files(nullptr),bytecode(nullptr),verbose(false) {}

    /* execution entry */
    void run(
        const codegen&,
        const linker&,
        const std::vector<string>&,
        const bool);
};

void vm::init(
    const std::vector<string>& strs,
    const std::vector<f64>&    nums,
    const std::vector<opcode>& code,
    const std::vector<string>& filenames,
    const std::vector<string>& argv) {
    ngc.init(strs,argv);
    cnum=nums.data();
    cstr=strs.data();
    bytecode=code.data();
    files=filenames.data();

    /* set canary and program counter */
    pc=0;
    localr=memr=nullptr;
    funcr=upvalr=nil;
    canary=stack+STACK_DEPTH-1; // stack[STACK_DEPTH-1]
    top=stack;

    /* clear main stack */
    for(u32 i=0;i<STACK_DEPTH;++i) {
        stack[i]=nil;
    }
}

void vm::valinfo(var& val) {
    const nas_val* p=val.val.gcobj;
    switch(val.type) {
        case vm_none: std::clog<<"| null |";break;
        case vm_ret:  std::clog<<"| pc   | 0x"<<std::hex
                               <<val.ret()<<std::dec;break;
        case vm_addr: std::clog<<"| addr | 0x"<<std::hex
                               <<(u64)val.addr()<<std::dec;break;
        case vm_cnt:  std::clog<<"| cnt  | "<<val.cnt();break;
        case vm_nil:  std::clog<<"| nil  |";break;
        case vm_num:  std::clog<<"| num  | "<<val.num();break;
        case vm_str:  std::clog<<"| str  | <0x"<<std::hex<<(u64)p
                               <<"> "<<rawstr(val.str(),16)<<std::dec;break;
        case vm_func: std::clog<<"| func | <0x"<<std::hex<<(u64)p
                               <<"> entry:0x"<<val.func().entry
                               <<std::dec;break;
        case vm_upval:std::clog<<"| upval| <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> ["<<val.upval().size
                               <<" val]";break;
        case vm_vec:  std::clog<<"| vec  | <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> ["<<val.vec().size()
                               <<" val]";break;
        case vm_hash: std::clog<<"| hash | <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> {"<<val.hash().size()
                               <<" val}";break;
        case vm_obj:  std::clog<<"| obj  | <0x"<<std::hex<<(u64)p
                               <<"> obj:0x"<<(u64)val.obj().ptr
                               <<std::dec;break;
        case vm_co:   std::clog<<"| co   | <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> coroutine";break;
        default:      std::clog<<"| err  | <0x"<<std::hex<<(u64)p
                               <<std::dec<<"> unknown object";break;
    }
    std::clog<<"\n";
}

void vm::traceback() {
    /* bytecode[0].num is the global size */
    var* bottom=ngc.stack==stack?stack+bytecode[0].num:ngc.stack;
    var* ctx_top=ngc.stack==stack?top:ngc.top;
    std::stack<u32> ret;
    for(var* i=bottom;i<=ctx_top;++i) {
        if (i->type==vm_ret && i->ret()!=0) {
            ret.push(i->ret());
        }
    }
    ret.push(pc); // store the position program crashed
    std::clog<<"trace back ("<<(ngc.stack==stack?"main":"coroutine")<<")\n";
    codestream::set(cnum,cstr,files);
    for(u32 p=0,same=0,prev=0xffffffff;!ret.empty();prev=p,ret.pop()) {
        if ((p=ret.top())==prev) {
            ++same;
            continue;
        }
        if (same) {
            std::clog
            <<"  0x"<<std::hex<<std::setw(6)<<std::setfill('0')
            <<prev<<std::dec<<"     "<<same<<" same call(s)\n";
        }
        same=0;
        std::clog<<"  "<<codestream(bytecode[p],p)<<"\n";
    }
    // the first called place has no same calls
}

void vm::stackinfo(const u32 limit=10) {
    /* bytecode[0].num is the global size */
    const u32 gsize=ngc.stack==stack?bytecode[0].num:0;
    var* t=top;
    var* bottom=ngc.stack+gsize;
    std::clog<<"stack (0x"<<std::hex<<(u64)bottom<<std::dec
             <<" <+"<<gsize<<">, limit "<<limit<<", total "
             <<(t<bottom? 0:(i64)(t-bottom+1))<<")\n";
    for(u32 i=0;i<limit && t>=bottom;++i,--t) {
        std::clog<<"  0x"<<std::hex
                 <<std::setw(6)<<std::setfill('0')
                 <<(u64)(t-ngc.stack)<<std::dec
                 <<"    ";
        valinfo(t[0]);
    }
}

void vm::reginfo() {
    std::clog<<"registers ("<<(ngc.cort?"coroutine":"main")<<")\n"<<std::hex
             <<"  [pc    ]    | pc   | 0x"<<pc<<"\n"
             <<"  [global]    | addr | 0x"<<(u64)stack<<"\n"
             <<"  [local ]    | addr | 0x"<<(u64)localr<<"\n"
             <<"  [memr  ]    | addr | 0x"<<(u64)memr<<"\n"
             <<"  [canary]    | addr | 0x"<<(u64)canary<<"\n"
             <<"  [top   ]    | addr | 0x"<<(u64)top<<"\n"
             <<std::dec;
    std::clog<<"  [funcr ]    ";valinfo(funcr);
    std::clog<<"  [upval ]    ";valinfo(upvalr);
}

void vm::gstate() {
    if (!bytecode[0].num || stack[0].type==vm_none) { // bytecode[0].op is op_intg
        return;
    }
    std::clog<<"global (0x"<<std::hex<<(u64)stack<<" <+0>)\n"<<std::dec;
    for(u32 i=0;i<bytecode[0].num;++i) {
        std::clog<<"  0x"<<std::hex<<std::setw(6)
                 <<std::setfill('0')<<i<<std::dec
                 <<"    ";
        valinfo(stack[i]);
    }
}

void vm::lstate() {
    if (!localr || !funcr.func().lsize) {
        return;
    }
    const u32 lsize=funcr.func().lsize;
    std::clog<<"local (0x"<<std::hex<<(u64)localr
             <<" <+"<<(u64)(localr-ngc.stack)<<">)\n"<<std::dec;
    for(u32 i=0;i<lsize;++i) {
        std::clog<<"  0x"<<std::hex<<std::setw(6)
                 <<std::setfill('0')<<i<<std::dec
                 <<"    ";
        valinfo(localr[i]);
    }
}

void vm::ustate() {
    if (funcr.type==vm_nil || funcr.func().upval.empty()) {
        return;
    }
    std::clog<<"upvalue\n";
    auto& upval=funcr.func().upval;
    for(u32 i=0;i<upval.size();++i) {
        std::clog<<"  -> upval["<<i<<"]:\n";
        auto& uv=upval[i].upval();
        for(u32 j=0;j<uv.size;++j) {
            std::clog<<"     0x"<<std::hex<<std::setw(6)
                     <<std::setfill('0')<<j<<std::dec
                     <<" ";
            valinfo(uv[j]);
        }
    }
}

void vm::detail() {
    reginfo();
    gstate();
    lstate();
    ustate();
}

void vm::die(const string& str) {
    std::cerr<<"[vm] error: "<<str<<"\n";
    traceback();
    stackinfo();

    // show verbose crash info
    if (verbose) {
        detail();
    }

    if (ngc.stack==stack) {
        // in main context, exit directly
        std::exit(1);
    } else {
        // in coroutine, shut down the coroutine
        // and return to main context
        pc=0; // mark coroutine 'dead'
        ngc.ctxreserve();
        top[0]=nil;
    }
}

inline bool vm::cond(var& val) {
    if (val.type==vm_num) {
        return val.num();
    } else if (val.type==vm_str) {
        const f64 num=str2num(val.str().c_str());
        return std::isnan(num)? !val.str().empty():num;
    }
    return false;
}

void vm::o_intg() {
    // global values store on stack
    top+=imm[pc];
    --top;// point to the top
}

void vm::o_intl() {
    top[0].func().local.resize(imm[pc],nil);
    top[0].func().lsize=imm[pc];
}

void vm::o_loadg() {
    stack[imm[pc]]=(top--)[0];
}

void vm::o_loadl() {
    localr[imm[pc]]=(top--)[0];
}

void vm::o_loadu() {
    funcr.func().upval[(imm[pc]>>16)&0xffff]
         .upval()[imm[pc]&0xffff]=(top--)[0];
}

void vm::o_pnum() {
    (++top)[0]=var::num(cnum[imm[pc]]);
}

void vm::o_pnil() {
    (++top)[0]=nil;
}

void vm::o_pstr() {
    (++top)[0]=ngc.strs[imm[pc]];
}

void vm::o_newv() {
    var newv=ngc.alloc(vm_vec);
    auto& vec=newv.vec().elems;
    vec.resize(imm[pc]);
    // use top-=imm[pc]-1 here will cause error if imm[pc] is 0
    top=top-imm[pc]+1;
    for(u32 i=0;i<imm[pc];++i) {
        vec[i]=top[i];
    }
    top[0]=newv;
}

void vm::o_newh() {
    (++top)[0]=ngc.alloc(vm_hash);
}

void vm::o_newf() {
    (++top)[0]=ngc.alloc(vm_func);
    nas_func& func=top[0].func();
    func.entry=imm[pc];
    func.psize=1;

    /* this means you create a new function in local scope */
    if (localr) {
        func.upval=funcr.func().upval;
        // function created in the same local scope shares one closure
        // so this size & stk setting has no problem
        var upval=(upvalr.type==vm_nil)?ngc.alloc(vm_upval):upvalr;
        upval.upval().size=funcr.func().lsize;
        upval.upval().stk=localr;
        func.upval.push_back(upval);
        upvalr=upval;
    }
}

void vm::o_happ() {
    top[-1].hash().elems[cstr[imm[pc]]]=top[0];
    --top;
}

void vm::o_para() {
    nas_func& func=top[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[imm[pc]]=func.psize;
    func.local[func.psize++]=var::none();
}

void vm::o_deft() {
    var val=top[0];
    nas_func& func=(--top)[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[imm[pc]]=func.psize;
    func.local[func.psize++]=val;
}

void vm::o_dyn() {
    top[0].func().dpara=imm[pc];
}

void vm::o_lnot() {
    var val=top[0];
    switch(val.type) {
        case vm_nil:top[0]=one;break;
        case vm_num:top[0]=val.num()?zero:one;break;
        case vm_str:{
            const f64 num=str2num(val.str().c_str());
            if (std::isnan(num)) {
                top[0]=var::num((f64)val.str().empty());
            } else {
                top[0]=num?zero:one;
            }
        } break;
        default:{
            die("incorrect value type");
            return;
        } break;
    }
}

void vm::o_usub() {
    top[0]=var::num(-top[0].tonum());
}

void vm::o_bnot() {
    top[0]=var::num(~static_cast<int32_t>(top[0].num()));
}

void vm::o_btor() {
    top[-1]=var::num(static_cast<int32_t>(top[-1].tonum())|static_cast<int32_t>(top[0].tonum()));
    --top;
}

void vm::o_btxor() {
    top[-1]=var::num(static_cast<int32_t>(top[-1].tonum())^static_cast<int32_t>(top[0].tonum()));
    --top;
}

void vm::o_btand() {
    top[-1]=var::num(static_cast<int32_t>(top[-1].tonum())&static_cast<int32_t>(top[0].tonum()));
    --top;
}

#define op_calc(type)\
    top[-1]=var::num(top[-1].tonum() type top[0].tonum());\
    --top;

void vm::o_add() {op_calc(+);}
void vm::o_sub() {op_calc(-);}
void vm::o_mul() {op_calc(*);}
void vm::o_div() {op_calc(/);}
void vm::o_lnk() {
    top[-1]=ngc.newstr(top[-1].tostr()+top[0].tostr());
    --top;
}

#define op_calc_const(type)\
    top[0]=var::num(top[0].tonum() type cnum[imm[pc]]);

void vm::o_addc() {op_calc_const(+);}
void vm::o_subc() {op_calc_const(-);}
void vm::o_mulc() {op_calc_const(*);}
void vm::o_divc() {op_calc_const(/);}
void vm::o_lnkc() {
    top[0]=ngc.newstr(top[0].tostr()+cstr[imm[pc]]);
}

// top[0] stores the value of memr[0], to avoid being garbage-collected
// so when the calculation ends, top-=1, then top-=imm[pc]
// because this return value is meaningless if on stack when imm[pc]=1
// like this: func{a+=c;}(); the result of 'a+c' will no be used later, imm[pc]=1
// but if b+=a+=c; the result of 'a+c' will be used later, imm[pc]=0
#define op_calc_eq(type)\
    top[-1]=memr[0]=var::num(memr[0].tonum() type top[-1].tonum());\
    memr=nullptr;\
    top-=imm[pc]+1;

void vm::o_addeq() {op_calc_eq(+);}
void vm::o_subeq() {op_calc_eq(-);}
void vm::o_muleq() {op_calc_eq(*);}
void vm::o_diveq() {op_calc_eq(/);}
void vm::o_lnkeq() {
    top[-1]=memr[0]=ngc.newstr(memr[0].tostr()+top[-1].tostr());
    memr=nullptr;
    top-=imm[pc]+1;
}

void vm::o_bandeq() {
    top[-1]=memr[0]=var::num(i32(memr[0].tonum())&i32(top[-1].tonum()));
    memr=nullptr;
    top-=imm[pc]+1;
}

void vm::o_boreq() {
    top[-1]=memr[0]=var::num(i32(memr[0].tonum())|i32(top[-1].tonum()));
    memr=nullptr;
    top-=imm[pc]+1;
}

void vm::o_bxoreq() {
    top[-1]=memr[0]=var::num(i32(memr[0].tonum())^i32(top[-1].tonum()));
    memr=nullptr;
    top-=imm[pc]+1;
}

// top[0] stores the value of memr[0], to avoid being garbage-collected
// so when the calculation ends, top-=imm[pc]>>31
// because this return value is meaningless if on stack when imm[pc]>>31=1
// like this: func{a+=1;}(); the result of 'a+1' will no be used later, imm[pc]>>31=1
// but if b+=a+=1; the result of 'a+1' will be used later, imm[pc]>>31=0
#define op_calc_eq_const(type)\
    top[0]=memr[0]=var::num(memr[0].tonum() type cnum[imm[pc]]);\
    memr=nullptr;

void vm::o_addeqc() {op_calc_eq_const(+);}
void vm::o_subeqc() {op_calc_eq_const(-);}
void vm::o_muleqc() {op_calc_eq_const(*);}
void vm::o_diveqc() {op_calc_eq_const(/);}
void vm::o_lnkeqc() {
    top[0]=memr[0]=ngc.newstr(memr[0].tostr()+cstr[imm[pc]]);
    memr=nullptr;
}

#define op_calc_eq_const_and_pop(type)\
    top[0]=memr[0]=var::num(memr[0].tonum() type cnum[imm[pc]]);\
    memr=nullptr;\
    --top;

void vm::o_addecp() {op_calc_eq_const_and_pop(+);}
void vm::o_subecp() {op_calc_eq_const_and_pop(-);}
void vm::o_mulecp() {op_calc_eq_const_and_pop(*);}
void vm::o_divecp() {op_calc_eq_const_and_pop(/);}
void vm::o_lnkecp() {
    top[0]=memr[0]=ngc.newstr(memr[0].tostr()+cstr[imm[pc]]);
    memr=nullptr;
    --top;
}

void vm::o_meq() {
    // pop old memr[0] and replace it
    // the reason why we should get memr and push the old value on stack
    // is that when lnkeq/lnkeqc is called, there will be
    // a new gc object vm_str which is returned by gc::alloc
    // this may cause gc, so we should temporarily put it on stack
    memr[0]=top[-1];
    memr=nullptr;
    top-=imm[pc]+1;
}

void vm::o_eq() {
    var val2=top[0];
    var val1=(--top)[0];
    if (val1.type==vm_nil && val2.type==vm_nil) {
        top[0]=one;
    } else if (val1.type==vm_str && val2.type==vm_str) {
        top[0]=(val1.str()==val2.str())?one:zero;
    } else if ((val1.type==vm_num || val2.type==vm_num)
        && val1.type!=vm_nil && val2.type!=vm_nil) {
        top[0]=(val1.tonum()==val2.tonum())?one:zero;
    } else {
        top[0]=(val1==val2)?one:zero;
    }
}

void vm::o_neq() {
    var val2=top[0];
    var val1=(--top)[0];
    if (val1.type==vm_nil && val2.type==vm_nil) {
        top[0]=zero;
    } else if (val1.type==vm_str && val2.type==vm_str) {
        top[0]=(val1.str()!=val2.str())?one:zero;
    } else if ((val1.type==vm_num || val2.type==vm_num)
        && val1.type!=vm_nil && val2.type!=vm_nil) {
        top[0]=(val1.tonum()!=val2.tonum())?one:zero;
    } else {
        top[0]=(val1!=val2)?one:zero;
    }
}

#define op_cmp(type)\
    --top;\
    top[0]=(top[0].tonum() type top[1].tonum())?one:zero;

void vm::o_less() {op_cmp(<);}
void vm::o_leq() {op_cmp(<=);}
void vm::o_grt() {op_cmp(>);}
void vm::o_geq() {op_cmp(>=);}

#define op_cmp_const(type)\
    top[0]=(top[0].tonum() type cnum[imm[pc]])?one:zero;

void vm::o_lessc() {op_cmp_const(<);}
void vm::o_leqc() {op_cmp_const(<=);}
void vm::o_grtc() {op_cmp_const(>);}
void vm::o_geqc() {op_cmp_const(>=);}

void vm::o_pop() {
    --top;
}

void vm::o_jmp() {
    pc=imm[pc]-1;
}

void vm::o_jt() {
    // jump true needs to reserve the result on stack
    // because conditional expression in nasal has return value
    if (cond(top[0])) {
        pc=imm[pc]-1;
    }
}

void vm::o_jf() {
    // jump false doesn't need to reserve result
    if (!cond(top[0])) {
        pc=imm[pc]-1;
    }
    --top;
}

void vm::o_cnt() {
    if (top[0].type!=vm_vec) {
        die("must use vector in forindex/foreach");
        return;
    }
    (++top)[0]=var::cnt(-1);
}

void vm::o_findex() {
    if ((usize)(++top[0].cnt())>=top[-1].vec().size()) {
        pc=imm[pc]-1;
        return;
    }
    top[1]=var::num(top[0].cnt());
    ++top;
}

void vm::o_feach() {
    auto& ref=top[-1].vec().elems;
    if ((usize)(++top[0].cnt())>=ref.size()) {
        pc=imm[pc]-1;
        return;
    }
    top[1]=ref[top[0].cnt()];
    ++top;
}

void vm::o_callg() {
    // get main stack directly
    (++top)[0]=stack[imm[pc]];
}

void vm::o_calll() {
    (++top)[0]=localr[imm[pc]];
}

void vm::o_upval() {
    (++top)[0]=funcr.func().upval[(imm[pc]>>16)&0xffff]
                    .upval()[imm[pc]&0xffff];
}

void vm::o_callv() {
    var val=top[0];
    var vec=(--top)[0];
    if (vec.type==vm_vec) {
        top[0]=vec.vec().get_val(val.tonum());
        if (top[0].type==vm_none) {
            die("out of range:"+std::to_string(val.tonum()));
            return;
        }
    } else if (vec.type==vm_hash) {
        if (val.type!=vm_str) {
            die("must use string as the key");
            return;
        }
        top[0]=vec.hash().get_val(val.str());
        if (top[0].type==vm_none) {
            die("cannot find member \""+val.str()+"\"");
            return;
        } else if (top[0].type==vm_func) {
            top[0].func().local[0]=val; // 'me'
        }
    } else if (vec.type==vm_str) {
        string& str=vec.str();
        i32 num=val.tonum();
        i32 len=str.length();
        if (num<-len || num>=len) {
            die("out of range:"+std::to_string(val.tonum()));
            return;
        }
        top[0]=var::num(f64((u8)str[num>=0? num:num+len]));
    } else {
        die("must call a vector/hash/string");
        return;
    }
}

void vm::o_callvi() {
    var val=top[0];
    if (val.type!=vm_vec) {
        die("must use a vector");
        return;
    }
    // cannot use operator[],because this may cause overflow
    (++top)[0]=val.vec().get_val(imm[pc]);
    if (top[0].type==vm_none) {
        die("out of range:"+std::to_string(imm[pc]));
        return;
    }
}

void vm::o_callh() {
    var val=top[0];
    if (val.type!=vm_hash) {
        die("must call a hash");
        return;
    }
    top[0]=val.hash().get_val(cstr[imm[pc]]);
    if (top[0].type==vm_none) {
        die("member \""+cstr[imm[pc]]+"\" does not exist");
        return;
    } else if (top[0].type==vm_func) {
        top[0].func().local[0]=val; // 'me'
    }
}

void vm::o_callfv() {
    u32 argc=imm[pc]; // arguments counter
    var* local=top-argc+1; // arguments begin address
    if (local[-1].type!=vm_func) {
        die("must call a function");
        return;
    }
    auto& func=local[-1].func();
    var tmp=local[-1];
    local[-1]=funcr;
    funcr=tmp;
    // top-argc+lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if (top-argc+func.lsize+3>=canary) {
        die("stack overflow");
        return;
    }
    // parameter size is func->psize-1, 1 is reserved for "me"
    u32 psize=func.psize-1;
    if (argc<psize && func.local[argc+1].type==vm_none) {
        die("lack argument(s)");
        return;
    }

    var dynamic=nil;
    if (func.dpara>=0) { // load dynamic arguments
        dynamic=ngc.alloc(vm_vec);
        for(u32 i=psize;i<argc;++i) {
            dynamic.vec().elems.push_back(local[i]);
        }
    }
    // should reset stack top after allocating vector
    // because this may cause gc
    // then all the available values the vector needs
    // are all outside the stack top and may be
    // collected incorrectly
    top=local+func.lsize;

    u32 min_size=(std::min)(psize,argc); // avoid error in MSVC
    for(u32 i=min_size;i>=1;--i) { // load arguments
        local[i]=local[i-1];
    }
    local[0]=func.local[0];// load "me"
    // load local scope & default arguments
    for(u32 i=min_size+1;i<func.lsize;++i) {
        local[i]=func.local[i];
    }
    if (func.dpara>=0) {
        local[psize+1]=dynamic;
    }

    top[0]=upvalr;
    (++top)[0]=var::addr(localr);
    (++top)[0]=var::ret(pc);
    pc=func.entry-1;
    localr=local;
    upvalr=nil;
}

void vm::o_callfh() {
    auto& hash=top[0].hash().elems;
    if (top[-1].type!=vm_func) {
        die("must call a function");
        return;
    }
    auto& func=top[-1].func();
    var tmp=top[-1];
    top[-1]=funcr;
    funcr=tmp;
    // top -1(hash) +lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if (top+func.lsize+2>=canary) {
        die("stack overflow");
        return;
    }
    if (func.dpara>=0) {
        die("special call cannot use dynamic argument");
        return;
    }

    var* local=top;
    top+=func.lsize;
    for(u32 i=0;i<func.lsize;++i) {
        local[i]=func.local[i];
    }
    
    for(auto& i:func.keys) {
        auto& key=cstr[i.first];
        if (hash.count(key)) {
            local[i.second]=hash[key];
        } else if (local[i.second].type==vm_none) {
            die("lack argument(s): \""+key+"\"");
            return;
        }
    }

    top[0]=upvalr;
    (++top)[0]=var::addr(localr);
    (++top)[0]=var::ret(pc); // rewrite top with vm_ret
    pc=func.entry-1;
    localr=local;
    upvalr=nil;
}

void vm::o_callb() {
    // reserve place for builtin function return,
    // this code is written for coroutine
    (++top)[0]=nil;

    // if running a builtin function about coroutine
    // (top) will be set to another context.top, instead of main_context.top
    var tmp=(*builtin[imm[pc]].func)(localr,ngc);

    // so we use tmp variable to store this return value
    // and set it to top[0] later
    top[0]=tmp;

    // if get none, this means errors occurred when calling this native function
    if (top[0].type==vm_none) {
        die("native function error");
        return;
    }
}

void vm::o_slcbeg() {
    // +--------------+
    // | slice_vector | <-- top[0]
    // +--------------+
    // | resource_vec | <-- top[-1]
    // +--------------+
    (++top)[0]=ngc.alloc(vm_vec);
    if (top[-1].type!=vm_vec) {
        die("must slice a vector");
        return;
    }
}

void vm::o_slcend() {
    top[-1]=top[0];
    --top;
}

void vm::o_slc() {
    var val=(top--)[0];
    var res=top[-1].vec().get_val(val.tonum());
    if (res.type==vm_none) {
        die("index "+std::to_string(val.tonum())+" out of range");
        return;
    }
    top[0].vec().elems.push_back(res);
}

void vm::o_slc2() {
    var val2=(top--)[0];
    var val1=(top--)[0];
    auto& ref=top[-1].vec().elems;
    auto& aim=top[0].vec().elems;

    u8 type1=val1.type,type2=val2.type;
    i32 num1=val1.tonum();
    i32 num2=val2.tonum();
    i32 size=ref.size();
    if (type1==vm_nil && type2==vm_nil) {
        num1=0;
        num2=size-1;
    } else if (type1==vm_nil && type2!=vm_nil) {
        num1=num2<0? -size:0;
    } else if (type1!=vm_nil && type2==vm_nil) {
        num2=num1<0? -1:size-1;
    }

    if (num1<-size || num1>=size || num2<-size || num2>=size) {
        die("index "+std::to_string(num1)+":"+std::to_string(num2)+" out of range");
        return;
    } else if (num1<=num2) {
        for(i32 i=num1;i<=num2;++i) {
            aim.push_back(i>=0?ref[i]:ref[i+size]);
        }
    }
}

void vm::o_mcallg() {
    memr=stack+imm[pc];
    (++top)[0]=memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

void vm::o_mcalll() {
    memr=localr+imm[pc];
    (++top)[0]=memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

void vm::o_mupval() {
    memr=&(funcr.func().upval[(imm[pc]>>16)&0xffff].upval()[imm[pc]&0xffff]);
    (++top)[0]=memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

void vm::o_mcallv() {
    var val=top[0];     // index
    var vec=(--top)[0]; // mcall vector, reserved on stack to avoid gc
    if (vec.type==vm_vec) {
        memr=vec.vec().get_mem(val.tonum());
        if (!memr) {
            die("index "+std::to_string(val.tonum())+" out of range");
            return;
        }
    } else if (vec.type==vm_hash) { // do mcallh but use the mcallv way
        if (val.type!=vm_str) {
            die("key must be string");
            return;
        }
        nas_hash& ref=vec.hash();
        string& str=val.str();
        memr=ref.get_mem(str);
        if (!memr) {
            ref.elems[str]=nil;
            memr=ref.get_mem(str);
        }
    } else {
        die("cannot get memory space in this type");
        return;
    }
}

void vm::o_mcallh() {
    var hash=top[0]; // mcall hash, reserved on stack to avoid gc
    if (hash.type!=vm_hash) {
        die("must call a hash");
        return;
    }
    auto& ref=hash.hash();
    auto& str=cstr[imm[pc]];
    memr=ref.get_mem(str);
    if (!memr) { // create a new key
        ref.elems[str]=nil;
        memr=ref.get_mem(str);
    }
}

void vm::o_ret() {
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
*   +-------------+ <- local pointer stored in localr
*   | old funcr   | <- old function stored in funcr
*   +-------------+
*/
    var  ret  =top[0];
    var* local=localr;
    var  func =funcr;
    var  up   =upvalr;

    pc    =top[-1].ret();
    localr=top[-2].addr();
    upvalr=top[-3];

    top=local-1;
    funcr=top[0];
    top[0]=ret; // rewrite func with returned value

    if (up.type==vm_upval) { // synchronize upvalue
        auto& upval=up.upval();
        auto size=func.func().lsize;
        upval.onstk=false;
        upval.elems.resize(size);
        for(u32 i=0;i<size;++i) {
            upval.elems[i]=local[i];
        }
    }
    // cannot use gc.cort to judge,
    // because there maybe another function call inside
    if (!pc) {
        ngc.ctxreserve();
    }
}

void vm::run(
    const codegen& gen,
    const linker& linker,
    const std::vector<string>& argv,
    const bool detail) {
    verbose=detail;
    init(gen.strs(),gen.nums(),gen.codes(),linker.filelist(),argv);
#ifndef _MSC_VER
    const void* oprs[]={
        &&vmexit, &&intg,   &&intl,   &&loadg,
        &&loadl,  &&loadu,  &&pnum,   &&pnil,
        &&pstr,   &&newv,   &&newh,   &&newf,
        &&happ,   &&para,   &&deft,   &&dyn,
        &&lnot,   &&usub,   &&bnot,   &&btor,
        &&btxor,  &&btand,  &&add,    &&sub,
        &&mul,    &&div,    &&lnk,    &&addc,
        &&subc,   &&mulc,   &&divc,   &&lnkc,
        &&addeq,  &&subeq,  &&muleq,  &&diveq,
        &&lnkeq,  &&bandeq, &&boreq,  &&bxoreq,
        &&addeqc, &&subeqc, &&muleqc, &&diveqc,
        &&lnkeqc, &&addecp, &&subecp, &&mulecp,
        &&divecp, &&lnkecp, &&meq,    &&eq,
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
    for(auto& i:gen.codes()) {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    // goto the first operand
    goto *code[pc];
#else
    typedef void (vm::*nafunc)();
    const nafunc oprs[]={
        nullptr,       &vm::o_intg,
        &vm::o_intl,   &vm::o_loadg,
        &vm::o_loadl,  &vm::o_loadu,
        &vm::o_pnum,   &vm::o_pnil,
        &vm::o_pstr,   &vm::o_newv,
        &vm::o_newh,   &vm::o_newf,
        &vm::o_happ,   &vm::o_para,
        &vm::o_deft,   &vm::o_dyn,
        &vm::o_lnot,   &vm::o_usub,
        &vm::o_bnot,   &vm::o_btor,
        &vm::o_btxor,  &vm::o_btand,
        &vm::o_add,    &vm::o_sub,
        &vm::o_mul,    &vm::o_div,
        &vm::o_lnk,    &vm::o_addc,
        &vm::o_subc,   &vm::o_mulc,
        &vm::o_divc,   &vm::o_lnkc,
        &vm::o_addeq,  &vm::o_subeq,
        &vm::o_muleq,  &vm::o_diveq,
        &vm::o_lnkeq,  &vm::o_bandeq,
        &vm::o_boreq,  &vm::o_bxoreq,
        &vm::o_addeqc, &vm::o_subeqc,
        &vm::o_muleqc, &vm::o_diveqc,
        &vm::o_lnkeqc, &vm::o_addecp,
        &vm::o_subecp, &vm::o_mulecp,
        &vm::o_divecp, &vm::o_lnkecp,
        &vm::o_meq,    &vm::o_eq,
        &vm::o_neq,    &vm::o_less,
        &vm::o_leq,    &vm::o_grt,
        &vm::o_geq,    &vm::o_lessc,
        &vm::o_leqc,   &vm::o_grtc,
        &vm::o_geqc,   &vm::o_pop,
        &vm::o_jmp,    &vm::o_jt,
        &vm::o_jf,     &vm::o_cnt,
        &vm::o_findex, &vm::o_feach,
        &vm::o_callg,  &vm::o_calll,
        &vm::o_upval,  &vm::o_callv,
        &vm::o_callvi, &vm::o_callh,
        &vm::o_callfv, &vm::o_callfh,
        &vm::o_callb,  &vm::o_slcbeg,
        &vm::o_slcend, &vm::o_slc,
        &vm::o_slc2,   &vm::o_mcallg,
        &vm::o_mcalll, &vm::o_mupval,
        &vm::o_mcallv, &vm::o_mcallh,
        &vm::o_ret
    };
    std::vector<nafunc> code;
    for(auto& i:gen.codes()) {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    while(code[pc]) {
        (this->*code[pc])();
        if (top>=canary) {
            die("stack overflow");
        }
        ++pc;
    }
#endif

vmexit:
    if (detail) {
        ngc.info();
    }
    ngc.clear();
    imm.clear();
    return;

#ifndef _MSC_VER
// may cause stackoverflow
#define exec_check(op) {\
        op();\
        if (top<canary)\
            goto *code[++pc];\
        die("stack overflow");\
        goto *code[++pc];\
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
lnot:   exec_nodie(o_lnot  ); // -0
usub:   exec_nodie(o_usub  ); // -0
bnot:   exec_nodie(o_bnot  ); // -0
btor:   exec_nodie(o_btor  ); // -1
btxor:  exec_nodie(o_btxor ); // -1
btand:  exec_nodie(o_btand ); // -1
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
bandeq: exec_nodie(o_bandeq); // -1
boreq:  exec_nodie(o_boreq ); // -1
bxoreq: exec_nodie(o_bxoreq); // -1
addeqc: exec_nodie(o_addeqc); // -0
subeqc: exec_nodie(o_subeqc); // -0
muleqc: exec_nodie(o_muleqc); // -0
diveqc: exec_nodie(o_diveqc); // -0
lnkeqc: exec_nodie(o_lnkeqc); // -0
addecp: exec_nodie(o_addecp); // -1
subecp: exec_nodie(o_subecp); // -1
mulecp: exec_nodie(o_mulecp); // -1
divecp: exec_nodie(o_divecp); // -1
lnkecp: exec_nodie(o_lnkecp); // -1
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
