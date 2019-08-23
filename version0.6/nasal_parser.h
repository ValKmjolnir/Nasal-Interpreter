#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

#include <cstring>
#include <stack>
#include <list>
#include <queue>

struct parse_unit
{
	int line;
	std::string content;
};

class nasal_parser
{
	private:
		std::stack<parse_unit> parser;
	public:
		void parse_process(std::list<token>& lexer)
		{
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				parse_unit temp;
				temp.line=(*i).line;
				temp.content=(*i).content;
				parser.push(temp);
			}
			std::cout<<">>[Parser] Complete scanning."<<std::endl;
			return;
		}
		void print_parser()
		{
			std::stack<parse_unit> temp;
			while(!parser.empty())
			{
				temp.push(parser.top());
				parser.pop();
			}
			int nowline=0;
			if(temp.empty())
			{
				std::cout<<">>[Parser] Empty stack."<<std::endl;
				return;
			}
			while(!temp.empty())
			{
				if(temp.top().line==nowline)
					std::cout<<temp.top().content<<" ";
				else
				{
					nowline=temp.top().line;
					std::cout<<std::endl<<"line "<<nowline<<": "<<temp.top().content<<" ";
				}
				temp.pop();
			}
			std::cout<<std::endl;
			return;
		}
};

#endif
