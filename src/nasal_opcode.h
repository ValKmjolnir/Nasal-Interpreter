#pragma once

#include "nasal.h"
#include "natives/builtin.h"

#include <iostream>

namespace nasal {

enum op_code_type: u8 {
    op_exit,    // stop the virtual machine
    op_repl,    // in repl mode: print value on stack top
    op_intl,    // local scope size
    op_loadg,   // load global value
    op_loadl,   // load local value
    op_loadu,   // load upvalue
    op_dup,     // copy value on stack top
    op_pnum,    // push constant number to the stack
    op_pnil,    // push constant nil to the stack
    op_pstr,    // push constant std::string to the stack
    op_newv,    // push new vector with initial values from stack
    op_newh,    // push new hash to the stack
    op_newf,    // push new function to the stack
    op_happ,    // hash append
    op_para,    // normal  parameter
    op_deft,    // default parameter
    op_dyn,     // dynamic parameter
    op_lnot,    // ! logical negation
    op_usub,    // - negation
    op_bnot,    // ~ bitwise not static_cast<i32>
    op_btor,    // | bitwise or
    op_btxor,   // ^ bitwise xor
    op_btand,   // & bitwise and
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
    op_addeq,   // += maybe pop stack top
    op_subeq,   // -= maybe pop stack top
    op_muleq,   // *= maybe pop stack top
    op_diveq,   // /= maybe pop stack top
    op_lnkeq,   // ~= maybe pop stack top
    op_btandeq, // &= maybe pop stack top
    op_btoreq,  // |= maybe pop stack top
    op_btxoreq, // ^= maybe pop stack top
    op_addeqc,  // += const don't pop stack top
    op_subeqc,  // -= const don't pop stack top
    op_muleqc,  // *= const don't pop stack top
    op_diveqc,  // /= const don't pop stack top
    op_lnkeqc,  // ~= const don't pop stack top
    op_addecp,  // += const and pop stack top
    op_subecp,  // -= const and pop stack top
    op_mulecp,  // *= const and pop stack top
    op_divecp,  // /= const and pop stack top
    op_lnkecp,  // ~= concat const std::string and pop stack top
    op_meq,     // = maybe pop stack top
    op_eq,      // == compare operator
    op_neq,     // != compare operator
    op_less,    // < compare operator
    op_leq,     // <= compare operator
    op_grt,     // > compare operator
    op_geq,     // >= compare operator
    op_lessc,    // < const compare operator
    op_leqc,    // <= const compare operator
    op_grtc,    // > const compare operator
    op_geqc,    // >= const compare operator
    op_pop,     // pop a value out of stack top
    op_jmp,     // jump absolute address with no condition
    op_jt,      // used in operator and/or, jmp when condition is true and DO NOT POP
    op_jf,      // used in conditional/loop, jmp when condition is false and POP STACK
    op_cnt,     // add counter for forindex/foreach
    op_findex,  // index counter on the top of forindex_stack plus 1
    op_feach,   // index counter on the top of forindex_stack plus 1 and get the value in vector
    op_callg,   // get value in global scope
    op_calll,   // get value in local scope
    op_upval,   // get value in closure, high 16 as the index of upval, low 16 as the index of local
    op_callv,   // call vec[index]
    op_callvi,  // call vec[immediate] (used in multi-assign/multi-define)
    op_callh,   // call hash.label
    op_callfv,  // call function(vector as parameters)
    op_callfh,  // call function(hash as parameters)
    op_callb,   // call native functions
    op_slcbeg,  // begin of slice like: vec[1, 2, 3:6, 0, -1]
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

static std::unordered_map<op_code_type, std::string> operand_name_table = {
    {op_code_type::op_exit, "exit  "},
    {op_code_type::op_repl, "repl  "},
    {op_code_type::op_intl, "intl  "},
    {op_code_type::op_loadg, "loadg "},
    {op_code_type::op_loadl, "loadl "},
    {op_code_type::op_loadu, "loadu "},
    {op_code_type::op_dup, "dup   "},
    {op_code_type::op_pnum, "pnum  "},
    {op_code_type::op_pnil, "pnil  "},
    {op_code_type::op_pstr, "pstr  "},
    {op_code_type::op_newv, "newv  "},
    {op_code_type::op_newh, "newh  "},
    {op_code_type::op_newf, "newf  "},
    {op_code_type::op_happ, "happ  "},
    {op_code_type::op_para, "para  "},
    {op_code_type::op_deft, "def   "},
    {op_code_type::op_dyn, "dyn   "},
    {op_code_type::op_lnot, "lnot  "},
    {op_code_type::op_usub, "usub  "},
    {op_code_type::op_bnot, "bitnot"},
    {op_code_type::op_btor, "bitor "},
    {op_code_type::op_btxor, "bitxor"},
    {op_code_type::op_btand, "bitand"},
    {op_code_type::op_add, "add   "},
    {op_code_type::op_sub, "sub   "},
    {op_code_type::op_mul, "mult  "},
    {op_code_type::op_div, "div   "},
    {op_code_type::op_lnk, "lnk   "},
    {op_code_type::op_addc, "addc  "},
    {op_code_type::op_subc, "subc  "},
    {op_code_type::op_mulc, "multc "},
    {op_code_type::op_divc, "divc  "},
    {op_code_type::op_lnkc, "lnkc  "},
    {op_code_type::op_addeq, "addeq "},
    {op_code_type::op_subeq, "subeq "},
    {op_code_type::op_muleq, "muleq "},
    {op_code_type::op_diveq, "diveq "},
    {op_code_type::op_lnkeq, "lnkeq "},
    {op_code_type::op_btandeq, "bandeq"},
    {op_code_type::op_btoreq, "boreq "},
    {op_code_type::op_btxoreq, "bxoreq"},
    {op_code_type::op_addeqc, "addeqc"},
    {op_code_type::op_subeqc, "subeqc"},
    {op_code_type::op_muleqc, "muleqc"},
    {op_code_type::op_diveqc, "diveqc"},
    {op_code_type::op_lnkeqc, "lnkeqc"},
    {op_code_type::op_addecp, "addecp"},
    {op_code_type::op_subecp, "subecp"},
    {op_code_type::op_mulecp, "mulecp"},
    {op_code_type::op_divecp, "divecp"},
    {op_code_type::op_lnkecp, "lnkecp"},
    {op_code_type::op_meq, "meq   "},
    {op_code_type::op_eq, "eq    "},
    {op_code_type::op_neq, "neq   "},
    {op_code_type::op_less, "less  "},
    {op_code_type::op_leq, "leq   "},
    {op_code_type::op_grt, "grt   "},
    {op_code_type::op_geq, "geq   "},
    {op_code_type::op_lessc, "lessc "},
    {op_code_type::op_leqc, "leqc  "},
    {op_code_type::op_grtc, "grtc  "},
    {op_code_type::op_geqc, "geqc  "},
    {op_code_type::op_pop, "pop   "},
    {op_code_type::op_jmp, "jmp   "},
    {op_code_type::op_jt, "jt    "},
    {op_code_type::op_jf, "jf    "},
    {op_code_type::op_cnt, "cnt   "},
    {op_code_type::op_findex, "findx "},
    {op_code_type::op_feach, "feach "},
    {op_code_type::op_callg, "callg "},
    {op_code_type::op_calll, "calll "},
    {op_code_type::op_upval, "upval "},
    {op_code_type::op_callv, "callv "},
    {op_code_type::op_callvi, "callvi"},
    {op_code_type::op_callh, "callh "},
    {op_code_type::op_callfv, "callfv"},
    {op_code_type::op_callfh, "callfh"},
    {op_code_type::op_callb, "callb "},
    {op_code_type::op_slcbeg, "slcbeg"},
    {op_code_type::op_slcend, "slcend"},
    {op_code_type::op_slc, "slice "},
    {op_code_type::op_slc2, "slice2"},
    {op_code_type::op_mcallg, "mcallg"},
    {op_code_type::op_mcalll, "mcalll"},
    {op_code_type::op_mupval, "mupval"},
    {op_code_type::op_mcallv, "mcallv"},
    {op_code_type::op_mcallh, "mcallh"},
    {op_code_type::op_ret, "ret   "}
};

struct opcode {
    u8  op;   // opcode
    u16 fidx; // source code file index
    u64 num;  // immediate num
    u64 line; // location line of source code
    opcode() = default;
    opcode(const opcode&) = default;
    opcode& operator=(const opcode&) = default;
};

class codestream {
private:
    opcode code;
    const u64 index;
    inline static const f64* const_number = nullptr;
    inline static const std::string* const_string = nullptr;
    inline static const nasal_builtin_table* natives = nullptr;
    inline static const std::string* files = nullptr;
    
public:
    codestream(const opcode& c, const u64 i): code(c), index(i) {}
    static void set(const f64*,
                    const std::string*,
                    const nasal_builtin_table*,
                    const std::string* file_list = nullptr);
    void dump(std::ostream&) const;
};

std::ostream& operator<<(std::ostream&, const codestream&);

}