#pragma once

#include "nasal.h"
#include "nasal_vm.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>

namespace nasal {
namespace repl {

struct info {
    bool in_repl_mode = false;
    std::string repl_file_name = "<nasal-repl>";
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
    std::unique_ptr<vm> runtime;

private:
    std::string readline(std::string);
    bool check_need_more_input();
    void update_temp_file();
    void help();
    bool run();

public:
    repl(): runtime(std::unique_ptr<vm>(new vm)) {
        runtime->set_repl_mode_flag(true);
    }
    void execute();
};

}
}
