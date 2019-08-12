#ifndef __NASAL_PARSE_H__
#define __NASAL_PARSE_H__
#include "nasal_lexer.h"

struct parse_unit
{
	int type;
	std::string content;
	int line;
};

class parse
{
	private:
		parse_unit *content_array;
		parse_unit *statement;
		int len;
	public:
		parse();
		~parse();
		void content_array_set_empty();
		void statement_set_empty();
};

parse::parse()
{
	len=0;
	content_array=new parse_unit[4096];
	statement=new parse_unit[1024];
}
parse::~parse()
{
	if(content_array)
		delete []content_array;
	if(statement)
		delete []statement;
}
void parse::content_array_set_empty()
{
	for(int i=0;i<4096;++i)
	{
		content_array[i].line=0;
		content_array[i].type=0;
	}
	return;
}
void parse::statement_set_empty()
{
	for(int i=0;i<1024;++i)
	{
		statement[i].line=0;
		statement[i].type=0;
	}
	return;
}

parse nasal_parse;

#endif
