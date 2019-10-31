#ifndef __NASAL_VM_H__
#define __NASAL_VM_H__

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

class nasal_code_gen
{
	private:
		abstract_syntax_tree root;
	public:
		nasal_code_gen()
		{
			root.set_clear();
			return;
		}
		void set_root(abstract_syntax_tree& tree)
		{
			root.set_clear();
			root=tree;
			std::cout<<">>[Code] runtime got the ast-root: "<<((void *)&tree)<<"->"<<((void *)&root)<<"."<<std::endl;
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
#endif
