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
    bool exist(const std::string& name) const {
        return used_files.find(name) != used_files.end();
    }

    void register_file(const std::string& name) {
        if (exist(name)) {
            return;
        }
        used_files.insert(name);
        ordered_used_files.push_back(name);
        file_index_map.emplace(name, ordered_used_files.size() - 1);
    }

    const auto size() const {
        return used_files.size();
    }

    const auto& get_ordered_file_list() const {
        return ordered_used_files;
    }

    auto file_index(const std::string& f) const {
        return file_index_map.at(f);
    }
};

}
