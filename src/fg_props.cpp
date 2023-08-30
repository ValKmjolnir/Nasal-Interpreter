#include "fg_props.h"

#include <fstream>

var builtin_logprint(var* local, gc& ngc) {
    var level = local[1];
    var elems = local[2];
    if (elems.type!=vm_vec) {
        return nas_err("logprint", "received argument is not vector.");
    }
    std::ofstream out("fgfs.log", std::ios::app);
    switch (static_cast<u32>(level.num())) {
        case SG_LOG_BULK: out << "[LOG_BULK]"; break;
        case SG_LOG_DEBUG: out << "[LOG_DEBUG]"; break;
        case SG_LOG_INFO: out << "[LOG_INFO]"; break;
        case SG_LOG_WARN: out << "[LOG_WARN]"; break;
        case SG_LOG_ALERT: out << "[LOG_ALERT]"; break;
        case SG_DEV_WARN: out << "[DEV_WARN]"; break;
        case SG_DEV_ALERT: out << "[DEV_ALERT]"; break;
        case SG_MANDATORY_INFO: out << "[MANDATORY_INFO]"; break;
        default:
            return nas_err("logprint",
                "incorrect log level " +
                std::to_string(level.num()));
    }
    for(auto& i : elems.vec().elems) {
        out << i << " ";
    }
    out << "\n";
    return nil;
}

nasal_builtin_table flight_gear_native[] = {
    {"_logprint", builtin_logprint},
    {nullptr, nullptr}
};