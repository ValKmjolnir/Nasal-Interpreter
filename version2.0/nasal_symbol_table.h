#ifndef __NASAL_SYMBOL_TABLE_H__
#define __NASAL_SYMBOL_TABLE_H__

struct symbol_table_unit
{
    std::string symbol_name;
    int symbol_number;
    int symbol_line;
    symbol_table_unit()
    {
        symbol_name="";
        symbol_number=0;
        symbol_line=-1;
        return;
    }
    symbol_table_unit(const symbol_table_unit& tmp)
    {
        symbol_name=tmp.symbol_name;
        symbol_number=tmp.symbol_number;
        symbol_line=tmp.symbol_line;
        return;
    }
    symbol_table_unit& operator=(const symbol_table_unit& tmp)
    {
        symbol_line=tmp.symbol_line;
        symbol_name=tmp.symbol_name;
        symbol_number=tmp.symbol_number;
        return *this;
    }
};

class nasal_symbol_table
{
    private:
        int error;
        std::list<symbol_table_unit> global_symbol_dictionary;
        std::list<symbol_table_unit> global_scope;
        std::list<symbol_table_unit> local_symbol_dictionary;
        std::list<std::list<symbol_table_unit> > block_list;
        void symbol_table_block_generate(abstract_syntax_tree&);
    public:
        nasal_symbol_table();
        ~nasal_symbol_table();
        void set_scope_clear();
        void print_symbol_table();
        void global_scope_add_symbol(symbol_table_unit);
        void block_list_add_scope();
        void block_list_del_scope();
        void block_scope_add_symbol(symbol_table_unit);
        int search_symbol_id(symbol_table_unit);
        int get_error();
        void symbol_table_main_generate(abstract_syntax_tree&);
};

nasal_symbol_table::nasal_symbol_table()
{
    error=0;
    global_symbol_dictionary.clear();
    global_scope.clear();
    local_symbol_dictionary.clear();
    block_list.clear();
    return;
}

nasal_symbol_table::~nasal_symbol_table()
{
    error=0;
    global_symbol_dictionary.clear();
    global_scope.clear();
    local_symbol_dictionary.clear();
    block_list.clear();
    return;
}

void nasal_symbol_table::set_scope_clear()
{
    error=0;
    global_symbol_dictionary.clear();
    global_scope.clear();
    local_symbol_dictionary.clear();
    block_list.clear();
    return;
}

void nasal_symbol_table::print_symbol_table()
{
    if(!global_symbol_dictionary.empty())
    {
        std::cout<<">>[Symbol] global scope:"<<std::endl;
        for(std::list<symbol_table_unit>::iterator i=global_symbol_dictionary.begin();i!=global_symbol_dictionary.end();++i)
            std::cout<<"  line "<<i->symbol_line<<": "<<i->symbol_name<<"|"<<i->symbol_number<<std::endl;
    }
    else
        std::cout<<">>[Symbol] empty global symbol list."<<std::endl;
    if(!local_symbol_dictionary.empty())
    {
        std::cout<<">>[Symbol] local scope:"<<std::endl;
        for(std::list<symbol_table_unit>::iterator i=local_symbol_dictionary.begin();i!=local_symbol_dictionary.end();++i)
            std::cout<<"  line "<<i->symbol_line<<": "<<i->symbol_name<<"|"<<i->symbol_number<<std::endl;
    }
    else
        std::cout<<">>[Symbol] empty local symbol list."<<std::endl;
    return;
}

void nasal_symbol_table::global_scope_add_symbol(symbol_table_unit tmp)
{
    int symbol_number_gen=0;
    for(std::list<symbol_table_unit>::iterator i=global_scope.begin();i!=global_scope.end();++i)
    {
        // this conditional expression is used to avoid wrong use of this function
        if(i->symbol_name==tmp.symbol_name)
            return;
        symbol_number_gen=i->symbol_number;
    }
    tmp.symbol_number=symbol_number_gen+1;
    global_scope.push_back(tmp);
    global_symbol_dictionary.push_back(tmp);
    return;
}

void nasal_symbol_table::block_list_add_scope()
{
    std::list<symbol_table_unit> new_scope;
    block_list.push_back(new_scope);
    return;
}

void nasal_symbol_table::block_list_del_scope()
{
    if(block_list.empty())
    {
        ++error;
        std::cout<<">>[Symbol-error] fatal error: empty block_scope_list."<<std::endl;
    }
    else
        block_list.pop_back();
    return;
}

void nasal_symbol_table::block_scope_add_symbol(symbol_table_unit tmp)
{
    if(!block_list.empty())
    {
        int symbol_number_gen=0;
        for(std::list<symbol_table_unit>::iterator i=block_list.back().begin();i!=block_list.back().end();++i)
        {
            // this conditional expression is used to avoid wrong use of this function
            if(i->symbol_name==tmp.symbol_name)
                return;
            symbol_number_gen=i->symbol_number;
        }
        tmp.symbol_number=symbol_number_gen+1;
        block_list.back().push_back(tmp);
        local_symbol_dictionary.push_back(tmp);
    }
    else
    {
        ++error;
        std::cout<<">>[Symbol-error] fatal error: empty block_scope_list."<<std::endl;
    }
    return;
}

int nasal_symbol_table::search_symbol_id(symbol_table_unit tmp)
{
    // in both global and local scope
    // symbol_number begins with 1
    int ret_number=-1;
    if(!block_list.empty())
        for(std::list<std::list<symbol_table_unit> >::iterator i=block_list.begin();i!=block_list.end();++i)
            for(std::list<symbol_table_unit>::iterator j=i->begin();j!=i->end();++j)
                if(j->symbol_name==tmp.symbol_name)
                    ret_number=j->symbol_number;
    if(ret_number==-1)
        for(std::list<symbol_table_unit>::iterator i=global_scope.begin();i!=global_scope.end();++i)
            if(i->symbol_name==tmp.symbol_name)
                ret_number=i->symbol_number;
    if(ret_number==-1)
    {
        ++error;
        std::cout<<">>[Symbol-error] line "<<tmp.symbol_line<<": cannot find var named \'"<<tmp.symbol_name<<"\' ."<<std::endl;
    }
    return ret_number;
}

int nasal_symbol_table::get_error()
{
    return error;
}

void nasal_symbol_table::symbol_table_main_generate(abstract_syntax_tree& root)
{
    this->set_scope_clear();
    for(std::list<abstract_syntax_tree>::iterator i=root.get_children().begin();i!=root.get_children().end();++i)
    {
        if(i->get_node_type()==__definition)
        {
            symbol_table_unit tmp;
            tmp.symbol_line=i->get_children().front().get_node_line();
            tmp.symbol_name=i->get_children().front().get_var_name();
            this->global_scope_add_symbol(tmp);
            this->symbol_table_block_generate(i->get_children().back());
        }
        else
            this->symbol_table_block_generate(*i);
    }
    std::cout<<">>[Symbol] symbol table generating complete. "<<error<<" error(s)."<<std::endl;
    return;
}

void nasal_symbol_table::symbol_table_block_generate(abstract_syntax_tree& node)
{
    return;
}

#endif