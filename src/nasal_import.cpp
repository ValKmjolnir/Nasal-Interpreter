#include "nasal_import.h"

linker::linker(): show_path(false), lib_loaded(false) {
    char sep = is_windows()? ';':':';
    std::string PATH = getenv("PATH");
    usize last = 0, pos = PATH.find(sep, 0);
    while(pos!=std::string::npos) {
        std::string dirpath = PATH.substr(last, pos-last);
        if (dirpath.length()) {
            envpath.push_back(dirpath);
        }
        last = pos+1;
        pos = PATH.find(sep, last);
    }
    if (last!=PATH.length()) {
        envpath.push_back(PATH.substr(last));
    }
}

std::string linker::get_path(call_expr* node) {
    if (node->get_calls()[0]->get_type()==expr_type::ast_callf) {
        auto tmp = (call_function*)node->get_calls()[0];
        return ((string_literal*)tmp->get_argument()[0])->get_content();
    }
    auto fpath = std::string(".");
    for(auto i : node->get_calls()) {
        fpath += (is_windows()? "\\":"/") + ((call_hash*)i)->get_field();
    }
    return fpath + ".nas";
}

std::string linker::find_file(
    const std::string& filename, const span& location) {
    // first add file name itself into the file path
    std::vector<std::string> fpath = {filename};

    // generate search path from environ path
    for(const auto& p : envpath) {
        fpath.push_back(p + (is_windows()? "\\":"/") + filename);
    }

    // search file
    for(const auto& i : fpath) {
        if (access(i.c_str(), F_OK)!=-1) {
            return i;
        }
    }

    // we will find lib.nas in nasal std directory
    if (filename=="lib.nas") {
        return is_windows()?
            find_file("std\\lib.nas", location):
            find_file("std/lib.nas", location);
    }
    if (!show_path) {
        err.err("link",
            "in <" + location.file + ">: " +
            "cannot find file <" + filename + ">\n");
        err.err("link", "use <-d> to get detail search path");
        return "";
    }
    std::string paths = "";
    for(const auto& i : fpath) {
        paths += "  -> " + i + "\n";
    }
    err.err("link",
        "in <" + location.file + ">: " +
        "cannot find file <" + filename + "> in these paths:\n" + paths);
    return "";
}

bool linker::import_check(expr* node) {
/*
    call
    |_id:import
    |_callh:std
    |_callh:file
*/
    if (node->get_type()!=expr_type::ast_call) {
        return false;
    }
    auto tmp = (call_expr*)node;
    if (tmp->get_first()->get_type()!=expr_type::ast_id) {
        return false;
    }
    if (((identifier*)tmp->get_first())->get_name()!="import") {
        return false;
    }
    if (!tmp->get_calls().size()) {
        return false;
    }
    // import.xxx.xxx;
    if (tmp->get_calls()[0]->get_type()==expr_type::ast_callh) {
        for(auto i : tmp->get_calls()) {
            if (i->get_type()!=expr_type::ast_callh) {
                return false;
            }
        }
        return true;
    }
    // import("xxx");
    if (tmp->get_calls().size()!=1) {
        return false;
    }
/*
    call
    |_id:import
    |_call_func
      |_string:'filename'
*/
    if (tmp->get_calls()[0]->get_type()!=expr_type::ast_callf) {
        return false;
    }
    auto func_call = (call_function*)tmp->get_calls()[0];
    if (func_call->get_argument().size()!=1) {
        return false;
    }
    if (func_call->get_argument()[0]->get_type()!=expr_type::ast_str) {
        return false;
    }
    return true;
}

bool linker::exist(const std::string& file) {
    // avoid importing the same file
    for(const auto& fname : files) {
        if (file==fname) {
            return true;
        }
    }
    files.push_back(file);
    return false;
}

void linker::link(code_block* new_tree_root, code_block* old_tree_root) {
    // add children of add_root to the back of root
    for(auto& i : old_tree_root->get_expressions()) {
        new_tree_root->add_expression(i);
    }
    // clean old root
    old_tree_root->get_expressions().clear();
}

code_block* linker::import_regular_file(call_expr* node) {
    lexer lex;
    parse par;
    // get filename
    auto filename = get_path(node);
    // clear this node
    for(auto i : node->get_calls()) {
        delete i;
    }
    node->get_calls().clear();
    auto location = node->get_first()->get_location();
    delete node->get_first();
    node->set_first(new nil_expr(location));

    // avoid infinite loading loop
    filename = find_file(filename, node->get_location());
    if (!filename.length() || exist(filename)) {
        return new code_block({0, 0, 0, 0, filename});
    }
    
    // start importing...
    if (lex.scan(filename).geterr()) {
        err.err("link", "error occurred when analysing <" + filename + ">");
    }
    if (par.compile(lex).geterr())  {
        err.err("link", "error occurred when analysing <" + filename + ">");
    }
    auto tmp = par.swap(nullptr);

    // check if tmp has 'import'
    return load(tmp, files.size()-1);
}

code_block* linker::import_nasal_lib() {
    lexer lex;
    parse par;
    auto filename = find_file("lib.nas", {0, 0, 0, 0, files[0]});
    if (!filename.length()) {
        return new code_block({0, 0, 0, 0, filename});
    }

    // avoid infinite loading loop
    if (exist(filename)) {
        return new code_block({0, 0, 0, 0, filename});
    }
    
    // start importing...
    if (lex.scan(filename).geterr()) {
        err.err("link", "error occurred when analysing library <" + filename + ">");
    }
    if (par.compile(lex).geterr())  {
        err.err("link", "error occurred when analysing library <" + filename + ">");
    }
    auto tmp = par.swap(nullptr);

    // check if tmp has 'import'
    return load(tmp, files.size()-1);
}

code_block* linker::load(code_block* root, u16 fileindex) {
    auto tree = new code_block({0, 0, 0, 0, files[fileindex]});
    if (!lib_loaded) {
        auto tmp = import_nasal_lib();
        link(tree, tmp);
        delete tmp;
        lib_loaded = true;
    }
    for(auto i : root->get_expressions()) {
        if (!import_check(i)) {
            break;
        }
        auto tmp = import_regular_file((call_expr*)i);
        link(tree, tmp);
        delete tmp;
    }
    // add root to the back of tree
    auto file_head = new file_info(
        {0, 0, 0, 0, files[fileindex]}, fileindex, files[fileindex]);
    tree->add_expression(file_head);
    link(tree, root);
    return tree;
}

const error& linker::link(
    parse& parse, const std::string& self, bool spath = false) {
    show_path = spath;
    // initializing
    files = {self};
    // scan root and import files
    // then generate a new ast and return to import_ast
    // the main file's index is 0
    auto new_tree_root = load(parse.tree(), 0);
    auto old_tree_root = parse.swap(new_tree_root);
    delete old_tree_root;
    return err;
}
