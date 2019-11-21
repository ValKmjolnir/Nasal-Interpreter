#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

class var;

class abstract_syntax_tree
{
	protected:
		int type;
		int line;
		double number;
		std::string str;
		std::string name;
		std::list<abstract_syntax_tree> children;
	public:
		abstract_syntax_tree()
		{
			type=0;
			line=0;
			number=0;
			str="";
			name="";
			children.clear();
			return;
		}
		abstract_syntax_tree(const abstract_syntax_tree& p)
		{
			type=p.type;
			line=p.line;
			number=p.number;
			str=p.str;
			name=p.name;
			children=p.children;
			return;
		}
		~abstract_syntax_tree()
		{
			children.clear();
			return;
		}
		abstract_syntax_tree& operator=(const abstract_syntax_tree& p)
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
		void set_clear()
		{
			type=0;
			line=0;
			number=0;
			str="";
			name="";
			children.clear();
			return;
		}
		void print_tree(const int n)
		{
			std::string _str="";
			for(int i=0;i<n;++i)
				_str+="| ";
			std::cout<<_str;
			print_detail_token(type);
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
		void set_type(const int _type)
		{
			type=_type;
			return;
		}
		void set_line(const int _line)
		{
			line=_line;
			return;
		}
		void set_string(std::string _str)
		{
			str=_str;
			return;
		}
		void set_number(std::string _str)
		{
			if(_str=="nil")
			{
				number=0;
				return;
			}
			if((int)_str.length()>2 && (_str[1]=='x' || _str[1]=='o'))
			{
				double num=0;
				double pw=1;
				if(_str[1]=='x')
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
				else
					for(int i=(int)_str.length()-1;i>1;--i)
					{
						num+=(_str[i]-'0')*pw;
						pw*=8;
					}
				number=num;
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
				number=0;
				double pw=1;
				for(int i=(int)_str.length()-1;i>=0;--i)
				{
					number+=(_str[i]-'0')*pw;
					pw*=10;
				}
			}
			else
			{
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
			}
			return;
		}
		void set_name(std::string _str)
		{
			name=_str;
			return;
		}
		void add_child(abstract_syntax_tree p)
		{
			children.push_back(p);
			return;
		}
		int get_type()
		{
			return type;
		}
		int get_line()
		{
			return line;
		}
		double get_number()
		{
			return number;
		}
		std::string get_string()
		{
			return str;
		}
		std::string get_name()
		{
			return name;
		}
		std::list<abstract_syntax_tree>& get_children()
		{
			return children;
		}
		var calculation();
		bool condition_check();
		var call_identifier();
		var* get_var_addr();
		var assignment();
		void run_root();
		int run_loop();
		int run_ifelse();
		var run_func(std::list<var>,var);
		int run_block();
};

#endif
