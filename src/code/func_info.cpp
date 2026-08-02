#include "code/func_info.hpp"

namespace nasal {

void func_info::dump(std::ostream& os) const {
    os << "func (";
    for (const auto& i: param_table) {
        os << i;
        if (i != param_table.back()) {
            os << ", ";
        }
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
