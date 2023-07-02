#pragma once

#include "nasal_import.h"
#include "nasal_err.h"
#include "nasal_opcode.h"
#include "nasal_vm.h"

#include <cstring>
#include <algorithm>
#include <unordered_map>

class dbg:public vm {
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
    bool  next;
    usize fsize;
    u16   bk_fidx;
    u32   bk_line;
    error& src;

    std::vector<std::string> parse(const std::string&);
    u16 file_index(const std::string&) const;
    void err();
    void help();
    void list_file() const;
    void call_sort(const u64*) const;
    void step_info();
    void interact();

public:
    dbg(error& err):
        next(false), fsize(0),
        bk_fidx(0), bk_line(0),
        src(err) {}
    void run(
        const codegen&,
        const linker&,
        const std::vector<std::string>&
    );
};
