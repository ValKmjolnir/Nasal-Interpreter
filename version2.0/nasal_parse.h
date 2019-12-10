#ifndef __NASAL_PARSE_H__
#define __NASAL_PARSE_H__

class nasal_parse
{
	private:
		std::stack<token> parse;
		token this_token;
		int error;
		int warning;
		abstract_syntax_tree root;
	public:
		// basic
		void print_detail_token();
		void get_token_list(std::list<token>&);
		void get_token();
		
		// abstract_syntax_tree generation
		void main_generate();
};

void nasal_parse::print_detail_token()
{
	std::stack<token> tmp=parse;
	std::string space="";
	int line=1;
	std::cout<<line<<"\t";
	while(!tmp.empty())
	{
		if(tmp.top().line!=line)
		{
			line=tmp.top().line;
			std::cout<<std::endl<<line<<"\t"<<space;
		}
		print_parse_token(tmp.top().type);
		if(tmp.top().type==__left_brace)
			space+=' ';
		std::cout<<" ";
		tmp.pop();
		if(!tmp.empty() && tmp.top().type==__right_brace)
		{
			std::string str="";
			for(int i=0;i<space.length()-1;++i)
				str+=space[i];
			space=str;
		}
	}
	std::cout<<std::endl;
	return;
}

void nasal_parse::get_token_list(std::list<token>& detail_token)
{
	std::stack<token> tmp;
	for(std::list<token>::iterator i=detail_token.begin();i!=detail_token.end();++i)
		tmp.push(*i);
	while(!tmp.empty())
	{
		parse.push(tmp.top());
		tmp.pop();
	}
	return;
}

void nasal_parse::get_token()
{
	if(!parse.empty())
	{
		this_token=parse.top();
		parse.pop();
	}
	else
	{
		this_token.type=__stack_end;
		this_token.str="__stack_end";
	}
	return;
}

void nasal_parse::main_generate()
{
	error=0;
	warning=0;
	root.set_clear();
	root.set_line(1);
	root.set_type(__root);
	
	while(!parse.empty())
	{
		get_token();
	}
	return;
}

#endif
