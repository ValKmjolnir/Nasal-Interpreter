#ifndef __AST_GENERATOR_H__
#define __AST_GENERATOR_H__

class ast_generator
{
	private:
		ast_tree_node root;
		std::stack<parse_unit> parse;
		std::stack<ast_tree_node> node_cache;
		bool can_run;
	public:
		ast_generator()
		{
			can_run=true;
		}
		bool number_expr_gen()
		{
			number_expr t;
			t.set_line(parse.top().line);
			t.set_number(parse.top().content);
			if(node_cache.empty())
			{
				node_cache.push(t);
				return true;
			}
			int type=node_cache.top().return_type();
			if((type==__add_operator || type==__sub_operator || type==__mul_operator || type==__div_operator || type==__link_operator) && node_cache.top().child_num()<=1)
			{
				node_cache.top().add_child(t);
				return true;
			}
			else
			{
				node_cache.push(t);
				return true;
			}
			return true;
		}
		bool string_expr_gen()
		{
			string_expr t;
			t.set_line(parse.top().line);
			std::string tstr=parse.top().content;
			std::string str="";
			for(int i=1;i<(int)tstr.length()-1;++i)
				str+=tstr[i];
			t.set_str(str);
			if(node_cache.empty())
			{
				node_cache.push(t);
				return true;
			}
			int type=node_cache.top().return_type();
			if((type==__add_operator || type==__sub_operator || type==__mul_operator || type==__div_operator || type==__link_operator) && node_cache.top().child_num()<2)
			{
				node_cache.top().add_child(t);
				return true;
			}
			else
			{
				node_cache.push(t);
				return true;
			}
			return true;
		}
		bool operator_expr_gen()
		{
			operator_expr t;
			t.set_line(parse.top().line);
			t.set_operator_type(parse.top().type);
			if(node_cache.empty())
			{
				if(t.return_type()==__add_operator || t.return_type()==__sub_operator)
				{
					number_expr nullnum;
					nullnum.set_line(parse.top().line);
					std::string strnum="0";
					nullnum.set_number(strnum);
					t.add_child(nullnum);
					node_cache.push(t);
					return true;
				}
				else
				{
					std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missed number or string."<<std::endl;
					return false;
				}
			}
			int cache_top_type=node_cache.top().return_type();
			if(cache_top_type==__number || cache_top_type==__string)
			{
				t.add_child(node_cache.top());
				node_cache.pop();
				node_cache.push(t);
				return true;
			}
			else if(cache_top_type==__add_operator || cache_top_type==__sub_operator || cache_top_type==__mul_operator || cache_top_type==__div_operator || cache_top_type==__link_operator)
			{
				t.add_child(node_cache.top());
				node_cache.pop();
				node_cache.push(t);
				return true;
			}
			return true;
		}
		void input_token_stack(std::stack<parse_unit>& temp)
		{
			parse=temp;
			return;
		}
		void gen_main_prog()
		{
			can_run=true;
			root.clear_tree();
			while(!parse.empty())
			{
				int type=parse.top().type;
				bool is_correct=false;
				switch(type)
				{
					case __number:is_correct=number_expr_gen();break;
					case __string:is_correct=string_expr_gen();break;
					case __add_operator:is_correct=operator_expr_gen();break;
					case __sub_operator:is_correct=operator_expr_gen();break;
					case __mul_operator:is_correct=operator_expr_gen();break;
					case __div_operator:is_correct=operator_expr_gen();break;
					case __link_operator:is_correct=operator_expr_gen();break;
					case __semi:is_correct=true;break;
					default:
						is_correct=false;
						std::cout<<">>[Error] parse error in line "<<parse.top().line<<":";
						print_token(parse.top().type);
						std::cout<<" at an incorrect place."<<std::endl;
						break;
				}
				if(!is_correct)
					can_run=false;
				parse.pop();
			}
			std::stack<ast_tree_node> temp_cache;
			while(!node_cache.empty())
			{
				temp_cache.push(node_cache.top());
				node_cache.pop();
			}
			while(!temp_cache.empty())
			{
				root.add_child(temp_cache.top());
				temp_cache.pop();
			}
			return;
		}
		void run()
		{
			if(can_run)
				root.print(0);
			else
				std::cout<<">>[Parse] Error(s) occurred,stop."<<std::endl;
			return;
		}
};

#endif
