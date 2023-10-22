#pragma once

#include "nasal.h"
#include "nasal_builtin.h"

#include <iostream>

namespace nasal {

enum op_code_type:u8 {
    op_exit,   // stop the virtual machine
    op_repl,   // in repl mode: print value on stack top
    op_intl,   // local scope size
    op_loadg,  // load global value
    op_loadl,  // load local value
    op_loadu,  // load upvalue
    op_pnum,   // push constant number to the stack
    op_pnil,   // push constant nil to the stack
    op_pstr,   // push constant std::string to the stack
    op_newv,   // push new vector with initial values from stack
    op_newh,   // push new hash to the stack
    op_newf,   // push new function to the stack
    op_happ,   // hash append
    op_para,   // normal  parameter
    op_deft,   // default parameter
    op_dyn,    // dynamic parameter
    op_lnot,   // ! logical negation
    op_usub,   // - negation
    op_bnot,   // ~ bitwise not static_cast<i32>
    op_btor,   // | bitwise or
    op_btxor,  // ^ bitwise xor
    op_btand,  // & bitwise and
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
    op_addeq,  // += maybe pop stack top
    op_subeq,  // -= maybe pop stack top
    op_muleq,  // *= maybe pop stack top
    op_diveq,  // /= maybe pop stack top
    op_lnkeq,  // ~= maybe pop stack top
    op_btandeq,// &= maybe pop stack top
    op_btoreq, // |= maybe pop stack top
    op_btxoreq,// ^= maybe pop stack top
    op_addeqc, // += const don't pop stack top
    op_subeqc, // -= const don't pop stack top
    op_muleqc, // *= const don't pop stack top
    op_diveqc, // /= const don't pop stack top
    op_lnkeqc, // ~= const don't pop stack top
    op_addecp, // += const and pop stack top
    op_subecp, // -= const and pop stack top
    op_mulecp, // *= const and pop stack top
    op_divecp, // /= const and pop stack top
    op_lnkecp, // ~= concat const std::string and pop stack top
    op_meq,    // = maybe pop stack top
    op_eq,     // == compare operator
    op_neq,    // != compare operator
    op_less,   // < compare operator
    op_leq,    // <= compare operator
    op_grt,    // > compare operator
    op_geq,    // >= compare operator
    op_lessc,  // < const compare operator
    op_leqc,   // <= const compare operator
    op_grtc,   // > const compare operator
    op_geqc,   // >= const compare operator
    op_pop,    // pop a value out of stack top
    op_jmp,    // jump absolute address with no condition
    op_jt,     // used in operator and/or,jmp when condition is true and DO NOT POP
    op_jf,     // used in conditional/loop,jmp when condition is false and POP STACK
    op_cnt,    // add counter for forindex/foreach
    op_findex, // index counter on the top of forindex_stack plus 1
    op_feach,  // index counter on the top of forindex_stack plus 1 and get the value in vector
    op_callg,  // get value in global scope
    op_calll,  // get value in local scope
    op_upval,  // get value in closure, high 16 as the index of upval, low 16 as the index of local
    op_callv,  // call vec[index]
    op_callvi, // call vec[immediate] (used in multi-assign/multi-define)
    op_callh,  // call hash.label
    op_callfv, // call function(vector as parameters)
    op_callfh, // call function(hash as parameters)
    op_callb,  // call native functions
    op_slcbeg, // begin of slice like: vec[1, 2, 3:6, 0, -1]
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

struct opcode {
    u8  op;   // opcode
    u16 fidx; // source code file index
    u32 num;  // immediate num
    u32 line; // location line of source code
    opcode() = default;
    opcode(const opcode&) = default;
    opcode& operator=(const opcode&) = default;
};

class codestream {
private:
    opcode code;
    const u32 index;
    inline static const f64* const_number = nullptr;
    inline static const std::string* const_string = nullptr;
    inline static const nasal_builtin_table* natives = nullptr;
    inline static const std::string* files = nullptr;
    
public:
    codestream(const opcode& c, const u32 i): code(c), index(i) {}
    static void set(
        const f64*, const std::string*,
        const nasal_builtin_table*,
        const std::string* file_list = nullptr
    );
    void dump(std::ostream&) const;
};

std::ostream& operator<<(std::ostream&, const codestream&);

extern const char* opname[];

}