#include "cli/cli.h"

#include <iostream>

namespace nasal::cli {

cli_config parse(const std::vector<std::string>& args) {
    cli_config result;

    for(const auto& arg : args) {
        if (cli_options.count(arg)) {
            result.options.insert(cli_options.at(arg));
        } else if (!result.input_file_path.length()) {
            result.input_file_path = arg;
        } else {
            result.nasal_vm_args.push_back(arg);
        }
    }

    if (result.input_file_path.length() && result.options.empty()) {
        result.options.insert(option::cli_execute);
    }
    return result;
}

}