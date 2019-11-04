#ifndef __BALLOON_PARSE_H__
#define __BALLOON_PARSE_H__

class balloon_parse
{
	private:
		std::stack<token> detail;
		token this_token;
		int error;
		int warning;
	public:
		void get_token()
		{
			if(!detail.empty())
			{
				this_token=detail.top();
				detail.pop();
			}
			else
				this_token.type=0;
			return;
		}
		void get_detail_token_stream(std::list<token>& tk_list)
		{
			std::stack<token> temp;
			for(std::list<token>::iterator i=tk_list.begin();i!=tk_list.end();++i)
				temp.push(*i);
			while(!temp.empty())
			{
				detail.push(temp.top());
				temp.pop();
			}
			return;
		}
};

#endif
