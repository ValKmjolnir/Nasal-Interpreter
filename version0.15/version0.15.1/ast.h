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


#endif
