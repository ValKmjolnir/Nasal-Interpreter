#pragma once

#include "nasal.h"
#include "nasal_type.h"

#include <chrono>

namespace nasal {

struct gc_stat {
    u64 object_size[GC_TYPE_SIZE];
    u64 gc_cycle_trigger_count[GC_TYPE_SIZE];
    u64 alloc_count[GC_TYPE_SIZE];

    u64 total_mark_count = 0;
    u64 total_sweep_count = 0;

    i64 total_mark_time = 0;
    i64 total_sweep_time = 0;

    i64 max_mark_time = 0;
    i64 max_sweep_time = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

    void stamp() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    void elapsed_mark_time() {
        auto end = std::chrono::high_resolution_clock::now();
        auto mark_time = (end - start_time).count();
        ++total_mark_count;
        total_mark_time += mark_time;
        max_mark_time = max_mark_time > mark_time ? max_mark_time : mark_time;
    }

    void elapsed_sweep_time() {
        auto end = std::chrono::high_resolution_clock::now();
        auto sweep_time = (end - start_time).count();
        ++total_sweep_count;
        total_sweep_time += sweep_time;
        max_sweep_time = max_sweep_time > sweep_time ? max_sweep_time : sweep_time;
    }

    void init();
    f64 gc_time_ms() const;
    f64 avg_time_ms() const;
    f64 avg_mark_time_ms() const;
    f64 avg_sweep_time_ms() const;
    f64 max_mark_time_ms() const;
    f64 max_sweep_time_ms() const;

    void dump_info() const;
};

}