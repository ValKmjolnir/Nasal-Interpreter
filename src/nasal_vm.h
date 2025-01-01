#pragma once

#include <iomanip>
#include <stack>
#include <cstring>
#include <sstream>

#include "nasal_import.h"
#include "nasal_gc.h"
#include "nasal_codegen.h"
#include "util/util.h"

#ifdef _MSC_VER
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4102)
#endif

namespace nasal {

class vm {
protected:

    /* vm context */
    context ctx;

    /* constants */
    const f64* const_number = nullptr;
    const std::string* const_string = nullptr;
    std::vector<u64> imm; // immediate number table

    /* nasal native functions */
    std::vector<nasal_builtin_table> native_function;
    
    /* garbage collector */
    gc ngc;

    /* main stack */
    var* global = nullptr; // used to store global variables
    usize global_size = 0; // mark size of global variables

    /* values used for debugger */
    const std::string* files = nullptr; // file name list
    const opcode* bytecode = nullptr; // bytecode buffer address
    std::vector<std::string> global_symbol_name; // global symbol name

    /* variables for repl mode */
    bool is_repl_mode = false;
    bool first_exec_flag = true;
    bool allow_repl_output = false;

    /* limited mode, will not load unsafe system api if switched on */
    bool flag_limited_mode = false;

protected:
    /* vm initializing function */
    void vm_init_enrty(const std::vector<std::string>&,
                       const std::vector<f64>&,
                       const std::vector<nasal_builtin_table>&,
                       const std::vector<opcode>&,
                       const std::unordered_map<std::string, u64>&,
                       const std::vector<std::string>&,
                       const std::vector<std::string>&);
    void context_and_global_init();

protected:
    /* debug functions */
    bool verbose = false;
    void return_address_info(const var&);
    void memory_address_info(const var&);
    void raw_string_info(var&);
    void upvalue_info(var&);
    void vector_value_info(var&);
    void hash_value_info(var&, const usize);
    void coroutine_value_info(var&);
    void namespace_value_info(var&, const usize);
    void value_name_form(const var&);
    void value_info(var&);
    void function_detail_info(const nas_func&);
    void function_call_trace();
    void trace_back();
    void stack_info(const u64);
    void register_info();
    void global_state();
    void local_state();
    void upvalue_state();
    void all_state_detail();

protected:
    std::string report_lack_arguments(u32, const nas_func&) const;
    std::string report_special_call_lack_arguments(var*, const nas_func&) const;
    std::string report_key_not_found(const std::string&, const nas_hash&) const;
    std::string report_out_of_range(f64, usize) const;
    std::string type_name_string(const var&) const;
    void die(const std::string&);

protected:
    /* vm calculation functions*/
    inline bool boolify(const var&);

protected:
    /* vm operands */
    inline void o_repl();
    inline void o_intl();
    inline void o_loadg();
    inline void o_loadl();
    inline void o_loadu();
    inline void o_dup();
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

protected:
    // for debugger and MSVC(does not support labels as values)
    typedef void (vm::*nasal_vm_func)();
    const nasal_vm_func operand_function[op_ret + 1] = {
        nullptr,
        &vm::o_repl,
        &vm::o_intl,
        &vm::o_loadg,
        &vm::o_loadl,
        &vm::o_loadu,
        &vm::o_dup,
        &vm::o_pnum,
        &vm::o_pnil,
        &vm::o_pstr,
        &vm::o_newv,
        &vm::o_newh,
        &vm::o_newf,
        &vm::o_happ,
        &vm::o_para,
        &vm::o_deft,
        &vm::o_dyn,
        &vm::o_lnot,
        &vm::o_usub,
        &vm::o_bnot,
        &vm::o_btor,
        &vm::o_btxor,
        &vm::o_btand,
        &vm::o_add,
        &vm::o_sub,
        &vm::o_mul,
        &vm::o_div,
        &vm::o_lnk,
        &vm::o_addc,
        &vm::o_subc,
        &vm::o_mulc,
        &vm::o_divc,
        &vm::o_lnkc,
        &vm::o_addeq,
        &vm::o_subeq,
        &vm::o_muleq,
        &vm::o_diveq,
        &vm::o_lnkeq,
        &vm::o_bandeq,
        &vm::o_boreq,
        &vm::o_bxoreq,
        &vm::o_addeqc,
        &vm::o_subeqc,
        &vm::o_muleqc,
        &vm::o_diveqc,
        &vm::o_lnkeqc,
        &vm::o_addecp,
        &vm::o_subecp,
        &vm::o_mulecp,
        &vm::o_divecp,
        &vm::o_lnkecp,
        &vm::o_meq,
        &vm::o_eq,
        &vm::o_neq,
        &vm::o_less,
        &vm::o_leq,
        &vm::o_grt,
        &vm::o_geq,
        &vm::o_lessc,
        &vm::o_leqc,
        &vm::o_grtc,
        &vm::o_geqc,
        &vm::o_pop,
        &vm::o_jmp,
        &vm::o_jt,
        &vm::o_jf,
        &vm::o_cnt,
        &vm::o_findex,
        &vm::o_feach,
        &vm::o_callg,
        &vm::o_calll,
        &vm::o_upval,
        &vm::o_callv,
        &vm::o_callvi,
        &vm::o_callh,
        &vm::o_callfv,
        &vm::o_callfh,
        &vm::o_callb,
        &vm::o_slcbeg,
        &vm::o_slcend,
        &vm::o_slc,
        &vm::o_slc2,
        &vm::o_mcallg,
        &vm::o_mcalll,
        &vm::o_mupval,
        &vm::o_mcallv,
        &vm::o_mcallh,
        &vm::o_ret
    };

public:

    /* constructor of vm instance */
    vm() {
        ctx.stack = new var[VM_STACK_DEPTH];
        global = new var[VM_STACK_DEPTH];
    }
    ~vm() {
        delete[] ctx.stack;
        delete[] global;
    }

    /* execution entry */
    void run(const codegen&,                   // get generated code
             const linker&,                    // get list of used files
             const std::vector<std::string>&); // get command line arguments

    /* set detail report info flag */
    void set_detail_report_info(bool flag) {
        verbose = flag;
    }

    /* set repl mode flag */
    void set_repl_mode_flag(bool flag) {
        is_repl_mode = flag;
    }

    /* set repl output flag */
    void set_allow_repl_output_flag(bool flag) {
        allow_repl_output = flag;
    }

    /* set limit mode flag */
    void set_limit_mode_flag(bool flag) {
        flag_limited_mode = flag;
    }
};

inline bool vm::boolify(const var& val) {
    if (val.is_num()) {
        return val.num();
    } else if (val.is_str()) {
        const f64 num = util::str_to_num(val.str().c_str());
        return std::isnan(num)? !val.str().empty() : num;
    } else if (val.is_vec()) {
        return val.vec().size() > 0;
    } else if (val.is_hash()) {
        return val.hash().size() > 0;
    } else if (val.is_func() || val.is_ghost() || val.is_coroutine()) {
        return true;
    } else if (val.is_map()) {
        return val.map().size() > 0;
    }
    return false;
}

inline void vm::o_repl() {
    // reserved for repl mode stack top value output
    if (allow_repl_output) {
        std::cout << ctx.top[0] << "\n";
    }
}

inline void vm::o_intl() {
    ctx.top[0].func().local.resize(imm[ctx.pc], nil);
    ctx.top[0].func().local_size = imm[ctx.pc];
}

inline void vm::o_loadg() {
    global[imm[ctx.pc]] = (ctx.top--)[0];
}

inline void vm::o_loadl() {
    ctx.localr[imm[ctx.pc]] = (ctx.top--)[0];
}

inline void vm::o_loadu() {
    ctx.funcr.func().upval[(imm[ctx.pc]>>16) & 0xffff]
                    .upval()[imm[ctx.pc] & 0xffff] = (ctx.top--)[0];
}

inline void vm::o_dup() {
    ctx.top[1] = ctx.top[0];
    ++ctx.top;
}

inline void vm::o_pnum() {
    (++ctx.top)[0] = var::num(const_number[imm[ctx.pc]]);
}

inline void vm::o_pnil() {
    (++ctx.top)[0] = nil;
}

inline void vm::o_pstr() {
    (++ctx.top)[0] = ngc.strs[imm[ctx.pc]];
}

inline void vm::o_newv() {
    var newv = ngc.alloc(vm_type::vm_vec);
    auto& vec = newv.vec().elems;
    vec.resize(imm[ctx.pc]);
    // use top-=imm[pc]-1 here will cause error if imm[pc] is 0
    ctx.top = ctx.top - imm[ctx.pc] + 1;

    for(u64 i = 0; i<imm[ctx.pc]; ++i) {
        vec[i] = ctx.top[i];
    }
    ctx.top[0] = newv;
}

inline void vm::o_newh() {
    (++ctx.top)[0] = ngc.alloc(vm_type::vm_hash);
}

inline void vm::o_newf() {
    (++ctx.top)[0] = ngc.alloc(vm_type::vm_func);
    auto& func = ctx.top[0].func();
    func.entry = imm[ctx.pc];
    func.parameter_size = 1;

    /* this means you create a new function in local scope */
    if (ctx.localr) {
        // copy upval scope list from upper level function
        func.upval = ctx.funcr.func().upval;

        // function created in the same local scope shares same closure
        var upval = (ctx.upvalr.is_nil())?
            ngc.alloc(vm_type::vm_upval):
            ctx.upvalr;
        // if no upval scope exists, now it's time to create one
        if (ctx.upvalr.is_nil()) {
            upval.upval().size = ctx.funcr.func().local_size;
            upval.upval().stack_frame_offset = ctx.localr;
            ctx.upvalr = upval;
        }

        func.upval.push_back(upval);
    }
}

inline void vm::o_happ() {
    ctx.top[-1].hash().elems[const_string[imm[ctx.pc]]] = ctx.top[0];
    --ctx.top;
}

inline void vm::o_para() {
    auto& func = ctx.top[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[const_string[imm[ctx.pc]]] = func.parameter_size;
    func.local[func.parameter_size++] = var::none();
}

inline void vm::o_deft() {
    var val = ctx.top[0];
    auto& func = (--ctx.top)[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[const_string[imm[ctx.pc]]] = func.parameter_size;
    func.local[func.parameter_size++] = val;
}

inline void vm::o_dyn() {
    ctx.top[0].func().dynamic_parameter_index = imm[ctx.pc];
    ctx.top[0].func().dynamic_parameter_name = const_string[imm[ctx.pc]];
}

inline void vm::o_lnot() {
    var val = ctx.top[0];
    switch(val.type) {
        case vm_type::vm_nil: ctx.top[0] = one; break;
        case vm_type::vm_num: ctx.top[0] = val.num()? zero : one; break;
        case vm_type::vm_str: {
            const f64 num = util::str_to_num(val.str().c_str());
            if (std::isnan(num)) {
                ctx.top[0] = var::num(static_cast<f64>(val.str().empty()));
            } else {
                ctx.top[0] = num? zero:one;
            }
        } break;
        default:
            die("cannot do not-operation on " + type_name_string(val));
            return;
    }
}

inline void vm::o_usub() {
    ctx.top[0] = var::num(-ctx.top[0].to_num());
}

inline void vm::o_bnot() {
    ctx.top[0] = var::num(~static_cast<int32_t>(ctx.top[0].num()));
}

inline void vm::o_btor() {
    ctx.top[-1] = var::num(
        static_cast<i32>(ctx.top[-1].to_num()) |
        static_cast<i32>(ctx.top[0].to_num())
    );
    --ctx.top;
}

inline void vm::o_btxor() {
    ctx.top[-1] = var::num(
        static_cast<i32>(ctx.top[-1].to_num()) ^
        static_cast<i32>(ctx.top[0].to_num())
    );
    --ctx.top;
}

inline void vm::o_btand() {
    ctx.top[-1] = var::num(
        static_cast<i32>(ctx.top[-1].to_num()) &
        static_cast<i32>(ctx.top[0].to_num())
    );
    --ctx.top;
}

#define op_calc(type)\
    ctx.top[-1] = var::num(ctx.top[-1].to_num() type ctx.top[0].to_num());\
    --ctx.top;

inline void vm::o_add() { op_calc(+); }
inline void vm::o_sub() { op_calc(-); }
inline void vm::o_mul() { op_calc(*); }
inline void vm::o_div() { op_calc(/); }
inline void vm::o_lnk() {
    // concat two vectors into one
    if (ctx.top[-1].is_vec() && ctx.top[0].is_vec()) {
        ngc.temp = ngc.alloc(vm_type::vm_vec);
        for(auto& i : ctx.top[-1].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        for(auto& i : ctx.top[0].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        ctx.top[-1] = ngc.temp;
        ngc.temp = nil;
        --ctx.top;
        return;
    }
    // concat strings
    ctx.top[-1] = ngc.newstr(ctx.top[-1].to_str() + ctx.top[0].to_str());
    --ctx.top;
}

#define op_calc_const(type)\
    ctx.top[0] = var::num(ctx.top[0].to_num() type const_number[imm[ctx.pc]]);

inline void vm::o_addc() { op_calc_const(+); }
inline void vm::o_subc() { op_calc_const(-); }
inline void vm::o_mulc() { op_calc_const(*); }
inline void vm::o_divc() { op_calc_const(/); }
inline void vm::o_lnkc() {
    ctx.top[0] = ngc.newstr(ctx.top[0].to_str() + const_string[imm[ctx.pc]]);
}

// top[0] stores the value of memr[0], to avoid being garbage-collected
// so when the calculation ends, top-=1, then top-=imm[pc]
// because this return value is meaningless if on stack when imm[pc] = 1
// like this: func{a+=c;}(); the result of 'a+c' will no be used later, imm[pc] = 1
// but if b+=a+=c; the result of 'a+c' will be used later, imm[pc] = 0
#define op_calc_eq(type)\
    ctx.top[-1] = ctx.memr[0] = var::num(\
        ctx.memr[0].to_num() type ctx.top[-1].to_num()\
    );\
    ctx.memr = nullptr;\
    ctx.top -= imm[ctx.pc] + 1;

inline void vm::o_addeq() { op_calc_eq(+); }
inline void vm::o_subeq() { op_calc_eq(-); }
inline void vm::o_muleq() { op_calc_eq(*); }
inline void vm::o_diveq() { op_calc_eq(/); }
inline void vm::o_lnkeq() {
    // concat two vectors into one
    if (ctx.top[-1].is_vec() && ctx.memr[0].is_vec()) {
        ngc.temp = ngc.alloc(vm_type::vm_vec);
        for(auto i : ctx.memr[0].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        for(auto i : ctx.top[-1].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        ctx.top[-1] = ctx.memr[0] = ngc.temp;
        ngc.temp = nil;
        ctx.memr = nullptr;
        ctx.top -= imm[ctx.pc]+1;
        return;
    }

    ctx.top[-1] = ctx.memr[0] = ngc.newstr(
        ctx.memr[0].to_str() + ctx.top[-1].to_str()
    );
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc] + 1;
}

inline void vm::o_bandeq() {
    ctx.top[-1] = ctx.memr[0] = var::num(
        static_cast<i32>(ctx.memr[0].to_num())&
        static_cast<i32>(ctx.top[-1].to_num())
    );
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

inline void vm::o_boreq() {
    ctx.top[-1] = ctx.memr[0] = var::num(
        static_cast<i32>(ctx.memr[0].to_num())|
        static_cast<i32>(ctx.top[-1].to_num())
    );
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

inline void vm::o_bxoreq() {
    ctx.top[-1] = ctx.memr[0] = var::num(
        static_cast<i32>(ctx.memr[0].to_num()) ^
        static_cast<i32>(ctx.top[-1].to_num())
    );
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc] + 1;
}

// top[0] stores the value of memr[0], to avoid being garbage-collected
// so when the calculation ends, top-=imm[pc]>>31
// because this return value is meaningless if on stack when imm[pc]>>31=1
// like this: func{a+=1;}(); the result of 'a+1' will no be used later, imm[pc]>>31=1
// but if b+=a+=1; the result of 'a+1' will be used later, imm[pc]>>31=0
#define op_calc_eq_const(type)\
    ctx.top[0] = ctx.memr[0] = var::num(\
        ctx.memr[0].to_num() type const_number[imm[ctx.pc]]\
    );\
    ctx.memr = nullptr;

inline void vm::o_addeqc() { op_calc_eq_const(+); }
inline void vm::o_subeqc() { op_calc_eq_const(-); }
inline void vm::o_muleqc() { op_calc_eq_const(*); }
inline void vm::o_diveqc() { op_calc_eq_const(/); }
inline void vm::o_lnkeqc() {
    ctx.top[0] = ctx.memr[0] = ngc.newstr(
        ctx.memr[0].to_str() + const_string[imm[ctx.pc]]
    );
    ctx.memr = nullptr;
}

#define op_calc_eq_const_and_pop(type)\
    ctx.top[0] = ctx.memr[0] = var::num(\
        ctx.memr[0].to_num() type const_number[imm[ctx.pc]]\
    );\
    ctx.memr = nullptr;\
    --ctx.top;

inline void vm::o_addecp() { op_calc_eq_const_and_pop(+); }
inline void vm::o_subecp() { op_calc_eq_const_and_pop(-); }
inline void vm::o_mulecp() { op_calc_eq_const_and_pop(*); }
inline void vm::o_divecp() { op_calc_eq_const_and_pop(/); }
inline void vm::o_lnkecp() {
    ctx.top[0] = ctx.memr[0] = ngc.newstr(
        ctx.memr[0].to_str() + const_string[imm[ctx.pc]]
    );
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
    if (val1.is_nil() && val2.is_nil()) {
        ctx.top[0] = one;
    } else if (val1.is_str() && val2.is_str()) {
        ctx.top[0] = (val1.str()==val2.str())? one : zero;
    } else if ((val1.is_num() || val2.is_num())
        && !val1.is_nil() && !val2.is_nil()) {
        ctx.top[0] = (val1.to_num()==val2.to_num())? one : zero;
    } else {
        ctx.top[0] = (val1==val2)? one : zero;
    }
}

inline void vm::o_neq() {
    var val2 = ctx.top[0];
    var val1 = (--ctx.top)[0];
    if (val1.is_nil() && val2.is_nil()) {
        ctx.top[0] = zero;
    } else if (val1.is_str() && val2.is_str()) {
        ctx.top[0] = (val1.str()!=val2.str())? one : zero;
    } else if ((val1.is_num() || val2.is_num())
        && !val1.is_nil() && !val2.is_nil()) {
        ctx.top[0] = (val1.to_num()!=val2.to_num())? one : zero;
    } else {
        ctx.top[0] = (val1!=val2)? one : zero;
    }
}

#define op_cmp(type)\
    --ctx.top;\
    ctx.top[0] = (ctx.top[0].to_num() type ctx.top[1].to_num())? one : zero;

inline void vm::o_less() { op_cmp(<); }
inline void vm::o_leq() { op_cmp(<=); }
inline void vm::o_grt() { op_cmp(>); }
inline void vm::o_geq() { op_cmp(>=); }

#define op_cmp_const(type)\
    ctx.top[0] = (ctx.top[0].to_num() type const_number[imm[ctx.pc]])? one : zero;

inline void vm::o_lessc() { op_cmp_const(<); }
inline void vm::o_leqc() { op_cmp_const(<=); }
inline void vm::o_grtc() { op_cmp_const(>); }
inline void vm::o_geqc() { op_cmp_const(>=); }

inline void vm::o_pop() {
    --ctx.top;
}

inline void vm::o_jmp() {
    ctx.pc = imm[ctx.pc] - 1;
}

inline void vm::o_jt() {
    // jump true needs to reserve the result on stack
    // because conditional expression in nasal has return value
    if (boolify(ctx.top[0])) {
        ctx.pc = imm[ctx.pc] - 1;
    }
}

inline void vm::o_jf() {
    // jump false doesn't need to reserve result
    if (!boolify(ctx.top[0])) {
        ctx.pc = imm[ctx.pc] - 1;
    }
    --ctx.top;
}

inline void vm::o_cnt() {
    if (!ctx.top[0].is_vec()) {
        die("must use vector in forindex/foreach but get " +
            type_name_string(ctx.top[0])
        );
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
    (++ctx.top)[0] = global[imm[ctx.pc]];
}

inline void vm::o_calll() {
    (++ctx.top)[0] = ctx.localr[imm[ctx.pc]];
}

inline void vm::o_upval() {
    (++ctx.top)[0] = ctx.funcr.func()
        .upval[(imm[ctx.pc] >> 16) & 0xffff]
        .upval()[imm[ctx.pc] & 0xffff];
}

inline void vm::o_callv() {
    var val = ctx.top[0];
    var vec = (--ctx.top)[0];
    if (vec.is_vec()) {
        ctx.top[0] = vec.vec().get_value(val.to_num());
        if (ctx.top[0].is_none()) {
            die(report_out_of_range(val.to_num(), vec.vec().size()));
            return;
        }
    } else if (vec.is_hash()) {
        if (!val.is_str()) {
            die("must use string as the key but get " + type_name_string(val));
            return;
        }
        ctx.top[0] = vec.hash().get_value(val.str());
        if (ctx.top[0].is_none()) {
            die(report_key_not_found(val.str(), vec.hash()));
            return;
        } else if (ctx.top[0].is_func()) {
            ctx.top[0].func().local[0] = val; // 'me'
        }
    } else if (vec.is_str()) {
        const auto& str = vec.str();
        i32 num = val.to_num();
        i32 len = str.length();
        if (num<-len || num>=len) {
            die(report_out_of_range(num, str.size()));
            return;
        }
        ctx.top[0] = var::num(
            static_cast<f64>(static_cast<u8>(str[num>=0? num : num + len]))
        );
    } else if (vec.is_map()) {
        if (!val.is_str()) {
            die("must use string as the key but get " + type_name_string(val));
            return;
        }
        ctx.top[0] = vec.map().get_value(val.str());
        if (ctx.top[0].is_none()) {
            die("cannot find symbol \"" + val.str() + "\"");
            return;
        }
    } else {
        die("must call a vector/hash/string but get " + type_name_string(vec));
        return;
    }
}

inline void vm::o_callvi() {
    var val = ctx.top[0];
    if (!val.is_vec()) {
        die("must use a vector but get " + type_name_string(val));
        return;
    }
    // cannot use operator[], because this may cause overflow
    (++ctx.top)[0] = val.vec().get_value(imm[ctx.pc]);
    if (ctx.top[0].is_none()) {
        die(report_out_of_range(imm[ctx.pc], val.vec().size()));
        return;
    }
}

inline void vm::o_callh() {
    var val = ctx.top[0];
    if (!val.is_hash() && !val.is_map()) {
        die("must call a hash but get " + type_name_string(val));
        return;
    }

    const auto& str = const_string[imm[ctx.pc]];
    if (val.is_hash()) {
        ctx.top[0] = val.hash().get_value(str);
    } else {
        ctx.top[0] = val.map().get_value(str);
    }

    // report key not found if get_value returns none
    if (ctx.top[0].is_none()) {
        val.is_hash()
            ? die(report_key_not_found(str, val.hash()))
            : die("cannot find symbol \"" + str + "\"");
        return;
    }

    // if get function from hash, set 'me'
    if (ctx.top[0].is_func() && val.is_hash()) {
        ctx.top[0].func().local[0] = val; // 'me'
    }
}

inline void vm::o_callfv() {
    const auto argc = imm[ctx.pc]; // arguments counter
    var* local = ctx.top - argc + 1; // arguments begin address
    if (!local[-1].is_func()) {
        die("must call a function but get " + type_name_string(local[-1]));
        return;
    }
    const auto& func = local[-1].func();

    // swap funcr with local[-1]
    var tmp = local[-1];
    local[-1] = ctx.funcr;
    ctx.funcr = tmp;

    // top-argc+lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if (ctx.top-argc+func.local_size+3>=ctx.canary) {
        die("stack overflow");
        return;
    }
    // parameter size is func->psize-1, 1 is reserved for "me"
    const u64 parameter_size = func.parameter_size-1;
    if (argc<parameter_size && func.local[argc+1].is_none()) {
        die(report_lack_arguments(argc, func));
        return;
    }

    // load dynamic argument, default nil, for better performance
    var dynamic = nil;
    if (func.dynamic_parameter_index>=0) {
        // load dynamic argument
        dynamic = ngc.alloc(vm_type::vm_vec);
        for(u64 i = parameter_size; i<argc; ++i) {
            dynamic.vec().elems.push_back(local[i]);
        }
    } else if (parameter_size<argc) {
        // load arguments to default dynamic argument "arg", located at stack+1
        dynamic = ngc.alloc(vm_type::vm_vec);
        for(u64 i = parameter_size; i<argc; ++i) {
            dynamic.vec().elems.push_back(local[i]);
        }
    }

    // should reset stack top after allocating vector
    // because this may cause gc
    // then all the available values the vector needs
    // are all outside the stack top and may be
    // collected incorrectly
    ctx.top = local + func.local_size;

    // use (std::min) to avoid compilation error in MSVC
    // MSVC windows.h uses macro std::min
    const u64 min_size = (std::min)(parameter_size, argc);

    // load arguments
    for(u64 i = min_size; i>=1; --i) {
        local[i] = local[i-1];
    }
    local[0] = func.local[0]; // load "me"

    // load local scope & default arguments
    for(u64 i = min_size + 1; i<func.local_size; ++i) {
        local[i] = func.local[i];
    }

    // load dynamic argument
    local[func.dynamic_parameter_index >= 0
            ? parameter_size + 1
            : func.local_size - 1] = dynamic;

    ctx.top[0] = ctx.upvalr;
    (++ctx.top)[0] = var::addr(ctx.localr);
    (++ctx.top)[0] = var::ret(ctx.pc);
    ctx.pc = func.entry-1;
    ctx.localr = local;
    ctx.upvalr = nil;
}

inline void vm::o_callfh() {
    const auto& hash = ctx.top[0].hash().elems;
    if (!ctx.top[-1].is_func()) {
        die("must call a function but get " + type_name_string(ctx.top[-1]));
        return;
    }
    const auto& func = ctx.top[-1].func();
    var tmp = ctx.top[-1];
    ctx.top[-1] = ctx.funcr;
    ctx.funcr = tmp;

    // top -1(hash) +lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if (ctx.top+func.local_size+2>= ctx.canary) {
        die("stack overflow");
        return;
    }
    // dynamic parameter is not allowed in this kind of function call
    if (func.dynamic_parameter_index>=0) {
        die("special call cannot use dynamic argument \"" +
            const_string[func.dynamic_parameter_index] + "\""
        );
        return;
    }

    var* local = ctx.top;
    ctx.top += func.local_size;
    for(u32 i = 0; i<func.local_size; ++i) {
        local[i] = func.local[i];
    }
    
    bool lack_arguments_flag = false;
    for(const auto& i : func.keys) {
        const auto& key = i.first;
        if (hash.count(key)) {
            local[i.second] = hash.at(key);
        } else if (local[i.second].is_none()) {
            lack_arguments_flag = true;
        }
    }
    if (lack_arguments_flag) {
        die(report_special_call_lack_arguments(local, func));
        return;
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
    auto function_pointer = native_function[imm[ctx.pc]].func;
    var result = (*function_pointer)(&ctx, &ngc);

    // so we use tmp variable to store this return value
    // and set it to top[0] later
    ctx.top[0] = result;

    // if get none, this means errors occurred when calling this native function
    if (ctx.top[0].is_none()) {
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
    (++ctx.top)[0] = ngc.alloc(vm_type::vm_vec);
    if (!ctx.top[-1].is_vec()) {
        die("must slice a vector but get " + type_name_string(ctx.top[-1]));
        return;
    }
}

inline void vm::o_slcend() {
    ctx.top[-1] = ctx.top[0];
    --ctx.top;
}

inline void vm::o_slc() {
    var val = (ctx.top--)[0];
    var res = ctx.top[-1].vec().get_value(val.to_num());
    if (res.is_none()) {
        die(report_out_of_range(val.to_num(), ctx.top[-1].vec().size()));
        return;
    }
    ctx.top[0].vec().elems.push_back(res);
}

inline void vm::o_slc2() {
    var val2 = (ctx.top--)[0];
    var val1 = (ctx.top--)[0];
    const auto& ref = ctx.top[-1].vec().elems;
    auto& aim = ctx.top[0].vec().elems;

    vm_type type1 = val1.type;
    vm_type type2 = val2.type;
    i32 num1 = val1.to_num();
    i32 num2 = val2.to_num();
    i32 size = ref.size();
    if (val1.is_nil() && val2.is_nil()) {
        num1 = 0;
        num2 = size-1;
    } else if (val1.is_nil() && type2!=vm_type::vm_nil) {
        num1 = num2<0? -size:0;
    } else if (type1!=vm_type::vm_nil && val2.is_nil()) {
        num2 = num1<0? -1:size-1;
    }

    if (num1<-size || num1>=size || num2<-size || num2>=size) {
        die("index " + std::to_string(num1) + ":" +
            std::to_string(num2) + " out of range, real size is " +
            std::to_string(size)
        );
        return;
    } else if (num1<=num2) {
        for(i32 i = num1; i<=num2; ++i) {
            aim.push_back(i>=0? ref[i]:ref[i+size]);
        }
    }
}

inline void vm::o_mcallg() {
    ctx.memr = global+imm[ctx.pc];
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
           .upval[(imm[ctx.pc]>>16) & 0xffff]
           .upval()[imm[ctx.pc] & 0xffff]
    );
    (++ctx.top)[0] = ctx.memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

inline void vm::o_mcallv() {
    var val = ctx.top[0];     // index
    var vec = (--ctx.top)[0]; // mcall vector, reserved on stack to avoid gc
    if (vec.is_vec()) {
        ctx.memr = vec.vec().get_memory(val.to_num());
        if (!ctx.memr) {
            die(report_out_of_range(val.to_num(), vec.vec().size()));
            return;
        }
    } else if (vec.is_hash()) { // do mcallh but use the mcallv way
        if (!val.is_str()) {
            die("must use string as the key but get " + type_name_string(val));
            return;
        }
        auto& ref = vec.hash();
        const auto& str = val.str();
        ctx.memr = ref.get_memory(str);
        if (!ctx.memr) {
            ref.elems[str] = nil;
            ctx.memr = ref.get_memory(str);
        }
    } else if (vec.is_map()) {
        if (!val.is_str()) {
            die("must use string as the key but get " + type_name_string(val));
            return;
        }
        auto& ref = vec.map();
        const auto& str = val.str();
        ctx.memr = ref.get_memory(str);
        if (!ctx.memr) {
            die("cannot find symbol \"" + str + "\"");
        }
    } else {
        die("cannot get memory space in this type");
        return;
    }
}

inline void vm::o_mcallh() {
    // mcall hash, reserved on stack to avoid gc, so do not do ctx.top--
    var hash = ctx.top[0];
    if (!hash.is_hash() && !hash.is_map()) {
        die("must call a hash/namespace but get " + type_name_string(hash));
        return;
    }

    const auto& key = const_string[imm[ctx.pc]];

    // map is for nasal namespace type, for example `globals`
    if (hash.is_map()) {
        ctx.memr = hash.map().get_memory(key);
        if (!ctx.memr) {
            die("cannot find symbol \"" + key + "\"");
        }
        return;
    }

    // call hash member
    auto& ref = hash.hash();
    ctx.memr = ref.get_memory(key);
    // create a new key if not exists
    if (!ctx.memr) {
        ref.elems[key] = nil;
        ctx.memr = ref.get_memory(key);
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

    // synchronize upvalue
    if (up.is_upval()) {
        auto& upval = up.upval();
        auto size = func.func().local_size;
        upval.on_stack = false;
        upval.elems.resize(size);
        for(u64 i = 0; i < size; ++i) {
            upval.elems[i] = local[i];
        }
    }

    // cannot use gc.cort to judge,
    // because there maybe another function call inside but return here
    // coroutine function ends with setting pc to 0
    if (!ctx.pc) {
        ngc.context_reserve();
    }
}

}
