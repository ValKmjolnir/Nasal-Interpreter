#include "natives/subprocess.h"

namespace nasal {

var builtin_subprocess_run(context* ctx, gc* ngc) {
    return nil;
}

nasal_builtin_table subprocess_native[] = {
    {"__subprocess_run", builtin_subprocess_run},
    {nullptr, nullptr}
};

}