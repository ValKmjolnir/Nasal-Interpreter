#pragma once

#include <string>
#include <unordered_map>

namespace nasal {

class virtual_source_registry {
private:
    std::unordered_map<std::string, std::string> sources;

public:
    static auto& instance() {
        static virtual_source_registry instance;
        return instance;
    }

    void regist(const std::string& name, const std::string& source) {
        sources[name] = source;
    }

    auto* get(const std::string& name) const {
        return sources.find(name) != sources.end() ? &sources.at(name) : nullptr;
    }

    const auto size() const {
        return sources.size();
    }
};

}
