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
		std::list<std::list<var> > scope_list;
		var error_var;
	public:
		var_scope_manager()
		{
			scope_list.clear();
			std::string str="__nas_strc_lang_error_ret";
			error_var.set_name(str);
			error_var.set_type(__null_type);
			return;
		}
		void set_clear()
		{
			scope_list.clear();
			return;
		}
		var& search_var(std::string str)
		{
			std::list<std::list<var>>::iterator i=scope_list.end();
			--i;
			for(;;--i)
			{
				for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
					if(j->get_name()==str)
						return *j;
				if(i==scope_list.begin())
					break;
			}
			std::cout<<std::endl<<">>[Runtime-error] could not find the var '"<<str<<"' ."<<std::endl;
			return error_var;
		}
		void add_var(var new_var)
		{
			if(scope_list.empty())
			{
				std::cout<<std::endl<<">>[Runtime-error] empty scope list."<<std::endl;
				return;
			}
			std::list<std::list<var>>::iterator i=scope_list.end();
			--i;
			for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
			{
				if(j->get_name()==new_var.get_name())
				{
					std::cout<<std::endl<<">>[Runtime-error] redeclaration of var '"<<new_var.get_name()<<"' ."<<std::endl;
					return;
				}
			}
			i->push_back(new_var);
			return;
		}
		void add_new_scope()
		{
			std::list<var> new_list;
			scope_list.push_back(new_list);
			return;
		}
		void pop_last_scope()
		{
			if(!scope_list.empty())
				scope_list.pop_back();
			else
				std::cout<<std::endl<<">>[Runtime-error] scope poped empty thing."<<std::endl;
			return;
		}
};

class nasal_vm
{
	private:
		abstract_syntax_tree root;
		var_scope_manager scope;
	public:
		nasal_vm()
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
		
};
#endif
