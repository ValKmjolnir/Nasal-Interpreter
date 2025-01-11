#pragma once

#include <cstring>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

namespace nasal::cli {

enum class option {
    cli_help,
    cli_version,
    cli_repl_mode,
    cli_view_ast,
    cli_view_raw_ast,
    cli_view_code,
    cli_show_symbol,
    cli_execute,
    cli_show_execute_time,
    cli_detail_info,
    cli_show_referenced_file,
    cli_debug_mode,
    cli_profile,
    cli_profile_all,
    cli_limit_mode
};

struct cli_config {
    std::string input_file_path = "";
    std::unordered_set<option> options = {};
    std::vector<std::string> nasal_vm_args = {};

    bool has(option opt) const {
        return options.count(opt);
    }
};

const std::unordered_map<std::string, option> cli_options = {
    {"-h", option::cli_help},
    {"--help", option::cli_help},
    {"-v", option::cli_version},
    {"--version", option::cli_version},
    {"-r", option::cli_repl_mode},
    {"--repl", option::cli_repl_mode},
    {"-a", option::cli_view_ast},
    {"--ast", option::cli_view_ast},
    {"--raw-ast", option::cli_view_raw_ast},
    {"-c", option::cli_view_code},
    {"--code", option::cli_view_code},
    {"-s", option::cli_show_symbol},
    {"--symbol", option::cli_show_symbol},
    {"-e", option::cli_execute},
    {"--exec", option::cli_execute},
    {"-t", option::cli_show_execute_time},
    {"--time", option::cli_show_execute_time},
    {"-d", option::cli_detail_info},
    {"--detail", option::cli_detail_info},
    {"-f", option::cli_show_referenced_file},
    {"--ref-file", option::cli_show_referenced_file},
    {"-dbg", option::cli_debug_mode},
    {"--debug", option::cli_debug_mode},
    {"--prof", option::cli_profile},
    {"--prof-all", option::cli_profile_all},
    {"--limit", option::cli_limit_mode}
};

cli_config parse(const std::vector<std::string>&);

std::ostream& help(std::ostream&);
std::ostream& nasal_format_help(std::ostream&);

std::ostream& logo(std::ostream&);
std::ostream& version(std::ostream&);
std::ostream& nasal_format_version(std::ostream&);

}