#include <list>
#include <stack>
#include <cstring>
#include <iostream>

enum token_type
{
	e=1,E,_E,T,_T,F,I,__add,__sub,__mul,__div,__left_curve,__right_curve,sharp
};

const int max_len=3;
struct cmp_seq
{
	int tokens[max_len];
	int res;
};
cmp_seq par[]=
{
	{{T,_E},                        E},
	{{__add,T,_E},                 _E},
	{{__sub,T,_E},                 _E},
	{{e},                          _E},
	{{F,_T},                        T},
	{{__mul,F,_T},                 _T},
	{{__div,F,_T},                 _T},
	{{e},                          _T},
	{{__left_curve,E,__right_curve},F},
	{{I},                           F}
};
int num_of_par=sizeof(par)/sizeof(cmp_seq);

cmp_seq first_set[]=
{
	{{__left_curve,I},E},
	{{__left_curve,I},T},
	{{__left_curve,I},F},
	{{__add,__sub,e},_E},
	{{__mul,__div,e},_T},
};
int num_of_set=sizeof(first_set)/sizeof(cmp_seq);

bool isEnd(int type)
{
	return ((type==__left_curve) || (type==__right_curve) || (type==I) || (type==__add) || (type==__sub) || (type==__mul) || (type==__div) || (type==e) || (type==sharp));
}
bool isHead(int head,int type)
{
	for(int i=0;i<num_of_set;++i)
	{
		if(type==first_set[i].res)
		{
			for(int j=0;j<max_len;++j)
				if(head==first_set[i].tokens[j])
					return true;
		}
	}
	return false;
}
int correct_token_seq(int head,int type)
{
	for(int i=0;i<num_of_par;++i)
	{
		if((type==par[i].res) && (isHead(head,par[i].tokens[0]) || (head==par[i].tokens[0])))
			return i;
	}
	for(int i=0;i<num_of_par;++i)
	{
		if((type==par[i].res) && (par[i].tokens[0]==e))
			return i;
	}
	return -1;
}
int place_operator_end(int type)
{
	for(int i=0;i<num_of_par;++i)
	{
		if((type==par[i].res) && (par[i].tokens[0]==e))
			return i;
	}
	return -1;
}



void print_token(int type)
{
	std::string str="";
	switch(type)
	{
		case e:
			str="e";
			break;
		case E:
			str="E";
			break;
		case _E:
			str="E\'";
			break;
		case T:
			str="T";
			break;
		case _T:
			str="T\'";
			break;
		case F:
			str="F";
			break;
		case I:
			str="i";
			break;
		case __add:
			str="+";
			break;
		case __sub:
			str="-";
			break;
		case __mul:
			str="*";
			break;
		case __div:
			str="/";
			break;
		case __left_curve:
			str="(";
			break;
		case __right_curve:
			str=")";
			break;
		case sharp:
			str="#";
			break;
	}
	std::cout<<str;
	return;
}

class PDA
{
	private:
		std::stack<int> main_stack;
		std::stack<int> comp_stack;
	public:
		PDA(std::string& text)
		{
			main_stack.push(sharp);
			for(int i=(int)text.length()-1;i>=0;--i)
			{
				if(text[i]=='(')
					main_stack.push(__left_curve);
				else if(text[i]==')')
					main_stack.push(__right_curve);
				else if(text[i]=='i')
					main_stack.push(I);
				else if(text[i]=='+')
					main_stack.push(__add);
				else if(text[i]=='-')
					main_stack.push(__sub);
				else if(text[i]=='*')
					main_stack.push(__mul);
				else if(text[i]=='/')
					main_stack.push(__div);
			}
			comp_stack.push(sharp);
			comp_stack.push(E);
		}
		void print_stack(std::stack<int>& temp)
		{
			std::stack<int> t;
			while(!temp.empty())
			{
				t.push(temp.top());
				print_token(t.top());
				temp.pop();
			}
			while(!t.empty())
			{
				temp.push(t.top());
				t.pop();
			}
			return;
		}
		void print_main_and_comp()
		{
			print_stack(main_stack);
			std::cout<<"		";
			print_stack(comp_stack);
			std::cout<<std::endl;
			return;
		}
		void work()
		{
			print_main_and_comp();
			while((main_stack.top()!=sharp) || (comp_stack.top()!=sharp))
			{
				if(!isEnd(comp_stack.top()))
				{
					if(isHead(main_stack.top(),comp_stack.top()))
					{
						int i=correct_token_seq(main_stack.top(),comp_stack.top());
						comp_stack.pop();
						for(int j=max_len-1;j>=0;--j)
							if(par[i].tokens[j])
								comp_stack.push(par[i].tokens[j]);
					}
					else if(!isHead(main_stack.top(),comp_stack.top()))
					{
						//to check if this can be extended as 'e'
						int i=place_operator_end(comp_stack.top());
						if(i!=-1)
							comp_stack.pop();
						else
						{
							std::cout<<"error."<<std::endl;
							return;
						}
					}
					else
					{
						std::cout<<"error."<<std::endl;
						return;
					}
					print_main_and_comp();
				}
				else
				{
					if(main_stack.top()==comp_stack.top())
					{
						main_stack.pop();
						comp_stack.pop();
					}
					else if(comp_stack.top()==e)
						comp_stack.pop();
					else
					{
						std::cout<<"error."<<std::endl;
						return;
					}
					print_main_and_comp();
				}
			}
			std::cout<<"accept."<<std::endl;
			return;
		}
};

int main()
{
	std::string text;
	while(1)
	{
		std::cin>>text;
		PDA m(text);
		m.work();
	}
	return 0;
}
