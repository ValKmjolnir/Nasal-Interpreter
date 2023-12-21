#include "nasal_import.h"
#include "symbol_finder.h"

#include <memory>
#include <unordered_set>

namespace nasal {

experimental_linker::experimental_linker():
    this_file(""), library_file_path(""), report_show_path_flag(false) {
    const auto seperator = is_windows()? ';':':';
    const auto PATH = std::string(getenv("PATH"));
    usize last = 0, position = PATH.find(seperator, 0);
    while(position!=std::string::npos) {
        std::string dirpath = PATH.substr(last, position-last);
        if (dirpath.length()) {
            envpath.push_back(dirpath);
        }
        last = position+1;
        position = PATH.find(seperator, last);
    }
    if (last!=PATH.length()) {
        envpath.push_back(PATH.substr(last));
    }
}

bool experimental_linker::import_check(expr* node) {
    if (node->get_type()==expr_type::ast_use) {
        return true;
    }
/*
    call
    |_id:import
    |_call_func
      |_string:'filename'
*/
    if (node->get_type()!=expr_type::ast_call) {
        return false;
    }
    auto call_node = reinterpret_cast<call_expr*>(node);
    auto first_expr = call_node->get_first();
    if (first_expr->get_type()!=expr_type::ast_id) {
        return false;
    }
    if (reinterpret_cast<identifier*>(first_expr)->get_name()!="import") {
        return false;
    }
    if (!call_node->get_calls().size()) {
        return false;
    }

    // import("xxx");
    if (call_node->get_calls().size()!=1) {
        return false;
    }
    auto maybe_func_call = call_node->get_calls()[0];
    if (maybe_func_call->get_type()!=expr_type::ast_callf) {
        return false;
    }
    auto func_call = reinterpret_cast<call_function*>(maybe_func_call);
    if (func_call->get_argument().size()!=1) {
        return false;
    }
    if (func_call->get_argument()[0]->get_type()!=expr_type::ast_str) {
        return false;
    }
    return true;
}

std::string experimental_linker::get_path(expr* node) {
    auto file_relative_path = std::string("");
    const auto& path = reinterpret_cast<use_stmt*>(node)->get_path();
    for(auto i : path) {
        file_relative_path += i->get_name();
        if (i!=path.back()) {
            file_relative_path += (is_windows()? "\\":"/");
        }
    }
    return file_relative_path + ".nas";
}

std::string experimental_linker::find_real_file_path(
    const std::string& filename, const span& location) {
    // first add file name itself into the file path
    std::vector<fs::path> path_list = {filename};

    // generate search path from environ path
    for(const auto& p : envpath) {
        path_list.push_back(fs::path(p)/filename);
    }

    // search file
    for(const auto& path : path_list) {
        if (fs::exists(path)) {
            return path.str();
        }
    }

    // we will find lib.nas in nasal std directory
    if (filename=="lib.nas") {
        return is_windows()?
            find_real_file_path("std\\lib.nas", location):
            find_real_file_path("std/lib.nas", location);
    }
    if (!report_show_path_flag) {
        err.err("link",
            "in <" + location.file + ">: " +
            "cannot find file <" + filename + ">, " +
            "use <-d> to get detail search path"
        );
        return "";
    }
    auto path_list_info = std::string("");
    for(const auto& path : path_list) {
        path_list_info += "  -> " + path.str() + "\n";
    }
    err.err("link",
        "in <" + location.file + ">: " +
        "cannot find file <" + filename +
        "> in these paths:\n" + path_list_info
    );
    return "";
}

std::string experimental_linker::generate_module_real_name(const std::string& file_path) {
    auto error_name = "module@[" + file_path + "]";
    if (!file_path.length()) {
        return error_name;
    }

    // check file suffix and get file suffix position
    auto suffix_position = file_path.find(".nas");
    if (suffix_position==std::string::npos) {
        err.warn("link",
            "get invalid module name from <" + file_path + ">, " +
            "will not be easily accessed. " +
            "\".nas\" suffix is required."
        );
        return error_name;
    }
    if (suffix_position+4!=file_path.length()) {
        err.warn("link",
            "get invalid module name from <" + file_path + ">, " +
            "will not be easily accessed. " +
            "only one \".nas\" suffix is required in the path."
        );
        return error_name;
    }

    // only get the file name as module name, directory path is not included
    auto split_position = file_path.find_last_of("/");
    // find "\\" in windows platform
    if (split_position==std::string::npos) {
        split_position = file_path.find_last_of("\\");
    }

    // split file path to get module name
    auto module_name = split_position==std::string::npos?
        file_path.substr(0, suffix_position):
        file_path.substr(split_position+1, suffix_position-split_position-1);
    
    // check validation of module name
    if (!module_name.length()) {
        err.warn("link",
            "get empty module name from <" + file_path + ">, " +
            "will not be easily accessed."
        );
        return module_name;
    }
    if (std::isdigit(module_name[0]) ||
        module_name.find(".")!=std::string::npos ||
        module_name.find("-")!=std::string::npos) {
        err.warn("link",
            "get module <" + module_name + "> from <" + file_path + ">, " +
            "will not be easily accessed."
        );
    }
    return module_name;
}

return_expr* experimental_linker::generate_module_return(code_block* block) {
    auto finder = std::unique_ptr<symbol_finder>(new symbol_finder);
    auto result = new return_expr(block->get_location());
    auto value = new hash_expr(block->get_location());
    result->set_value(value);
    for(const auto& i : finder->do_find(block)) {
        auto pair = new hash_pair(block->get_location());
        // do not export symbol begins with '_'
        if (i.name.length() && i.name[0]=='_') {
            continue;
        }
        pair->set_name(i.name);
        pair->set_value(new identifier(block->get_location(), i.name));
        value->add_member(pair);
    }
    return result;
}

definition_expr* experimental_linker::generate_module_definition(code_block* block) {
    auto def = new definition_expr(block->get_location());
    def->set_identifier(new identifier(
        block->get_location(),
        generate_module_real_name(block->get_location().file)
    ));
    
    auto call = new call_expr(block->get_location());
    auto func = new function(block->get_location());
    func->set_code_block(block);
    func->get_code_block()->add_expression(generate_module_return(block));
    call->set_first(func);
    call->add_call(new call_function(block->get_location()));

    def->set_value(call);
    return def;
}

code_block* experimental_linker::compile_core(const std::string& path) {
    lexer nasal_lexer;
    parse nasal_parser;
    if (nasal_lexer.scan(path).geterr()) {
        err.err("link",
            "error occurred when analysing library <" + path + ">"
        );
        return new code_block({0, 0, 0, 0, path});
    }
    if (nasal_parser.compile(nasal_lexer).geterr())  {
        err.err("link",
            "error occurred when analysing library <" + path + ">"
        );
        return new code_block({0, 0, 0, 0, path});
    }
    return nasal_parser.swap(nullptr);
}

code_block* experimental_linker::experimental_load_module(expr* import_expr) {
    const auto module_path = find_real_file_path(
        get_path(import_expr),
        import_expr->get_location()
    );
    if (library_file_path.length() && module_path == library_file_path) {
        err.err("link",
            import_expr->get_location(),
            "cannot load library file <" + library_file_path + "> as module"
        );
        return new code_block({0, 0, 0, 0, module_path});
    }
    if (!module_path.length()) {
        return new code_block({0, 0, 0, 0, module_path});
    }
    auto res = compile_core(module_path);
    used_files.push_back(module_path);
    return res;
}

code_block* experimental_linker::experimental_load_library() {
    auto library_path = find_real_file_path(
        "lib.nas",
        {0, 0, 0, 0, this_file}
    );
    if (!library_file_path.empty()) {
        err.err("link",
            "library <" + library_file_path + "> is already loaded"
        );
        return new code_block({0, 0, 0, 0, library_path});
    }
    library_file_path = library_path;
    auto res = compile_core(library_path);
    used_files.push_back(library_path);
    return res;
}

const error& experimental_linker::link(parse& nasal_parser, bool report_show_path = false) {
    auto program = nasal_parser.tree();
    this_file = program->get_location().file;
    report_show_path_flag = report_show_path;
    used_files = {this_file};

    auto library_code_block = experimental_load_library();
    std::vector<code_block*> bfs;
    bfs.push_back(program);
    bfs.push_back(library_code_block);

    while(!bfs.empty()) {
        auto block = bfs.back();
        bfs.pop_back();
        for(auto& i : block->get_expressions()) {
            if (!import_check(i)) {
                continue;
            }
            auto res = experimental_load_module(i);
            auto def = generate_module_definition(res);
            delete i;
            i = def;
            bfs.push_back(res);
        }
    }

    // load library before another modules
    auto new_code_block = new code_block(program->get_location());
    for(auto i : library_code_block->get_expressions()) {
        new_code_block->add_expression(i);
    }
    library_code_block->get_expressions().clear();
    delete library_code_block;

    // add module hashmap in globals
    auto def = new definition_expr({0, 0, 0, 0, this_file});
    def->set_identifier(new identifier({0, 0, 0, 0, this_file}, "nasal@modules"));
    def->set_value(new hash_expr({0, 0, 0, 0, this_file}));
    new_code_block->add_expression(def);

    // load program itself
    for(auto i : program->get_expressions()) {
        new_code_block->add_expression(i);
    }
    program->get_expressions().clear();
    delete program;
    nasal_parser.swap(new_code_block);
    return err;
}

}
