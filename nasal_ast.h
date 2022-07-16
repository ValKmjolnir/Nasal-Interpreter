#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

enum ast_node
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
    "hashmember",
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
    "return",
    nullptr
};

class nasal_ast
{
private:
    uint32_t    _line;
    uint32_t    _type;
    double      _num;
    std::string _str;
    std::vector<nasal_ast> _child;
public:
    nasal_ast(const uint32_t l=0,const uint32_t t=ast_null):_line(l),_type(t),_num(0){}
    nasal_ast(const nasal_ast&);
    nasal_ast(nasal_ast&&);
    void print(int,bool);
    void clear();
    
    nasal_ast& operator=(const nasal_ast&);
    nasal_ast& operator=(nasal_ast&&);
    nasal_ast& operator[](const int index){return _child[index];}
    const nasal_ast& operator[](const int index) const {return _child[index];}
    size_t size() const {return _child.size();}
    
    void add(nasal_ast&& ast){_child.push_back(std::move(ast));}
    void add(const nasal_ast& ast){_child.push_back(ast);}
    void set_line(const uint32_t l){_line=l;}
    void set_type(const uint32_t t){_type=t;}
    void set_str(const std::string& s){_str=s;}
    void set_num(const double n){_num=n;}

    inline uint32_t line() const {return _line;}
    inline uint32_t type() const {return _type;}
    inline double   num()  const {return _num;}
    inline const std::string& str() const {return _str;}
    inline const std::vector<nasal_ast>& child() const {return _child;}
    inline std::vector<nasal_ast>& child(){return _child;}
};

nasal_ast::nasal_ast(const nasal_ast& tmp):
    _str(tmp._str),_child(tmp._child)
{
    _line=tmp._line;
    _type=tmp._type;
    _num =tmp._num;
}

nasal_ast::nasal_ast(nasal_ast&& tmp)
{
    _line=tmp._line;
    _type=tmp._type;
    _num =tmp._num;
    _str.swap(tmp._str);
    _child.swap(tmp._child);
}

nasal_ast& nasal_ast::operator=(const nasal_ast& tmp)
{
    _line=tmp._line;
    _type=tmp._type;
    _num=tmp._num;
    _str=tmp._str;
    _child=tmp._child;
    return *this;
}

nasal_ast& nasal_ast::operator=(nasal_ast&& tmp)
{
    _line=tmp._line;
    _type=tmp._type;
    _num=tmp._num;
    _str.swap(tmp._str);
    _child.swap(tmp._child);
    return *this;
}

void nasal_ast::clear()
{
    _line=0;
    _num=0;
    _str="";
    _type=ast_null;
    _child.clear();
}

void nasal_ast::print(int depth,bool last=false)
{
    static std::vector<std::string> intentation={};
    for(auto& i:intentation)
        std::cout<<i;
    std::cout<<ast_name[_type];
    if(
        _type==ast_str ||
        _type==ast_id ||
        _type==ast_default ||
        _type==ast_dynamic ||
        _type==ast_callh)
        std::cout<<":"<<rawstr(_str);
    else if(_type==ast_num || _type==ast_file)
        std::cout<<":"<<_num;
    std::cout<<'\n';
    if(last && depth)
        intentation.back()="  ";
    else if(!last && depth)
#ifdef _WIN32
        intentation.back()="| ";
#else
        intentation.back()="│ ";
#endif
    for(uint32_t i=0;i<_child.size();++i)
    {
#ifdef _WIN32
        intentation.push_back(i==_child.size()-1?"`-":"|-");
#else
        intentation.push_back(i==_child.size()-1?"└─":"├─");
#endif
        _child[i].print(depth+1,i==_child.size()-1);
        intentation.pop_back();
    }
}

#endif