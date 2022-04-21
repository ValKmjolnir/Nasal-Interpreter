#ifndef __NASAL_IMPORT_H__
#define __NASAL_IMPORT_H__

class nasal_import
{
private:
    bool lib_loaded;
    nasal_err& nerr;
    std::vector<std::string> files;
    bool check_import(const nasal_ast&);
    bool check_exist(const std::string&);
    void linker(nasal_ast&,nasal_ast&&);
    nasal_ast file_import(nasal_ast&);
    nasal_ast lib_import();
    nasal_ast load(nasal_ast&,uint16_t);
public:
    nasal_import(nasal_err& e):lib_loaded(false),nerr(e){}
    void link(nasal_parse&,const std::string&);
    const std::vector<std::string>& get_file() const {return files;}
};

bool nasal_import::check_import(const nasal_ast& node)
{
/*
only this kind of node can be recognized as 'import':
    call
    |_id:import
    |_call_func
      |_string:'filename'
*/
    return (
        node.type()==ast_call &&
        node.size()==2 &&
        node[0].str()=="import" &&
        node[1].type()==ast_callf &&
        node[1].size()==1 &&
        node[1][0].type()==ast_str
    );
}

bool nasal_import::check_exist(const std::string& file)
{
    // avoid importing the same file
    for(auto& fname:files)
        if(file==fname)
            return true;
    files.push_back(file);
    return false;
}

void nasal_import::linker(nasal_ast& root,nasal_ast&& add_root)
{
    // add children of add_root to the back of root
    for(auto& i:add_root.child())
        root.add(std::move(i));
}

nasal_ast nasal_import::file_import(nasal_ast& node)
{
    nasal_lexer lex(nerr);
    nasal_parse par(nerr);
    // get filename and set node to ast_null
    std::string filename=node[1][0].str();
    node.clear();

    // avoid infinite loading loop
    if(check_exist(filename))
        return {0,ast_root};
    
    // start importing...
    lex.scan(filename);
    par.compile(lex);
    nasal_ast tmp=std::move(par.ast());
    // check if tmp has 'import'
    return load(tmp,files.size()-1);
}

nasal_ast nasal_import::lib_import()
{
    nasal_lexer lex(nerr);
    nasal_parse par(nerr);

    const std::vector<std::string> libpath=
    {
        "lib.nas",
        "stl/lib.nas"
    };

    std::string filename;
    for(auto& i:libpath)
    {
        std::ifstream fin(i);
        if(!fin.fail())
        {
            filename=i;
            fin.close();
            break;
        }
        fin.close();
    }

    // avoid infinite loading loop
    if(check_exist(filename))
        return {0,ast_root};
    
    // start importing...
    lex.scan(filename);
    par.compile(lex);
    nasal_ast tmp=std::move(par.ast());
    // check if tmp has 'import'
    return load(tmp,files.size()-1);
}

nasal_ast nasal_import::load(nasal_ast& root,uint16_t fileindex)
{
    nasal_ast new_root(0,ast_root);
    if(!lib_loaded)
    {
        linker(new_root,lib_import());
        lib_loaded=true;
    }
    for(auto& i:root.child())
        if(check_import(i))
            linker(new_root,file_import(i));
    // add root to the back of new_root
    nasal_ast file_head(0,ast_file);
    file_head.set_num(fileindex);
    new_root.add(std::move(file_head));
    linker(new_root,std::move(root));
    return new_root;
}

void nasal_import::link(nasal_parse& parse,const std::string& self)
{
    // initializing
    files={self};
    // scan root and import files,then generate a new ast and return to import_ast
    // the main file's index is 0
    parse.ast()=load(parse.ast(),0);
}

#endif