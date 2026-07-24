#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "util/type_alias.hpp"

namespace nasal {

class resource_manager {
private:
    std::unordered_set<std::string> used_files;
    std::vector<std::string> ordered_used_files;
    std::unordered_map<std::string, usize> file_index_map;

public:
    bool exist(const std::string&) const;
    void register_file(const std::string&);

    const auto size() const {
        return used_files.size();
    }

    const auto& get_ordered_file_list() const {
        return ordered_used_files;
    }

    const auto& get_file_index_map() const {
        return file_index_map;
    }
};

}
