#pragma once

#include "nasal.hpp"
#include "opcode/opcode.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace nasal {

class codestream_dump;

class codestream {
private:
    const f64* const_number = nullptr;
    const std::string* const_string = nullptr;
    const nasal_builtin_info* natives = nullptr;
    const std::string* files = nullptr;
    std::vector<std::string> global_variable;

public:
    codestream(const f64*,
               const std::string*,
               const std::unordered_map<std::string, u64>&,
               const nasal_builtin_info*,
               const std::string* file_list = nullptr);
    codestream(const f64*,
               const std::string*,
               const std::vector<std::string>&,
               const nasal_builtin_info*,
               const std::string* file_list = nullptr);
    codestream_dump create(const opcode&, u64);
    void dump(std::ostream&, const codestream_dump&) const;
};

class codestream_dump {
private:
    opcode code;
    u64 index;
    codestream& cs;

public:
    codestream_dump(codestream& cs, opcode o, u64 i): cs(cs), code(o), index(i) {}
    const auto get_code() const { return code; }
    const auto get_index() const { return index; }
    const auto& get_codestream() const { return cs; }

    friend std::ostream& operator<<(std::ostream&, const codestream_dump&);
};

}
