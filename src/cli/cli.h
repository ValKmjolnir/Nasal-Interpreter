#pragma once

#include <cstring>
#include <sstream>
#include <unordered_map>

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
    cli_show_detail_info,
    cli_show_referenced_file,
    cli_debug_mode,
    cli_profile,
    cli_profile_all,
    cli_limit_mode
};

using config = std::unordered_map<option, std::string>;

const std::unordered_map<std::string, option> options = {
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
    {"-f", option::cli_show_referenced_file},
    {"--ref-file", option::cli_show_referenced_file},
    {"-dbg", option::cli_debug_mode},
    {"--debug", option::cli_debug_mode},
    {"--prof", option::cli_profile},
    {"--prof-all", option::cli_profile_all},
    {"--limit", option::cli_limit_mode}
};

}