#pragma once

#include "nasal.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>

namespace repl {

class repl {
private:
    std::vector<std::string> source;

private:
    std::string readline(std::string);
    bool check_need_more_input();
    void update_temp_file();
    void help();
    bool run();

public:
    void execute();
};

}