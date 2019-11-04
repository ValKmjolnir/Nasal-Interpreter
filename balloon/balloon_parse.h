#ifndef __BALLOON_PARSE_H__
#define __BALLOON_PARSE_H__

class balloon_parse
{
	private:
		std::stack<token> parse;
		token this_token;
		int error;
		int warning;
		abstract_syntax_tree root;
	public:
		void get_token()
		{
			if(!parse.empty())
			{
				this_token=parse.top();
				parse.pop();
			}
			else
				this_token.type=0;
			return;
		}
		void get_detail_token_stream(std::list<token>& tk_list)
		{
			root.set_clear();
			while(!parse.empty())
				parse.pop();
			if(tk_list.empty())
				return;
			std::stack<token> temp;
			for(std::list<token>::iterator i=tk_list.begin();i!=tk_list.end();++i)
				temp.push(*i);
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			return;
		}
		void print_parse_stack()
		{
			if(parse.empty())
				return;
			std::stack<token> temp;
			int line=parse.top().line;
			std::cout<<line<<"	";
			while(!parse.empty())
			{
				if(parse.top().line!=line)
				{
					line=parse.top().line;
					std::cout<<std::endl<<line<<"	";
				}
				temp.push(parse.top());
				std::cout<<" ";
				print_detail_token(temp.top().type);
				parse.pop();
			}
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			std::cout<<std::endl;
			return;
		}
		int get_error()
		{
			return error;
		}
		void parse_main();
};

void balloon_parse::parse_main()
{
	root.set_type(__root);
	error=0;
	warning=0;
	
	while(!parse.empty())
	{
		get_token();
		switch(this_token.type)
		{
			case __var:parse.push(this_token);break;
			case __number:
			case __string:parse.push(this_token);break;
			case __id:parse.push(this_token);break;
			case __if:parse.push(this_token);break;
			case __while:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": ";
				print_detail_token(this_token.type);
				std::cout<<" should not appear in this scope."<<std::endl;
				break;
		}
	}
	std::cout<<">>[Parse] complete generating."<<error<<" error(s),"<<warning<<" warning(s)."<<std::endl;
	return;
}
#endif
