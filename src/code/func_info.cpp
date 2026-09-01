#include "code/func_info.hpp"
#include "code/compilation.hpp"

namespace nasal {

void func_info::dump(std::ostream& os, const compilation& comp) const {
    os << "func (";
    for (u64 i = 0; i < param_table.size(); i++) {
        os << param_table[i];
        if (default_param_flags[i]) {
            os << " = ";
            default_param_values[i].dump(os, comp);
        }
        os << (i != param_table.size() - 1 ? ", " : "");
    }

    if (dynamic_parameter_index != -1) {
        if (param_table.size()) {
            os << ", ";
        }
        os << dynamic_parameter_name << "...";
    }

    os << ") { 0x" << std::hex << entry << std::dec << " }";
}

}
