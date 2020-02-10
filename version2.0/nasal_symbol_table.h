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
        int global_number;
        int local_number;
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
        bool search_scope(symbol_table_unit);
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
    global_number=0;
    local_number=0;
    global_symbol_dictionary.clear();
    global_scope.clear();
    local_symbol_dictionary.clear();
    local_list.clear();
    return;
}

nasal_symbol_table::~nasal_symbol_table()
{
    error=0;
    global_number=0;
    local_number=0;
    global_symbol_dictionary.clear();
    global_scope.clear();
    local_symbol_dictionary.clear();
    local_list.clear();
    return;
}

void nasal_symbol_table::set_scope_clear()
{
    error=0;
    global_number=0;
    local_number=0;
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
        std::cout<<">> [Symbol] global scope:"<<std::endl;
        for(std::list<symbol_table_unit>::iterator i=global_symbol_dictionary.begin();i!=global_symbol_dictionary.end();++i)
            std::cout<<"  line "<<i->symbol_line<<": "<<i->symbol_name<<"|"<<i->symbol_number<<std::endl;
    }
    else
        std::cout<<">> [Symbol] empty global symbol list."<<std::endl;
    if(!local_symbol_dictionary.empty())
    {
        std::cout<<">>[Symbol] local scope:"<<std::endl;
        for(std::list<symbol_table_unit>::iterator i=local_symbol_dictionary.begin();i!=local_symbol_dictionary.end();++i)
            std::cout<<"  line "<<i->symbol_line<<": "<<i->symbol_name<<"|"<<i->symbol_number<<std::endl;
    }
    else
        std::cout<<">> [Symbol] empty local symbol list."<<std::endl;
    return;
}

void nasal_symbol_table::global_scope_add_symbol(symbol_table_unit tmp)
{
    // this conditional expression is used to avoid wrong use of this function
    // repeat symbol will get the same number
    for(std::list<symbol_table_unit>::iterator i=global_scope.begin();i!=global_scope.end();++i)
        if(i->symbol_name==tmp.symbol_name)
            return;
    ++global_number;
    tmp.symbol_number=global_number;
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
        std::cout<<">> [Symbol-error] fatal error: empty block_scope_list."<<std::endl;
    }
    else
        local_list.pop_back();
    
    if(local_list.empty())
        local_number=0;
    return;
}

void nasal_symbol_table::local_scope_add_symbol(symbol_table_unit tmp)
{
    if(!local_list.empty())
    {
        // if in the last scope there is a sym which symbol_name is the same as tmp.symbol_name
        // then the symbol will get the same number
        std::list<std::list<symbol_table_unit> >::iterator last_scope=local_list.end();--last_scope;
        for(std::list<std::list<symbol_table_unit> >::iterator i=local_list.begin();i!=local_list.end();++i)
                for(std::list<symbol_table_unit>::iterator j=i->begin();j!=i->end();++j)
                    if((i==last_scope) && (j->symbol_name==tmp.symbol_name))
                        return;
        ++local_number;
        tmp.symbol_number=local_number;
        local_list.back().push_back(tmp);
        local_symbol_dictionary.push_back(tmp);
    }
    else
    {
        ++error;
        std::cout<<">> [Symbol-error] fatal error: empty block_scope_list."<<std::endl;
    }
    return;
}

int nasal_symbol_table::search_symbol_id(symbol_table_unit tmp)
{
    // local scope
    for(std::list<std::list<symbol_table_unit> >::iterator i=local_list.begin();i!=local_list.end();++i)
        for(std::list<symbol_table_unit>::iterator j=i->begin();j!=i->end();++j)
            if(j->symbol_name==tmp.symbol_name)
                return j->symbol_number;
    // global scope
    for(std::list<symbol_table_unit>::iterator i=global_scope.begin();i!=global_scope.end();++i)
        if(i->symbol_name==tmp.symbol_name)
            return i->symbol_number;
    // error
    ++error;
    std::cout<<">> [Symbol-error] line "<<tmp.symbol_line<<": cannot find var named \'"<<tmp.symbol_name<<"\' ."<<std::endl;
    return -1;
}

bool nasal_symbol_table::search_scope(symbol_table_unit tmp)
{
	// this function must be used when search_symbol_id()!=-1
	// because if seach_symbol_id()==-1 it means it haven't found this symbol
	// true means global scope
	// false means local scope
    for(std::list<std::list<symbol_table_unit> >::iterator i=local_list.begin();i!=local_list.end();++i)
        for(std::list<symbol_table_unit>::iterator j=i->begin();j!=i->end();++j)
            if(j->symbol_name==tmp.symbol_name)
                return false;
    for(std::list<symbol_table_unit>::iterator i=global_scope.begin();i!=global_scope.end();++i)
        if(i->symbol_name==tmp.symbol_name)
            return true;
    return true;
}

int nasal_symbol_table::get_error()
{
    // god knows why i wrote this sh*t.
    return error;
}

void nasal_symbol_table::symbol_table_main_generate(abstract_syntax_tree& root)
{
    this->set_scope_clear();
    for(std::list<abstract_syntax_tree>::iterator i=root.get_children().begin();i!=root.get_children().end();++i)
    {
        // only definition will call this->global_scope_add_symbol(symbol_table_unit)
        if(i->get_node_type()==__definition)
        {
            symbol_table_unit tmp;
            tmp.symbol_line=i->get_children().front().get_node_line();
            tmp.symbol_name=i->get_children().front().get_var_name();
            this->global_scope_add_symbol(tmp);
            i->get_children().front().set_symbol_number(this->search_symbol_id(tmp));
            i->get_children().front().set_var_scope(this->search_scope(tmp));
            this->symbol_table_block_generate(i->get_children().back());
        }
        // when in main_generate loop,local scope is totally empty
        else
            this->symbol_table_block_generate(*i);
    }
    std::cout<<">> [Symbol] symbol table generating complete. "<<error<<" error(s)."<<std::endl;
    return;
}

void nasal_symbol_table::symbol_table_block_generate(abstract_syntax_tree& node)
{
    int node_type=node.get_node_type();
    if(node_type==__function)
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
                    i->set_var_scope(this->search_scope(tmp));
                }
                else
                {
                    symbol_table_unit tmp;
                    tmp.symbol_line=i->get_children().front().get_node_line();
                    tmp.symbol_name=i->get_children().front().get_var_name();
                    this->local_scope_add_symbol(tmp);
                    i->get_children().front().set_symbol_number(this->search_symbol_id(tmp));
                    i->get_children().front().set_var_scope(this->search_scope(tmp));
                    this->symbol_table_block_generate(i->get_children().back());
                }
            }
        this->symbol_table_block_generate(node.get_children().back());
        this->local_list_del_scope();
    }
    else if(node_type==__conditional)
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
    else if(node_type==__while)
    {
        for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
            this->symbol_table_block_generate(*i);
    }
    else if(node_type==__for || node_type==__foreach || node_type==__forindex)
    {
        this->local_list_add_scope();
        for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
        {
            if(i->get_node_type()==__definition)
            {
                symbol_table_unit tmp;
                tmp.symbol_line=i->get_children().front().get_node_line();
                tmp.symbol_name=i->get_children().front().get_var_name();
                this->local_scope_add_symbol(tmp);
                i->get_children().front().set_symbol_number(this->search_symbol_id(tmp));
                i->get_children().front().set_var_scope(this->search_scope(tmp));
                this->symbol_table_block_generate(i->get_children().back());
            }
            else
                this->symbol_table_block_generate(*i);
        }
        this->local_list_del_scope();
    }
    else if(node_type==__normal_statement_block)
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
                i->get_children().front().set_var_scope(this->search_scope(tmp));
                this->symbol_table_block_generate(i->get_children().back());
            }
            else
                this->symbol_table_block_generate(*i);
        }
    }
    else if((node_type==__id) || (node_type==__call_hash))
    {
        symbol_table_unit tmp;
        tmp.symbol_line=node.get_node_line();
        tmp.symbol_name=node.get_var_name();
        int id_symbol_number=this->search_symbol_id(tmp);
        node.set_symbol_number(id_symbol_number);
        // if this id does not exist in scopes
        // then search_scope will not be called
        if(id_symbol_number>0)
        	node.set_var_scope(this->search_scope(tmp));
        if(!node.get_children().empty())
            for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
                this->symbol_table_block_generate(*i);
    }
    else if(node_type==__hash)
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
	            i->get_children().front().set_var_scope(this->search_scope(tmp));
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
