#pragma once

#include <iostream>
#include <string>

#include "nasal.hpp"

namespace nasal {

struct span {
    u64 begin_line;
    u64 begin_column;
    u64 end_line;
    u64 end_column;
    std::string file;

    void dump_begin(std::ostream& out) const {
        out << file << ":" << begin_line << ":" << begin_column + 1;
    }
};

}
