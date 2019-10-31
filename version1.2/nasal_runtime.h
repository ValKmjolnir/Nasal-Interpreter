#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class var
{
	private:
		int type;
		std::string name;
		double number;
		std::string str;
		std::list<var> var_list;
		std::list<var> var_hash;
		abstract_syntax_tree function;
	public:
		var()
		{
			type=0;
			name="";
			number=0;
			str="";
			function.set_clear();
		}
		var(const var& p)
		{
			type=p.type;
			name=p.name;
			number=p.number;
			str=p.str;
			function=p.function;
		}
		var& operator=(const var& p)
		{
			type=p.type;
			name=p.name;
			number=p.number;
			str=p.str;
			function=p.function;
			return *this;
		}
		void print_information()
		{
			std::cout<<"[ type: ";print_token(type);std::cout<<" ]";
			std::cout<<"[ name: "<<name<<" ]";
			std::cout<<"[ number: "<<number<<" ]";
			std::cout<<"[ string: "<<str<<" ]"<<std::endl;
			std::cout<<"[ function: ";
			function.print_tree(1);
			std::cout<<" ]"<<std::endl;
			return;
		}
		void print_detail()
		{
			switch(type)
			{
				case __function:
					std::cout<<std::endl<<">>[Runtime-error] function type cannot be printed."<<std::endl;
					break;
				case __number:
					std::cout<<number;
					break;
				case __string:
					for(int i=1;i<str.length()-1;++i)
					{
						if(str[i]!='\\')
							std::cout<<str[i];
						else
						{
							switch(str[i+1])
							{
								case 'n':std::cout<<"\n";++i;break;
								case 't':std::cout<<"\t";++i;break;
								case 'r':std::cout<<"\r";++i;break;
								case '\\':std::cout<<"\\";++i;break;
								case '\'':std::cout<<"\'";++i;break;
								case '\"':std::cout<<"\"";++i;break;
								default:std::cout<<"\\";break;
							}
						}
					}
					break;
			}
			return;
		}
		void set_type(const int var_type)
		{
			type=var_type;
			return;
		}
		void set_name(std::string var_name_str)
		{
			name=var_name_str;
			return;
		}
		void set_number(const double var_number)
		{
			number=var_number;
			return;
		}
		void set_string(std::string s)
		{
			str=s;
			return;
		}
		void set_function(abstract_syntax_tree func)
		{
			function=func;
			return;
		}
		void set_list(std::list<var> p)
		{
			var_list=p;
			return;
		}
		void append_list(var p)
		{
			var_list.push_back(p);
			return;
		}
		void set_hash(std::list<var> p)
		{
			var_hash=p;
			return;
		}
		void append_hash(var p)
		{
			var_hash.push_back(p);
			return;
		}
		int get_type()
		{
			return type;
		}
		std::string get_name()
		{
			return name;
		}
		double get_number()
		{
			return number;
		}
		std::string get_string()
		{
			return str;
		}
		abstract_syntax_tree get_function()
		{
			return function;
		}
		std::list<var> get_list()
		{
			return var_list;
		}
		std::list<var> get_hash()
		{
			return var_hash;
		}
};

class var_scope_manager
{
	private:
		std::list<std::list<var>> scope_list;
		std::list<int> scope_type;
		var error_var;
	public:
		var_scope_manager()
		{
			scope_list.clear();
			std::string str="__nas_strc_error_ret";
			error_var.set_name(str);
			error_var.set_type(__null_type);
			return;
		}
		void set_clear()
		{
			scope_list.clear();
			scope_type.clear();
			return;
		}
		var& search_var(std::string str)
		{
			for(std::list<std::list<var>>::iterator i=scope_list.begin();i!=scope_list.end();++i)
				for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
					if(j->get_name()==str)
						return *j;
			std::cout<<std::endl<<">>[Runtime-error] could not find the var '"<<str<<"' ."<<std::endl;
			return error_var;
		}
		void add_var(var new_var)
		{
			std::list<std::list<var>>::iterator i=scope_list.begin();
			std::list<int>::iterator t=scope_type.begin();
			// get global scopes
			for(;i!=scope_list.end();++i,++t)
			{
				if(*t!=__function)
				{
					for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
						if(j->get_name()==new_var.get_name())
						{
							std::cout<<std::endl<<">>[Runtime-error] redeclaration of var '"<<new_var.get_name()<<"' ."<<std::endl;
							return;
						}
				}
			}
			// get parameters_list scope
			i=scope_list.end();
			--i;
			t=scope_type.end();
			--t;
			if(*t==__function)
			{
				for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
					if(j->get_name()==new_var.get_name())
					{
						std::cout<<std::endl<<">>[Runtime-error] redeclaration of var '"<<new_var.get_name()<<"' ."<<std::endl;
						return;
					}
			}
			if(!scope_list.empty())
			{
				i=scope_list.end();
				--i;
				i->push_back(new_var);
			}
			else
				std::cout<<std::endl<<">>[Runtime-error] empty scope list."<<std::endl;
			return;
		}
		void add_new_scope(int type)
		{
			std::list<var> new_list;
			scope_list.push_back(new_list);
			scope_type.push_back(type);
			return;
		}
		void pop_last_scope()
		{
			if(!scope_list.empty())
			{
				scope_list.pop_back();
				scope_type.pop_back();
			}
			else
				std::cout<<std::endl<<">>[Runtime-error] scope poped empty thing."<<std::endl;
			return;
		}
};

class nasal_runtime
{
	private:
		abstract_syntax_tree root;
		var_scope_manager scope;
	public:
		nasal_runtime()
		{
			root.set_clear();
			return;
		}
		void run()
		{
			std::cout<<">>[Runtime] process begins."<<std::endl;
			int time_beg,time_end;
			time_beg=time(NULL);
			scope.set_clear();
			run_root(root);
			time_end=time(NULL);
			std::cout<<std::endl<<">>[Runtime] process exited after "<<time_beg-time_end<<" sec(s)."<<std::endl;
			return;
		}
		void set_root(abstract_syntax_tree& tree)
		{
			root.set_clear();
			root=tree;
			std::cout<<">>[Runtime] runtime got the ast-root: "<<((void *)&tree)<<"->"<<((void *)&root)<<"."<<std::endl;
			return;
		}
		void run_definition(abstract_syntax_tree& tree);
		void run_assignment(abstract_syntax_tree& tree);
		void run_loop(abstract_syntax_tree& tree);
		void run_if_else(abstract_syntax_tree& tree);
		void run_block(abstract_syntax_tree& tree,int run_type);
		void run_root(abstract_syntax_tree& tree);
		var run_calculation(abstract_syntax_tree& tree);
		var run_function(abstract_syntax_tree& tree);
		var list_generation(abstract_syntax_tree& tree);
		var hash_generation(abstract_syntax_tree& tree);
		var list_search(abstract_syntax_tree& tree);
		var hash_search(abstract_syntax_tree& tree);
		var identifier_call(abstract_syntax_tree& tree);
		var scalar_call(abstract_syntax_tree& tree);
};

void nasal_runtime::run_root(abstract_syntax_tree& tree)
{
	scope.add_new_scope(__root);
	if(!tree.get_children().empty())
	{
		for(std::list<abstract_syntax_tree>::iterator i=tree.get_children().begin();i!=tree.get_children().end();++i)
		{
			switch(i->get_type())
			{
				case __add_operator:
				case __sub_operator:
				case __mul_operator:
				case __div_operator:
				case __link_operator:
				case __and_operator:
				case __or_operator:
				case __nor_operator:
				case __cmp_equal:
				case __cmp_not_equal:
				case __cmp_less:
				case __cmp_less_or_equal:
				case __cmp_more:
				case __cmp_more_or_equal:
					run_calculation(*i);
					break;
				case __definition:
					run_definition(*i);
					break;
				case __assignment:
					run_assignment(*i);
					break;
				case __while:
				case __for:
				case __forindex:
				case __foreach:
					run_loop(*i);
					break;
				case __ifelse:
					run_if_else(*i);
					break;
				case __call_function:
					run_function(*i);
					break;
				case __id:
				case __hash_search:
				case __list_search:
					identifier_call(*i);
					break;
				case __number:
				case __string:
					scalar_call(*i);
					break;
				default:
					std::cout<<">>[Debug] error occurred."<<std::endl;
					scope.pop_last_scope();
					return;
					break;
			}
		}
	}
	scope.pop_last_scope();
	return;
}

void nasal_runtime::run_definition(abstract_syntax_tree& tree)
{
	var new_var;
	std::list<abstract_syntax_tree>::iterator iter=tree.get_children().begin();
	std::string var_name=iter->get_var_name();
	++iter;
	if(iter==tree.get_children().end())
	{
		new_var.set_type(__null_type);
		new_var.set_name(var_name);
		scope.add_var(new_var);
		return;
	}
	else
	{
		switch(iter->get_type())
		{
			case __add_operator:
			case __sub_operator:
			case __mul_operator:
			case __div_operator:
			case __link_operator:
			case __and_operator:
			case __or_operator:
			case __nor_operator:
			case __cmp_equal:
			case __cmp_not_equal:
			case __cmp_less:
			case __cmp_less_or_equal:
			case __cmp_more:
			case __cmp_more_or_equal:
				new_var=run_calculation(*iter);
				break;
			case __call_function:
				new_var=run_function(*iter);
				break;
			case __id:
			case __hash_search:
			case __list_search:
				break;
			case __number:
			case __string:
				new_var=scalar_call(*iter);
				break;
			case __function:
				new_var.set_type(__function);
				new_var.set_function(*iter);
				break;
			case __list:
				break;
			case __hash:
				break;
			default:
				std::cout<<">>[Debug] error occurred."<<std::endl;
				return;
				break;
		}
		new_var.set_name(var_name);
		scope.add_var(new_var);
	}
	return;
}
void nasal_runtime::run_assignment(abstract_syntax_tree& tree)
{
	return;
}
void nasal_runtime::run_loop(abstract_syntax_tree& tree)
{
	return;
}
void nasal_runtime::run_if_else(abstract_syntax_tree& tree)
{
	return;
}
void nasal_runtime::run_block(abstract_syntax_tree& tree,int run_type)
{
	scope.add_new_scope(run_type);
	
	scope.pop_last_scope();
	return;
}

var nasal_runtime::run_calculation(abstract_syntax_tree& tree)
{
	var ret_var;
	var left_child;
	var right_child;
	
	std::list<abstract_syntax_tree>::iterator i;
	i=tree.get_children().begin();
	switch(i->get_type())
	{
		case __id:left_child=identifier_call(*i);break;
		case __number:
		case __string:left_child=scalar_call(*i);break;
		case __call_function:left_child=run_function(*i);break;
		case __list_search:left_child=list_search(*i);break;
		case __hash_search:left_child=hash_search(*i);break;
		case __or_operator:
		case __and_operator:
		case __add_operator:
		case __mul_operator:
		case __div_operator:
		case __link_operator:
		case __nor_operator:
		case __sub_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_less_or_equal:
		case __cmp_more:
		case __cmp_more_or_equal:left_child=run_calculation(*i);break;
		default:std::cout<<">>[Runtime-error] invalid var."<<std::endl;break;
	}
	++i;
	if(i==tree.get_children().end())
		return left_child;
	switch(i->get_type())
	{
		case __id:right_child=identifier_call(*i);break;
		case __number:
		case __string:right_child=scalar_call(*i);break;
		case __call_function:right_child=run_function(*i);break;
		case __list_search:right_child=list_search(*i);break;
		case __hash_search:right_child=hash_search(*i);break;
		case __or_operator:
		case __and_operator:
		case __add_operator:
		case __mul_operator:
		case __div_operator:
		case __link_operator:
		case __nor_operator:
		case __sub_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_less_or_equal:
		case __cmp_more:
		case __cmp_more_or_equal:right_child=run_calculation(*i);break;
		default:std::cout<<">>[Runtime-error] invalid var."<<std::endl;break;
	}
	switch(tree.get_type())
	{
		case __or_operator:
		case __and_operator:
			break;
		case __add_operator:
		case __mul_operator:
		case __div_operator:
		case __link_operator:
			break;
		case __nor_operator:
		case __sub_operator:
			break;
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_less_or_equal:
		case __cmp_more:
		case __cmp_more_or_equal:
			break;
	}
	return ret_var;
}
var nasal_runtime::run_function(abstract_syntax_tree& tree)
{
	var ret_var;
	scope.add_new_scope(__function);
	if(tree.get_var_name()=="print")
	{
		std::list<var> dynamic_para;
		var list_member;
		for(std::list<abstract_syntax_tree>::iterator i=tree.get_children().begin();i!=tree.get_children().end();++i)
		{
			switch(i->get_type())
			{
				case __id:list_member=identifier_call(*i);break;
				case __number:
				case __string:list_member=scalar_call(*i);break;
				case __call_function:list_member=run_function(*i);break;
				case __list_search:list_member=list_search(*i);break;
				case __hash_search:list_member=hash_search(*i);break;
				case __or_operator:
				case __and_operator:
				case __add_operator:
				case __mul_operator:
				case __div_operator:
				case __link_operator:
				case __nor_operator:
				case __sub_operator:
				case __cmp_equal:
				case __cmp_not_equal:
				case __cmp_less:
				case __cmp_less_or_equal:
				case __cmp_more:
				case __cmp_more_or_equal:list_member=run_calculation(*i);break;
				default:std::cout<<">>[Runtime-error] invalid var."<<std::endl;break;
			}
			dynamic_para.push_back(list_member);
		}
		for(std::list<var>::iterator i=dynamic_para.begin();i!=dynamic_para.end();++i)
			i->print_detail();
	}
	scope.pop_last_scope();
	return ret_var;
}
var nasal_runtime::list_generation(abstract_syntax_tree& tree)
{
	var new_list;
	new_list.set_type(__list);
	return new_list;
}
var nasal_runtime::hash_generation(abstract_syntax_tree& tree)
{
	var new_hash;
	new_hash.set_type(__hash);
	return new_hash;
}
var nasal_runtime::list_search(abstract_syntax_tree& tree)
{
	var ret_var;
	return ret_var;
}
var nasal_runtime::hash_search(abstract_syntax_tree& tree)
{
	var ret_var;
	return ret_var;
}
var nasal_runtime::identifier_call(abstract_syntax_tree& tree)
{
	var ret_var;
	ret_var=scope.search_var(tree.get_var_name());
	if(ret_var.get_type()==__null_type)
		return ret_var;
	if(ret_var.get_type()==__number)
		std::cout<<ret_var.get_number()<<std::endl;
	else if(ret_var.get_type()==__string)
		std::cout<<ret_var.get_string()<<std::endl;
	return ret_var;
}
var nasal_runtime::scalar_call(abstract_syntax_tree& tree)
{
	var ret_var;
	if(tree.get_type()==__number)
	{
		ret_var.set_type(__number);
		ret_var.set_number(tree.get_var_number());
	}
	else if(tree.get_type()==__string)
	{
		ret_var.set_type(__string);
		ret_var.set_string(tree.get_var_string());
	}
	return ret_var;
}
#endif
