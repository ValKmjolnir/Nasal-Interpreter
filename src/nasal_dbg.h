#pragma once

#include "nasal_import.h"
#include "nasal_err.h"
#include "nasal_opcode.h"
#include "nasal_vm.h"

#include <cstring>
#include <algorithm>
#include <unordered_map>

namespace nasal {

// count detail operand calling
// and show them before each line of the source file
class operand_line_counter {
private:
    static const usize operand_size = op_code_type::op_ret + 1;
    u64 operand_counter[operand_size];
    std::vector<std::string> file_name_list;
    std::vector<std::vector<u64>> file_line_counter;
    std::vector<std::vector<std::string>> file_contents;

private:
    void init_counter();
    void load_file_line_counter(const std::vector<std::string>&);

public:
    void init(const std::vector<std::string>&);
    void dump_operand_count() const;
    void dump_all_code_line_counter(std::ostream&) const;
    void dump_this_file_line_counter(std::ostream&) const;
    void add_operand_counter(usize index) {
        operand_counter[index] += index<operand_size? 1:0;
    }
    void add_code_line_counter(usize fidx, usize line) {
        auto& vec = file_line_counter[fidx];
        vec[line==0? line: line-1]++;
    }
};

class dbg: public vm {
private:
    typedef void (dbg::*nasal_vm_func)();
    const nasal_vm_func operand_function[op_ret + 1] = {
        nullptr,        &dbg::o_repl,
        &dbg::o_intl,   &dbg::o_loadg,
        &dbg::o_loadl,  &dbg::o_loadu,
        &dbg::o_pnum,   &dbg::o_pnil,
        &dbg::o_pstr,   &dbg::o_newv,
        &dbg::o_newh,   &dbg::o_newf,
        &dbg::o_happ,   &dbg::o_para,
        &dbg::o_deft,   &dbg::o_dyn,
        &dbg::o_lnot,   &dbg::o_usub,
        &dbg::o_bnot,   &dbg::o_btor,
        &dbg::o_btxor,  &dbg::o_btand,
        &dbg::o_add,    &dbg::o_sub,
        &dbg::o_mul,    &dbg::o_div,
        &dbg::o_lnk,    &dbg::o_addc,
        &dbg::o_subc,   &dbg::o_mulc,
        &dbg::o_divc,   &dbg::o_lnkc,
        &dbg::o_addeq,  &dbg::o_subeq,
        &dbg::o_muleq,  &dbg::o_diveq,
        &dbg::o_lnkeq,  &dbg::o_bandeq,
        &dbg::o_boreq,  &dbg::o_bxoreq,
        &dbg::o_addeqc, &dbg::o_subeqc,
        &dbg::o_muleqc, &dbg::o_diveqc,
        &dbg::o_lnkeqc, &dbg::o_addecp,
        &dbg::o_subecp, &dbg::o_mulecp,
        &dbg::o_divecp, &dbg::o_lnkecp,
        &dbg::o_meq,    &dbg::o_eq,
        &dbg::o_neq,    &dbg::o_less,
        &dbg::o_leq,    &dbg::o_grt,
        &dbg::o_geq,    &dbg::o_lessc,
        &dbg::o_leqc,   &dbg::o_grtc,
        &dbg::o_geqc,   &dbg::o_pop,
        &dbg::o_jmp,    &dbg::o_jt,
        &dbg::o_jf,     &dbg::o_cnt,
        &dbg::o_findex, &dbg::o_feach,
        &dbg::o_callg,  &dbg::o_calll,
        &dbg::o_upval,  &dbg::o_callv,
        &dbg::o_callvi, &dbg::o_callh,
        &dbg::o_callfv, &dbg::o_callfh,
        &dbg::o_callb,  &dbg::o_slcbeg,
        &dbg::o_slcend, &dbg::o_slc,
        &dbg::o_slc2,   &dbg::o_mcallg,
        &dbg::o_mcalll, &dbg::o_mupval,
        &dbg::o_mcallv, &dbg::o_mcallh,
        &dbg::o_ret
    };

private:
    enum class cmd_kind {
        cmd_error,
        cmd_help,
        cmd_backtrace,
        cmd_continue,
        cmd_list_file,
        cmd_global,
        cmd_local,
        cmd_upval,
        cmd_register,
        cmd_show_all,
        cmd_next,
        cmd_break_point,
        cmd_exit
    };

private:
    const std::unordered_map<std::string, cmd_kind> command_table = {
        {"h", cmd_kind::cmd_help},
        {"help", cmd_kind::cmd_help},
        {"bt", cmd_kind::cmd_backtrace},
        {"backtrace", cmd_kind::cmd_backtrace},
        {"c", cmd_kind::cmd_continue},
        {"continue", cmd_kind::cmd_continue},
        {"f", cmd_kind::cmd_list_file},
        {"file", cmd_kind::cmd_list_file},
        {"g", cmd_kind::cmd_global},
        {"global", cmd_kind::cmd_global},
        {"l", cmd_kind::cmd_local},
        {"local", cmd_kind::cmd_local},
        {"u", cmd_kind::cmd_upval},
        {"upval", cmd_kind::cmd_upval},
        {"r", cmd_kind::cmd_register},
        {"register", cmd_kind::cmd_register},
        {"a", cmd_kind::cmd_show_all},
        {"all", cmd_kind::cmd_show_all},
        {"n", cmd_kind::cmd_next},
        {"next", cmd_kind::cmd_next},
        {"bk", cmd_kind::cmd_break_point},
        {"break", cmd_kind::cmd_break_point},
        {"q", cmd_kind::cmd_exit},
        {"exit", cmd_kind::cmd_exit}
    };
    cmd_kind get_cmd_type(const std::string& cmd) const {
        return command_table.count(cmd)?
            command_table.at(cmd):cmd_kind::cmd_error;
    }

private:
    bool next;
    usize fsize;
    u16 break_file_index;
    u64 break_line;
    error src;

private:
    operand_line_counter counter;
    bool do_operand_count;

private:
    std::vector<std::string> parse(const std::string&);
    u16 file_index(const std::string&) const;
    void err();
    void help();
    void list_file() const;
    void step_info();
    void interact();

public:
    dbg():
        next(true), fsize(0),
        break_file_index(0), break_line(0),
        do_operand_count(false) {}
    void run(const codegen&,
             const linker&,
             const std::vector<std::string>&,
             bool,
             bool);
};

}
