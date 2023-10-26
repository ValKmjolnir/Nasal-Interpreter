#include "nasal_dbg.h"

namespace nasal {

void debug_prof_data::init_counter() {
    for(usize i = 0; i<debug_prof_data::operand_size; ++i) {
        operand_counter[i] = 0;
    }
}

void debug_prof_data::load_file_line_counter(
    const std::vector<std::string>& file_list) {
    file_name_list = file_list;
    file_line_counter = {};
    file_contents = {};
    flstream fs;
    for(usize i =0; i<file_list.size(); ++i) {
        fs.load(file_list[i]);
        file_contents.push_back(fs.file_content());
        file_line_counter.push_back({});
        file_line_counter.back().resize(fs.size(), 0);
    }
}

void debug_prof_data::init(const std::vector<std::string>& file_list) {
    init_counter();
    load_file_line_counter(file_list);
}

void debug_prof_data::dump_counter() const {
    typedef std::pair<u32, u64> op_count;
    std::vector<op_count> opcall;
    u64 total = 0;
    for(usize i = 0; i<debug_prof_data::operand_size; ++i) {
        total += operand_counter[i];
        opcall.push_back({i, operand_counter[i]});
    }
    std::sort(opcall.begin(), opcall.end(),
        [](const op_count& a, const op_count& b) {
            return a.second>b.second;
        }
    );
    std::clog << "\noperands call info (<1% ignored)\n";
    for(const auto& i : opcall) {
        u64 rate = i.second*100/total;
        if (!rate) {
            break;
        }
        std::clog << " " << opname[i.first] << " : ";
        std::clog << i.second << " (" << rate << "%)\n";
    }
    std::clog << " total  : " << total << '\n';
}

void debug_prof_data::dump_code_line_counter(std::ostream& os) const {
    u64 max_call_time = 0;
    for(const auto& context : file_line_counter) {
        for(const auto& count : context) {
            max_call_time = count>max_call_time? count:max_call_time;
        }
    }
    auto pad_length = std::to_string(max_call_time).length();
    for(usize i = 0; i<file_name_list.size(); ++i) {
        os << "\ncode profiling data of " << file_name_list[i] << ":\n";
        const auto& context = file_contents[i];
        const auto& counter = file_line_counter[i];
        for(usize j = 0; j<context.size(); ++j) {
            os << " " << std::right << std::setw(pad_length);
            os << std::setfill(' ');
            os << (counter[j]==0? "":std::to_string(counter[j]));
            os << " " << context[j] << "\n";
        }
    }
}

void debug_prof_data::dump_this_file_line_counter(std::ostream& os) const {
    u64 max_call_time = 0;
    for(const auto& count : file_line_counter[0]) {
        max_call_time = count>max_call_time? count:max_call_time;
    }
    auto pad_length = std::to_string(max_call_time).length();

    os << "\ncode profiling data of " << file_name_list[0] << ":\n";
    const auto& context = file_contents[0];
    const auto& counter = file_line_counter[0];
    for(usize i = 0; i<context.size(); ++i) {
        os << " " << std::right << std::setw(pad_length);
        os << std::setfill(' ');
        os << (counter[i]==0? "":std::to_string(counter[i]));
        os << " " << context[i] << "\n";
    }
}

std::vector<std::string> dbg::parse(const std::string& cmd) {
    std::vector<std::string> res;
    usize last = 0, pos = cmd.find(" ", 0);
    while(pos!=std::string::npos) {
        if (pos>last) {
            res.push_back(cmd.substr(last, pos-last));
        }
        last = pos+1;
        pos = cmd.find(" ", last);
    }
    if (last<cmd.length()) {
        res.push_back(cmd.substr(last));
    }
    return res;
}

u16 dbg::file_index(const std::string& filename) const {
    for(u16 i = 0; i<fsize; ++i) {
        if (filename==files[i]) {
            return i;
        }
    }
    return 65535;
}

void dbg::err() {
    std::cerr
    << "incorrect command\n"
    << "input \'h\' to get help\n";
}

void dbg::help() {
    std::clog
    << "<option>\n"
    << "  h,   help      | get help\n"
    << "  bt,  backtrace | get function call trace\n"
    << "  c,   continue  | run program until break point or exit\n"
    << "  f,   file      | see all the compiled files\n"
    << "  g,   global    | see global values\n"
    << "  l,   local     | see local values\n"
    << "  u,   upval     | see upvalue\n"
    << "  r,   register  | show vm register detail\n"
    << "  a,   all       | show global,local and upvalue\n"
    << "  n,   next      | execute next bytecode\n"
    << "  q,   exit      | exit debugger\n"
    << "<option> <filename> <line>\n"
    << "  bk,  break     | set break point\n";
}

void dbg::list_file() const {
    for(usize i = 0; i<fsize; ++i) {
        std::clog << "[" << i << "] " << files[i] << "\n";
    }
}

void dbg::step_info() {
    u32 line = bytecode[ctx.pc].line==0? 0:bytecode[ctx.pc].line-1;
    u32 begin = (line>>3)==0? 0:((line>>3)<<3);
    u32 end = (1+(line>>3))<<3;
    src.load(files[bytecode[ctx.pc].fidx]);
    std::clog << "\nsource code:\n";
    for(u32 i = begin; i<end && i<src.size(); ++i) {
        std::clog << (i==line? back_white:reset);
        std::clog << (i==line? "--> ":"    ") << src[i] << reset << "\n";
    }

    begin = (ctx.pc>>3)==0? 0:((ctx.pc>>3)<<3);
    end = (1+(ctx.pc>>3))<<3;
    codestream::set(const_number, const_string, native_function.data(), files);
    std::clog << "\nnext bytecode:\n";
    for(u32 i = begin; i<end && bytecode[i].op!=op_exit; ++i) {
        std::clog
        << (i==ctx.pc? back_white:reset)
        << (i==ctx.pc? "--> ":"    ")
        << codestream(bytecode[i], i)
        << reset << "\n";
    }
    stack_info(10);
}

void dbg::interact() {
    // special operand, end execution
    if (bytecode[ctx.pc].op==op_exit) {
        return;
    }

    // do not need interact while doing profiling
    if (do_profiling) {
        return;
    }

    // is not break point and is not next stop command
    const auto& code = bytecode[ctx.pc];
    if ((code.fidx!=break_file_index || code.line!=break_line) && !next) {
        return;
    }

    next = false;
    std::string cmd;
    step_info();
    while(true) {
        std::clog << ">> ";
        std::getline(std::cin, cmd);
        auto res = parse(cmd);
        if (res.size()==0) {
            step_info();
        } else if (res.size()==1) {
            switch(get_cmd_type(res[0])) {
                case dbg_cmd::cmd_help: help(); break;
                case dbg_cmd::cmd_backtrace:
                    function_call_trace();
                    trace_back();
                    break;
                case dbg_cmd::cmd_continue: return;
                case dbg_cmd::cmd_list_file: list_file(); break;
                case dbg_cmd::cmd_global: global_state(); break;
                case dbg_cmd::cmd_local: local_state(); break;
                case dbg_cmd::cmd_upval: upvalue_state(); break;
                case dbg_cmd::cmd_register: register_info(); break;
                case dbg_cmd::cmd_show_all: all_state_detail(); break;
                case dbg_cmd::cmd_next: next = true; return;
                case dbg_cmd::cmd_exit: std::exit(0);
                default: err(); break;
            }
        } else if (res.size()==3 &&
            get_cmd_type(res[0])==dbg_cmd::cmd_break_point) {
            break_file_index = file_index(res[1]);
            if (break_file_index==65535) {
                std::clog << "cannot find file named `" << res[1] << "`\n";
                continue;
            }
            i32 tmp = atoi(res[2].c_str());
            if (tmp<=0) {
                std::clog << "incorrect line number `" << res[2] << "`\n";
            } else {
                break_line = tmp;
            }
        } else {
            err();
        }
    }
}

void dbg::run(
    const codegen& gen,
    const linker& linker,
    const std::vector<std::string>& argv,
    bool profile,
    bool show_all_prof_result) {

    set_detail_report_info(true);
    do_profiling = profile || show_all_prof_result;

    const auto& file_list = linker.get_file_list();
    fsize = file_list.size();
    init(
        gen.strs(),
        gen.nums(),
        gen.natives(),
        gen.codes(),
        gen.globals(),
        file_list,
        argv
    );
    data.init(file_list);

    std::vector<u32> code;
    std::vector<u16> code_file_index;
    std::vector<u32> code_line;
    for(auto& i : gen.codes()) {
        code.push_back(i.op);
        code_file_index.push_back(i.fidx);
        code_line.push_back(i.line);
        imm.push_back(i.num);
    }
    while(operand_function[code[ctx.pc]]) {
        interact();
        data.add_operand_counter(code[ctx.pc]);
        data.add_code_line_counter(code_file_index[ctx.pc], code_line[ctx.pc]);
        (this->*operand_function[code[ctx.pc]])();
        if (ctx.top>=ctx.canary) {
            die("stack overflow");
        }
        ++ctx.pc;
    }

    data.dump_counter();
    if (do_profiling) {
        show_all_prof_result?
            data.dump_code_line_counter(std::clog):
            data.dump_this_file_line_counter(std::clog);
    }
    ngc.info();
    ngc.clear();
    imm.clear();
    return;
}

}
