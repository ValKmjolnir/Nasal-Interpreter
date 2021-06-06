#ifndef __NASAL_IMPORT_H__
#define __NASAL_IMPORT_H__

class nasal_import
{
private:
    nasal_lexer    import_lex;
    nasal_parse    import_par;
    nasal_ast      import_ast;
    std::vector<std::string> filename_table;
    int error;
    void die(std::string,std::string);
    bool check_import(nasal_ast&);
    bool check_exist(std::string);
    void linker(nasal_ast&,nasal_ast&);
    nasal_ast file_import(nasal_ast&);
    nasal_ast load(nasal_ast&);
public:
    int  get_error();
    void link(nasal_ast&);
    nasal_ast& get_root();
};

void nasal_import::die(std::string filename,std::string error_stage)
{
    ++error;
    std::cout<<">> [import] in <\""<<filename<<"\">: error(s) occurred in "<<error_stage<<".\n";
    return;
}

bool nasal_import::check_import(nasal_ast& node)
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
    std::vector<nasal_ast>& ref_vec=node.get_children();
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

bool nasal_import::check_exist(std::string filename)
{
    // avoid importing the same file
    for(auto& fname:filename_table)
        if(filename==fname)
            return true;
    filename_table.push_back(filename);
    return false;
}

void nasal_import::linker(nasal_ast& root,nasal_ast& add_root)
{
    // add children of add_root to the back of root
    for(auto& i:add_root.get_children())
        root.add_child(i);
    return;
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
    import_lex.openfile(filename);
    import_lex.scanner();
    if(import_lex.get_error())
    {
        die(filename,"lexer");
        return tmp;
    }
    import_par.set_toklist(import_lex.get_token_list());
    import_lex.get_token_list().clear();
    import_par.main_process();
    if(import_par.get_error())
    {
        die(filename,"parser");
        return tmp;
    }
    tmp=import_par.get_root();
    import_par.get_root().clear();
    // check if tmp has 'import'
    return load(tmp);
}

nasal_ast nasal_import::load(nasal_ast& root)
{
    nasal_ast new_root(0,ast_root);
    for(auto& i:root.get_children())
        if(check_import(i))
        {
            nasal_ast tmp=file_import(i);
            // add tmp to the back of new_root
            linker(new_root,tmp);
        }
    // add root to the back of new_root
    linker(new_root,root);
    // oops,i think it is not efficient if the root is too ... large?
    return new_root;
}

void nasal_import::link(nasal_ast& root)
{
    // initializing
    error=0;
    filename_table.clear();
    import_ast.clear();
    // scan root and import files,then generate a new ast and return to import_ast
    import_ast=load(root);
    return;
}

nasal_ast& nasal_import::get_root()
{
    return import_ast;
}

int nasal_import::get_error()
{
    return error;
}

#endif