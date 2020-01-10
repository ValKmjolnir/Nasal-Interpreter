#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

class abstract_syntax_tree
{
	private:
		int line;
		int type;
		double number;
		std::string str;
		std::string name;
		std::list<abstract_syntax_tree> children;
	public:
		// basic
		abstract_syntax_tree();
		abstract_syntax_tree(const abstract_syntax_tree&);
		~abstract_syntax_tree();
		abstract_syntax_tree& operator=(const abstract_syntax_tree&);
		
		// main functions
		void set_clear();
		void print_tree(const int);
		void set_type(const int);
		void set_line(const int);
		void set_string(std::string);
		void set_number(std::string);
		void set_name(std::string);
		void add_child(abstract_syntax_tree);
		int get_type();
		int get_line();
		double get_number();
		std::string get_string();
		std::string get_name();
		std::list<abstract_syntax_tree>& get_children();
};

abstract_syntax_tree::abstract_syntax_tree()
{
	type=0;
	line=0;
	number=0;
	str="";
	name="";
	children.clear();
	return;
}

abstract_syntax_tree::abstract_syntax_tree(const abstract_syntax_tree& p)
{
	type=p.type;
	line=p.line;
	number=p.number;
	str=p.str;
	name=p.name;
	children=p.children;
	return;
}

abstract_syntax_tree::~abstract_syntax_tree()
{
	children.clear();
	return;
}

abstract_syntax_tree& abstract_syntax_tree::operator=(const abstract_syntax_tree& p)
{
	type=p.type;
	line=p.line;
	number=p.number;
	str=p.str;
	name=p.name;
	children.clear();
	children=p.children;
	return *this;
}

void abstract_syntax_tree::set_clear()
{
	type=0;
	line=0;
	number=0;
	str="";
	name="";
	children.clear();
	return;
}

void abstract_syntax_tree::print_tree(const int n)
{
	std::string __str="";
	for(int i=0;i<n;++i)
		__str+="| ";
	std::cout<<__str;
	print_parse_token(type);
	switch(type)
	{
		case __number:std::cout<<": "<<number;break;
		case __string:std::cout<<": "<<str;break;
		case __id:
		case __dynamic_id:
		case __call_array:
		case __call_hash:
		case __call_function:std::cout<<": "<<name;break;
	}
	std::cout<<std::endl;
	if(!children.empty())
	{
		for(auto i=children.begin();i!=children.end();++i)
			i->print_tree(n+1);
	}
	return;
}

void abstract_syntax_tree::set_type(const int __type)
{
	type=__type;
	return;
}

void abstract_syntax_tree::set_line(const int __line)
{
	if(__line>=0)
		line=__line;
	else
		line=0;
	return;
}

void abstract_syntax_tree::set_string(std::string __str)
{
	str=__str;
	return;
}

void abstract_syntax_tree::set_number(std::string __str)
{
	number=trans_string_to_number(__str);
	return;
}

void abstract_syntax_tree::set_name(std::string __str)
{
	name=__str;
	return;
}

void abstract_syntax_tree::add_child(abstract_syntax_tree p)
{
	// use abstract_syntax_tree instead of abstract_syntax_tree&
	// because when this function get a 'p' from returned value of
	// another function,this may result in s SIGSEGV.
	children.push_back(p);
	return;
}

int abstract_syntax_tree::get_type()
{
	return type;
}

int abstract_syntax_tree::get_line()
{
	return line;
}

double abstract_syntax_tree::get_number()
{
	return number;
}

std::string abstract_syntax_tree::get_string()
{
	return str;
}

std::string abstract_syntax_tree::get_name()
{
	return name;
}

std::list<abstract_syntax_tree>& abstract_syntax_tree::get_children()
{
	return children;
}
#endif
