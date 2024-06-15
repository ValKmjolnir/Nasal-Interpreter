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
    static const usize operand_size = opcode_type::op_ret + 1;
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
    usize file_list_size;
    u16 break_file_index;
    u64 break_line;
    error src;

private:
    operand_line_counter counter;
    bool do_operand_count;

private:
    std::vector<std::string> parse(const std::string&);
    u16 file_index(const std::string&) const;
    void err() const;
    void help() const;
    void list_file() const;
    void step_info();
    void interact();

public:
    dbg(): next(true), file_list_size(0),
           break_file_index(0), break_line(0),
           do_operand_count(false) {}
    void run(const codegen&,
             const linker&,
             const std::vector<std::string>&,
             bool,
             bool);
};

}
