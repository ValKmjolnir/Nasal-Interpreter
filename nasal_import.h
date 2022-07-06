#ifndef __NASAL_IMPORT_H__
#define __NASAL_IMPORT_H__

class nasal_import
{
private:
    bool lib_loaded;
    nasal_err& nerr;
    std::vector<std::string> files;
    bool imptchk(const nasal_ast&);
    bool exist(const std::string&);
    void linker(nasal_ast&,nasal_ast&&);
    std::string path(const nasal_ast&);
    nasal_ast fimpt(nasal_ast&);
    nasal_ast libimpt();
    nasal_ast load(nasal_ast&,uint16_t);
public:
    nasal_import(nasal_err& e):lib_loaded(false),nerr(e){}
    void link(nasal_parse&,const std::string&);
    const std::vector<std::string>& get_file() const {return files;}
};

std::string nasal_import::path(const nasal_ast& node)
{
    if(node[1].type()==ast_callf)
        return node[1][0].str();
    std::string fpath=".";
    for(size_t i=1;i<node.size();++i)
#ifndef _WIN32
        fpath+="/"+node[i].str();
#else
        fpath+="\\"+node[i].str();
#endif
    fpath+=".nas";
    return fpath;
}

bool nasal_import::imptchk(const nasal_ast& node)
{
// only these two kinds of node can be recognized as 'import':
/*
    call
    |_id:import
    |_callh:stl
    |_callh:file
*/
    if(node.type()==ast_call && node.size()>=2 && node[0].str()=="import" && node[1].type()==ast_callh)
    {
        for(size_t i=1;i<node.size();++i)
            if(node[i].type()!=ast_callh)
                return false;
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
        node.size()==2 &&
        node[0].str()=="import" &&
        node[1].type()==ast_callf &&
        node[1].size()==1 &&
        node[1][0].type()==ast_str
    );
}

bool nasal_import::exist(const std::string& file)
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

nasal_ast nasal_import::fimpt(nasal_ast& node)
{
    nasal_lexer lex(nerr);
    nasal_parse par(nerr);
    // get filename and set node to ast_null
    std::string filename=path(node);
    node.clear();

    // avoid infinite loading loop
    if(exist(filename))
        return {0,ast_root};
    if(access(filename.c_str(),F_OK)==-1)
    {
        nerr.err("link","cannot open file <"+filename+">");
        return {0,ast_root};
    }
    
    // start importing...
    lex.scan(filename);
    par.compile(lex);
    nasal_ast tmp=std::move(par.ast());
    // check if tmp has 'import'
    return load(tmp,files.size()-1);
}

nasal_ast nasal_import::libimpt()
{
    nasal_lexer lex(nerr);
    nasal_parse par(nerr);

    const std::vector<std::string> libpath=
    {
#ifdef __WIN32
        ".\\lib.nas",
        ".\\stl\\lib.nas"
#else
        "./lib.nas",
        "./stl/lib.nas"
#endif
    };
    std::string filename="";
    for(auto& i:libpath)
        if(access(i.c_str(),F_OK)!=-1)
        {
            filename=i;
            break;
        }
    if(!filename.length())
    {
        std::string paths="";
        for(auto& i:libpath)
            paths+="  "+i+"\n";
        nerr.err("link","cannot find lib file in these paths:\n"+paths);
        nerr.chkerr();
        return {0,ast_root};
    }

    // avoid infinite loading loop
    if(exist(filename))
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
        linker(new_root,libimpt());
        lib_loaded=true;
    }
    for(auto& i:root.child())
        if(imptchk(i))
            linker(new_root,fimpt(i));
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