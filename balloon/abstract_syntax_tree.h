#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

class abstract_syntax_tree
{
	private:
		int type;
		double var_number;
		std::string var_string;
		std::string var_name;
		std::list<abstract_syntax_tree> children;
	public:
		abstract_syntax_tree()
		{
			type=0;
			var_number=0;
			var_string="";
			var_name="";
			children.clear();
			return;
		}
		abstract_syntax_tree(const abstract_syntax_tree& p)
		{
			type=p.type;
			var_number=p.var_number;
			var_string=p.var_string;
			var_name=p.var_name;
			children=p.children;
			return;
		}
		abstract_syntax_tree& operator=(const abstract_syntax_tree& p)
		{
			type=p.type;
			var_number=p.var_number;
			var_string=p.var_string;
			var_name=p.var_name;
			children.clear();
			children=p.children;
			return *this;
		}
		void set_clear()
		{
			type=0;
			var_number=0;
			var_string="";
			var_name="";
			children.clear();
			return;
		}
		void print_tree(const int n)
		{
			std::string str="";
			for(int i=0;i<n;++i)
				str+="| ";
			std::cout<<str;
			print_detail_token(type);
			switch(type)
			{
				case __number:std::cout<<": "<<var_number;break;
				case __string:std::cout<<": "<<var_string;break;
				case __id:
				case __call_array:
				case __call_hash:
				case __call_function:std::cout<<": "<<var_name;break;
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
		void set_string(std::string str)
		{
			var_string=str;
			return;
		}
		void set_number(std::string str)
		{
			if(str=="nil")
			{
				var_number=0;
				return;
			}
			if((int)str.length()>2 && (str[1]=='x' || str[1]=='o'))
			{
				if(str[1]=='x')
				{
					int num=0;
					int pw=1;
					for(int i=(int)str.length()-1;i>1;--i)
					{
						if('0'<=str[i] && str[i]<='9')
							num+=(str[i]-'0')*pw;
						else if('a'<=str[i] && str[i]<='f')
							num+=(10+str[i]-'a')*pw;
						else if('A'<=str[i] && str[i]<='F')
							num+=(10+str[i]-'A')*pw;
						pw<<=4;
					}
					var_number=(double)num;
				}
				else
				{
					int num=0;
					int pw=1;
					for(int i=(int)str.length()-1;i>1;--i)
					{
						num+=(str[i]-'0')*pw;
						pw<<=3;
					}
					var_number=(double)num;
				}
				return;
			}
			int dot_place=-1;
			for(int i=0;i<(int)str.length();++i)
				if(str[i]=='.')
				{
					dot_place=i;
					break;
				}
			if(dot_place==-1)
			{
				var_number=0;
				double pw=1;
				for(int i=(int)str.length()-1;i>=0;--i)
				{
					var_number+=(str[i]-'0')*pw;
					pw*=10;
				}
			}
			else
			{
				var_number=0;
				double pw=0.1;
				for(int i=dot_place+1;i<(int)str.length();++i)
				{
					var_number+=(str[i]-'0')*pw;
					pw/=10;
				}
				pw=1;
				for(int i=dot_place-1;i>=0;--i)
				{
					var_number+=(str[i]-'0')*pw;
					pw*=10;
				}
			}
			return;
		}
		void set_name(std::string& str)
		{
			var_name=str;
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
		double get_number()
		{
			return var_number;
		}
		std::string get_string()
		{
			return var_string;
		}
		std::string get_name()
		{
			return var_name;
		}
		std::list<abstract_syntax_tree>& get_children()
		{
			return children;
		}
};

#endif
