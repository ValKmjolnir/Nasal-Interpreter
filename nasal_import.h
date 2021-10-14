#ifndef __NASAL_IMPORT_H__
#define __NASAL_IMPORT_H__

class nasal_import
{
private:
    uint32_t                 error;
    nasal_lexer              import_lex;
    nasal_parse              import_par;
    nasal_ast                import_ast;
    std::vector<std::string> filename_table;
    void      die(const std::string&,const char*);
    bool      check_import(const nasal_ast&);
    bool      check_exist(const std::string&);
    void      linker(nasal_ast&,nasal_ast&&);
    nasal_ast file_import(nasal_ast&);
    nasal_ast load(nasal_ast&,uint16_t);
public:
    uint32_t  err(){return error;}
    void      link(nasal_ast&,const std::string&);
    const nasal_ast& get_root(){return import_ast;}
    const std::vector<std::string>& get_file(){return filename_table;}
};

void nasal_import::die(const std::string& filename,const char* error_stage)
{
    ++error;
    std::cout<<"[import] in <\""<<filename<<"\">: error(s) occurred in "<<error_stage<<".\n";
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
    if(node.get_type()!=ast_call)
        return false;
    const std::vector<nasal_ast>& ref_vec=node.get_children();
    if(ref_vec.size()!=2)
        return false;
    if(ref_vec[0].get_str()!="import")
        return false;
    if(ref_vec[1].get_type()!=ast_callf)
        return false;
    if(ref_vec[1].get_children().size()!=1 || ref_vec[1].get_children()[0].get_type()!=ast_str)
        return false;
    return true;
}

bool nasal_import::check_exist(const std::string& file)
{
    // avoid importing the same file
    for(auto& fname:filename_table)
        if(file==fname)
            return true;
    filename_table.push_back(file);
    return false;
}

void nasal_import::linker(nasal_ast& root,nasal_ast&& add_root)
{
    // add children of add_root to the back of root
    for(auto& i:add_root.get_children())
        root.add_child(std::move(i));
}

nasal_ast nasal_import::file_import(nasal_ast& node)
{
    // initializing
    nasal_ast tmp(0,ast_root);

    // get filename and set node to ast_null
    std::string filename=node.get_children()[1].get_children()[0].get_str();
    node.clear();

    // avoid infinite loading loop
    if(check_exist(filename))
        return tmp;
    
    // start importing...
    import_lex.open(filename);
    import_lex.scan();
    if(import_lex.err())
    {
        die(filename,"lexer");
        return tmp;
    }
    import_par.compile(import_lex.get_tokens());
    if(import_par.err())
    {
        die(filename,"parser");
        return tmp;
    }
    tmp=std::move(import_par.get_root());
    // check if tmp has 'import'
    return load(tmp,filename_table.size()-1);
}

nasal_ast nasal_import::load(nasal_ast& root,uint16_t fileindex)
{
    nasal_ast new_root(0,ast_root);
    for(auto& i:root.get_children())
        if(check_import(i))
            linker(new_root,file_import(i));
    // add root to the back of new_root
    nasal_ast file_head(0,ast_file);
    file_head.set_num(fileindex);
    new_root.add_child(std::move(file_head));
    linker(new_root,std::move(root));
    return new_root;
}

void nasal_import::link(nasal_ast& root,const std::string& self)
{
    // initializing
    error=0;
    filename_table.clear();
    filename_table.push_back(self);
    import_ast.clear();
    // scan root and import files,then generate a new ast and return to import_ast
    // the main file's index is 0
    import_ast=load(root,0);
}

#endif