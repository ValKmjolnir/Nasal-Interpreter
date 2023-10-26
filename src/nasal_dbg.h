#pragma once

#include "nasal_import.h"
#include "nasal_err.h"
#include "nasal_opcode.h"
#include "nasal_vm.h"

#include <cstring>
#include <algorithm>
#include <unordered_map>

namespace nasal {

class debug_prof_data {
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
    void dump_counter() const;
    void dump_code_line_counter(std::ostream&) const;
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
    enum class dbg_cmd {
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
    const std::unordered_map<std::string, dbg_cmd> command_table = {
        {"h", dbg_cmd::cmd_help},
        {"help", dbg_cmd::cmd_help},
        {"bt", dbg_cmd::cmd_backtrace},
        {"backtrace", dbg_cmd::cmd_backtrace},
        {"c", dbg_cmd::cmd_continue},
        {"continue", dbg_cmd::cmd_continue},
        {"f", dbg_cmd::cmd_list_file},
        {"file", dbg_cmd::cmd_list_file},
        {"g", dbg_cmd::cmd_global},
        {"global", dbg_cmd::cmd_global},
        {"l", dbg_cmd::cmd_local},
        {"local", dbg_cmd::cmd_local},
        {"u", dbg_cmd::cmd_upval},
        {"upval", dbg_cmd::cmd_upval},
        {"r", dbg_cmd::cmd_register},
        {"register", dbg_cmd::cmd_register},
        {"a", dbg_cmd::cmd_show_all},
        {"all", dbg_cmd::cmd_show_all},
        {"n", dbg_cmd::cmd_next},
        {"next", dbg_cmd::cmd_next},
        {"bk", dbg_cmd::cmd_break_point},
        {"break", dbg_cmd::cmd_break_point},
        {"q", dbg_cmd::cmd_exit},
        {"exit", dbg_cmd::cmd_exit}
    };
    dbg_cmd get_cmd_type(const std::string& cmd) const {
        return command_table.count(cmd)?
            command_table.at(cmd):dbg_cmd::cmd_error;
    }

private:
    bool next;
    usize fsize;
    u16 break_file_index;
    u32 break_line;
    error src;

private:
    debug_prof_data data;
    bool do_profiling;

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
        do_profiling(false) {}
    void run(
        const codegen&,
        const linker&,
        const std::vector<std::string>&,
        bool,
        bool
    );
};

}
