#pragma once

#include <iomanip>
#include <stack>

#include "nasal_new_import.h"
#include "nasal_new_gc.h"
#include "nasal_new_codegen.h"

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
    const f64* cnum=nullptr;    // constant numbers
    const std::string* cstr=nullptr; // constant symbols and strings
    std::vector<u32> imm;       // immediate number table
    
    /* garbage collector */
    gc ngc;

    /* main stack */
    var stack[STACK_DEPTH];

    /* values used for debugger */
    const std::string* files=nullptr;    // file name list
    const opcode* bytecode=nullptr; // bytecode buffer address

    /* vm initializing function */
    void init(
        const std::vector<std::string>&,
        const std::vector<f64>&,
        const std::vector<opcode>&,
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
    vm(): ngc(&ctx), verbose(false) {}

    /* execution entry */
    void run(
        const codegen&,
        const linker&,
        const std::vector<std::string>&,
        const bool
    );
};
