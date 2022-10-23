#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

#include <vector>
#include <cstring>

enum ast_node:u32
{
    ast_null=0,      // null node
    ast_root,        // mark the root node of ast
    ast_block,       // expression block 
    ast_file,        // used to store which file the sub-tree is on, only used in main block
    ast_nil,         // nil keyword
    ast_num,         // number, basic value type
    ast_str,         // string, basic value type
    ast_id,          // identifier
    ast_func,        // func keyword
    ast_hash,        // hash, basic value type
    ast_vec,         // vector, basic value type
    ast_pair,        // pair of key and value in hashmap
    ast_call,        // mark a sub-tree of calling an identifier
    ast_callh,       // id.name
    ast_callv,       // id[index]
    ast_callf,       // id()
    ast_subvec,      // id[index:index]
    ast_args,        // mark a sub-tree of function parameters
    ast_default,     // default parameter
    ast_dynamic,     // dynamic parameter
    ast_and,         // and keyword
    ast_or,          // or keyword
    ast_equal,       // =
    ast_addeq,       // +=
    ast_subeq,       // -=
    ast_multeq,      // *=
    ast_diveq,       // /=
    ast_lnkeq,       // ~=
    ast_cmpeq,       // ==
    ast_neq,         // !=
    ast_less,        // <
    ast_leq,         // <=
    ast_grt,         // >
    ast_geq,         // >=
    ast_add,         // +
    ast_sub,         // -
    ast_mult,        // *
    ast_div,         // /
    ast_link,        // ~
    ast_neg,         // -
    ast_not,         // ~
    ast_trino,       // ?:
    ast_for,         // for keyword
    ast_forindex,    // forindex keyword
    ast_foreach,     // foreach keyword
    ast_while,       // while
    ast_iter,        // iterator, used in forindex/foreach
    ast_conditional, // mark a sub-tree of conditional expression
    ast_if,          // if keyword
    ast_elsif,       // elsif keyword
    ast_else,        // else keyword
    ast_multi_id,    // multi identifiers sub-tree
    ast_multi_scalar,// multi value sub-tree
    ast_def,         // definition
    ast_multi_assign,// multi assignment sub-tree
    ast_continue,    // continue keyword, only used in loop
    ast_break,       // break keyword, only used in loop
    ast_ret          // return keyword, only used in function block
};

const char* ast_name[]=
{
    "null",
    "root",
    "block",
    "file",
    "nil",
    "num",
    "str",
    "id",
    "func",
    "hash",
    "vec",
    "pair",
    "call",
    "callh",
    "callv",
    "callf",
    "subvec",
    "args",
    "default",
    "dynamic",
    "and",
    "or",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "~=",
    "==",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
    "+",
    "-",
    "*",
    "/",
    "~",
    "unary-",
    "unary!",
    "trino",
    "for",
    "forindex",
    "foreach",
    "while",
    "iter",
    "conditional",
    "if",
    "elsif",
    "else",
    "multi-id",
    "multi-scalar",
    "def",
    "multi-assign",
    "continue",
    "break",
    "return"
};

class ast
{
private:
    u32 node_line;
    u32 node_col;
    u32 node_type;
    f64 node_num;
    string node_str;
    std::vector<ast> node_child;
public:
    ast(const u32 l,const u32 c,const u32 t):
        node_line(l),node_col(c),node_type(t),node_num(0){}
    ast(const ast&);
    ast(ast&&);
    void print_tree();
    void print(u32,bool,std::vector<string>&);
    void clear();
    
    ast& operator=(const ast&);
    ast& operator=(ast&&);
    ast& operator[](usize n){return node_child[n];}
    const ast& operator[](usize n) const {return node_child[n];}
    usize size() const {return node_child.size();}
    
    void add(ast&& node){node_child.push_back(std::move(node));}
    void add(const ast& node){node_child.push_back(node);}
    void set_line(const u32 l){node_line=l;}
    void set_type(const u32 t){node_type=t;}
    void set_str(const string& s){node_str=s;}
    void set_num(const f64 n){node_num=n;}

    inline u32 line() const {return node_line;}
    inline u32 col()  const {return node_col;}
    inline u32 type() const {return node_type;}
    inline f64 num()  const {return node_num;}
    inline const string& str() const {return node_str;}
    inline const std::vector<ast>& child() const {return node_child;}
    inline std::vector<ast>& child(){return node_child;}
};

ast::ast(const ast& tmp):
    node_str(tmp.node_str),node_child(tmp.node_child)
{
    node_line=tmp.node_line;
    node_col=tmp.node_col;
    node_type=tmp.node_type;
    node_num =tmp.node_num;
}

ast::ast(ast&& tmp)
{
    node_line=tmp.node_line;
    node_col=tmp.node_col;
    node_type=tmp.node_type;
    node_num =tmp.node_num;
    node_str.swap(tmp.node_str);
    node_child.swap(tmp.node_child);
}

ast& ast::operator=(const ast& tmp)
{
    node_line=tmp.node_line;
    node_col=tmp.node_col;
    node_type=tmp.node_type;
    node_num=tmp.node_num;
    node_str=tmp.node_str;
    node_child=tmp.node_child;
    return *this;
}

ast& ast::operator=(ast&& tmp)
{
    node_line=tmp.node_line;
    node_col=tmp.node_col;
    node_type=tmp.node_type;
    node_num=tmp.node_num;
    node_str.swap(tmp.node_str);
    node_child.swap(tmp.node_child);
    return *this;
}

void ast::clear()
{
    node_line=node_col=0;
    node_num=0;
    node_str.clear();
    node_type=ast_null;
    node_child.clear();
}

void ast::print_tree()
{
    std::vector<string> tmp;
    print(0,false,tmp);
}

void ast::print(u32 depth,bool last,std::vector<string>& indent)
{
    for(auto& i:indent)
        std::cout<<i;
    std::cout<<ast_name[node_type];
    if(node_type==ast_str ||
       node_type==ast_id ||
       node_type==ast_default ||
       node_type==ast_dynamic ||
       node_type==ast_callh)
        std::cout<<":"<<rawstr(node_str);
    else if(node_type==ast_num || node_type==ast_file)
        std::cout<<":"<<node_num;
    std::cout<<"\n";
    if(last && depth)
        indent.back()="  ";
    else if(!last && depth)
#ifdef _WIN32
        indent.back()="| ";
#else
        indent.back()="│ ";
#endif
    for(u32 i=0;i<node_child.size();++i)
    {
#ifdef _WIN32
        indent.push_back(i==node_child.size()-1?"+-":"|-");
#else
        indent.push_back(i==node_child.size()-1?"└─":"├─");
#endif
        node_child[i].print(depth+1,i==node_child.size()-1,indent);
        indent.pop_back();
    }
}

#endif