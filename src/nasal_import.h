#pragma once

#ifndef _MSC_VER
#include <unistd.h>
#else
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#include <io.h>
#endif

#include "nasal.h"
#include "nasal_ast.h"
#include "nasal_lexer.h"
#include "nasal_parse.h"
#include "symbol_finder.h"

#include <cstring>
#include <sstream>
#include <vector>
#include <unordered_set>

namespace nasal {

class experimental_linker {
private:
    std::string this_file;
    std::string library_file_path;
    bool report_show_path_flag;
    error err;
    std::vector<std::string> used_files;
    std::vector<fs::path> envpath;

private:
    bool import_check(expr*);
    std::string get_path(expr*);
    std::string find_real_file_path(const std::string&, const span&);
    std::string generate_module_real_name(const std::string&);
    return_expr* generate_module_return(code_block*);
    definition_expr* generate_module_definition(code_block*);
    code_block* compile_core(const std::string&);
    code_block* experimental_load_module(expr*);
    code_block* experimental_load_library();

public:
    experimental_linker();
    const error& link(parse&, bool);
    const auto& get_file_list() const {return used_files;}
};

}
