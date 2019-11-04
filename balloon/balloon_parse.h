#ifndef __BALLOON_PARSE_H__
#define __BALLOON_PARSE_H__

class balloon_parse
{
	private:
		std::stack<token> parse;
		token this_token;
		int error;
		int warning;
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
		
};

#endif
