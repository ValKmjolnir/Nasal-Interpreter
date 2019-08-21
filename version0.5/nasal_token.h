#ifndef __NASAL_TOKEN_H__
#define __NASAL_TOKEN_H__

#include "nasal_var.h"

//for token and lexer
#define OPERATOR    1 //界符 or 运算符 
#define IDENTIFIER  2 //自定义标识符 
#define NUMBER      3 //数字 
#define RESERVEWORD 4 //关键字 
#define STRING      5 //字符串类型 

struct token_unit
{
	int type;
	std::string content;
	int line;
	token_unit *next;
};

class token_list
{
	private:
		token_unit *head;
		int list_range;
	public:
		token_list();
		~token_list();
		token_unit* get_head();
		int get_list_range();
		void print_line_token(const int);
		void delete_all();
		void append(const int,const int,std::string&);
		void print();
};

token_list nasal_lexer;
#endif
