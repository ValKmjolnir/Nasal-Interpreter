#include "nasal_opcode.h"
#include "util/util.h"

namespace nasal {

void codestream::set(const f64* number_list,
                     const std::string* string_list,
                     const std::unordered_map<std::string, u64>& globals,
                     const nasal_builtin_table* native_table,
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
                     const nasal_builtin_table* native_table,
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
    out << "    " << operand_name_table.at(static_cast<opcode_type>(op)) << "  ";

    switch(op) {
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