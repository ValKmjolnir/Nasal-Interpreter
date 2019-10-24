#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__
#include "ast.h"

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
		void set_type(const int var_type)
		{
			type=var_type;
			return;
		}
		void set_name(std::string& var_name_str)
		{
			name=var_name_str;
			return;
		}
		void set_number(const double var_number)
		{
			number=var_number;
			return;
		}
		void set_string(std::string& s)
		{
			str=s;
			return;
		}
		void set_function(abstract_syntax_tree& func)
		{
			function=func;
			return;
		}
		void set_list(std::list<var>& p)
		{
			var_list=p;
			return;
		}
		void append_list(var& p)
		{
			var_list.push_back(p);
			return;
		}
		void set_hash(std::list<var>& p)
		{
			var_hash=p;
			return;
		}
		void append_hash(var& p)
		{
			var_hash.push_back(p);
			return;
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
		std::list<var>& get_list()
		{
			return var_list;
		}
		std::list<var>& get_hash()
		{
			return var_hash;
		}
};

class var_list_manager
{
	private:
		std::list<var> var_list;
	public:
		var_list_manager()
		{
			var_list.clear();
			var null_var;
			var_list.push_back(null_var);
			return;
		}
		void add_var(var& tmp)
		{
			for(auto i=var_list.begin();i!=var_list.end();++i)
				if(i->get_name()==tmp.get_name())
				{
					std::cout<<">>[Runtime-error] redeclaration of '"<<tmp.get_name()<<"'."<<std::endl;
					return;
				}
			var_list.push_back(tmp);
			return;
		}
		void del_latest_var()
		{
			var_list.pop_back();
			return;
		}
		var& search_var(std::string& str)
		{
			for(auto i=var_list.begin();i!=var_list.end();++i)
				if(i->get_name()==str)
					return *i;
			std::cout<<">>[Runtime-error] '"<<str<<"' is not delclared in this scope."<<std::endl;
			return *var_list.begin();
		}
};

class runtime
{
	private:
		abstract_syntax_tree root;
	public:
		runtime()
		{
			root.set_clear();
			return;
		}
		void run_abstract_syntax_tree(abstract_syntax_tree& tmp_tree)
		{
			root.set_clear();
			root=tmp_tree;
			root.run_tree();
			return;
		}
};
#endif
