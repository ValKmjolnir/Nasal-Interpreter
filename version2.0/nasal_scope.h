#ifndef __NASAL_SCOPE_H__
#define __NASAL_SCOPE_H__

struct symbol_table_unit
{
    std::string symbol_name;
    int symbol_number;
    int symbol_line;
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

class nasal_scope
{
    private:
        int error;
        std::list<symbol_table_unit> main_scope;
        std::list<std::list<symbol_table_unit> > block_list;
    public:
        nasal_scope();
        ~nasal_scope();
        void print_symbol_table();
        void set_scope_clear();
        void main_scope_add_symbol(symbol_table_unit);
        void block_list_add_scope();
        void block_list_del_scope();
        void block_scope_add_symbol(symbol_table_unit);
};

nasal_scope::nasal_scope()
{
    error=0;
    main_scope.clear();
    block_list.clear();
    return;
}

nasal_scope::~nasal_scope()
{
    error=0;
    main_scope.clear();
    block_list.clear();
    return;
}

void nasal_scope::print_symbol_table()
{
    std::cout<<"main scope:"<<std::endl;
    if(!main_scope.empty())
        for(std::list<symbol_table_unit>::iterator i=main_scope.begin();i!=main_scope.end();++i)
            std::cout<<"line "<<i->symbol_line<<": "<<i->symbol_name<<" |symbol number: "<<i->symbol_number<<'.'<<std::endl;
    else
        std::cout<<"empty main scope."<<std::endl;
    std::cout<<"block scope:"<<std::endl;
    if(!block_list.empty())
    {
        for(std::list<std::list<symbol_table_unit> >::iterator i=block_list.begin();i!=block_list.end();++i)
        {
            for(std::list<symbol_table_unit>::iterator j=i->begin();j!=i->end();++j)
                std::cout<<"line "<<j->symbol_line<<": "<<j->symbol_name<<" |symbol number: "<<j->symbol_number<<'.'<<std::endl;
        } 
    }
    else
        std::cout<<"empty block scope."<<std::endl;
    return;
}

void nasal_scope::set_scope_clear()
{
    error=0;
    main_scope.clear();
    block_list.clear();
    return;
}

void nasal_scope::main_scope_add_symbol(symbol_table_unit tmp)
{
    int symbol_number_gen=0;
    if(!main_scope.empty())
        symbol_number_gen=main_scope.back().symbol_number;
    tmp.symbol_number=symbol_number_gen+1;
    main_scope.push_back(tmp);
    return;
}

void nasal_scope::block_list_add_scope()
{
    std::list<symbol_table_unit> new_scope;
    block_list.push_back(new_scope);
    return;
}

void nasal_scope::block_list_del_scope()
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

void nasal_scope::block_scope_add_symbol(symbol_table_unit tmp)
{
    if(!block_list.empty())
    {
        int symbol_number_gen=0;
        if(!block_list.back().empty())
            symbol_number_gen=block_list.back().back().symbol_number;
        tmp.symbol_number=symbol_number_gen+1;
        block_list.back().push_back(tmp);
    }
    else
    {
        ++error;
        std::cout<<">>[Symbol-error] fatal error: empty block_scope_list."<<std::endl;
    }
    return;
}

#endif