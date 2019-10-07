#ifndef __NASAL_ABSTRACT_SYNTAX_TREE_RUN_H__
#define __NASAL_ABSTRACT_SYNTAX_TREE_RUN_H__

class abstract_syntax_tree
{
	protected:
		int ast_node_type;
		std::list<abstract_syntax_tree> statement_list;
		std::list<abstract_syntax_tree> children;
		double var_number;
		std::string var_string;
	public:
		abstract_syntax_tree()
		{
			ast_node_type=__root;
			statement_list.clear();
			children.clear();
			var_number=0;
			var_string="";
		}
		abstract_syntax_tree(const abstract_syntax_tree& temp)
		{
			ast_node_type=temp.ast_node_type;
			if(temp.statement_list.empty())
				statement_list.clear();
			else
				statement_list=temp.statement_list;
			if(temp.children.empty())
				children.clear();
			else
				children=temp.children;
			var_number=temp.var_number;
			var_string=temp.var_string;
		}
		// for statement block
		void set_block()
		{
			ast_node_type=__block;
			return;
		}
		void add_statement(abstract_syntax_tree statement_head)
		{
			statement_list.push_back(statement_head);
			return;
		}
		// for sub-tree node operator
		void set_two_operator(const int operator_type,abstract_syntax_tree f_child,abstract_syntax_tree s_child)
		{
			ast_node_type=operator_type;
			children.push_back(f_child);
			children.push_back(s_child);
			return;
		}
		void set_one_operator(const int operator_type,abstract_syntax_tree f_child)
		{
			ast_node_type=operator_type;
			children.push_back(f_child);
			return;
		}
		// for leaf node string
		void set_node_to_string(std::string& str)
		{
			ast_node_type=__string;
			var_string=str;
			return;
		}
		// for leaf node number
		void set_node_to_number(std::string& str)
		{
			ast_node_type=__number;
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
};


#endif
