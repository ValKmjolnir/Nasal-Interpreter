#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

class ast_tree_node
{
	protected:
		int line;
		int type;
		std::list<ast_tree_node> children;// for node that can be extended
		
		double num;       // for number
		std::string str;   // for string
	public:
		ast_tree_node()
		{
			line=0;
			type=__root;
			children.clear();
			
			num=0;
			str="";
		}
		void set_line(int _line)
		{
			line=_line;
			return;
		}
		int return_type()
		{
			return type;
		}
		int return_line()
		{
			return line;
		}
		double run()
		{
			if(type==__number)
				return num;
			else if(type==__root)
			{
				std::cout<<">>[Result(s)]"<<std::endl;
				for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
					std::cout<<i->run()<<std::endl;
				return 1;
			}
			else
			{
				std::list<ast_tree_node>::iterator i=children.begin();
				double left_child=i->run();
				++i;
				double right_child=i->run();
				switch(type)
				{
					case __add_operator:return left_child+right_child;break;
					case __sub_operator:return left_child-right_child;break;
					case __mul_operator:return left_child*right_child;break;
					case __div_operator:return left_child/right_child;break;
				}
			}
			return 0;
		}
		void add_child(ast_tree_node& new_child)
		{
			children.push_back(new_child);
			return;
		}
		ast_tree_node& return_last_child()
		{
			std::list<ast_tree_node>::iterator i;
			for(i=children.begin();i!=children.end();++i)
				;
			--i;
			return *i;
		}
		void clear_tree()
		{
			line=0;
			type=__root;
			children.clear();
			
			num=0;
			str="";
			return;
		}
		int child_num()
		{
			int cnt=0;
			for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
				++cnt;
			return cnt;
		}
		void print(int tab_num)
		{
			for(int i=0;i<tab_num;++i)
				std::cout<<" ";
			if(type==__number)
			{
				std::cout<<"[ number:";
				std::cout<<num;
				std::cout<<" ]"<<std::endl;
				return;
			}
			else if(type==__string)
			{
				std::cout<<"[ string:"<<str<<" ]"<<std::endl;
				return;
			}
			std::cout<<"{ [ node type:";
			print_token(type);
			std::cout<<" ]"<<std::endl;
			for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
				i->print(tab_num+1);
			for(int i=0;i<tab_num;++i)
				std::cout<<" ";
			std::cout<<"}"<<std::endl;
			return;
		}
};

class struct_node_expr:public ast_tree_node
{
	public:
		struct_node_expr():ast_tree_node()
		{
			type=__root;
		}
		void set_type(int _type)
		{
			type=_type;
			return;
		}
};

class operator_expr:public ast_tree_node
{
	public:
		operator_expr():ast_tree_node()
		{
			type=__add_operator;
		}
		void set_operator_type(int oprt_type)
		{
			type=oprt_type;
			return;
		}
};
class number_expr:public ast_tree_node
{
	public:
		number_expr():ast_tree_node()
		{
			type=__number;
		}
		double return_num()
		{
			return num;
		}
		void set_number(std::string& str)
		{
			type=__number;
			double is_float=false;
			int DotPlace=0;
			for(int i=0;i<(int)str.length();++i)
				if(str[i]=='.')
				{
					is_float=true;
					DotPlace=i;
					break;
				}
			if(!is_float)
			{
				num=0;
				double acc=1;
				for(int i=(int)str.length()-1;i>=0;--i)
				{
					num+=acc*((double)(str[i]-'0'));
					acc*=10;
				}
			}
			else
			{
				num=0;
				double acc=1;
				double aff=0.1;
				for(int i=DotPlace+1;i<(int)str.length();++i)
				{
					num+=aff*((double)(str[i]-'0'));
					aff*=0.1;
				}
				for(int i=DotPlace-1;i>=0;--i)
				{
					num+=acc*((double)(str[i]-'0'));
					acc*=10;
				}
			}
			return;
		}
};
class string_expr:public ast_tree_node
{
	public:
		string_expr():ast_tree_node()
		{
			type=__string;
		}
		void set_str(std::string& t)
		{
			str=t;
			return;
		}
		std::string& return_str()
		{
			return str;
		}
};

#endif
