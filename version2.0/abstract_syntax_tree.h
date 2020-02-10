#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

class abstract_syntax_tree
{
	private:
		// basic elements
		int line;
		int symbol_number;
		bool symbol_is_global;
		int node_type;
		std::list<abstract_syntax_tree> children;

		// is this node is a number|string|identifier node
		// then the three elements below is of great use
		double var_number;
		std::string var_string;
		std::string var_name;
		// var_name is set for __id
	public:
		// basic
		abstract_syntax_tree();
		abstract_syntax_tree(const abstract_syntax_tree&);
		~abstract_syntax_tree();
		abstract_syntax_tree& operator=(const abstract_syntax_tree&);
		
		// main functions
		// print
		void print_tree();
		void print_tree_block(const int);

		// set
		void set_clear();
		void set_node_type(const int);
		void set_node_line(const int);
		void set_symbol_number(const int);
		void set_var_string(std::string);
		void set_var_number(std::string);
		void set_var_name(std::string);
		void set_var_scope(bool);// true means it exists in global scope
		void add_children(abstract_syntax_tree);

		// get
		int get_node_type();
		int get_node_line();
		int get_symbol_number();
		double get_var_number();
		std::string get_var_string();
		std::string get_var_name();
		bool get_var_scope();
		std::list<abstract_syntax_tree>& get_children();
};

abstract_syntax_tree::abstract_syntax_tree()
{
	node_type=__null_type;
	line=0;
	symbol_number=-1;
	symbol_is_global=false;
	var_number=0;
	var_string="";
	var_name="";
	children.clear();
	return;
}

abstract_syntax_tree::abstract_syntax_tree(const abstract_syntax_tree& tmp)
{
	node_type=tmp.node_type;
	line=tmp.line;
	symbol_number=tmp.symbol_number;
	symbol_is_global=tmp.symbol_is_global;
	var_number=tmp.var_number;
	var_string=tmp.var_string;
	var_name=tmp.var_name;
	children=tmp.children;
	return;
}

abstract_syntax_tree::~abstract_syntax_tree()
{
	node_type=__null_type;
	line=0;
	symbol_number=-1;
	symbol_is_global=false;
	var_number=0;
	var_string.clear();
	var_name.clear();
	children.clear();
	return;
}

abstract_syntax_tree& abstract_syntax_tree::operator=(const abstract_syntax_tree& tmp)
{
	node_type=tmp.node_type;
	line=tmp.line;
	symbol_number=tmp.symbol_number;
	symbol_is_global=tmp.symbol_is_global;
	var_number=tmp.var_number;
	var_string=tmp.var_string;
	var_name=tmp.var_name;
	children.clear();
	children=tmp.children;
	return *this;
}

void abstract_syntax_tree::print_tree()
{
	std::cout<<">> [Abstract-syntax-tree] get tree root: "<<(this)<<""<<std::endl;
	print_tree_block(1);
	return;
}

void abstract_syntax_tree::print_tree_block(const int n)
{
	std::string __str="";
	for(int i=0;i<n;++i)
		__str+="| ";
	std::cout<<__str;
	print_parse_token(node_type);
	switch(node_type)
	{
		case __number: std::cout<<": "<<var_number;break;
		case __string: std::cout<<": "<<var_string;break;
		case __id:
		case __dynamic_id:
		case __call_hash:
			std::cout<<": "<<var_name<<" (sym_num:"<<symbol_number<<"["<<(symbol_is_global? "global":"local")<<"])";
			break;
		case __call_vector:
		case __call_function:break;
		default:break;
	}
	std::cout<<std::endl;
	if(!children.empty())
	{
		for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
			i->print_tree_block(n+1);
	}
	return;
}

void abstract_syntax_tree::set_clear()
{
	node_type=__null_type;
	line=0;
	symbol_number=-1;
	var_number=0;
	var_string="";
	var_name="";
	children.clear();
	return;
}

void abstract_syntax_tree::set_node_type(const int __node_type)
{
	node_type=__node_type;
	return;
}

void abstract_syntax_tree::set_node_line(const int __line)
{
	if(__line>=0)
		line=__line;
	else
	{
		std::cout<<">> [Abstract-syntax-tree-warning] incorrect line under 0: "<<__line<<"."<<std::endl;
		line=0;
	}
	return;
}

void abstract_syntax_tree::set_symbol_number(const int __sym_num)
{
	symbol_number=__sym_num;
	return;
}

void abstract_syntax_tree::set_var_string(std::string __str)
{
	var_string=__str;
	return;
}

void abstract_syntax_tree::set_var_number(std::string __str)
{
	var_number=trans_string_to_number(__str);
	return;
}

void abstract_syntax_tree::set_var_name(std::string __str)
{
	var_name=__str;
	return;
}

void abstract_syntax_tree::set_var_scope(bool is_global)
{
	symbol_is_global=is_global;
	return;
}

void abstract_syntax_tree::add_children(abstract_syntax_tree p)
{
	// use abstract_syntax_tree instead of abstract_syntax_tree&
	// because when this function get a 'p' from returned value of
	// another function,this may result in s SIGSEGV.
	children.push_back(p);
	return;
}

int abstract_syntax_tree::get_node_type()
{
	return node_type;
}

int abstract_syntax_tree::get_node_line()
{
	return line;
}

int abstract_syntax_tree::get_symbol_number()
{
	return symbol_number;
}

double abstract_syntax_tree::get_var_number()
{
	return var_number;
}

std::string abstract_syntax_tree::get_var_string()
{
	return var_string;
}

std::string abstract_syntax_tree::get_var_name()
{
	return var_name;
}

bool abstract_syntax_tree::get_var_scope()
{
	return symbol_is_global;
}

std::list<abstract_syntax_tree>& abstract_syntax_tree::get_children()
{
	return children;
}
#endif
