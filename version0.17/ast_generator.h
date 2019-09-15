#ifndef __AST_GENERATOR_H__
#define __AST_GENERATOR_H__

class ast_generator
{
	private:
		ast_tree_node root;
		std::stack<parse_unit> parse;
		std::stack<ast_tree_node> node_cache;
		bool error;
	public:
		ast_generator()
		{
			error=false;
		}
		void input_token_stack(std::stack<parse_unit>& temp)
		{
			parse=temp;
			return;
		}
		void number_gen()
		{
			number_expr t;
			t.set_line(parse.top().line);
			t.set_number(parse.top().content);
			if(node_cache.empty())
				node_cache.push(t);
			else if((node_cache.top().return_type()==__add_operator || node_cache.top().return_type()==__sub_operator || node_cache.top().return_type()==__mul_operator || node_cache.top().return_type()==__div_operator)
					&& (node_cache.top().child_num()==1))
				node_cache.top().add_child(t);
			else
			{
				std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing \';\' at this place."<<std::endl;
				error=true;
			}
			return;
		}
		void add_sub_operator_gen()
		{
			operator_expr opt;
			opt.set_line(parse.top().line);
			opt.set_operator_type(parse.top().type);
			if(node_cache.empty())
			{
				number_expr t;
				t.set_line(parse.top().line);
				std::string temp_str="0";
				t.set_number(temp_str);
				opt.add_child(t);
				node_cache.push(opt);
			}
			else if(node_cache.top().return_type()==__number
					|| ((node_cache.top().return_type()==__add_operator
					|| node_cache.top().return_type()==__sub_operator
					|| node_cache.top().return_type()==__mul_operator
					|| node_cache.top().return_type()==__div_operator)
					&& node_cache.top().child_num()==2))
			{
				opt.add_child(node_cache.top());
				node_cache.pop();
				node_cache.push(opt);
			}
			else
			{
				std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number/calculation at this place."<<std::endl;
				error=true;
			}
			return;
		}
		void mul_div_operator_gen()
		{
			operator_expr opt;
			opt.set_line(parse.top().line);
			opt.set_operator_type(parse.top().type);
			if(node_cache.empty())
			{
				std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number at this place(you cannot use * or / as the begin of calculation)."<<std::endl;
				error=true;
				return;
			}
				
			if(node_cache.top().return_type()==__number)
			{
				// add new leaf node number to operator
				opt.add_child(node_cache.top());
				node_cache.pop();
				node_cache.push(opt);
			}
			else if(node_cache.top().return_type()==__add_operator || node_cache.top().return_type()==__sub_operator)
			{
				if(node_cache.top().child_num()!=2)
				{
					std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number at this place."<<std::endl;
					error=true;
				}
				else
				{
					// calcu * and / first so pick a number from +-'s children nodes
					opt.add_child(node_cache.top().return_last_child());
					parse.pop();
					// add the next number to fix the operator node's children list
					if(!parse.empty() && parse.top().type==__number)
					{
						number_expr t;
						t.set_line(parse.top().line);
						t.set_number(parse.top().content);
						opt.add_child(t);
					}
					else
					{
						std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number at this place."<<std::endl;
						error=true;
						return;
					}
					node_cache.top().return_last_child()=opt;
				}
			}
			else if(node_cache.top().return_type()==__mul_operator || node_cache.top().return_type()==__div_operator)
			{
				if(node_cache.top().child_num()!=2)
				{
					std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number at this place."<<std::endl;
					error=true;
				}
				else
				{
					opt.add_child(node_cache.top());
					node_cache.pop();
					node_cache.push(opt);
				}
			}
			return;
		}
		void gen_main_prog()
		{
			error=false;
			root.clear_tree();
			// initializing
			
			while(!node_cache.empty())
				node_cache.pop();
			while(!parse.empty())
			{
				int type=parse.top().type;
				bool is_correct=false;
				switch(type)
				{
					case __number:number_gen();break;
					case __add_operator:add_sub_operator_gen();break;
					case __sub_operator:add_sub_operator_gen();break;
					case __mul_operator:mul_div_operator_gen();break;
					case __div_operator:mul_div_operator_gen();break;
					case __semi:
						if(!node_cache.empty())
						{
							root.add_child(node_cache.top());
							node_cache.pop();
						}
						break;
					default:
						std::cout<<">>[Error] parse error in line "<<parse.top().line<<":";
						print_token(parse.top().type);
						std::cout<<" at an incorrect place."<<std::endl;
						break;
				}
				parse.pop();
			}
			if(error)
			{
				std::cout<<">>[Parse] Error(s) occurred."<<std::endl;
				std::cout<<">>[Parse] Complete parsing."<<std::endl;
			}
			else
			{
				std::cout<<">>[Parse] 0 error."<<std::endl;
				std::cout<<">>[Parse] Complete parsing."<<std::endl;
			}
			return;
		}
		void print_ast()
		{
			std::cout<<">>[Abstract-syntax-tree]"<<std::endl;
			root.print(0);
			return;
		}
		void run()
		{
			if(!error)
				root.run();
			else
				std::cout<<">>[Parse] Error(s) occurred,stop running the code."<<std::endl;
			return;
		}
};

#endif
