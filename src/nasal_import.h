#pragma once

#ifndef _MSC_VER
#include <unistd.h>
#else
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#include <io.h>
#endif

#ifdef _MSC_VER
#define F_OK 0
#endif

#include "nasal.h"
#include "nasal_ast.h"
#include "nasal_lexer.h"
#include "nasal_parse.h"
#include "symbol_finder.h"

#include <vector>

class linker{
private:
    bool show_path;
    bool lib_loaded;
    std::string this_file;
    std::string lib_path;
    error err;
    std::vector<std::string> files;
    std::vector<std::string> module_load_stack;
    std::vector<std::string> envpath;

    bool import_check(expr*);
    bool exist(const std::string&);
    u16 find(const std::string&);
    bool check_self_import(const std::string&);
    std::string generate_self_import_path(const std::string&);
    void link(code_block*, code_block*);
    std::string get_path(call_expr*);
    std::string find_file(const std::string&, const span&);
    code_block* import_regular_file(call_expr*);
    code_block* import_nasal_lib();
    std::string generate_module_name(const std::string&);
    return_expr* generate_module_return(code_block*);
    definition_expr* generate_module_definition(code_block*);
    code_block* load(code_block*, u16);

public:
    linker();
    const error& link(parse&, const std::string&, bool);
    const std::vector<std::string>& filelist() const {return files;}
    const std::string& get_this_file() const {return this_file;}
    const std::string& get_lib_path() const {return lib_path;}
};
