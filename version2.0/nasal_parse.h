#ifndef __NASAL_PARSE_H__
#define __NASAL_PARSE_H__

class nasal_parse
{
	private:
		std::stack<token> parse;
		std::stack<token> checked;
		token this_token;
		int error;
		int warning;
		abstract_syntax_tree root;
	public:
		// basic
		void print_detail_token();
		void get_token_list(std::list<token>&);
		void get_token();
		void push_token();
		int get_error();
		abstract_syntax_tree& get_root();
		
		// abstract_syntax_tree generation
		void main_generate();
		abstract_syntax_tree number_expr();
		abstract_syntax_tree string_expr();
		abstract_syntax_tree nil_expr();
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
			for(int i=line+1;i<tmp.top().line;++i)
				std::cout<<std::endl<<i<<"\t";
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
	while(!parse.empty())
		parse.pop();
	while(!checked.empty())
		checked.pop();
	// clear stack
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
		checked.push(this_token);
	}
	else
	{
		this_token.type=__stack_end;
		this_token.str="__stack_end";
		std::cout<<">>[Parse-error] empty token stack."<<std::endl;
	}
	return;
}

void nasal_parse::push_token()
{
	if(!checked.empty())
	{
		parse.push(checked.top());
		checked.pop();
	}
	else
		std::cout<<">>[Parse-error] empty checked token stack."<<std::endl;
	return;
}

int nasal_parse::get_error()
{
	return error;
}

abstract_syntax_tree& nasal_parse::get_root()
{
	return root;
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
		switch(this_token.type)
		{
			case __var:break;
			case __number:
			case __string:
			case __id:break;
			case __left_curve:break;
			case __left_bracket:break;
			case __if:break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:break;
			case __semi:break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": error token \'";
				print_parse_token(this_token.type);
				std::cout<<"\' in main scope."<<std::endl;
		}
	}
	std::cout<<">>[Parse] complete generation. "<<error<<" error(s), "<<warning<<" warning(s)."<<std::endl;
	return;
}

abstract_syntax_tree nasal_parse::number_expr()
{
	abstract_syntax_tree node;
	node.set_line(this_token.line);
	node.set_type(__number);
	node.set_number(this_token.str);
	return node;
}

abstract_syntax_tree nasal_parse::string_expr()
{
	abstract_syntax_tree node;
	node.set_line(this_token.line);
	node.set_type(__string);
	node.set_string(this_token.str);
	return node;
}

abstract_syntax_tree nasal_parse::nil_expr()
{
	abstract_syntax_tree node;
	node.set_line(this_token.line);
	node.set_type(__nil);
	return node;
}

#endif
