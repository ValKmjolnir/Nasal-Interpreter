#include "util/util.h"
#include "util/gc_stat.h"

#include <chrono>
#include <iomanip>

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

void gc_stat::dump_info() const {
    util::windows_code_page_manager wm;
    wm.set_utf8_output();

    using std::left;
    using std::setw;
    using std::setfill;
    using std::setprecision;

    const char* used_table_name[] = {
        "object type",
        "gc cycle",
        "alloc count",
        "object count",
        "detail",
        "time spend",
        "gc time",
        "avg time",
        "max gc",
        "max mark",
        "max sweep"
    };
    const char* name[] = {
        "string",
        "vector",
        "hashmap",
        "function",
        "upvalue",
        "ghost",
        "coroutine",
        "namespace"
    };

    // calculate max indent length
    usize indent = 0;
    for (auto tname : used_table_name) {
        auto len = strlen(tname);
        indent = indent<len? len:indent;
    }
    for (auto n : name) {
        auto len = strlen(n);
        indent = indent<len? len:indent;
    }
    for (u32 i = 0; i < GC_TYPE_SIZE; ++i) {
        auto len = std::to_string(gc_cycle_trigger_count[i]).length();
        indent = indent<len? len:indent;
        len = std::to_string(alloc_count[i]).length();
        indent = indent<len? len:indent;
        len = std::to_string(object_size[i]).length();
        indent = indent<len? len:indent;
    }

    auto indent_string = std::string("──");
    for (usize i = 0; i < indent; ++i) {
        indent_string += "─";
    }

    const auto first_line = "╭" + indent_string + "┬" +
                            indent_string + "┬" +
                            indent_string + "┬" +
                            indent_string + "╮";
    const auto second_line = "├" + indent_string + "┼" +
                             indent_string + "┼" +
                             indent_string + "┼" +
                             indent_string + "┤";
    const auto mid_line = "├" + indent_string + "┼" +
                          indent_string + "┴" +
                          indent_string + "┴" +
                          indent_string + "┤";
    const auto another_mid_line = "├" + indent_string + "┼" +
                                  indent_string + "─" +
                                  indent_string + "─" +
                                  indent_string + "┤";
    const auto last_line = "╰" + indent_string + "┴" +
                           indent_string + "─" +
                           indent_string + "─" +
                           indent_string + "╯";

    std::clog << "\n" << first_line << "\n";
    std::clog << "│ " << left << setw(indent) << setfill(' ') << "object type";
    std::clog << " │ " << left << setw(indent) << setfill(' ') << "gc cycle";
    std::clog << " │ " << left << setw(indent) << setfill(' ') << "alloc count";
    std::clog << " │ " << left << setw(indent) << setfill(' ') << "object count";
    std::clog << " │\n" << second_line << "\n" << std::internal;

    for (u32 i = 0; i < GC_TYPE_SIZE; ++i) {
        if (!gc_cycle_trigger_count[i] &&
            !alloc_count[i] &&
            !object_size[i]) {
            continue;
        }
        std::clog << "│ " << left << setw(indent) << setfill(' ') << name[i];
        std::clog << " │ " << left << setw(indent) << setfill(' ') << gc_cycle_trigger_count[i];
        std::clog << " │ " << left << setw(indent) << setfill(' ') << alloc_count[i];
        std::clog << " │ " << left << setw(indent) << setfill(' ') << object_size[i];
        std::clog << " │\n" << std::internal;
    }
    std::clog << mid_line << "\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "detail";
    std::clog << " │ " << left << setw(indent) << setfill(' ') << "time spend";
    std::clog << "   " << left << setw(indent) << setfill(' ') << " ";
    std::clog << "   " << left << setw(indent) << setfill(' ') << " ";
    std::clog << " │\n" << another_mid_line << "\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "gc time";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << gc_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "avg time";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << avg_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "avg mark";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << avg_mark_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "avg sweep";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << avg_sweep_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "max mark";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << max_mark_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "max sweep";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << max_sweep_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << last_line << "\n" << std::internal;

    wm.restore_code_page();
}

}