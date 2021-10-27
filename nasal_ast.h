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
    uint32_t    _line;
    uint32_t    _type;
    double      _num;
    std::string _str;
    std::vector<nasal_ast> _child;
public:
    nasal_ast(const uint32_t l=0,const uint32_t t=ast_null):_line(l),_type(t){}
    nasal_ast(const nasal_ast&);
    nasal_ast(nasal_ast&&);
    void print(const int);
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
    inline const std::string& str()  const {return _str;}
    inline const std::vector<nasal_ast>& child() const {return _child;}
    inline std::vector<nasal_ast>& child(){return _child;}
};

nasal_ast::nasal_ast(const nasal_ast& tmp)
{
    _line=tmp._line;
    _type=tmp._type;
    _num =tmp._num;
    _str =tmp._str;
    _child=tmp._child;
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

void nasal_ast::print(const int depth)
{
    for(int i=0;i<depth;++i)
        std::cout<<"|  ";
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
    for(auto& i:_child)
        i.print(depth+1);
}

#endif