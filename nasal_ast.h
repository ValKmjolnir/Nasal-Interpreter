#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

enum ast_node
{
    ast_null=0,
    ast_root,
    ast_block,
    ast_nil,ast_num,ast_str,ast_id,ast_func,ast_hash,ast_vec,
    ast_hashmember,ast_call,ast_callh,ast_callv,ast_callf,ast_subvec,
    ast_args,ast_default_arg,ast_dynamic_id,
    ast_and,ast_or,
    ast_equal,ast_addeq,ast_subeq,ast_multeq,ast_diveq,ast_lnkeq,
    ast_cmpeq,ast_neq,
    ast_less,ast_leq,
    ast_grt,ast_geq,
    ast_add,ast_sub,ast_mult,ast_div,ast_link,
    ast_neg,ast_not,
    ast_trino,
    ast_for,ast_forindex,ast_foreach,ast_while,ast_new_iter,
    ast_conditional,ast_if,ast_elsif,ast_else,
    ast_multi_id,ast_multi_scalar,
    ast_definition,ast_multi_assign,
    ast_continue,ast_break,ast_return
};

std::string ast_name(int type)
{
    switch(type)
    {
        case ast_null:         return "null";
        case ast_root:         return "root";
        case ast_block:        return "block";
        case ast_nil:          return "nil";
        case ast_num:          return "number";
        case ast_str:          return "string";
        case ast_id:           return "id";
        case ast_func:         return "function";
        case ast_hash:         return "hash";
        case ast_vec:          return "vector";
        case ast_hashmember:   return "hashmember";
        case ast_call:         return "call";
        case ast_callh:        return "callh";
        case ast_callv:        return "callv";
        case ast_callf:        return "callf";
        case ast_subvec:       return "subvec";
        case ast_args:         return "args";
        case ast_default_arg:  return "deflt_arg";
        case ast_dynamic_id:   return "dyn_id";
        case ast_and:          return "and";
        case ast_or:           return "or";
        case ast_equal:        return "=";
        case ast_addeq:        return "+=";
        case ast_subeq:        return "-=";
        case ast_multeq:       return "*=";
        case ast_diveq:        return "/=";
        case ast_lnkeq:        return "~=";
        case ast_cmpeq:        return "==";
        case ast_neq:          return "!=";
        case ast_less:         return "<";
        case ast_leq:          return "<=";
        case ast_grt:          return ">";
        case ast_geq:          return ">=";
        case ast_add:          return "+";
        case ast_sub:          return "-";
        case ast_mult:         return "*";
        case ast_div:          return "/";
        case ast_link:         return "~";
        case ast_neg:          return "unary-";
        case ast_not:          return "unary!";
        case ast_trino:        return "trino";
        case ast_for:          return "for";
        case ast_forindex:     return "forindex";
        case ast_foreach:      return "foreach";
        case ast_while:        return "while";
        case ast_new_iter:     return "iter";
        case ast_conditional:  return "conditional";
        case ast_if:           return "if";
        case ast_elsif:        return "elsif";
        case ast_else:         return "else";
        case ast_multi_id:     return "multi_id";
        case ast_multi_scalar: return "multi_scalar";
        case ast_definition:   return "def";
        case ast_multi_assign: return "multi_assign";
        case ast_continue:     return "continue";
        case ast_break:        return "break";
        case ast_return:       return "return";
    }
    return "null";
}

class nasal_ast
{
private:
    int         line;
    int         type;
    double      num;
    std::string str;
    std::vector<nasal_ast> children;
public:
    nasal_ast();
    nasal_ast(int,int);
    nasal_ast(const nasal_ast&);
    ~nasal_ast();
    nasal_ast&  operator=(const nasal_ast&);
    void        print_ast(int);
    void        clear();
    void        set_line(int);
    void        set_type(int);
    void        set_str(std::string&);
    void        set_num(double);
    void        add_child(nasal_ast);
    int         get_line();
    int         get_type();
    double      get_num();
    std::string get_str();
    std::vector<nasal_ast>& get_children();
};

nasal_ast::nasal_ast()
{
    line=0;
    type=ast_null;
    return;
}

nasal_ast::nasal_ast(int _line,int _type)
{
    line=_line;
    type=_type;
    return;
}

nasal_ast::nasal_ast(const nasal_ast& tmp)
{
    line=tmp.line;
    type=tmp.type;
    str =tmp.str;
    num =tmp.num;
    children=tmp.children;
    return;
}

nasal_ast::~nasal_ast()
{
    children.clear();
    return;
}

nasal_ast& nasal_ast::operator=(const nasal_ast& tmp)
{
    line=tmp.line;
    type=tmp.type;
    str=tmp.str;
    num=tmp.num;
    children=tmp.children;
    return *this;
}

void nasal_ast::clear()
{
    line=0;
    str="";
    num=0;
    type=ast_null;
    children.clear();
    return;
}

void nasal_ast::set_line(int l)
{
    line=l;
    return;
}

void nasal_ast::set_type(int t)
{
    type=t;
    return;
}

void nasal_ast::set_str(std::string& s)
{
    str=s;
    return;
}

void nasal_ast::set_num(double n)
{
    num=n;
    return;
}

void nasal_ast::add_child(nasal_ast ast)
{
    children.push_back(ast);
    return;
}

int nasal_ast::get_line()
{
    return line;
}

int nasal_ast::get_type()
{
    return type;
}

std::string nasal_ast::get_str()
{
    return str;
}

double nasal_ast::get_num()
{
    return num;
}

std::vector<nasal_ast>& nasal_ast::get_children()
{
    return children;
}

void nasal_ast::print_ast(int depth)
{
    std::string indentation="";
    for(int i=0;i<depth;++i)
        indentation+="|  ";
    std::cout<<indentation<<ast_name(type);
    if(type==ast_str || type==ast_id || type==ast_dynamic_id || type==ast_callh)
        std::cout<<":"<<str;
    else if(type==ast_num)
        std::cout<<":"<<num;
    std::cout<<'\n';
    int child_size=children.size();
    for(int i=0;i<child_size;++i)
        children[i].print_ast(depth+1);
    return;
}

#endif