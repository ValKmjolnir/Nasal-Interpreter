#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

enum ast_node
{
    ast_null=0,
    ast_root,ast_block,
    ast_file, // ast_file is only used to store which file the subtree is on,codegen will generate nothing
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
    ast_def,ast_multi_assign,
    ast_continue,ast_break,ast_ret
};

const char* ast_name[]=
{
    "null",
    "root","block",
    "file",
    "nil","num","str","id","func","hash","vec",
    "hashmember","call","callh","callv","callf","subvec",
    "args","deflt_arg","dyn_id",
    "and","or",
    "=","+=","-=","*=","/=","~=",
    "==","!=",
    "<","<=",
    ">",">=",
    "+","-","*","/","~",
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
    int         line;
    int         type;
    double      num;
    std::string str;
    std::vector<nasal_ast> children;
public:
    nasal_ast(){line=0;type=ast_null;}
    nasal_ast(int l,int t){line=l;type=t;}
    nasal_ast(const nasal_ast&);
    nasal_ast(nasal_ast&&);
    nasal_ast& operator=(const nasal_ast&);
    nasal_ast& operator=(nasal_ast&&);
    void   print_ast(int);
    void   clear();
    void   add_child(nasal_ast&& ast){children.push_back(std::move(ast));}
    void   set_line(int l){line=l;}
    void   set_type(int t){type=t;}
    void   set_str(std::string& s){str=s;}
    void   set_num(double n){num=n;}
    int    get_line(){return line;}
    int    get_type(){return type;}
    double get_num() {return num;}
    std::string& get_str(){return str;}
    std::vector<nasal_ast>& get_children(){return children;}

    int    get_line() const {return line;}
    int    get_type() const {return type;}
    double get_num()  const {return num;}
    const std::string& get_str() const {return str;}
    const std::vector<nasal_ast>& get_children() const {return children;}
};

nasal_ast::nasal_ast(const nasal_ast& tmp)
{
    line=tmp.line;
    type=tmp.type;
    num =tmp.num;
    str =tmp.str;
    children=tmp.children;
    return;
}

nasal_ast::nasal_ast(nasal_ast&& tmp)
{
    line=tmp.line;
    type=tmp.type;
    num =tmp.num;
    str.swap(tmp.str);
    children.swap(tmp.children);
    return;
}

nasal_ast& nasal_ast::operator=(const nasal_ast& tmp)
{
    line=tmp.line;
    type=tmp.type;
    num=tmp.num;
    str=tmp.str;
    children=tmp.children;
    return *this;
}

nasal_ast& nasal_ast::operator=(nasal_ast&& tmp)
{
    line=tmp.line;
    type=tmp.type;
    num=tmp.num;
    str.swap(tmp.str);
    children.swap(tmp.children);
    return *this;
}

void nasal_ast::clear()
{
    line=0;
    num=0;
    str="";
    type=ast_null;
    children.clear();
    return;
}

void nasal_ast::print_ast(int depth)
{
    for(int i=0;i<depth;++i)
        std::cout<<"|  ";
    std::cout<<ast_name[type];
    if(type==ast_str || type==ast_id || type==ast_default_arg || type==ast_dynamic_id || type==ast_callh)
    {
        std::cout<<":";
        raw_string(str);
    }
    else if(type==ast_num || type==ast_file)
        std::cout<<":"<<num;
    std::cout<<'\n';
    for(auto& i:children)
        i.print_ast(depth+1);
    return;
}

#endif