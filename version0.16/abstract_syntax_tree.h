#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

#include "nasal_parser.h"

class ast_tree_node
{
	protected:
		int line;
		int type;
		std::list<ast_tree_node> children;
		double num;
	public:
		ast_tree_node()
		{
			line=0;
			type=0;
			children.clear();
		}
		void set_line(int _line)
		{
			line=_line;
		}
		void set_type(int _type)
		{
			type=_type;
		}
		int return_line()
		{
			return line;
		}
		int return_type()
		{
			return type;
		}
		int child_num()
		{
			int cnt=0;
			for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
				++cnt;
			return cnt;
		}
		std::list<ast_tree_node>& return_list()
		{
			return children;
		}
		void print_all_tree(int tabnum=0)
		{
			for(int i=0;i<tabnum;++i)
				std::cout<<" ";
			if(type==__number)
			{
				std::cout<<"[ "<<num<<" ]"<<std::endl;
				return;
			}
			std::cout<<"{ ";
			print_token(type);
			std::cout<<std::endl;
			for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
			{
				i->print_all_tree(tabnum+1);
			}
			for(int i=0;i<tabnum;++i)
				std::cout<<" ";
			std::cout<<"}"<<std::endl;
			return;
		}
		double run()
		{
			if(type==__number)
				return num;
			if(type==__add_operator)
			{
				double sum=0;
				std::list<ast_tree_node>::iterator i=children.begin();
				sum=i->run()+(++i)->run();
				return sum;
			}
			else if(type==__sub_operator)
			{
				double sum=0;
				std::list<ast_tree_node>::iterator i=children.begin();
				sum=i->run()-(++i)->run();
				return sum;
			}
			else
			{
				for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
					std::cout<<i->run()<<std::endl;
				return 0;
			}
			return 0;
		}
};

class ast_node:public ast_tree_node
{
	public:
		ast_node():ast_tree_node()
		{
			;
		}
};

class ast_leaf:public ast_tree_node
{
	public:
		ast_leaf():ast_tree_node()
		{
			;
		}
};


class operator_expr:public ast_node
{
	public:
		operator_expr():ast_node()
		{
			;
		}
};

class number_node:public ast_leaf
{
	public:
		number_node():ast_leaf()
		{
			num=0;
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
#endif
