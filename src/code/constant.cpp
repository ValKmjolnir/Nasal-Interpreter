#include "code/constant.hpp"
#include "code/func_info.hpp"
#include "code/compilation.hpp"

namespace nasal {

void const_value::dump(std::ostream& os, const compilation& comp) const {
    switch (type_) {
        case type::CONST_NIL: os << "nil"; break;
        case type::CONST_NUM: os << comp.get_number_table().at(index_); break;
        case type::CONST_STR:
            os << "\"" << comp.get_string_table().at(index_) << "\"";
            break;
        case type::CONST_FUNC:
            comp.get_function_table().at(index_).dump(os, comp);
            break;
        default: os << "unknown";
    }
}

}