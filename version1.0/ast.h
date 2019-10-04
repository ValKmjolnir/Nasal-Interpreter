#ifndef __NASAL_ABSTRACT_SYNTAX_TREE_RUN_H__
#define __NASAL_ABSTRACT_SYNTAX_TREE_RUN_H__

class abstract_syntax_tree
{
	protected:
		int ast_node_type;
		std::list<abstract_syntax_tree> statement_list;
		double var_number;
		std::string var_string;
	public:
		abstract_syntax_tree()
		{
			ast_node_type=__root;
			statement_list.clear();
			var_number=0;
			var_string="";
		}
		abstract_syntax_tree(const abstract_syntax_tree& temp)
		{
			ast_node_type=temp.ast_node_type;
			statement_list=temp.statement_list;
			var_number=temp.var_number;
			var_string=temp.var_string;
		}
		void set_node_to_string(std::string& str)
		{
			ast_node_type=__string;
			var_string=str;
			return;
		}
		void set_node_to_number(std::string& str)
		{
			ast_node_type=__number;
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
