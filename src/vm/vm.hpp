#pragma once

#include <iomanip>
#include <cstring>
#include <sstream>
#include <atomic>
#include <cmath>

#include "parse/linker.hpp"
#include "vm/gc.hpp"
#include "nasal_codegen.hpp"
#include "util/util.hpp"
#include "natives/registry.hpp"

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
    std::vector<nasal_builtin_info> native_function;

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
    void vm_init_entry(const std::vector<std::string>&,
                       const std::vector<f64>&,
                       const std::vector<nasal_builtin_info>&,
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
    bool boolify(const var&);
    void set_frame(const nas_func&, var*);

protected:
    /* vm operands */
    void o_repl();
    void o_intl();
    void o_loadg();
    void o_loadl();
    void o_loadu();
    void o_dup();
    void o_pnum();
    void o_pnil();
    void o_pstr();
    void o_newv();
    void o_newh();
    void o_newf();
    void o_happ();
    void o_para();
    void o_default();
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
        &vm::o_default,
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
        ctx.ctor();
        global = new var[VM_STACK_DEPTH];
    }
    ~vm() {
        ctx.dtor();
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

    auto get_gc_time_ms() const {
        return ngc.get_gc_time_ms();
    }

    auto get_total_memory() const {
        return ngc.get_total_memory();
    }

    void set_interrupt_ptr(std::atomic<bool>* p) {
        interrupt_ptr = p;
    }

private:
    std::atomic<bool>* interrupt_ptr = nullptr;
};

}
