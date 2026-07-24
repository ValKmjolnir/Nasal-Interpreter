#pragma once

#include <iostream>
#include <string>

#include "nasal.hpp"

namespace nasal {

struct span {
    u32 begin_line;
    u32 begin_column;
    u32 end_line;
    u32 end_column;
    std::string file;

    void dump_begin(std::ostream& out) const {
        out << file << ":" << begin_line << ":" << begin_column + 1;
    }
};

}
