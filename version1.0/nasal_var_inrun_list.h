#ifndef __NASAL_VAR_INRUN_LIST_H__
#define __NASAL_VAR_INRUN_LIST_H__

struct var_list_unit
{
	std::string var_name;
	int num_place;
};

class nasal_var_map
{
	private:
		std::list<var_list_unit> var_list;
		int var_number;
	public:
		nasal_var_map()
		{
			var_list.clear();
			var_number=0;
		}
		void add_var(std::string& __str)
		{
			var_list_unit t;
			t.var_name=__str;
			for(std::list<var_list_unit>::iterator i=var_list.begin();i!=var_list.end();++i)
			{
				if(i->var_name==__str)
					return;
			}
			++var_number;
			t.num_place=var_number;
			var_list.push_back(t);
			return;
		}
		int return_last_var_num_place()
		{
			std::list<var_list_unit>::iterator i=var_list.end();
			--i;
			return i->num_place;
		}
		void print_all_var()
		{
			for(std::list<var_list_unit>::iterator i=var_list.begin();i!=var_list.end();++i)
				std::cout<<i->var_name<<": "<<i->num_place<<std::endl;
			return;
		}
};

struct block_list_unit
{
	var variable;
	int num_place;
};

class block_list
{
	private:
		std::list<block_list_unit> var_list;
		bool global;
	public:
		block_list(const bool is_global)
		{
			var_list.clear();
			global=is_global;
		}
		block_list(const block_list& temp)
		{
			var_list=temp.var_list;
			global=temp.global;
		}
		~block_list()
		{
			var_list.clear();
		}
		void block_add_var(var& varia,int number)
		{
			block_list_unit t;
			t.variable=varia;
			t.num_place=number;
			var_list.push_back(t);
			return;
		}
};

class general_in_run_list
{
	private:
		std::list<block_list> var_block_list;
	public:
		general_in_run_list()
		{
			var_block_list.clear();
		}
		~general_in_run_list()
		{
			var_block_list.clear();
		}
		void add_block(block_list temp)
		{
			var_block_list.push_back(temp);
			return;
		}
		void delete_last_block()
		{
			var_block_list.pop_back();
			return;
		}
};

#endif
