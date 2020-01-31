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
        std::list<symbol_table_unit> global_symbol_dictionary;// used to save symbols appeared in global scope
        std::list<symbol_table_unit> local_symbol_dictionary; // used to save symbols appeared in local scopes
        std::list<symbol_table_unit> global_scope;            // global scope
        std::list<std::list<symbol_table_unit> > local_list;  // local scopes
        void symbol_table_block_generate(abstract_syntax_tree&);
        void global_scope_add_symbol(symbol_table_unit);
        void local_list_add_scope();
        void local_list_del_scope();
        void local_scope_add_symbol(symbol_table_unit);
        int search_symbol_id(symbol_table_unit);
    public:
        nasal_symbol_table();
        ~nasal_symbol_table();
        void set_scope_clear();
        void print_symbol_table();
        int get_error();
        void symbol_table_main_generate(abstract_syntax_tree&);
};

nasal_symbol_table::nasal_symbol_table()
{
    error=0;
    global_symbol_dictionary.clear();
    global_scope.clear();
    local_symbol_dictionary.clear();
    local_list.clear();
    return;
}

nasal_symbol_table::~nasal_symbol_table()
{
    error=0;
    global_symbol_dictionary.clear();
    global_scope.clear();
    local_symbol_dictionary.clear();
    local_list.clear();
    return;
}

void nasal_symbol_table::set_scope_clear()
{
    error=0;
    global_symbol_dictionary.clear();
    global_scope.clear();
    local_symbol_dictionary.clear();
    local_list.clear();
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
        // repeat symbol will get the same number
        if(i->symbol_name==tmp.symbol_name)
            return;
        symbol_number_gen=i->symbol_number;
    }
    tmp.symbol_number=symbol_number_gen+1;
    global_scope.push_back(tmp);
    global_symbol_dictionary.push_back(tmp);
    return;
}

void nasal_symbol_table::local_list_add_scope()
{
    std::list<symbol_table_unit> new_scope;
    local_list.push_back(new_scope);
    return;
}

void nasal_symbol_table::local_list_del_scope()
{
    if(local_list.empty())
    {
        ++error;
        std::cout<<">>[Symbol-error] fatal error: empty block_scope_list."<<std::endl;
    }
    else
        local_list.pop_back();
    return;
}

void nasal_symbol_table::local_scope_add_symbol(symbol_table_unit tmp)
{
    if(!local_list.empty())
    {
        int symbol_number_gen=0;
        // get last symbol number
        for(std::list<std::list<symbol_table_unit> >::iterator i= local_list.begin();i!=local_list.end();++i)
            if(!i->empty())
                for(std::list<symbol_table_unit>::iterator j=i->begin();j!=i->end();++j)
                {
                    // this conditional expression is used to avoid wrong use of this function
                    // repeat symbol will get the same number
                    ++i;
                    if(i==local_list.end())
                    {
                        --i;
                        if(j->symbol_name==tmp.symbol_name)
                            return;
                    }
                    else
                        --i;
                    symbol_number_gen=j->symbol_number;
                }
        tmp.symbol_number=symbol_number_gen+1;
        local_list.back().push_back(tmp);
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
    if(!local_list.empty())
        for(std::list<std::list<symbol_table_unit> >::iterator i=local_list.begin();i!=local_list.end();++i)
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
            i->get_children().front().set_symbol_number(this->search_symbol_id(tmp));
            i->get_children().front().set_var_scope(true);
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
    if(node.get_node_type()==__function)
    {
        this->local_list_add_scope();
        if(node.get_children().front().get_node_type()==__parameters)
            for(std::list<abstract_syntax_tree>::iterator i=node.get_children().front().get_children().begin();i!=node.get_children().front().get_children().end();++i)
            {
                if(i->get_node_type()==__id || i->get_node_type()==__dynamic_id)
                {
                    symbol_table_unit tmp;
                    tmp.symbol_line=i->get_node_line();
                    tmp.symbol_name=i->get_var_name();
                    this->local_scope_add_symbol(tmp);
                    i->set_symbol_number(this->search_symbol_id(tmp));
                    i->set_var_scope(false);
                }
                else
                {
                    symbol_table_unit tmp;
                    tmp.symbol_line=i->get_children().front().get_node_line();
                    tmp.symbol_name=i->get_children().front().get_var_name();
                    this->local_scope_add_symbol(tmp);
                    i->get_children().front().set_symbol_number(this->search_symbol_id(tmp));
                    i->get_children().front().set_var_scope(false);
                    this->symbol_table_block_generate(i->get_children().back());
                }
            }
        this->symbol_table_block_generate(node.get_children().back());
        this->local_list_del_scope();
    }
    else if(node.get_node_type()==__conditional)
    {
        for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
        {
            this->local_list_add_scope();
            if(i->get_node_type()==__if || i->get_node_type()==__elsif)
                this->symbol_table_block_generate(i->get_children().front());
            this->symbol_table_block_generate(i->get_children().back());
            this->local_list_del_scope();
        }
    }
    else if(node.get_node_type()==__normal_statement_block)
    {
        for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
        {
            if(i->get_node_type()==__definition)
            {
                symbol_table_unit tmp;
                tmp.symbol_line=i->get_children().front().get_node_line();
                tmp.symbol_name=i->get_children().front().get_var_name();
                this->local_scope_add_symbol(tmp);
                i->get_children().front().set_symbol_number(this->search_symbol_id(tmp));
                i->get_children().front().set_var_scope(false);
                this->symbol_table_block_generate(i->get_children().back());
            }
            else
                this->symbol_table_block_generate(*i);
        }
    }
    else if(node.get_node_type()==__id)
    {
        symbol_table_unit tmp;
        tmp.symbol_line=node.get_node_line();
        tmp.symbol_name=node.get_var_name();
        int id_symbol_number=this->search_symbol_id(tmp);
        node.set_symbol_number(id_symbol_number);
    }
    else if(node.get_node_type()==__hash)
    {
        this->local_list_add_scope();
        symbol_table_unit me;
        me.symbol_line=node.get_node_line();
        me.symbol_name="me";
        this->local_scope_add_symbol(me);
        for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
        {
        	if(i->get_node_type()==__hash_member)
        	{
        		symbol_table_unit tmp;
	            tmp.symbol_line=i->get_children().front().get_node_line();
	            if(i->get_children().front().get_node_type()==__id)
	                tmp.symbol_name=i->get_children().front().get_var_name();
	            else if(i->get_children().front().get_node_type()==__string)
	                tmp.symbol_name=i->get_children().front().get_var_string();
	            this->local_scope_add_symbol(tmp);
	            i->get_children().front().set_symbol_number(this->search_symbol_id(tmp));
	            i->get_children().front().set_var_scope(false);
	            this->symbol_table_block_generate(i->get_children().back());
			}
			else
				this->symbol_table_block_generate(*i);
        }
        this->local_list_del_scope();
    }
    else
    {
        if(!node.get_children().empty())
            for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
                this->symbol_table_block_generate(*i);
    }
    return;
}

#endif
