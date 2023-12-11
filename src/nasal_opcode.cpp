#include "nasal_opcode.h"

namespace nasal {

const char* opname[] = {
    "exit  ", "repl  ", "intl  ", "loadg ",
    "loadl ", "loadu ", "pnum  ", "pnil  ",
    "pstr  ", "newv  ", "newh  ", "newf  ",
    "happ  ", "para  ", "def   ", "dyn   ",
    "lnot  ", "usub  ", "bitnot", "bitor ",
    "bitxor", "bitand", "add   ", "sub   ",
    "mult  ", "div   ", "lnk   ", "addc  ",
    "subc  ", "multc ", "divc  ", "lnkc  ",
    "addeq ", "subeq ", "muleq ", "diveq ",
    "lnkeq ", "bandeq", "boreq ", "bxoreq",
    "addeqc", "subeqc", "muleqc", "diveqc",
    "lnkeqc", "addecp", "subecp", "mulecp",
    "divecp", "lnkecp", "meq   ", "eq    ",
    "neq   ", "less  ", "leq   ", "grt   ",
    "geq   ", "lessc ", "leqc  ", "grtc  ",
    "geqc  ", "pop   ", "jmp   ", "jt    ",
    "jf    ", "cnt   ", "findx ", "feach ",
    "callg ", "calll ", "upval ", "callv ",
    "callvi", "callh ", "callfv", "callfh",
    "callb ", "slcbeg", "slcend", "slice ",
    "slice2", "mcallg", "mcalll", "mupval",
    "mcallv", "mcallh", "ret   "
};

void codestream::set(
    const f64* number_list,
    const std::string* string_list,
    const nasal_builtin_table* native_table,
    const std::string* file_list) {
    const_number = number_list;
    const_string = string_list;
    natives = native_table;
    files = file_list;
}

void codestream::dump(std::ostream& out) const {
    using std::setw;
    using std::setfill;
    using std::hex;
    using std::dec;
    auto op = code.op;
    auto num = code.num;
    out << hex << "0x"
        << setw(6) << setfill('0') << index << "     "
        << setw(2) << setfill('0') << static_cast<u32>(op) << " "
        << setw(2) << setfill('0') << ((num>>16)&0xff) << " "
        << setw(2) << setfill('0') << ((num>>8)&0xff) << " "
        << setw(2) << setfill('0') << (num&0xff) << "     "
        <<opname[op]<<"  "<<dec;
    switch(op) {
        case op_addeq: case op_subeq:
        case op_muleq: case op_diveq:
        case op_lnkeq: case op_meq:
        case op_btandeq: case op_btoreq:
        case op_btxoreq:
            out << hex << "0x" << num << dec << " sp-" << num; break;
        case op_addeqc: case op_subeqc:
        case op_muleqc:case op_diveqc:
            out << hex << "0x" << num << dec
                << " (" << const_number[num] << ")"; break;
        case op_lnkeqc:
            out << hex << "0x" << num << dec
                << " (" << rawstr(const_string[num], 16) << ")"; break;
        case op_addecp: case op_subecp:
        case op_mulecp: case op_divecp:
            out << hex << "0x" << num << dec
                << " (" << const_number[num] << ") sp-1"; break;
        case op_lnkecp:
            out << hex << "0x" << num << dec
                << " (" << rawstr(const_string[num], 16) << ") sp-1"; break;
        case op_addc: case op_subc:
        case op_mulc: case op_divc:
        case op_lessc: case op_leqc:
        case op_grtc: case op_geqc:
        case op_pnum:
            out << hex << "0x" << num << dec
                << " (" << const_number[num] << ")"; break;
        case op_callvi: case op_newv:
        case op_callfv: case op_repl:
        case op_intl: case op_findex:
        case op_feach: case op_newf:
        case op_jmp: case op_jt:
        case op_jf: case op_callg:
        case op_mcallg: case op_loadg:
        case op_calll: case op_mcalll:
        case op_loadl:
            out << hex << "0x" << num << dec; break;
        case op_callb:
            out << hex << "0x" << num << " <" << natives[num].name
                << "@0x" << reinterpret_cast<u64>(natives[num].func)
                << dec << ">"; break;
        case op_upval: case op_mupval:
        case op_loadu:
            out << hex << "0x" << ((num>>16)&0xffff)
                << "[0x" << (num&0xffff) << "]" << dec; break;
        case op_happ: case op_pstr:
        case op_lnkc: case op_callh:
        case op_mcallh: case op_para:
        case op_deft: case op_dyn:
            out << hex << "0x" << num << dec
                << " (" << rawstr(const_string[num], 16) << ")"; break;
        default:
            if (files) {
                out << hex << "0x" << num << dec;
            }
            break;
    }
    if (files) {
        out << " (" << files[code.fidx] << ":" << code.line << ")";
    }
}

std::ostream& operator<<(std::ostream& out, const codestream& ins) {
    ins.dump(out);
    return out;
}

}