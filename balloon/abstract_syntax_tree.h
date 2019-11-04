#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

class abstract_syntax_tree
{
	private:
		int type;
		double number;
		std::string str;
		std::string name;
		std::list<abstract_syntax_tree> children;
	public:
		abstract_syntax_tree()
		{
			type=__null_node;
			number=0;
			str="";
			name="";
			children.clear();
			return;
		}
		abstract_syntax_tree(const abstract_syntax_tree& p)
		{
			type=p.type;
			number=p.number;
			str=p.str;
			name=p.name;
			children=p.children;
			return;
		}
};

#endif
