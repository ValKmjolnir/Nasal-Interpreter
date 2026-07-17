#pragma once

#include <cstring>
#include <sstream>

namespace nasal::fs {

class path {
private:
    std::string file_system_path;

public:
    path(const path&) = default;
    path(const std::string& file_path): file_system_path(file_path) {}
    path& operator/(const path&);
    const char* c_str() const {
        return file_system_path.c_str();
    }
    const std::string& str() const {
        return file_system_path;
    }
};

bool exists(const path&);
bool is_regular(const path&);

}