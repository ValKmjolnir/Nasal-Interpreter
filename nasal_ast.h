#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

enum ast_node
{
    ast_null=0,ast_root,ast_block,
    ast_nil,ast_num,ast_str,ast_id,ast_func,ast_hash,ast_vec,
    ast_hashmember,ast_call,ast_call_hash,ast_call_vec,ast_call_func,ast_subvec,
    ast_args,ast_default_arg,ast_dynamic_id,
    ast_and,ast_or,
    ast_equal,ast_add_equal,ast_sub_equal,ast_mult_equal,ast_div_equal,ast_link_equal,
    ast_cmp_equal,ast_cmp_not_equal,
    ast_less,ast_leq,
    ast_grt,ast_geq,
    ast_add,ast_sub,ast_mult,ast_div,ast_link,
    ast_unary_sub,ast_unary_not,
    ast_trinocular,
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
        case ast_call_hash:    return "callh";
        case ast_call_vec:     return "callv";
        case ast_call_func:    return "callf";
        case ast_subvec:       return "subvec";
        case ast_args:         return "args";
        case ast_default_arg:  return "deflt_arg";
        case ast_dynamic_id:   return "dyn_id";
        case ast_and:          return "and";
        case ast_or:           return "or";
        case ast_equal:        return "=";
        case ast_add_equal:    return "+=";
        case ast_sub_equal:    return "-=";
        case ast_mult_equal:   return "*=";
        case ast_div_equal:    return "/=";
        case ast_link_equal:   return "~=";
        case ast_cmp_equal:    return "==";
        case ast_cmp_not_equal:return "!=";
        case ast_less:         return "<";
        case ast_leq:          return "<=";
        case ast_grt:          return ">";
        case ast_geq:          return ">=";
        case ast_add:          return "+";
        case ast_sub:          return "-";
        case ast_mult:         return "*";
        case ast_div:          return "/";
        case ast_link:         return "~";
        case ast_unary_sub:    return "unary-";
        case ast_unary_not:    return "unary!";
        case ast_trinocular:   return "trino";
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
    int line;
    int type;
    std::string str;
    double num;
    std::vector<nasal_ast> children;
public:
    nasal_ast(int,int);
    nasal_ast(const nasal_ast&);
    ~nasal_ast();
    nasal_ast& operator=(const nasal_ast&);
    void clear();
    void set_line(int);
    void set_type(int);
    void set_str(std::string&);
    void set_num(double);
    void add_child(nasal_ast);
    int         get_line();
    int         get_type();
    std::string get_str();
    double      get_num();
    std::vector<nasal_ast>& get_children();
    void print_ast(int);
};

nasal_ast::nasal_ast(int init_line=0,int init_type=ast_null)
{
    this->line=init_line;
    this->type=init_type;
    return;
}

nasal_ast::nasal_ast(const nasal_ast& tmp)
{
    this->line=tmp.line;
    this->type=tmp.type;
    this->str=tmp.str;
    this->num=tmp.num;
    this->children=tmp.children;
    return;
}

nasal_ast::~nasal_ast()
{
    this->children.clear();
    return;
}

nasal_ast& nasal_ast::operator=(const nasal_ast& tmp)
{
    this->line=tmp.line;
    this->type=tmp.type;
    this->str=tmp.str;
    this->num=tmp.num;
    this->children=tmp.children;
    return *this;
}

void nasal_ast::clear()
{
    this->line=0;
    this->str="";
    this->num=0;
    this->type=ast_null;
    this->children.clear();
    return;
}

void nasal_ast::set_line(int l)
{
    this->line=l;
    return;
}

void nasal_ast::set_type(int t)
{
    this->type=t;
    return;
}

void nasal_ast::set_str(std::string& s)
{
    this->str=s;
    return;
}

void nasal_ast::set_num(double n)
{
    this->num=n;
    return;
}

void nasal_ast::add_child(nasal_ast ast)
{
    children.push_back(ast);
    return;
}

int nasal_ast::get_line()
{
    return this->line;
}

int nasal_ast::get_type()
{
    return this->type;
}

std::string nasal_ast::get_str()
{
    return this->str;
}

double nasal_ast::get_num()
{
    return this->num;
}

std::vector<nasal_ast>& nasal_ast::get_children()
{
    return this->children;
}

void nasal_ast::print_ast(int depth)
{
    std::string indentation="";
    for(int i=0;i<depth;++i) indentation+="|  ";
    indentation+=ast_name(this->type);
    std::cout<<indentation;
    if(this->type==ast_str || this->type==ast_id || this->type==ast_dynamic_id || this->type==ast_call_hash)
        std::cout<<":"<<this->str;
    else if(this->type==ast_num)
        std::cout<<":"<<this->num;
    std::cout<<'\n';
    int child_size=this->children.size();
    for(int i=0;i<child_size;++i)
        this->children[i].print_ast(depth+1);
    return;
}

#endif