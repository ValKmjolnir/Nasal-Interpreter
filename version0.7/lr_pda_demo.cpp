#include <list>
#include <stack>
#include <cstring>
#include <iostream>

enum token_type
{
	S=1,a,T,__tri,__comma,__left_curve,__right_curve,sharp
};

const int max_len=5;
struct cmp_seq
{
	int tokens[max_len];
	int res;
};
cmp_seq par[]=
{
	{{a},                            S},
	{{__tri},                        S},
	{{__right_curve,T,__left_curve}, S},
	{{S},                            T},
	{{T,__comma,T},                  T},
};
int num_of_par=sizeof(par)/sizeof(cmp_seq);

void print_token(int type)
{
	std::string context="";
	switch(type)
	{
		case sharp:
			context="#";
			break;
		case S:
			context="S";
			break;
		case a:
			context="a";
			break;
		case T:
			context="T";
			break;
		case __tri:
			context="^";
			break;
		case __comma:
			context=",";
			break;
		case __left_curve:
			context="(";
			break;
		case __right_curve:
			context=")";
			break;
	}
	std::cout<<context;
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
			for(int i=0;i<(int)text.length();++i)
			{
				if(text[i]=='(')
					main_stack.push(__left_curve);
				else if(text[i]==')')
					main_stack.push(__right_curve);
				else if(text[i]=='a')
					main_stack.push(a);
				else if(text[i]=='^')
					main_stack.push(__tri);
				else if(text[i]==',')
					main_stack.push(__comma);
			}
			comp_stack.push(sharp);
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
		bool reducable()
		{
			std::stack<int> temp;
			for(int i=0;i<num_of_par;++i)
			{
				for(int j=max_len-1;j>=0;--j)
				{
					if(!par[i].tokens[j])
						continue;
					// if par[i].tokens[j] is 0 then continue until the tokens[j] is not 0
					temp.push(comp_stack.top());
					comp_stack.pop();
					if((par[i].tokens[j]!=temp.top()) || (comp_stack.empty() && par[i].tokens[j]))
					{
						while(!temp.empty())
						{
							comp_stack.push(temp.top());
							temp.pop();
						}
						break;
					}
					if((par[i].tokens[j]==temp.top()) && (j==0))
					{
						comp_stack.push(par[i].res);
						return true;
					}
				}
			}
			return false;
		}
		void work()
		{
			print_main_and_comp();
			while(main_stack.top()!=sharp)
			{
				
				comp_stack.push(main_stack.top());
				main_stack.pop();
				while(1)
				{
					if(!reducable())
						break;
					print_main_and_comp();
				}
			}
			print_main_and_comp();
			if(comp_stack.top()==T)
			{
				comp_stack.pop();
				if(comp_stack.top()==sharp)
					std::cout<<"accept."<<std::endl;
				else
					std::cout<<"error."<<std::endl;
			}
			else
				std::cout<<"error."<<std::endl;
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
