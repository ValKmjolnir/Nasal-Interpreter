#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

class ast_tree_node
{
	protected:
		int line;
		int type;
		std::list<ast_tree_node> children;// for node that can be extended
		
		long long int num; // for number
		double fnum;       // for number
		std::string str;   // for string
	public:
		ast_tree_node()
		{
			line=0;
			type=__root;
			children.clear();
			
			num=0;
			fnum=0;
			str="";
		}
		void add_child(ast_tree_node& new_child)
		{
			children.push_back(new_child);
			return;
		}
		void print()
		{
			if(type==__number)
			{
				std::cout<<"["<<num<<"]"<<std::endl;
				return;
			}
			else if(type==__string)
			{
				std::cout<<"["<<str<<"]"<<std::endl;
				return;
			}

			if(type==__root)
			{
				std::cout<<"{[";
				print_token(type);
				std::cout<<"]"<<std::endl;
			}
			for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
				i->print();
			std::cout<<"}"<<std::endl;
			return;
		}
};
class operator_expr:public ast_tree_node
{
	public:
		operator_expr():ast_tree_node()
		{
			type=__operator;
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
		void setnum(long long int n)
		{
			num=n;
			return;
		}
		void setfnum(double n)
		{
			fnum=n;
			return;
		}
		long long int return_num()
		{
			return num;
		}
		double return_fnum()
		{
			return fnum;
		}
};
class string_expr:public ast_tree_node
{
	public:
		string_expr():ast_tree_node()
		{
			type=__string;
		}
		void setstr(std::string& t)
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
