#pragma once

#include "nasal.h"
#include "nasal_vm.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <deque>

namespace nasal {
namespace repl {

struct info {
    bool in_repl_mode = false;
    const std::string repl_file_name = "<nasal-repl>";
    std::string repl_file_source = "";

    // singleton
    static info* instance() {
        static info info;
        return &info;
    }
};

class repl {
private:
    std::vector<std::string> source;
    std::deque<std::string> command_history;
    vm runtime;

private:
    void add_command_history(const std::string&);
    std::string readline(const std::string&);
    bool check_need_more_input();
    void update_temp_file();
    void help();
    bool run();

public:
    repl() {
        // set repl mode
        runtime.set_repl_mode_flag(true);
        // no detail report info
        runtime.set_detail_report_info(false);
        // set empty history
        command_history = {""};
    }
    void execute();
};

}
}
