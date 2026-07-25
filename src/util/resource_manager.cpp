#include "util/resource_manager.hpp"

namespace nasal {

bool resource_manager::exist(const std::string& name) const {
    return used_files.find(name) != used_files.end();
}

void resource_manager::register_file(const std::string& name) {
    if (exist(name)) {
        return;
    }
    used_files.insert(name);
    ordered_used_files.push_back(name);
    file_index_map.emplace(name, ordered_used_files.size() - 1);
}

}
