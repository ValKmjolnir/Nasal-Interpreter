#ifndef __NASAL_IMPORT_H__
#define __NASAL_IMPORT_H__

class nasal_import
{
private:
    uint32_t                 error;
    nasal_lexer              import_lex;
    nasal_parse              import_par;
    nasal_ast                import_ast;
    std::vector<std::string> file_table;
    void      die(const std::string&,const char*);
    bool      check_import(const nasal_ast&);
    bool      check_exist(const std::string&);
    void      linker(nasal_ast&,nasal_ast&&);
    nasal_ast file_import(nasal_ast&);
    nasal_ast load(nasal_ast&,uint16_t);
public:
    uint32_t  err(){return error;}
    void      link(nasal_parse&,const std::string&);
    const std::vector<std::string>& get_file() const {return file_table;}
};

void nasal_import::die(const std::string& file,const char* stage)
{
    ++error;
    std::cout<<"[import] in <\""<<file<<"\">: error(s) occurred in "<<stage<<".\n";
}

bool nasal_import::check_import(const nasal_ast& node)
{
/*
only this kind of node can be recognized as 'import':
    call
        id:import
        call_func
            string:'filename'
*/
    if(node.type()!=ast_call)
        return false;
    const std::vector<nasal_ast>& ref_vec=node.child();
    if(ref_vec.size()!=2)
        return false;
    if(ref_vec[0].str()!="import")
        return false;
    if(ref_vec[1].type()!=ast_callf)
        return false;
    if(ref_vec[1].child().size()!=1 || ref_vec[1].child()[0].type()!=ast_str)
        return false;
    return true;
}

bool nasal_import::check_exist(const std::string& file)
{
    // avoid importing the same file
    for(auto& fname:file_table)
        if(file==fname)
            return true;
    file_table.push_back(file);
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
    // get filename and set node to ast_null
    std::string filename=node.child()[1].child()[0].str();
    node.clear();

    // avoid infinite loading loop
    if(check_exist(filename))
        return {0,ast_root};
    
    // start importing...
    import_lex.scan(filename);
    if(import_lex.err())
    {
        die(filename,"lexer");
        return {0,ast_root};
    }
    import_par.compile(import_lex);
    if(import_par.err())
    {
        die(filename,"parser");
        return {0,ast_root};
    }
    nasal_ast tmp=std::move(import_par.ast());
    // check if tmp has 'import'
    return load(tmp,file_table.size()-1);
}

nasal_ast nasal_import::load(nasal_ast& root,uint16_t fileindex)
{
    nasal_ast new_root(0,ast_root);
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
    error=0;
    file_table={self};
    // scan root and import files,then generate a new ast and return to import_ast
    // the main file's index is 0
    parse.ast()=load(parse.ast(),0);
}

#endif