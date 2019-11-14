#ifndef __BALLOON_VAR_H__
#define __BALLOON_VAR_H__

enum var_type
{
	__null_type,
	__var_number,
	__var_string,
	__var_array,
	__var_hash,
	__var_function
};

void print_scalar(int type)
{
	std::string str="";
	switch(type)
	{
		case __null_type:   str="null";break;
		case __var_number:  str="number";break;
		case __var_string:  str="string";break;
		case __var_array:   str="array";break;
		case __var_hash:    str="hash";break;
		case __var_function:str="function";break;
		default:            str="unknown";break;
	}
	std::cout<<str;
	return;
}

class var
{
	private:
		int type;
		double number;
		std::string str;
		std::string name;
		std::list<var> balloon_array;
		std::list<var> balloon_hash;
		abstract_syntax_tree function;
	public:
		var()
		{
			type=__null_type;
			number=0;
			str="";
			name="";
			balloon_array.clear();
			balloon_hash.clear();
			function.set_clear();
			return;
		}
		var(const var& p)
		{
			type=p.type;
			number=p.number;
			str=p.str;
			name=p.name;
			balloon_array=p.balloon_array;
			balloon_hash=p.balloon_hash;
			function=p.function;
			return;
		}
		var& operator=(const var& p)
		{
			type=p.type;
			number=p.number;
			str=p.str;
			name=p.name;
			balloon_array.clear();
			balloon_hash.clear();
			function.set_clear();
			balloon_array=p.balloon_array;
			balloon_hash=p.balloon_hash;
			function=p.function;
			return *this;
		}
		void print_var()
		{
			std::cout<<"[type: ";
			print_scalar(type);
			std::cout<<",name: "<<name<<",number: "<<number<<",string: "<<str<<" ]";
			return;
		}
		void set_type(int);
		void set_name(std::string);
		void set_number(double);
		void set_string(std::string);
		void set_function(abstract_syntax_tree&);
		void append_array(var);
		void append_hash(var);
		int get_type();
		std::string get_name();
		double get_number();
		std::string get_string();
		var get_array_member(int);
		var* get_array_member_addr(int);
		var get_hash_member(std::string);
		var* get_hash_member_addr(std::string);
		abstract_syntax_tree& get_function();
};

var error_var;

void var::set_type(int _type)
{
	type=_type;
	return;
}

void var::set_name(std::string _name)
{
	name=_name;
	return;
}

void var::set_number(double _num)
{
	number=_num;
	return;
}

void var::set_string(std::string _str)
{
	str=_str;
	return;
}

void var::set_function(abstract_syntax_tree& p)
{
	function=p;
	return;
}

void var::append_array(var _new_var)
{
	balloon_array.push_back(_new_var);
	return;
}

void var::append_hash(var _new_var)
{
	balloon_hash.push_back(_new_var);
	return;
}

int var::get_type()
{
	return type;
}

std::string var::get_name()
{
	return name;
}

double var::get_number()
{
	return number;
}

std::string var::get_string()
{
	return str;
}

var var::get_array_member(int _place)
{
	if(balloon_array.empty())
	{
		std::cout<<">>[Runtime-error] overflow when searching member "<<_place<<" but the max size of array \'"<<name<<"\' is 0."<<std::endl;
		return error_var;
	}
	int cnt=0;
	for(std::list<var>::iterator i=balloon_array.begin();i!=balloon_array.end();++i,++cnt)
		if(cnt==_place)
			return *i;
	std::cout<<">>[Runtime-error] overflow when searching member "<<_place<<" but the max size of array \'"<<name<<"\' is "<<cnt<<"(this means 0-"<<cnt-1<<")."<<std::endl;
	return error_var;
}

var* var::get_array_member_addr(int _place)
{
	var* addr=NULL;
	if(balloon_array.empty())
	{
		std::cout<<">>[Runtime-error] overflow when searching member "<<_place<<" but the max size of array \'"<<name<<"\' is 0."<<std::endl;
		return &error_var;
	}
	int cnt=0;
	for(std::list<var>::iterator i=balloon_array.begin();i!=balloon_array.end();++i,++cnt)
		if(cnt==_place)
		{
			addr=&(*i);
			return addr;
		}
	std::cout<<">>[Runtime-error] overflow when searching member "<<_place<<" but the max size of array \'"<<name<<"\' is "<<cnt<<"(this means 0-"<<cnt-1<<")."<<std::endl;
	return &error_var;
}

var var::get_hash_member(std::string _name)
{
	for(std::list<var>::iterator i=balloon_hash.begin();i!=balloon_hash.end();++i)
		if(i->name==_name)
			return *i;
	std::cout<<">>[Runtime-error] hash \'"<<name<<"\' does not have a member named \'"<<_name<<"\'"<<std::endl;
	return error_var;
}

var* var::get_hash_member_addr(std::string _name)
{
	var* addr=NULL;
	for(std::list<var>::iterator i=balloon_hash.begin();i!=balloon_hash.end();++i)
		if(i->name==_name)
		{
			addr=&(*i);
			return addr;
		}
	std::cout<<">>[Runtime-error] hash \'"<<name<<"\' does not have a member named \'"<<_name<<"\'"<<std::endl;
	return &error_var;
}

abstract_syntax_tree& var::get_function()
{
	return function;
}
#endif
