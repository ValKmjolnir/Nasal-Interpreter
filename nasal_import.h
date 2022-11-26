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

class linker{
private:
    bool show_path;
    bool lib_loaded;
    error& err;
    std::vector<string> files;
    std::vector<string> envpath;

    bool imptchk(const ast&);
    bool exist(const string&);
    void link(ast&,ast&&);
    string path(const ast&);
    string findf(const string&);
    ast fimpt(ast&);
    ast libimpt();
    ast load(ast&,u16);
public:
    linker(error&);
    const error& link(parse&,const string&,bool);
    const std::vector<string>& filelist() const {return files;}
};

linker::linker(error& e):show_path(false),lib_loaded(false),err(e) {
#ifdef _WIN32
    char sep=';';
#else
    char sep=':';
#endif
    string PATH=getenv("PATH");
    usize last=0,pos=PATH.find(sep,0);
    while(pos!=string::npos) {
        string dirpath=PATH.substr(last,pos-last);
        if (dirpath.length()) {
            envpath.push_back(dirpath);
        }
        last=pos+1;
        pos=PATH.find(sep,last);
    }
    if (last!=PATH.length()) {
        envpath.push_back(PATH.substr(last));
    }
}

string linker::path(const ast& node) {
    if (node[1].type()==ast_callf) {
        return node[1][0].str();
    }
    string fpath=".";
    for(usize i=1;i<node.size();++i) {
#ifndef _WIN32
        fpath+="/"+node[i].str();
#else
        fpath+="\\"+node[i].str();
#endif
    }
    return fpath+".nas";
}

string linker::findf(const string& fname) {
    std::vector<string> fpath={fname};
    for(auto&p:envpath) {
#ifdef _WIN32
        fpath.push_back(p+"\\"+fname);
#else
        fpath.push_back(p+"/"+fname);
#endif
    }
    for(auto& i:fpath) {
        if (access(i.c_str(),F_OK)!=-1) {
            return i;
        }
    }
    if (fname=="lib.nas") {
#ifdef _WIN32
        return findf("stl\\lib.nas");
#else
        return findf("stl/lib.nas");
#endif
    }
    if (!show_path) {
        err.err("link","cannot find file <"+fname+">");
        return "";
    }
    string paths="";
    for(auto& i:fpath) {
        paths+="  "+i+"\n";
    }
    err.err("link","cannot find file <"+fname+"> in these paths:\n"+paths);
    return "";
}

bool linker::imptchk(const ast& node) {
// only these two kinds of node can be recognized as 'import':
/*
    call
    |_id:import
    |_callh:stl
    |_callh:file
*/
    if (node.type()==ast_call && node[0].str()=="import" && node.size()>=2 && node[1].type()==ast_callh) {
        for(usize i=1;i<node.size();++i) {
            if (node[i].type()!=ast_callh) {
                return false;
            }
        }
        return true;
    }
/*
    call
    |_id:import
    |_call_func
      |_string:'filename'
*/
    return (
        node.type()==ast_call &&
        node[0].str()=="import" &&
        node.size()==2 &&
        node[1].type()==ast_callf &&
        node[1].size()==1 &&
        node[1][0].type()==ast_str
    );
}

bool linker::exist(const string& file) {
    // avoid importing the same file
    for(auto& fname:files) {
        if (file==fname) {
            return true;
        }
    }
    files.push_back(file);
    return false;
}

void linker::link(ast& root,ast&& add_root) {
    // add children of add_root to the back of root
    for(auto& i:add_root.child()) {
        root.add(std::move(i));
    }
}

ast linker::fimpt(ast& node) {
    lexer lex(err);
    parse par(err);
    // get filename and set node to ast_null
    string filename=path(node);
    node.clear();

    // avoid infinite loading loop
    filename=findf(filename);
    if (!filename.length() || exist(filename)) {
        return {0,0,ast_root};
    }
    
    // start importing...
    lex.scan(filename);
    par.compile(lex);
    ast tmp=std::move(par.tree());
    // check if tmp has 'import'
    return load(tmp,files.size()-1);
}

ast linker::libimpt() {
    lexer lex(err);
    parse par(err);
    string filename=findf("lib.nas");
    if (!filename.length()) {
        return {0,0,ast_root};
    }

    // avoid infinite loading loop
    if (exist(filename)) {
        return {0,0,ast_root};
    }
    
    // start importing...
    lex.scan(filename);
    par.compile(lex);
    ast tmp=std::move(par.tree());
    // check if tmp has 'import'
    return load(tmp,files.size()-1);
}

ast linker::load(ast& root,u16 fileindex) {
    ast tree(0,0,ast_root);
    if (!lib_loaded) {
        link(tree,libimpt());
        lib_loaded=true;
    }
    for(auto& i:root.child()) {
        if (imptchk(i)) {
            link(tree,fimpt(i));
        } else {
            break;
        }
    }
    // add root to the back of tree
    ast file_head(0,0,ast_file);
    file_head.set_num(fileindex);
    tree.add(std::move(file_head));
    link(tree,std::move(root));
    return tree;
}

const error& linker::link(parse& parse,const string& self,bool spath=false) {
    show_path=spath;
    // initializing
    files={self};
    // scan root and import files,then generate a new ast and return to import_ast
    // the main file's index is 0
    parse.tree()=load(parse.tree(),0);
    return err;
}
