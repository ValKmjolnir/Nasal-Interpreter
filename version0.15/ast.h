#ifndef __AST_H__
#define __AST_H__

#include "token_type.h"
#include <stack>
#include <list>
#include <cstring>

enum tree_node_type
{
	__syntax_tree_begin=128,
	__error_syntax,
	__null_statement,
	__definition,
	__assignment,
	__loop,
	__choose
};

struct node
{
	int type;
	int line;
	std::string content;
	std::list<node> children;
};

class abstract_syntax_tree
{
	private:
		node tree;
		std::stack<parse_unit> parser_stack;
	public:
		abstract_syntax_tree()
		{
			tree.type=__syntax_tree_begin;
			tree.line=0;
			tree.content="";
			tree.children.clear();
		}
		void init_parser_stack(std::stack<parse_unit>& temp)
		{
			parser_stack=temp;
			return;
		}
		void tree_generator(std::list<int>& error_list)
		{
			while(!parser_stack.empty())
			{
				int type=parser_stack.top().type;
				node child_node;
				child_node.line=parser_stack.top().line;
				child_node.content="";
				child_node.children.clear();
				switch(type)
				{
					case __var:break;
					case __id:break;
					case __number:break;
					case __string:break;
					case __if:break;
					case __for:break;
					case __forindex:break;
					case __foreach:break;
					case __while:break;
					case __semi:child_node.type=__null_statement;break;
					default:child_node.type=__error_syntax;break;
				}
				if(child_node.type==__error_syntax)
					error_list.push_back(parser_stack.top().line);
				parser_stack.pop();
			}
			return;
		}
		void run()
		{
			;
			return;
		}
};

#endif
