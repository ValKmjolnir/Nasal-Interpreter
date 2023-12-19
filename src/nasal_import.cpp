#include "nasal_import.h"
#include "symbol_finder.h"

#include <memory>
#include <unordered_set>

namespace nasal {

linker::linker(): show_path_flag(false), library_loaded(false), this_file("") {
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

std::string linker::get_path(expr* node) {
    if (node->get_type()==expr_type::ast_use) {
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
    auto call_node = reinterpret_cast<call_expr*>(node);
    auto arguments = reinterpret_cast<call_function*>(call_node->get_calls()[0]);
    auto content = reinterpret_cast<string_literal*>(arguments->get_argument()[0]);
    return content->get_content();
}

std::string linker::find_real_file_path(
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
    if (!show_path_flag) {
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

bool linker::import_check(expr* node) {
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

bool linker::check_exist_or_record_file(const std::string& file) {
    // avoid importing the same file
    for(const auto& name : imported_files) {
        if (file==name) {
            return true;
        }
    }
    imported_files.push_back(file);
    return false;
}

bool linker::check_self_import(const std::string& file) {
    for(const auto& name : module_load_stack) {
        if (file==name) {
            return true;
        }
    }
    return false;
}

std::string linker::generate_self_import_path(const std::string& filename) {
    std::string res = "";
    for(const auto& i : module_load_stack) {
        res += "[" + i + "] -> ";
    }
    return res + "[" + filename + "]";
}

void linker::link(code_block* new_tree_root, code_block* old_tree_root) {
    // add children of add_root to the back of root
    for(auto& i : old_tree_root->get_expressions()) {
        new_tree_root->add_expression(i);
    }
    // clean old root
    old_tree_root->get_expressions().clear();
}

code_block* linker::import_regular_file(
    expr* node, std::unordered_set<std::string>& used_modules) {
    // get filename
    auto filename = get_path(node);

    // avoid infinite loading loop
    filename = find_real_file_path(filename, node->get_location());
    // if get empty string(error) or this file is used before, do not parse
    if (!filename.length() || used_modules.count(filename)) {
        return new code_block({0, 0, 0, 0, filename});
    }

    // check self import, avoid infinite loading loop
    if (check_self_import(filename)) {
        err.err("link",
            node->get_location(),
            "self-referenced module <" + filename + ">, " +
            "reference path: " + generate_self_import_path(filename)
        );
        return new code_block({0, 0, 0, 0, filename});
    }
    check_exist_or_record_file(filename);
    
    module_load_stack.push_back(filename);
    if (module_load_stack.size()>MAX_RECURSION_DEPTH) {
        err.err("link",
            node->get_location(),
            "too deep module import stack (>" +
            std::to_string(MAX_RECURSION_DEPTH) + ")."
        );
        return new code_block({0, 0, 0, 0, filename});
    }
    // start importing...
    lexer nasal_lexer;
    parse nasal_parser;
    if (nasal_lexer.scan(filename).geterr()) {
        err.err("link",
            node->get_location(),
            "error occurred when analysing <" + filename + ">"
        );
        return new code_block({0, 0, 0, 0, filename});
    }
    if (nasal_parser.compile(nasal_lexer).geterr())  {
        err.err("link",
            node->get_location(),
            "error occurred when analysing <" + filename + ">"
        );
        return new code_block({0, 0, 0, 0, filename});
    }
    // swap result out
    auto parse_result = nasal_parser.swap(nullptr);

    // check if parse result has 'import'
    auto result = load(parse_result, filename);
    module_load_stack.pop_back();
    return result;
}

code_block* linker::import_nasal_lib() {
    auto path = find_real_file_path(
        "lib.nas", {0, 0, 0, 0, this_file}
    );
    if (!path.length()) {
        return new code_block({0, 0, 0, 0, path});
    }

    // avoid infinite loading library
    if (check_exist_or_record_file(path)) {
        return new code_block({0, 0, 0, 0, path});
    }
    
    // start importing...
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
    // swap result out
    auto parse_result = nasal_parser.swap(nullptr);
    // check if library has 'import' (in fact it should not)
    return load(parse_result, path);
}

std::string linker::generate_module_name(const std::string& file_path) {
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

return_expr* linker::generate_module_return(code_block* block) {
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

definition_expr* linker::generate_module_definition(code_block* block) {
    auto def = new definition_expr(block->get_location());
    def->set_identifier(new identifier(
        block->get_location(),
        generate_module_name(block->get_location().file)
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

code_block* linker::load(code_block* program_root, const std::string& filename) {
    auto tree = new code_block({0, 0, 0, 0, filename});
    // load library, this ast will be linked with root directly
    // so no extra namespace is generated
    if (!library_loaded) {
        auto nasal_lib_code_block = import_nasal_lib();
        // insert nasal lib code to the back of tree
        link(tree, nasal_lib_code_block);
        delete nasal_lib_code_block;
        library_loaded = true;
    }

    // load imported modules
    std::unordered_set<std::string> used_modules = {};
    for(auto& import_node : program_root->get_expressions()) {
        if (!import_check(import_node)) {
            break;
        }
        // parse file and get ast
        auto module_code_block = import_regular_file(import_node, used_modules);
        auto replace_node = new null_expr(import_node->get_location());
        // after importing the regular file as module, delete this node
        delete import_node;
        // and replace the node with null_expr node
        import_node = replace_node;

        // avoid repeatedly importing the same module
        const auto& module_path = module_code_block->get_location().file;
        if (used_modules.count(module_path)) {
            delete module_code_block;
            continue;
        }
        
        // then we generate a function warping the code block,
        // and export the necessary global symbols in this code block
        // by generate a return statement, with a hashmap return value
        used_modules.insert(module_path);
        tree->add_expression(generate_module_definition(module_code_block));
    }

    // insert program root to the back of tree
    link(tree, program_root);
    return tree;
}

const error& linker::link(parse& parse, bool spath = false) {
    // switch for showing path when errors occur
    show_path_flag = spath;

    // initializing file map
    this_file = parse.tree()->get_location().file;
    imported_files = {this_file};
    module_load_stack = {this_file};

    // scan root and import files
    // then generate a new ast and return to import_ast
    // dfs load file
    auto new_tree_root = load(parse.tree(), this_file);
    auto old_tree_root = parse.swap(new_tree_root);
    delete old_tree_root;
    return err;
}

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

std::string experimental_linker::generate_module_definition_name(const std::string& file_path) {
    return "auto_generated_module@[" + file_path + "]";
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
        generate_module_definition_name(block->get_location().file)
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
    if (load_mapper.count(module_path)) {
        return load_mapper.at(module_path);
    }
    auto res = compile_core(module_path);
    load_mapper.insert({module_path, res});
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
    load_mapper.insert({library_path, res});
    return res;
}

const error& experimental_linker::link(parse& nasal_parser, bool report_show_path = false) {
    auto program = nasal_parser.tree();
    this_file = program->get_location().file;
    report_show_path_flag = report_show_path;
    used_files = {this_file};
    std::vector<std::string> load_order = {};
    load_mapper.clear();
    load_mapper.insert({this_file, program});

    auto library_code_block = experimental_load_library();
    std::vector<code_block*> dfs;
    dfs.push_back(program);
    dfs.push_back(library_code_block);

    while(!dfs.empty()) {
        auto block = dfs.back();
        dfs.pop_back();
        std::vector<code_block*> temp;
        for(auto& i : block->get_expressions()) {
            if (i->get_type()!=expr_type::ast_use) {
                continue;
            }
            auto res = experimental_load_module(i);
            load_order.push_back(res->get_location().file);

            if (res->get_expressions().size()) {
                temp.push_back(res);
            } else {
                delete res;
            }
        }

        for(auto i = temp.rbegin(); i!=temp.rend(); i++) {
            dfs.push_back(*i);
        }
    }

    for(auto& block : load_mapper) {
        for(auto& j : block.second->get_expressions()) {
            if (j->get_type()!=expr_type::ast_use) {
                continue;
            }
            auto res = experimental_load_module(j);
            const auto name = generate_module_real_name(res->get_location().file);
            const auto def_name = generate_module_definition_name(res->get_location().file);
            auto def = new definition_expr(j->get_location());
            def->set_identifier(new identifier(j->get_location(), name));
            def->set_value(new identifier(j->get_location(), def_name));

            delete j;
            j = def;
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

    // load modules by order
    std::unordered_set<std::string> loaded;
    for(auto iter = load_order.rbegin(); iter!=load_order.rend(); ++iter) {
        if (loaded.count(*iter)) {
            continue;
        }
        loaded.insert(*iter);
        new_code_block->add_expression(
            generate_module_definition(load_mapper.at(*iter))
        );
    }
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
