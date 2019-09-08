#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__

#include "token_type.h"
#include <iostream>
#include <cstring>
#include <list>


enum node_type
{
	__root=1,
	__subtree,
	__leaf,
	__numbernode,
	__stringnode,
	__operator,
	__calculation,
};

void print_node_type(int type)
{
	std::string str="";
	switch(type)
	{
		case __root:str="root";break;
		case __subtree:str="subtree";break;
		case __leaf:str="leaf";break;
		case __numbernode:str="number";break;
		case __stringnode:str="string";break;
		case __operator:str="operator";break;
		case __calculation:str="calc";break;
	}
	std::cout<<str;
	return;
}

class ASTree
{
	protected:
		int line;
		int type;
		std::list<ASTree> children;
	public:
		ASTree()
		{
			line=0;
			type=__root;
			children.clear();
		}
		virtual std::list<ASTree>& return_child()
		{
			return children;
		}
		virtual void set_data(int _line,int _type)
		{
			line=_line;
			type=_type;
			return;
		}
		void print_tree()
		{
			if(type==__leaf || type==__number || type==__string)
			{
				std::cout<<"( ";
				print_token(type);
				std::cout<<" )";
				return;
			}
			std::cout<<"( ";
			print_node_type(type);
			std::cout<<" ";
			for(std::list<ASTree>::iterator i=children.begin();i!=children.end();++i)
			{
				i->print_tree();
			}
			std::cout<<" )";
			return;
		}
		virtual int return_type()
		{
			return type;
		}
};

class ASTList:public ASTree
{
	public:
		ASTList()
		{
			line=0;
			type=__subtree;
			children.clear();
		}
};

class ASTLeaf:public ASTree
{
	public:
		ASTLeaf()
		{
			line=0;
			type=__leaf;
			children.clear();
		}
};

class NumberNode:public ASTLeaf
{
	private:
		long long int number;
		double fnumber;
		bool isFloat;
	public:
		NumberNode()
		{
			line=0;
			type=__numbernode;
			number=0;
			fnumber=0;
			isFloat=false;
			children.clear();
		}
		void setnumber(std::string& str)
		{
			isFloat=false;
			int DotPlace=0;
			for(int i=0;i<(int)str.length();++i)
				if(str[i]=='.')
				{
					isFloat=true;
					DotPlace=i;
					break;
				}
			if(!isFloat)
			{
				number=0;
				long long int acc=1;
				for(int i=(int)str.length()-1;i>=0;--i)
				{
					number+=acc*((long long int)(str[i]-'0'));
					acc*=10;
				}
			}
			else
			{
				fnumber=0;
				double acc=1;
				double aff=0.1;
				for(int i=DotPlace+1;i<(int)str.length();++i)
				{
					fnumber+=aff*((double)(str[i]-'0'));
					aff*=0.1;
				}
				for(int i=DotPlace-1;i>=0;--i)
				{
					fnumber+=acc*((double)(str[i]-'0'));
					acc*=10;
				}
			}
			return;
		}
};
class StringNode:public ASTLeaf
{
	private:
		std::string str;
	public:
		StringNode()
		{
			line=0;
			type=__stringnode;
			str="";
			children.clear();
		}
};

class OperatorNode:public ASTList
{
	private:
		int operator_type;
	public:
		OperatorNode()
		{
			line=0;
			type=__operator;
			operator_type=0;
			children.clear();
		}
		void set_operator_type(std::string& str)
		{
			char c=str[0];
			switch(c)
			{
				case '+':operator_type=__add_operator;break;
				case '-':operator_type=__sub_operator;break;
				case '*':operator_type=__mul_operator;break;
				case '/':operator_type=__div_operator;break;
				case '~':operator_type=__link_operator;break;
			}
			return;
		}
};
class CalculationNode:public ASTList
{
	public:
		CalculationNode()
		{
			line=0;
			type=__calculation;
			children.clear();
		}
};

#endif
