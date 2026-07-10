#include "nasal_opcode.hpp"
#include "util/util.hpp"

namespace nasal {

const char* opcode::name(opcode_type op) {
    const std::unordered_map<opcode_type, const char*> operand_name_table = {
        { opcode_type::op_exit,    "exit  " },
        { opcode_type::op_repl,    "repl  " },
        { opcode_type::op_intl,    "intl  " },
        { opcode_type::op_loadg,   "loadg " },
        { opcode_type::op_loadl,   "loadl " },
        { opcode_type::op_loadu,   "loadu " },
        { opcode_type::op_dup,     "dup   " },
        { opcode_type::op_pnum,    "pnum  " },
        { opcode_type::op_pnil,    "pnil  " },
        { opcode_type::op_pstr,    "pstr  " },
        { opcode_type::op_newv,    "newv  " },
        { opcode_type::op_newh,    "newh  " },
        { opcode_type::op_newf,    "newf  " },
        { opcode_type::op_happ,    "happ  " },
        { opcode_type::op_para,    "para  " },
        { opcode_type::op_deft,    "def   " },
        { opcode_type::op_dyn,     "dyn   " },
        { opcode_type::op_lnot,    "lnot  " },
        { opcode_type::op_usub,    "usub  " },
        { opcode_type::op_bnot,    "bitnot" },
        { opcode_type::op_btor,    "bitor " },
        { opcode_type::op_btxor,   "bitxor" },
        { opcode_type::op_btand,   "bitand" },
        { opcode_type::op_add,     "add   " },
        { opcode_type::op_sub,     "sub   " },
        { opcode_type::op_mul,     "mult  " },
        { opcode_type::op_div,     "div   " },
        { opcode_type::op_lnk,     "lnk   " },
        { opcode_type::op_addc,    "addc  " },
        { opcode_type::op_subc,    "subc  " },
        { opcode_type::op_mulc,    "multc " },
        { opcode_type::op_divc,    "divc  " },
        { opcode_type::op_lnkc,    "lnkc  " },
        { opcode_type::op_addeq,   "addeq " },
        { opcode_type::op_subeq,   "subeq " },
        { opcode_type::op_muleq,   "muleq " },
        { opcode_type::op_diveq,   "diveq " },
        { opcode_type::op_lnkeq,   "lnkeq " },
        { opcode_type::op_btandeq, "bandeq" },
        { opcode_type::op_btoreq,  "boreq " },
        { opcode_type::op_btxoreq, "bxoreq" },
        { opcode_type::op_addeqc,  "addeqc" },
        { opcode_type::op_subeqc,  "subeqc" },
        { opcode_type::op_muleqc,  "muleqc" },
        { opcode_type::op_diveqc,  "diveqc" },
        { opcode_type::op_lnkeqc,  "lnkeqc" },
        { opcode_type::op_addecp,  "addecp" },
        { opcode_type::op_subecp,  "subecp" },
        { opcode_type::op_mulecp,  "mulecp" },
        { opcode_type::op_divecp,  "divecp" },
        { opcode_type::op_lnkecp,  "lnkecp" },
        { opcode_type::op_meq,     "meq   " },
        { opcode_type::op_eq,      "eq    " },
        { opcode_type::op_neq,     "neq   " },
        { opcode_type::op_less,    "less  " },
        { opcode_type::op_leq,     "leq   " },
        { opcode_type::op_grt,     "grt   " },
        { opcode_type::op_geq,     "geq   " },
        { opcode_type::op_lessc,   "lessc " },
        { opcode_type::op_leqc,    "leqc  " },
        { opcode_type::op_grtc,    "grtc  " },
        { opcode_type::op_geqc,    "geqc  " },
        { opcode_type::op_pop,     "pop   " },
        { opcode_type::op_jmp,     "jmp   " },
        { opcode_type::op_jt,      "jt    " },
        { opcode_type::op_jf,      "jf    " },
        { opcode_type::op_cnt,     "cnt   " },
        { opcode_type::op_findex,  "findx " },
        { opcode_type::op_feach,   "feach " },
        { opcode_type::op_callg,   "callg " },
        { opcode_type::op_calll,   "calll " },
        { opcode_type::op_upval,   "upval " },
        { opcode_type::op_callv,   "callv " },
        { opcode_type::op_callvi,  "callvi" },
        { opcode_type::op_callh,   "callh " },
        { opcode_type::op_callfv,  "callfv" },
        { opcode_type::op_callfh,  "callfh" },
        { opcode_type::op_callb,   "callb " },
        { opcode_type::op_slcbeg,  "slcbeg" },
        { opcode_type::op_slcend,  "slcend" },
        { opcode_type::op_slc,     "slice " },
        { opcode_type::op_slc2,    "slice2" },
        { opcode_type::op_mcallg,  "mcallg" },
        { opcode_type::op_mcalll,  "mcalll" },
        { opcode_type::op_mupval,  "mupval" },
        { opcode_type::op_mcallv,  "mcallv" },
        { opcode_type::op_mcallh,  "mcallh" },
        { opcode_type::op_ret,     "ret   " }
    };

    if (operand_name_table.count(op)) {
        return operand_name_table.at(op);
    }
    return "unknown";
}

void codestream::set(const f64* number_list,
                     const std::string* string_list,
                     const std::unordered_map<std::string, u64>& globals,
                     const nasal_builtin_info* native_table,
                     const std::string* file_list) {
    const_number = number_list;
    const_string = string_list;
    natives = native_table;
    files = file_list;

    global_variable.resize(globals.size());
    for (auto& [name, index]: globals) {
        global_variable[index] = name;
    }
}

void codestream::set(const f64* number_list,
                     const std::string* string_list,
                     const std::vector<std::string>& globals,
                     const nasal_builtin_info* native_table,
                     const std::string* file_list) {
    const_number = number_list;
    const_string = string_list;
    natives = native_table;
    files = file_list;

    global_variable = globals;
}

void codestream::dump(std::ostream& out) const {
    using std::setw;
    using std::setfill;
    using std::hex;
    using std::dec;

    const auto op = code.op;
    const auto num = code.num;

    // dump operand index and bytecode(hex format)
    out << hex << "0x"
        << setw(8) << setfill('0') << index << "    "
        << setw(2) << setfill('0') << static_cast<u32>(op) << ":" << dec;

    // dump immediate number(hex format)
    for (i32 i = 64-8; i>=0; i -= 8) {
        auto this_byte = ((num>>i)&0xff);
        out << hex << setw(2) << setfill('0') << this_byte << dec << " ";
    }

    // dump operand name
    out << "    " << opcode::name(static_cast<opcode_type>(op)) << "  ";

    switch (op) {
        case op_addeq:
        case op_subeq:
        case op_muleq:
        case op_diveq:
        case op_lnkeq:
        case op_meq:
        case op_btandeq:
        case op_btoreq:
        case op_btxoreq:
            out << hex << "0x" << num << dec << " sp-" << num;
            break;
        case op_addeqc:
        case op_subeqc:
        case op_muleqc:
        case op_diveqc:
            out << hex << "0x" << num << dec;
            out << " (" << const_number[num] << ")";
            break;
        case op_lnkeqc:
            out << hex << "0x" << num << dec;
            out << " (\"" << util::rawstr(const_string[num], 32) << "\")";
            break;
        case op_addecp:
        case op_subecp:
        case op_mulecp:
        case op_divecp:
            out << hex << "0x" << num << dec;
            out << " (" << const_number[num] << ") sp-1";
            break;
        case op_lnkecp:
            out << hex << "0x" << num << dec;
            out << " (\"" << util::rawstr(const_string[num], 32) << "\") sp-1";
            break;
        case op_addc:
        case op_subc:
        case op_mulc:
        case op_divc:
        case op_lessc:
        case op_leqc:
        case op_grtc:
        case op_geqc:
        case op_pnum:
            out << hex << "0x" << num << dec;
            out << " (" << const_number[num] << ")";
            break;
        case op_callvi:
        case op_newv:
        case op_callfv:
        case op_repl:
        case op_intl:
        case op_findex:
        case op_feach:
        case op_newf:
        case op_jmp:
        case op_jt:
        case op_jf:
        case op_calll:
        case op_mcalll:
        case op_loadl:
            out << hex << "0x" << num << dec; break;
        case op_loadg:
        case op_mcallg:
        case op_callg:
            out << hex << "0x" << num << dec;
            out << " (" << util::rawstr(global_variable[num], 32) << ")";
            break;
        case op_callb:
            out << hex << "0x" << num << dec;
            out << " <" << natives[num].name << "@0x";
            out << hex << reinterpret_cast<u64>(natives[num].func) << dec;
            out << ">";
            break;
        case op_upval:
        case op_mupval:
        case op_loadu:
            out << hex << "0x" << ((num>>16)&0xffff)
                << "[0x" << (num&0xffff) << "]" << dec; break;
        case op_happ:
        case op_pstr:
        case op_lnkc:
        case op_callh:
        case op_mcallh:
        case op_para:
        case op_deft:
        case op_dyn:
            out << hex << "0x" << num << dec;
            out << " (\"" << util::rawstr(const_string[num], 32) << "\")";
            break;
        default:
            if (files) {
                out << hex << "0x" << num << dec;
            }
            break;
    }

    // if file list is loaded, dump file location info
    if (files) {
        out << " (" << files[code.fidx] << ":" << code.line << ")";
    }
}

std::ostream& operator<<(std::ostream& out, const codestream& ins) {
    ins.dump(out);
    return out;
}

}