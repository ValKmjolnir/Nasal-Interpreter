#pragma once

#include <string>
#include <vector>

#include "nasal.hpp"

namespace nasal {

class filestream {
protected:
    std::string file;
    std::vector<std::string> res;

public:
    filestream(): file("") {}
    void load(const std::string&);
    const std::string& operator[](usize n) const { return res[n]; }
    const auto& name() const { return file; }
    const auto& file_content() const { return res; }
    usize size() const { return res.size(); }
};

}