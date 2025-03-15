#include "util/gc_stat.h"

#include <chrono>

namespace nasal {

void gc_stat::init() {
    for (i64 i = 0; i < GC_TYPE_SIZE; i++) {
        object_size[i] = 0;
        alloc_count[i] = 0;
        gc_cycle_trigger_count[i] = 0;
    }

    total_mark_count = 0;
    total_sweep_count = 0;

    total_mark_time = 0;
    total_sweep_time = 0;

    max_mark_time = 0;
    max_sweep_time = 0;

    flag_concurrent_mark_triggered = false;
}

f64 gc_stat::gc_time_ms() const {
    const auto den = std::chrono::high_resolution_clock::duration::period::den;
    return ((total_mark_time + total_sweep_time) * 1000.0) / den;
}

f64 gc_stat::avg_time_ms() const {
    u64 total_gc_cycle = 0;
    for (i64 i = 0; i < GC_TYPE_SIZE; i++) {
        total_gc_cycle += gc_cycle_trigger_count[i];
    }
    return gc_time_ms() / total_mark_count;
}

f64 gc_stat::avg_mark_time_ms() const {
    const auto den = std::chrono::high_resolution_clock::duration::period::den;
    return (total_mark_time * 1000.0) / den / total_mark_count;
}

f64 gc_stat::avg_sweep_time_ms() const {
    const auto den = std::chrono::high_resolution_clock::duration::period::den;
    return (total_sweep_time * 1000.0) / den / total_sweep_count;
}

f64 gc_stat::max_mark_time_ms() const {
    const auto den = std::chrono::high_resolution_clock::duration::period::den;
    return (max_mark_time * 1000.0) / den;
}

f64 gc_stat::max_sweep_time_ms() const {
    const auto den = std::chrono::high_resolution_clock::duration::period::den;
    return (max_sweep_time * 1000.0) / den;
}

}