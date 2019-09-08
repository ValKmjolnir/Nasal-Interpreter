#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include "abstract_syntax_tree.h"

class code_generator
{
	private:
		ast_node root;
		std::stack<parse_unit> parse;
		std::stack<ast_tree_node> node_stack;
	public:
		code_generator()
		{
			root.set_type(__root);
		}
		void number_generator()
		{
			number_node t;
			t.set_line(parse.top().line);
			t.set_number(parse.top().content);
			
			if(node_stack.empty())
			{
				node_stack.push(t);
				return;
			}
			int top_type=node_stack.top().return_type();
			if((top_type==__add_operator || top_type==__sub_operator) && node_stack.top().child_num()<2)
				node_stack.top().return_list().push_back(t);
			else
				node_stack.push(t);
			return;
		}
		void operator_generator()
		{
			operator_expr t;
			t.set_line(parse.top().line);
			t.set_type(parse.top().type);
			if(node_stack.empty())
			{
				node_stack.push(t);
				return;
			}
			int top_type=node_stack.top().return_type();
			if(top_type==__number || top_type==__add_operator || top_type==__sub_operator)
			{
				t.return_list().push_back(node_stack.top());
				node_stack.pop();
				node_stack.push(t);
			}
			return;
		}
		void input_stack(std::stack<parse_unit>& temp)
		{
			while(!node_stack.empty())
				node_stack.pop();
			while(!parse.empty())
				parse.pop();
			parse=temp;
			return;
		}
		void run()
		{
			while(!parse.empty())
			{
				switch(parse.top().type)
				{
					case __number:number_generator();break;
					case __add_operator:operator_generator();break;
					case __sub_operator:operator_generator();break;
					default:std::cout<<">>[Error] line "<<parse.top().line<<" parse error.\n";break;
				}
				parse.pop();
			}
			while(!node_stack.empty())
			{
				root.return_list().push_back(node_stack.top());
				node_stack.pop();
			}
			std::cout<<">>[Parse] 0 error(s)."<<std::endl;
			std::cout<<">>[Parse] Complete checking."<<std::endl;
			root.print_all_tree();
			std::cout<<std::endl;
			return;
		}
};

#endif
