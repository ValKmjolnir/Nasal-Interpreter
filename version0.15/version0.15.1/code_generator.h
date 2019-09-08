#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include "nasal_parser.h"
#include "abstract_syntax_tree.h"
#include <stack>

class code_generator
{
	private:
		ASTree root;
		std::stack<ASTree> node_stack;
		std::stack<parse_unit> temp;
	public:
		code_generator()
		{
			root.set_data(0,__root);
		}
		void number_node_gen()
		{
			NumberNode t;
			t.setnumber(temp.top().content);
			t.set_data(temp.top().line,__number);
			if(node_stack.empty())
				node_stack.push(t);
			else if(node_stack.top().return_type()==__operator)
				node_stack.top().return_child().push_back(t);
			else
				node_stack.push(t);
			return;
		}
		void operator_node_gen()
		{
			OperatorNode t;
			t.set_operator_type(temp.top().content);
			t.set_data(temp.top().line,__operator);
			if(node_stack.empty())
			{
				std::cout<<"line "<<temp.top().line<<":parse error.\n";
				return;
			}
			if(node_stack.top().return_type()==__number || node_stack.top().return_type()==__operator)
			{
				t.return_child().push_back(node_stack.top());
				node_stack.pop();
				node_stack.push(t);
			}
			return;
		}
		void run()
		{
			root.print_tree();
			std::cout<<std::endl;
			return;
		}
		void stack_input(std::stack<parse_unit>& parse)
		{
			while(!node_stack.empty())
				node_stack.pop();
			temp=parse;
			while(!temp.empty())
			{
				switch(temp.top().type)
				{
					case __number:number_node_gen();break;
					case __add_operator:operator_node_gen();break;
					case __sub_operator:operator_node_gen();break;
					default:std::cout<<"line "<<temp.top().line<<":parse error.\n";break;
				}
				temp.pop();
			}
			while(!node_stack.empty())
			{
				root.return_child().push_back(node_stack.top());
				node_stack.pop();
			}
			return;
		}
};

#endif
