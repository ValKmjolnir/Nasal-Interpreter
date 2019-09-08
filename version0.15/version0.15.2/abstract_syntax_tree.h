#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

#include "nasal_parser.h"

class ast_tree_node
{
	protected:
		int line;
		int type;
		std::list<ast_tree_node> children;
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
		virtual void print_all_tree()
		{
			if(type==__number)
			{
				std::cout<<"( num )";
				return;
			}
			std::cout<<"( ";
			print_token(type);
			for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
			{
				i->print_all_tree();
			}
			std::cout<<" )";
			return;
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
	private:
		long long int num;
		double fnum;
		bool is_float;
	public:
		number_node():ast_leaf()
		{
			num=0;
			fnum=0;
			is_float=false;
		}
		void set_number(std::string& str)
		{
			type=__number;
			is_float=false;
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
				long long int acc=1;
				for(int i=(int)str.length()-1;i>=0;--i)
				{
					num+=acc*((long long int)(str[i]-'0'));
					acc*=10;
				}
			}
			else
			{
				fnum=0;
				double acc=1;
				double aff=0.1;
				for(int i=DotPlace+1;i<(int)str.length();++i)
				{
					fnum+=aff*((double)(str[i]-'0'));
					aff*=0.1;
				}
				for(int i=DotPlace-1;i>=0;--i)
				{
					fnum+=acc*((double)(str[i]-'0'));
					acc*=10;
				}
			}
			return;
		}
};
#endif
