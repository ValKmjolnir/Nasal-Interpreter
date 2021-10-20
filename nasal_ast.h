#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

enum ast_node
{
    ast_null=0,
    ast_root,
    ast_block,
    ast_file, // ast_file is only used to store which file the subtree is on,codegen will generate nothing
    ast_nil,ast_num,ast_str,ast_id,ast_func,ast_hash,ast_vec,
    ast_hashmember,
    ast_call,ast_callh,ast_callv,ast_callf,
    ast_subvec,
    ast_args,ast_default,ast_dynamic,
    ast_and,ast_or,
    ast_equal,
    ast_addeq,ast_subeq,
    ast_multeq,ast_diveq,
    ast_lnkeq,
    ast_cmpeq,ast_neq,
    ast_less,ast_leq,
    ast_grt,ast_geq,
    ast_add,ast_sub,
    ast_mult,ast_div,
    ast_link,
    ast_neg,ast_not,
    ast_trino,
    ast_for,ast_forindex,ast_foreach,ast_while,ast_new_iter,
    ast_conditional,ast_if,ast_elsif,ast_else,
    ast_multi_id,ast_multi_scalar,
    ast_def,ast_multi_assign,
    ast_continue,ast_break,ast_ret
};

const char* ast_name[]=
{
    "null",
    "root",
    "block",
    "file",
    "nil","num","str","id","func","hash","vec",
    "hashmember",
    "call","callh","callv","callf",
    "subvec",
    "args","default","dynamic",
    "and","or",
    "=",
    "+=","-=",
    "*=","/=",
    "~=",
    "==","!=",
    "<","<=",
    ">",">=",
    "+","-",
    "*","/",
    "~",
    "unary-","unary!",
    "trino",
    "for","forindex","foreach","while","iter",
    "conditional","if","elsif","else",
    "multi_id","multi_scalar",
    "def","multi_assign",
    "continue","break","return"
};

class nasal_ast
{
private:
    uint32_t    text_line;
    uint32_t    node_type;
    double      node_num;
    std::string node_str;
    std::vector<nasal_ast> node_child;
public:
    nasal_ast(const uint32_t l=0,const uint32_t t=ast_null):text_line(l),node_type(t){}
    nasal_ast(const nasal_ast&);
    nasal_ast(nasal_ast&&);
    void print(const int);
    void clear();
    
    nasal_ast& operator=(const nasal_ast&);
    nasal_ast& operator=(nasal_ast&&);
    nasal_ast& operator[](const int index){return node_child[index];}
    const nasal_ast& operator[](const int index) const {return node_child[index];}
    size_t size() const {return node_child.size();}
    
    void add(nasal_ast&& ast){node_child.push_back(std::move(ast));}
    void add(const nasal_ast& ast){node_child.push_back(ast);}
    void set_line(const uint32_t l){text_line=l;}
    void set_type(const uint32_t t){node_type=t;}
    void set_str(const std::string& s){node_str=s;}
    void set_num(const double n){node_num=n;}

    inline const uint32_t     line() const {return text_line;}
    inline const uint32_t     type() const {return node_type;}
    inline const double       num()  const {return node_num;}
    inline const std::string& str()  const {return node_str;}
    inline const std::vector<nasal_ast>& child() const {return node_child;}
    inline std::vector<nasal_ast>& child(){return node_child;}
};

nasal_ast::nasal_ast(const nasal_ast& tmp)
{
    text_line=tmp.text_line;
    node_type=tmp.node_type;
    node_num =tmp.node_num;
    node_str =tmp.node_str;
    node_child=tmp.node_child;
}

nasal_ast::nasal_ast(nasal_ast&& tmp)
{
    text_line=tmp.text_line;
    node_type=tmp.node_type;
    node_num =tmp.node_num;
    node_str.swap(tmp.node_str);
    node_child.swap(tmp.node_child);
}

nasal_ast& nasal_ast::operator=(const nasal_ast& tmp)
{
    text_line=tmp.text_line;
    node_type=tmp.node_type;
    node_num=tmp.node_num;
    node_str=tmp.node_str;
    node_child=tmp.node_child;
    return *this;
}

nasal_ast& nasal_ast::operator=(nasal_ast&& tmp)
{
    text_line=tmp.text_line;
    node_type=tmp.node_type;
    node_num=tmp.node_num;
    node_str.swap(tmp.node_str);
    node_child.swap(tmp.node_child);
    return *this;
}

void nasal_ast::clear()
{
    text_line=0;
    node_num=0;
    node_str="";
    node_type=ast_null;
    node_child.clear();
}

void nasal_ast::print(const int depth)
{
    for(int i=0;i<depth;++i)
        std::cout<<"|  ";
    std::cout<<ast_name[node_type];
    if(
        node_type==ast_str ||
        node_type==ast_id ||
        node_type==ast_default ||
        node_type==ast_dynamic ||
        node_type==ast_callh)
        std::cout<<":"<<raw_string(node_str);
    else if(node_type==ast_num || node_type==ast_file)
        std::cout<<":"<<node_num;
    std::cout<<'\n';
    for(auto& i:node_child)
        i.print(depth+1);
}

#endif