#ifndef __NASAL_WEB_H__
#define __NASAL_WEB_H__

#include "nasal.h"

#ifdef __cplusplus
extern "C" {
#endif

// Main API functions
NASAL_EXPORT void* nasal_init();
NASAL_EXPORT void nasal_cleanup(void* context);
NASAL_EXPORT void nasal_set_timeout(void* context, int seconds);
NASAL_EXPORT const char* nasal_eval(void* context, const char* code, int show_time);
NASAL_EXPORT const char* nasal_get_error(void* context);

// REPL 
NASAL_EXPORT void* nasal_repl_init();
NASAL_EXPORT void nasal_repl_cleanup(void* repl_context);
NASAL_EXPORT void nasal_repl_set_timeout(void* repl_context, int seconds);
NASAL_EXPORT const char* nasal_repl_eval(void* repl_context, const char* line);
NASAL_EXPORT int nasal_repl_is_complete(void* repl_context, const char* line);
NASAL_EXPORT const char* nasal_repl_get_version();

#ifdef __cplusplus
}
#endif

#endif
