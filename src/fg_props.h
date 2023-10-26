#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

namespace nasal {

#define SG_LOG_BULK 1
#define SG_LOG_DEBUG 2
#define SG_LOG_INFO 3
#define SG_LOG_WARN 4
#define SG_LOG_ALERT 5
#define SG_DEV_WARN 7
#define SG_DEV_ALERT 8
#define SG_MANDATORY_INFO 9

var builtin_logprint(context*, gc*);

extern nasal_builtin_table flight_gear_native[];

}
