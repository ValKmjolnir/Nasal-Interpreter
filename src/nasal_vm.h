#pragma once

#include <iomanip>
#include <stack>

#include "nasal_import.h"
#include "nasal_gc.h"
#include "nasal_codegen.h"

#ifdef _MSC_VER
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4102)
#endif

class vm {
protected:

    /* registers and constants of vm */
    context ctx;

    /* constants */
    const f64* cnum = nullptr; // constant numbers
    const std::string* cstr = nullptr; // constant symbols and strings
    std::vector<u32> imm; // immediate number table
    std::vector<nasal_builtin_table> native;
    
    /* garbage collector */
    gc ngc;

    /* main stack */
    var stack[STACK_DEPTH];

    /* values used for debugger */
    const std::string* files = nullptr; // file name list
    const opcode* bytecode = nullptr; // bytecode buffer address

    /* vm initializing function */
    void init(
        const std::vector<std::string>&,
        const std::vector<f64>&,
        const std::vector<nasal_builtin_table>&,
        const std::vector<opcode>&,
        const std::unordered_map<std::string, i32>&,
        const std::vector<std::string>&,
        const std::vector<std::string>&);

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
    void die(const std::string&);

    /* vm calculation functions*/
    inline bool cond(var&);

    /* vm operands */
    inline void o_intg();
    inline void o_intl();
    inline void o_loadg();
    inline void o_loadl();
    inline void o_loadu();
    inline void o_pnum();
    inline void o_pnil();
    inline void o_pstr();
    inline void o_newv();
    inline void o_newh();
    inline void o_newf();
    inline void o_happ();
    inline void o_para();
    inline void o_deft();
    inline void o_dyn();
    inline void o_lnot();
    inline void o_usub();
    inline void o_bnot();
    inline void o_btor();
    inline void o_btxor();
    inline void o_btand();
    inline void o_add();
    inline void o_sub();
    inline void o_mul();
    inline void o_div();
    inline void o_lnk();
    inline void o_addc();
    inline void o_subc();
    inline void o_mulc();
    inline void o_divc();
    inline void o_lnkc();
    inline void o_addeq();
    inline void o_subeq();
    inline void o_muleq();
    inline void o_diveq();
    inline void o_lnkeq();
    inline void o_bandeq();
    inline void o_boreq();
    inline void o_bxoreq();
    inline void o_addeqc();
    inline void o_subeqc();
    inline void o_muleqc();
    inline void o_diveqc();
    inline void o_lnkeqc();
    inline void o_addecp();
    inline void o_subecp();
    inline void o_mulecp();
    inline void o_divecp();
    inline void o_lnkecp();
    inline void o_meq();
    inline void o_eq();
    inline void o_neq();
    inline void o_less();
    inline void o_leq();
    inline void o_grt();
    inline void o_geq();
    inline void o_lessc();
    inline void o_leqc();
    inline void o_grtc();
    inline void o_geqc();
    inline void o_pop();
    inline void o_jmp();
    inline void o_jt();
    inline void o_jf();
    inline void o_cnt();
    inline void o_findex();
    inline void o_feach();
    inline void o_callg();
    inline void o_calll();
    inline void o_upval();
    inline void o_callv();
    inline void o_callvi();
    inline void o_callh();
    inline void o_callfv();
    inline void o_callfh();
    inline void o_callb();
    inline void o_slcbeg();
    inline void o_slcend();
    inline void o_slc();
    inline void o_slc2();
    inline void o_mcallg();
    inline void o_mcalll();
    inline void o_mupval();
    inline void o_mcallv();
    inline void o_mcallh();
    inline void o_ret();

public:

    /* constructor of vm instance */
    vm(): ngc(&ctx), verbose(false) {}

    /* execution entry */
    void run(
        const codegen&,
        const linker&,
        const std::vector<std::string>&,
        const bool);
};

inline bool vm::cond(var& val) {
    if (val.type==vm_num) {
        return val.num();
    } else if (val.type==vm_str) {
        const f64 num = str2num(val.str().c_str());
        return std::isnan(num)? !val.str().empty():num;
    }
    return false;
}

inline void vm::o_intg() {
    // global values store on stack
    ctx.top += imm[ctx.pc];
    // point to the top
    --ctx.top;
}

inline void vm::o_intl() {
    ctx.top[0].func().local.resize(imm[ctx.pc], nil);
    ctx.top[0].func().lsize = imm[ctx.pc];
}

inline void vm::o_loadg() {
    stack[imm[ctx.pc]] = (ctx.top--)[0];
}

inline void vm::o_loadl() {
    ctx.localr[imm[ctx.pc]] = (ctx.top--)[0];
}

inline void vm::o_loadu() {
    ctx.funcr.func().upval[(imm[ctx.pc]>>16)&0xffff]
         .upval()[imm[ctx.pc]&0xffff] = (ctx.top--)[0];
}

inline void vm::o_pnum() {
    (++ctx.top)[0] = var::num(cnum[imm[ctx.pc]]);
}

inline void vm::o_pnil() {
    (++ctx.top)[0] = nil;
}

inline void vm::o_pstr() {
    (++ctx.top)[0] = ngc.strs[imm[ctx.pc]];
}

inline void vm::o_newv() {
    var newv = ngc.alloc(vm_vec);
    auto& vec = newv.vec().elems;
    vec.resize(imm[ctx.pc]);
    // use top-=imm[pc]-1 here will cause error if imm[pc] is 0
    ctx.top = ctx.top-imm[ctx.pc]+1;
    for(u32 i = 0; i<imm[ctx.pc]; ++i) {
        vec[i] = ctx.top[i];
    }
    ctx.top[0] = newv;
}

inline void vm::o_newh() {
    (++ctx.top)[0] = ngc.alloc(vm_hash);
}

inline void vm::o_newf() {
    (++ctx.top)[0] = ngc.alloc(vm_func);
    auto& func = ctx.top[0].func();
    func.entry = imm[ctx.pc];
    func.psize = 1;

    /* this means you create a new function in local scope */
    if (ctx.localr) {
        func.upval = ctx.funcr.func().upval;
        // function created in the same local scope shares one closure
        // so this size & stk setting has no problem
        var upval = (ctx.upvalr.type==vm_nil)? ngc.alloc(vm_upval):ctx.upvalr;
        upval.upval().size = ctx.funcr.func().lsize;
        upval.upval().stk = ctx.localr;
        func.upval.push_back(upval);
        ctx.upvalr = upval;
    }
}

inline void vm::o_happ() {
    ctx.top[-1].hash().elems[cstr[imm[ctx.pc]]] = ctx.top[0];
    --ctx.top;
}

inline void vm::o_para() {
    auto& func = ctx.top[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[imm[ctx.pc]] = func.psize;
    func.local[func.psize++] = var::none();
}

inline void vm::o_deft() {
    var val = ctx.top[0];
    auto& func = (--ctx.top)[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[imm[ctx.pc]] = func.psize;
    func.local[func.psize++] = val;
}

inline void vm::o_dyn() {
    ctx.top[0].func().dpara = imm[ctx.pc];
}

inline void vm::o_lnot() {
    var val = ctx.top[0];
    switch(val.type) {
        case vm_nil: ctx.top[0] = one; break;
        case vm_num: ctx.top[0] = val.num()? zero:one; break;
        case vm_str: {
            const f64 num = str2num(val.str().c_str());
            if (std::isnan(num)) {
                ctx.top[0] = var::num((f64)val.str().empty());
            } else {
                ctx.top[0] = num? zero:one;
            }
        } break;
        default:
            die("incorrect value type");
            return;
    }
}

inline void vm::o_usub() {
    ctx.top[0] = var::num(-ctx.top[0].tonum());
}

inline void vm::o_bnot() {
    ctx.top[0] = var::num(~static_cast<int32_t>(ctx.top[0].num()));
}

inline void vm::o_btor() {
    ctx.top[-1] = var::num(
        static_cast<int32_t>(ctx.top[-1].tonum())|
        static_cast<int32_t>(ctx.top[0].tonum()));
    --ctx.top;
}

inline void vm::o_btxor() {
    ctx.top[-1] = var::num(
        static_cast<int32_t>(ctx.top[-1].tonum())^
        static_cast<int32_t>(ctx.top[0].tonum()));
    --ctx.top;
}

inline void vm::o_btand() {
    ctx.top[-1] = var::num(
        static_cast<int32_t>(ctx.top[-1].tonum())&
        static_cast<int32_t>(ctx.top[0].tonum()));
    --ctx.top;
}

#define op_calc(type)\
    ctx.top[-1] = var::num(ctx.top[-1].tonum() type ctx.top[0].tonum());\
    --ctx.top;

inline void vm::o_add() {op_calc(+);}
inline void vm::o_sub() {op_calc(-);}
inline void vm::o_mul() {op_calc(*);}
inline void vm::o_div() {op_calc(/);}
inline void vm::o_lnk() {
    if (ctx.top[-1].type==vm_vec && ctx.top[0].type==vm_vec) {
        ngc.temp = ngc.alloc(vm_vec);
        for(auto i : ctx.top[-1].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        for(auto i : ctx.top[0].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        ctx.top[-1] = ngc.temp;
        ngc.temp = nil;
        --ctx.top;
        return;
    }
    ctx.top[-1] = ngc.newstr(ctx.top[-1].tostr()+ctx.top[0].tostr());
    --ctx.top;
}

#define op_calc_const(type)\
    ctx.top[0] = var::num(ctx.top[0].tonum() type cnum[imm[ctx.pc]]);

inline void vm::o_addc() {op_calc_const(+);}
inline void vm::o_subc() {op_calc_const(-);}
inline void vm::o_mulc() {op_calc_const(*);}
inline void vm::o_divc() {op_calc_const(/);}
inline void vm::o_lnkc() {
    ctx.top[0] = ngc.newstr(ctx.top[0].tostr()+cstr[imm[ctx.pc]]);
}

// top[0] stores the value of memr[0], to avoid being garbage-collected
// so when the calculation ends, top-=1, then top-=imm[pc]
// because this return value is meaningless if on stack when imm[pc] = 1
// like this: func{a+=c;}(); the result of 'a+c' will no be used later, imm[pc] = 1
// but if b+=a+=c; the result of 'a+c' will be used later, imm[pc] = 0
#define op_calc_eq(type)\
    ctx.top[-1] = ctx.memr[0] = var::num(ctx.memr[0].tonum() type ctx.top[-1].tonum());\
    ctx.memr = nullptr;\
    ctx.top -= imm[ctx.pc]+1;

inline void vm::o_addeq() {op_calc_eq(+);}
inline void vm::o_subeq() {op_calc_eq(-);}
inline void vm::o_muleq() {op_calc_eq(*);}
inline void vm::o_diveq() {op_calc_eq(/);}
inline void vm::o_lnkeq() {
    ctx.top[-1] = ctx.memr[0] = ngc.newstr(ctx.memr[0].tostr()+ctx.top[-1].tostr());
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

inline void vm::o_bandeq() {
    ctx.top[-1] = ctx.memr[0] = var::num(i32(ctx.memr[0].tonum())&i32(ctx.top[-1].tonum()));
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

inline void vm::o_boreq() {
    ctx.top[-1] = ctx.memr[0] = var::num(i32(ctx.memr[0].tonum())|i32(ctx.top[-1].tonum()));
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

inline void vm::o_bxoreq() {
    ctx.top[-1] = ctx.memr[0] = var::num(i32(ctx.memr[0].tonum())^i32(ctx.top[-1].tonum()));
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

// top[0] stores the value of memr[0], to avoid being garbage-collected
// so when the calculation ends, top-=imm[pc]>>31
// because this return value is meaningless if on stack when imm[pc]>>31=1
// like this: func{a+=1;}(); the result of 'a+1' will no be used later, imm[pc]>>31=1
// but if b+=a+=1; the result of 'a+1' will be used later, imm[pc]>>31=0
#define op_calc_eq_const(type)\
    ctx.top[0] = ctx.memr[0] = var::num(ctx.memr[0].tonum() type cnum[imm[ctx.pc]]);\
    ctx.memr = nullptr;

inline void vm::o_addeqc() {op_calc_eq_const(+);}
inline void vm::o_subeqc() {op_calc_eq_const(-);}
inline void vm::o_muleqc() {op_calc_eq_const(*);}
inline void vm::o_diveqc() {op_calc_eq_const(/);}
inline void vm::o_lnkeqc() {
    ctx.top[0] = ctx.memr[0] = ngc.newstr(ctx.memr[0].tostr()+cstr[imm[ctx.pc]]);
    ctx.memr = nullptr;
}

#define op_calc_eq_const_and_pop(type)\
    ctx.top[0] = ctx.memr[0] = var::num(ctx.memr[0].tonum() type cnum[imm[ctx.pc]]);\
    ctx.memr = nullptr;\
    --ctx.top;

inline void vm::o_addecp() {op_calc_eq_const_and_pop(+);}
inline void vm::o_subecp() {op_calc_eq_const_and_pop(-);}
inline void vm::o_mulecp() {op_calc_eq_const_and_pop(*);}
inline void vm::o_divecp() {op_calc_eq_const_and_pop(/);}
inline void vm::o_lnkecp() {
    ctx.top[0] = ctx.memr[0] = ngc.newstr(ctx.memr[0].tostr()+cstr[imm[ctx.pc]]);
    ctx.memr = nullptr;
    --ctx.top;
}

inline void vm::o_meq() {
    // pop old memr[0] and replace it
    // the reason why we should get memr and push the old value on stack
    // is that when lnkeq/lnkeqc is called, there will be
    // a new gc object vm_str which is returned by gc::alloc
    // this may cause gc, so we should temporarily put it on stack
    ctx.memr[0] = ctx.top[-1];
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

inline void vm::o_eq() {
    var val2 = ctx.top[0];
    var val1 = (--ctx.top)[0];
    if (val1.type==vm_nil && val2.type==vm_nil) {
        ctx.top[0] = one;
    } else if (val1.type==vm_str && val2.type==vm_str) {
        ctx.top[0] = (val1.str()==val2.str())?one:zero;
    } else if ((val1.type==vm_num || val2.type==vm_num)
        && val1.type!=vm_nil && val2.type!=vm_nil) {
        ctx.top[0] = (val1.tonum()==val2.tonum())?one:zero;
    } else {
        ctx.top[0] = (val1==val2)?one:zero;
    }
}

inline void vm::o_neq() {
    var val2 = ctx.top[0];
    var val1 = (--ctx.top)[0];
    if (val1.type==vm_nil && val2.type==vm_nil) {
        ctx.top[0] = zero;
    } else if (val1.type==vm_str && val2.type==vm_str) {
        ctx.top[0] = (val1.str()!=val2.str())?one:zero;
    } else if ((val1.type==vm_num || val2.type==vm_num)
        && val1.type!=vm_nil && val2.type!=vm_nil) {
        ctx.top[0] = (val1.tonum()!=val2.tonum())?one:zero;
    } else {
        ctx.top[0] = (val1!=val2)?one:zero;
    }
}

#define op_cmp(type)\
    --ctx.top;\
    ctx.top[0] = (ctx.top[0].tonum() type ctx.top[1].tonum())?one:zero;

inline void vm::o_less() {op_cmp(<);}
inline void vm::o_leq() {op_cmp(<=);}
inline void vm::o_grt() {op_cmp(>);}
inline void vm::o_geq() {op_cmp(>=);}

#define op_cmp_const(type)\
    ctx.top[0] = (ctx.top[0].tonum() type cnum[imm[ctx.pc]])?one:zero;

inline void vm::o_lessc() {op_cmp_const(<);}
inline void vm::o_leqc() {op_cmp_const(<=);}
inline void vm::o_grtc() {op_cmp_const(>);}
inline void vm::o_geqc() {op_cmp_const(>=);}

inline void vm::o_pop() {
    --ctx.top;
}

inline void vm::o_jmp() {
    ctx.pc = imm[ctx.pc]-1;
}

inline void vm::o_jt() {
    // jump true needs to reserve the result on stack
    // because conditional expression in nasal has return value
    if (cond(ctx.top[0])) {
        ctx.pc = imm[ctx.pc]-1;
    }
}

inline void vm::o_jf() {
    // jump false doesn't need to reserve result
    if (!cond(ctx.top[0])) {
        ctx.pc = imm[ctx.pc]-1;
    }
    --ctx.top;
}

inline void vm::o_cnt() {
    if (ctx.top[0].type!=vm_vec) {
        die("must use vector in forindex/foreach");
        return;
    }
    (++ctx.top)[0] = var::cnt(-1);
}

inline void vm::o_findex() {
    if ((usize)(++ctx.top[0].cnt())>=ctx.top[-1].vec().size()) {
        ctx.pc = imm[ctx.pc]-1;
        return;
    }
    ctx.top[1] = var::num(ctx.top[0].cnt());
    ++ctx.top;
}

inline void vm::o_feach() {
    auto& ref = ctx.top[-1].vec().elems;
    if ((usize)(++ctx.top[0].cnt())>=ref.size()) {
        ctx.pc = imm[ctx.pc]-1;
        return;
    }
    ctx.top[1] = ref[ctx.top[0].cnt()];
    ++ctx.top;
}

inline void vm::o_callg() {
    // get main stack directly
    (++ctx.top)[0] = stack[imm[ctx.pc]];
}

inline void vm::o_calll() {
    (++ctx.top)[0] = ctx.localr[imm[ctx.pc]];
}

inline void vm::o_upval() {
    (++ctx.top)[0] = ctx.funcr.func()
        .upval[(imm[ctx.pc]>>16)&0xffff]
        .upval()[imm[ctx.pc]&0xffff];
}

inline void vm::o_callv() {
    var val = ctx.top[0];
    var vec = (--ctx.top)[0];
    if (vec.type==vm_vec) {
        ctx.top[0] = vec.vec().get_val(val.tonum());
        if (ctx.top[0].type==vm_none) {
            die("out of range:"+std::to_string(val.tonum()));
            return;
        }
    } else if (vec.type==vm_hash) {
        if (val.type!=vm_str) {
            die("must use string as the key");
            return;
        }
        ctx.top[0] = vec.hash().get_val(val.str());
        if (ctx.top[0].type==vm_none) {
            die("cannot find member \""+val.str()+"\"");
            return;
        } else if (ctx.top[0].type==vm_func) {
            ctx.top[0].func().local[0] = val; // 'me'
        }
    } else if (vec.type==vm_str) {
        auto& str = vec.str();
        i32 num = val.tonum();
        i32 len = str.length();
        if (num<-len || num>=len) {
            die("out of range:"+std::to_string(val.tonum()));
            return;
        }
        ctx.top[0] = var::num(f64((u8)str[num>=0? num:num+len]));
    } else if (vec.type==vm_map) {
        if (val.type!=vm_str) {
            die("must use string as the key");
            return;
        }
        ctx.top[0] = vec.map().get_val(val.str());
        if (ctx.top[0].type==vm_none) {
            die("cannot find symbol \""+val.str()+"\"");
            return;
        }
    } else {
        die("must call a vector/hash/string");
        return;
    }
}

inline void vm::o_callvi() {
    var val = ctx.top[0];
    if (val.type!=vm_vec) {
        die("must use a vector");
        return;
    }
    // cannot use operator[],because this may cause overflow
    (++ctx.top)[0] = val.vec().get_val(imm[ctx.pc]);
    if (ctx.top[0].type==vm_none) {
        die("out of range:"+std::to_string(imm[ctx.pc]));
        return;
    }
}

inline void vm::o_callh() {
    var val = ctx.top[0];
    if (val.type!=vm_hash && val.type!=vm_map) {
        die("must call a hash");
        return;
    }
    const auto& str = cstr[imm[ctx.pc]];
    if (val.type==vm_hash) {
        ctx.top[0] = val.hash().get_val(str);
    } else {
        ctx.top[0] = val.map().get_val(str);
    }
    if (ctx.top[0].type==vm_none) {
        val.type==vm_hash? 
            die("member \"" + str + "\" does not exist"):
            die("cannot find symbol \"" + str + "\"");
        return;
    } else if (ctx.top[0].type==vm_func) {
        ctx.top[0].func().local[0] = val; // 'me'
    }
}

inline void vm::o_callfv() {
    u32 argc = imm[ctx.pc]; // arguments counter
    var* local = ctx.top-argc+1; // arguments begin address
    if (local[-1].type!=vm_func) {
        die("must call a function");
        return;
    }
    auto& func = local[-1].func();

    // swap funcr with local[-1]
    var tmp = local[-1];
    local[-1] = ctx.funcr;
    ctx.funcr = tmp;

    // top-argc+lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if (ctx.top-argc+func.lsize+3>=ctx.canary) {
        die("stack overflow");
        return;
    }
    // parameter size is func->psize-1, 1 is reserved for "me"
    u32 psize = func.psize-1;
    if (argc<psize && func.local[argc+1].type==vm_none) {
        die("lack argument(s)");
        return;
    }

    var dynamic = nil;
    if (func.dpara>=0) { // load dynamic arguments
        dynamic = ngc.alloc(vm_vec);
        for(u32 i = psize; i<argc; ++i) {
            dynamic.vec().elems.push_back(local[i]);
        }
    } else if (psize<argc) {
        // load arguments to "arg", located at stack+1
        dynamic = ngc.alloc(vm_vec);
        for(u32 i = psize; i<argc; ++i) {
            dynamic.vec().elems.push_back(local[i]);
        }
    }
    // should reset stack top after allocating vector
    // because this may cause gc
    // then all the available values the vector needs
    // are all outside the stack top and may be
    // collected incorrectly
    ctx.top = local+func.lsize;

    u32 min_size = (std::min)(psize, argc); // avoid error in MSVC
    for(u32 i = min_size; i>=1; --i) { // load arguments
        local[i] = local[i-1];
    }
    local[0] = func.local[0];// load "me"

    // load local scope & default arguments
    for(u32 i = min_size+1; i<func.lsize; ++i) {
        local[i] = func.local[i];
    }
    local[func.dpara>=0? psize+1:func.lsize-1] = dynamic;

    ctx.top[0] = ctx.upvalr;
    (++ctx.top)[0] = var::addr(ctx.localr);
    (++ctx.top)[0] = var::ret(ctx.pc);
    ctx.pc = func.entry-1;
    ctx.localr = local;
    ctx.upvalr = nil;
}

inline void vm::o_callfh() {
    auto& hash = ctx.top[0].hash().elems;
    if (ctx.top[-1].type!=vm_func) {
        die("must call a function");
        return;
    }
    auto& func = ctx.top[-1].func();
    var tmp = ctx.top[-1];
    ctx.top[-1] = ctx.funcr;
    ctx.funcr = tmp;

    // top -1(hash) +lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if (ctx.top+func.lsize+2>= ctx.canary) {
        die("stack overflow");
        return;
    }
    if (func.dpara>=0) {
        die("special call cannot use dynamic argument");
        return;
    }

    var* local = ctx.top;
    ctx.top += func.lsize;
    for(u32 i = 0; i<func.lsize; ++i) {
        local[i] = func.local[i];
    }
    
    for(const auto& i : func.keys) {
        auto& key = cstr[i.first];
        if (hash.count(key)) {
            local[i.second] = hash[key];
        } else if (local[i.second].type==vm_none) {
            die("lack argument(s): \""+key+"\"");
            return;
        }
    }

    ctx.top[0] = ctx.upvalr;
    (++ctx.top)[0] = var::addr(ctx.localr);
    (++ctx.top)[0] = var::ret(ctx.pc); // rewrite top with vm_ret
    ctx.pc=func.entry-1;
    ctx.localr = local;
    ctx.upvalr = nil;
}

inline void vm::o_callb() {
    // reserve place for native function return,
    // this code is written for coroutine
    (++ctx.top)[0] = nil;

    // if running a native function about coroutine
    // (top) will be set to another context.top, instead of main_context.top
    var tmp = (*native[imm[ctx.pc]].func)(ctx.localr, ngc);

    // so we use tmp variable to store this return value
    // and set it to top[0] later
    ctx.top[0] = tmp;

    // if get none, this means errors occurred when calling this native function
    if (ctx.top[0].type==vm_none) {
        die("error occurred in native function");
        return;
    }
}

inline void vm::o_slcbeg() {
    // +--------------+
    // | slice_vector | <-- top[0]
    // +--------------+
    // | resource_vec | <-- top[-1]
    // +--------------+
    (++ctx.top)[0] = ngc.alloc(vm_vec);
    if (ctx.top[-1].type!=vm_vec) {
        die("must slice a vector");
        return;
    }
}

inline void vm::o_slcend() {
    ctx.top[-1] = ctx.top[0];
    --ctx.top;
}

inline void vm::o_slc() {
    var val = (ctx.top--)[0];
    var res = ctx.top[-1].vec().get_val(val.tonum());
    if (res.type==vm_none) {
        die("index " + std::to_string(val.tonum()) + " out of range");
        return;
    }
    ctx.top[0].vec().elems.push_back(res);
}

inline void vm::o_slc2() {
    var val2 = (ctx.top--)[0];
    var val1 = (ctx.top--)[0];
    auto& ref = ctx.top[-1].vec().elems;
    auto& aim = ctx.top[0].vec().elems;

    u8 type1 = val1.type,type2=val2.type;
    i32 num1 = val1.tonum();
    i32 num2 = val2.tonum();
    i32 size = ref.size();
    if (type1==vm_nil && type2==vm_nil) {
        num1 = 0;
        num2 = size-1;
    } else if (type1==vm_nil && type2!=vm_nil) {
        num1 = num2<0? -size:0;
    } else if (type1!=vm_nil && type2==vm_nil) {
        num2 = num1<0? -1:size-1;
    }

    if (num1<-size || num1>=size || num2<-size || num2>=size) {
        die("index " + std::to_string(num1) + ":" +
            std::to_string(num2) + " out of range");
        return;
    } else if (num1<=num2) {
        for(i32 i = num1; i<=num2; ++i) {
            aim.push_back(i>=0? ref[i]:ref[i+size]);
        }
    }
}

inline void vm::o_mcallg() {
    ctx.memr = stack+imm[ctx.pc];
    (++ctx.top)[0] = ctx.memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

inline void vm::o_mcalll() {
    ctx.memr = ctx.localr+imm[ctx.pc];
    (++ctx.top)[0] = ctx.memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

inline void vm::o_mupval() {
    ctx.memr = &(
        ctx.funcr.func()
            .upval[(imm[ctx.pc]>>16)&0xffff]
            .upval()[imm[ctx.pc]&0xffff]);
    (++ctx.top)[0] = ctx.memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

inline void vm::o_mcallv() {
    var val = ctx.top[0];     // index
    var vec = (--ctx.top)[0]; // mcall vector, reserved on stack to avoid gc
    if (vec.type==vm_vec) {
        ctx.memr = vec.vec().get_mem(val.tonum());
        if (!ctx.memr) {
            die("index "+std::to_string(val.tonum())+" out of range");
            return;
        }
    } else if (vec.type==vm_hash) { // do mcallh but use the mcallv way
        if (val.type!=vm_str) {
            die("key must be string");
            return;
        }
        auto& ref = vec.hash();
        auto& str = val.str();
        ctx.memr = ref.get_mem(str);
        if (!ctx.memr) {
            ref.elems[str] = nil;
            ctx.memr = ref.get_mem(str);
        }
    } else if (vec.type==vm_map) {
        if (val.type!=vm_str) {
            die("key must be string");
            return;
        }
        auto& ref = vec.map();
        auto& str = val.str();
        ctx.memr = ref.get_mem(str);
        if (!ctx.memr) {
            die("cannot find symbol \"" + str + "\"");
        }
    } else {
        die("cannot get memory space in this type");
        return;
    }
}

inline void vm::o_mcallh() {
    var hash = ctx.top[0]; // mcall hash, reserved on stack to avoid gc
    if (hash.type!=vm_hash && hash.type!=vm_map) {
        die("must call a hash");
        return;
    }
    auto& str = cstr[imm[ctx.pc]];
    if (hash.type==vm_map) {
        ctx.memr = hash.map().get_mem(str);
        if (!ctx.memr) {
            die("cannot find symbol \"" + str + "\"");
        }
        return;
    }
    auto& ref = hash.hash();
    ctx.memr = ref.get_mem(str);
    if (!ctx.memr) { // create a new key
        ref.elems[str] = nil;
        ctx.memr = ref.get_mem(str);
    }
}

inline void vm::o_ret() {
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
    var  ret   = ctx.top[0];
    var* local = ctx.localr;
    var  func  = ctx.funcr;
    var  up    = ctx.upvalr;

    ctx.pc     = ctx.top[-1].ret();
    ctx.localr = ctx.top[-2].addr();
    ctx.upvalr = ctx.top[-3];

    ctx.top = local-1;
    ctx.funcr = ctx.top[0];
    ctx.top[0] = ret; // rewrite func with returned value

    if (up.type==vm_upval) { // synchronize upvalue
        auto& upval = up.upval();
        auto size = func.func().lsize;
        upval.onstk = false;
        upval.elems.resize(size);
        for(u32 i = 0; i<size; ++i) {
            upval.elems[i] = local[i];
        }
    }

    // cannot use gc.cort to judge,
    // because there maybe another function call inside but return here
    // coroutine function ends with setting pc to 0
    if (!ctx.pc) {
        ngc.ctxreserve();
    }
}