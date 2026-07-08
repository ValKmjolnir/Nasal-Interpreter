#include "natives/registry.hpp"

namespace nasal {

nasal_builtin_registry& nasal_builtin_registry::get() {
    static nasal_builtin_registry instance;
    return instance;
}

}