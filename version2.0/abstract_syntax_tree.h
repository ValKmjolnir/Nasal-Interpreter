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
	line=__line;
	return;
}

void abstract_syntax_tree::set_string(std::string __str)
{
	str=__str;
	return;
}

void abstract_syntax_tree::set_number(std::string _str)
{
	bool is_negative=false;
	if(_str.length()>1 && _str[0]=='-')
	{
		// this statements only used in "input" function
		// but in parse this statements are useless
		// because lexer recognizes a number that begins with a '0'~'9' char
		std::string temp="";
		for(int i=1;i<_str.length();++i)
			temp+=_str[i];
		_str=temp;
		is_negative=true;
	}
	if((int)_str.length()>2 && (_str[1]=='x' || _str[1]=='o'))
	{
		double num=0;
		double pw=1;
		if(_str[1]=='x') // hex
			for(int i=(int)_str.length()-1;i>1;--i)
			{
				if('0'<=_str[i] && _str[i]<='9')
					num+=(_str[i]-'0')*pw;
				else if('a'<=_str[i] && _str[i]<='f')
					num+=(10+_str[i]-'a')*pw;
				else if('A'<=_str[i] && _str[i]<='F')
					num+=(10+_str[i]-'A')*pw;
				pw*=16;
			}
		else // oct
			for(int i=(int)_str.length()-1;i>1;--i)
			{
				num+=(_str[i]-'0')*pw;
				pw*=8;
			}
		number=num;
		if(is_negative)
			number*=-1;
		return;
	}
	int dot_place=-1;
	for(int i=0;i<(int)_str.length();++i)
		if(_str[i]=='.')
		{
			dot_place=i;
			break;
		}
	if(dot_place==-1)
	{
		// integer
		number=0;
		double pw=1;
		for(int i=(int)_str.length()-1;i>=0;--i)
		{
			number+=(_str[i]-'0')*pw;
			pw*=10;
		}
		if(is_negative)
			number*=-1;
	}
	else
	{
		// float
		number=0;
		double pw=0.1;
		for(int i=dot_place+1;i<(int)_str.length();++i)
		{
			number+=(_str[i]-'0')*pw;
			pw/=10;
		}
		pw=1;
		for(int i=dot_place-1;i>=0;--i)
		{
			number+=(_str[i]-'0')*pw;
			pw*=10;
		}
		if(is_negative)
			number*=-1;
	}
	return;
}

void abstract_syntax_tree::set_name(std::string __str)
{
	name=__str;
	return;
}

void abstract_syntax_tree::add_child(abstract_syntax_tree p)
{
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
