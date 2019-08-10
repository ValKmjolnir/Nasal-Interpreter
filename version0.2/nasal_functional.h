#ifndef __NASAL_FUNCTIONAL_H__
#define __NASAL_FUNCTIONAL_H__

#include "nasal_var.h"

//for function
#define FUNC_BEGIN       0
#define FUNC_OPERATOR    1
#define FUNC_IDENTIFIER  2
#define FUNC_NUMBER      3
#define FUNC_RESERVEWORD 4
#define FUNC_STRING      5

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

struct parameter
{
	var param_var;
	parameter *next;
};

class func
{
	private:
		token_unit *statement_head;
		parameter *parameter_head;
	public:
		func();
		func(const func&);
		~func();
		func& operator=(const func&);
		void append_var(var&);
		void append_token(const int,const int,std::string &);
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
		void brace_check();
		bool def_function();
		bool def_array();
		bool def_hash();
		bool def_scalar();
		bool asi_function();
		bool asi_array();
		bool asi_hash();
		bool asi_scalar();
		void definition();
		void assignment();
		void total_run(token_list&);
};

#endif
