#ifndef __NASAL_SCALAR_H__
#define __NASAL_SCALAR_H__

class nasal_function
{
	private:
		std::map<std::string,int> local_scope;
		abstract_syntax_tree function_root;
	public:
		nasal_function()
		{
			local_scope.clear();
			function_root.set_clear();
			return;
		}
		~nasal_function()
		{
			local_scope.clear();
			function_root.set_clear();
			return;
		}
};

class nasal_scalar
{
	private:
		int type;
		std::string var_string;
		double var_number;
		std::vector<int> var_array;
		std::map<std::string,int> var_hash;
		nasal_function var_func;
	public:
		nasal_scalar()
		{
			type=scalar_nil;
			var_string="";
			var_number=0;
			var_array.clear();
			var_hash.clear();
			return;
		}
		nasal_scalar(const nasal_scalar& tmp)
		{
			type=tmp.type;
			var_string=tmp.var_string;
			var_number=tmp.var_number;
			var_array=tmp.var_array;
			var_hash=tmp.var_hash;
			return;
		}
		nasal_scalar& operator=(const nasal_scalar& tmp)
		{
			type=tmp.type;
			var_string=tmp.var_string;
			var_number=tmp.var_number;
			var_array=tmp.var_array;
			var_hash=tmp.var_hash;
			return *this;
		}
		void set_clear()
		{
			type=scalar_nil;
			var_string.clear();
			var_number=0;
			var_array.clear();
			var_hash.clear();
			return;
		}
		void set_type(const int tmp_type)
		{
			// scalar_function is the last enum in enum::scalar_type
			type=tmp_type>scalar_function? scalar_nil:tmp_type;
			return;
		}
		void set_number(const double tmp_number)
		{
			var_number=tmp_number;
			return;
		}
		void set_string(const std::string& tmp_str)
		{
			var_string=tmp_str;
			return;
		}
		void vector_add_new_member(const int addr)
		{
			var_array.push_back(addr);
			return;
		}
		void hash_add_new_member(const std::string member_name,const int addr)
		{
			if(var_hash.find(member_name)!=var_hash.end())
				std::cout<<">> [Runtime] "<<member_name<<" exists."<<std::endl;
			else
				var_hash[member_name]=addr;
			return;
		}
		int get_type()          {return type;}
		double get_number()     {return var_number;}
		std::string get_string(){return var_string;}
};

#endif
