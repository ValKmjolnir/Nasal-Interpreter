#include "nasal.hpp"
#include "vm/gc.hpp"
#include "natives/registry.hpp"

#include <fstream>

namespace nasal {

#define SG_LOG_BULK 1
#define SG_LOG_DEBUG 2
#define SG_LOG_INFO 3
#define SG_LOG_WARN 4
#define SG_LOG_ALERT 5
#define SG_DEV_WARN 7
#define SG_DEV_ALERT 8
#define SG_MANDATORY_INFO 9

var builtin_logprint(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    auto level = local[1];
    auto elems = local[2];
    if (!elems.is_vec()) {
        return nas_err("fg_env::logprint", "received argument is not vector.");
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
            return nas_err("fg_env::logprint",
                "incorrect log level " + std::to_string(level.num())
            );
    }
    for (auto& value : elems.vec().elems) {
        out << value << " ";
    }
    out << "\n";
    return nil;
}

void load_flightgear_builtin() {
    nasal_builtin_info flight_gear_native[] = {
        {"_logprint", builtin_logprint}
    };
    auto& registry = nasal_builtin_registry::get();
    for (auto& i : flight_gear_native) {
        registry.regist(i);
    }
}

}
