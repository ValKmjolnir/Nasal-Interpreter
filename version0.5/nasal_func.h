#ifndef __NASAL_FUNC_H__
#define __NASAL_FUNC_H__

#include "nasal_token.h"

//for function
#define FUNC_BEGIN       0
#define FUNC_OPERATOR    1
#define FUNC_IDENTIFIER  2
#define FUNC_NUMBER      3
#define FUNC_RESERVEWORD 4
#define FUNC_STRING      5

struct parameter
{
	std::string var_name;
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
		void append_var(std::string&,var&);
		void append_token(const int,const int,std::string &);
		void print_info();
};

#endif
