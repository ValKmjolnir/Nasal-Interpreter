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

class linker {
private:
    const u32 MAX_RECURSION_DEPTH = 256;
    bool show_path_flag;
    bool library_loaded;
    std::string this_file;
    error err;
    std::vector<std::string> imported_files;
    std::vector<std::string> module_load_stack;
    std::vector<fs::path> envpath;

private:
    bool import_check(expr*);
    bool check_exist_or_record_file(const std::string&);
    bool check_self_import(const std::string&);
    std::string generate_self_import_path(const std::string&);
    void link(code_block*, code_block*);
    std::string get_path(expr*);
    std::string find_real_file_path(const std::string&, const span&);
    code_block* import_regular_file(expr*, std::unordered_set<std::string>&);
    code_block* import_nasal_lib();
    std::string generate_module_name(const std::string&);
    return_expr* generate_module_return(code_block*);
    definition_expr* generate_module_definition(code_block*);
    code_block* load(code_block*, const std::string&);

public:
    linker();
    const error& link(parse&, bool);
    const auto& get_file_list() const {return imported_files;}
};

}
