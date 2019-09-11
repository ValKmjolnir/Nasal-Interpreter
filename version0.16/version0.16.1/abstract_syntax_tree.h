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
		void set_line(int _line)
		{
			line=_line;
			return;
		}
		int return_type()
		{
			return type;
		}
		void add_child(ast_tree_node& new_child)
		{
			children.push_back(new_child);
			return;
		}
		void clear_tree()
		{
			line=0;
			type=__root;
			children.clear();
			
			num=0;
			fnum=0;
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
				std::cout<<"[";
				if(num==0 && fnum!=0)
					std::cout<<fnum;
				else if(num!=0 && fnum==0)
					std::cout<<num;
				else
					std::cout<<0;
				std::cout<<"]"<<std::endl;
				return;
			}
			else if(type==__string)
			{
				std::cout<<"["<<str<<"]"<<std::endl;
				return;
			}
			std::cout<<"{[";
			print_token(type);
			std::cout<<"]"<<std::endl;
			for(std::list<ast_tree_node>::iterator i=children.begin();i!=children.end();++i)
				i->print(tab_num+1);
			for(int i=0;i<tab_num;++i)
				std::cout<<" ";
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
		long long int return_num()
		{
			return num;
		}
		double return_fnum()
		{
			return fnum;
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
