#include <iostream>
#include <cstring>
#include <list>

class ASTree
{
	protected:
		int line;
		int type;
		std::string content;
		std::list<ASTree> children;
	public:
		ASTree()
		{
			line=0;
			type=0;
			content="root";
			children.clear();
		}
		virtual ASTree& child(int n)
		{
			int cnt=0;
			std::list<ASTree>::iterator _child;
			for(std::list<ASTree>::iterator i=children.begin();i!=children.end();++i)
			{
				if(n==cnt)
				{
					_child=i;
					break;
				}
				++cnt;
			}
			return *_child;
		}
		virtual int num_children()
		{
			int cnt=0;
			type=0;
			for(std::list<ASTree>::iterator i=children.begin();i!=children.end();++i)
				++cnt;
			return cnt;
		}
		virtual std::list<ASTree>& return_child()
		{
			return children;
		}
		void print_tree()
		{
			if(content=="leaf")
			{
				std::cout<<"( "<<content<<" )";
				return;
			}
			std::cout<<"( "<<content<<" ";
			for(std::list<ASTree>::iterator i=children.begin();i!=children.end();++i)
			{
				i->print_tree();
			}
			std::cout<<" )";
			return;
		}
		virtual void setcontent(std::string str)
		{
			content=str;
			return;
		}
};

class ASTList:public ASTree
{
	public:
		ASTList()
		{
			line=0;
			type=0;
			content="subtree";
			children.clear();
		}
};

class ASTLeaf:public ASTree
{
	public:
		ASTLeaf()
		{
			line=0;
			content="leaf";
			children.clear();
		}
		ASTree& child(int n)
		{
			std::list<ASTree>::iterator _child=children.end();
			return *_child;
		}
		int num_children()
		{
			return 0;
		}
};

int main()
{
	ASTList root;
	ASTList subroot;
	ASTLeaf leaf;
	
	root.setcontent("root");
	
	leaf.setcontent("1");
	subroot.return_child().push_back(leaf);
	leaf.setcontent("2");
	subroot.return_child().push_back(leaf);
	subroot.setcontent("+");
	root.return_child().push_back(subroot);
	
	leaf.setcontent("2147483648");
	root.return_child().push_back(leaf);
	subroot.return_child().push_back(subroot);
	root.return_child().push_back(subroot);
	root.print_tree();
	return 0;
}
