#pragma once

#ifndef __nasver__
#define __nasver__ "11.4.0"
#endif

#include "util/type_alias.hpp"

// virtual machine stack depth, both global depth and value stack depth
const u32 VM_STACK_DEPTH = UINT16_MAX + 1;

// avoid error loading function bug in MSVC version nasal.exe
#ifdef _MSC_VER
#define NASAL_EXPORT extern "C" __declspec(dllexport)
#else
#define NASAL_EXPORT extern "C" __attribute__((visibility("default")))
#endif
