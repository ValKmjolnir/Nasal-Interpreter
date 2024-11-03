#pragma once

#include "nasal.h"

#ifdef _WIN32
    #define NASAL_EXPORT __declspec(dllexport)
#else
    #define NASAL_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Main API functions
NASAL_EXPORT void* nasal_init();
NASAL_EXPORT void nasal_cleanup(void* context);
NASAL_EXPORT const char* nasal_eval(void* context, const char* code);
NASAL_EXPORT const char* nasal_get_error(void* context);

#ifdef __cplusplus
}
#endif
