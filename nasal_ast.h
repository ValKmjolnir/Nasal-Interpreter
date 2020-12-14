#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

enum ast_node
{
    ast_null=0,ast_root,ast_block,
    ast_nil,ast_number,ast_string,ast_identifier,ast_function,ast_hash,ast_vector,
    ast_hashmember,ast_call,ast_call_hash,ast_call_vec,ast_call_func,ast_subvec,
    ast_args,ast_default_arg,ast_dynamic_id,
    ast_and,ast_or,
    ast_equal,ast_add_equal,ast_sub_equal,ast_mult_equal,ast_div_equal,ast_link_equal,
    ast_cmp_equal,ast_cmp_not_equal,
    ast_less_than,ast_less_equal,
    ast_greater_than,ast_greater_equal,
    ast_add,ast_sub,ast_mult,ast_div,ast_link,
    ast_unary_sub,ast_unary_not,
    ast_trinocular,
    ast_for,ast_forindex,ast_foreach,ast_while,ast_new_iter,
    ast_conditional,ast_if,ast_elsif,ast_else,
    ast_multi_id,ast_multi_scalar,
    ast_definition,ast_multi_assign,
    ast_continue,ast_break,ast_return
};

std::string ast_str(int type)
{
    std::string str;
    switch(type)
    {
        case ast_null:         str="null";break;
        case ast_root:         str="root";break;
        case ast_block:        str="block";break;
        case ast_nil:          str="nil";break;
        case ast_number:       str="number";break;
        case ast_string:       str="string";break;
        case ast_identifier:   str="id";break;
        case ast_function:     str="function";break;
        case ast_hash:         str="hash";break;
        case ast_vector:       str="vector";break;
        case ast_hashmember:   str="hashmember";break;
        case ast_call:         str="call";break;
        case ast_call_hash:    str="call_hash";break;
        case ast_call_vec:     str="call_vector";break;
        case ast_call_func:    str="call_func";break;
        case ast_subvec:       str="subvec";break;
        case ast_args:         str="arguments";break;
        case ast_default_arg:  str="default_arg";break;
        case ast_dynamic_id:   str="dynamic_id";break;
        case ast_and:          str="and";break;
        case ast_or:           str="or";break;
        case ast_equal:        str="=";break;
        case ast_add_equal:    str="+=";break;
        case ast_sub_equal:    str="-=";break;
        case ast_mult_equal:   str="*=";break;
        case ast_div_equal:    str="/=";break;
        case ast_link_equal:   str="~=";break;
        case ast_cmp_equal:    str="==";break;
        case ast_cmp_not_equal:str="!=";break;
        case ast_less_than:    str="<";break;
        case ast_less_equal:   str="<=";break;
        case ast_greater_than: str=">";break;
        case ast_greater_equal:str=">=";break;
        case ast_add:          str="+";break;
        case ast_sub:          str="-";break;
        case ast_mult:         str="*";break;
        case ast_div:          str="/";break;
        case ast_link:         str="~";break;
        case ast_unary_sub:    str="unary-";break;
        case ast_unary_not:    str="unary!";break;
        case ast_trinocular:   str="trinocular";break;
        case ast_for:          str="for";break;
        case ast_forindex:     str="forindex";break;
        case ast_foreach:      str="foreach";break;
        case ast_while:        str="while";break;
        case ast_new_iter:     str="new_iterator";break;
        case ast_conditional:  str="conditional";break;
        case ast_if:           str="if";break;
        case ast_elsif:        str="elsif";break;
        case ast_else:         str="else";break;
        case ast_multi_id:     str="multi_id";break;
        case ast_multi_scalar: str="multi_scalar";break;
        case ast_definition:   str="definition";break;
        case ast_multi_assign: str="multi_assignment";break;
        case ast_continue:     str="continue";break;
        case ast_break:        str="break";break;
        case ast_return:       str="return";break;
    }
    return str;
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
    indentation+=ast_str(this->type);
    std::cout<<indentation;
    if(this->type==ast_string || this->type==ast_identifier || this->type==ast_dynamic_id || this->type==ast_call_hash)
        std::cout<<":"<<this->str;
    else if(this->type==ast_number)
        std::cout<<":"<<this->num;
    std::cout<<std::endl;
    int child_size=this->children.size();
    for(int i=0;i<child_size;++i)
        this->children[i].print_ast(depth+1);
    return;
}

#endif