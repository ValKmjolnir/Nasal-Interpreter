#pragma once

#ifndef __nasver__
#define __nasver__ "11.2"
#endif

#include <cstdint>

// abbreviation of some useful basic type
using i32 = std::int32_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using usize = std::size_t;
using f64 = double;

// virtual machine stack depth, both global depth and value stack depth
const u32 VM_STACK_DEPTH = UINT16_MAX;
