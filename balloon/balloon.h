#ifndef __BALLOON_H__
#define __BALLOON_H__

/*
	Balloon language made by ValKmjolnir
	place: NUAA QQ: 896693328 e-mail: 896693328@qq.com
	Balloon is a real subset of Nasal language
	Nasal language is created for FlightGear
	Nasal language seems like ECMAscript
	So you may find some similarities between nas & js
*/


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>
#include <list>
#include <stack>
#include <ctime>

void alert_sound()
{
	printf("\a");
	return;
}

#include "balloon_type.h"                 // place some enums
#include "abstract_syntax_tree.h"         // ast
#include "balloon_var.h"                  // var
/* global varia in balloon_var.h :
	var error_var;                        // give an error_var to get 
*/
#include "balloon_lexer.h"                // lexer
/* global varia in balloon_lexer.h :
	struct token;                         // used to get token from resource codes
	std::string reserve_word[15];         // place many reserve words
	int is_reserve_word(std::string str); // check the string is a reserve word or not and return the number of reserve word
	bool check_number(std::string str);   // check the string can be processed to number or not
*/
#include "balloon_parse.h"                // parser
#include "balloon_scope.h"                // place to store vars
/* global varia in balloon_scope.h :
	balloon_scope scope;                  // make a place to vars
*/
#include "abstract_syntax_tree.cpp"       // runtime
/* global varia in abstract_syntax_tree.cpp :
	int exit_type;                        // record the state of runtime
	std::stack<var> ret_stack;            // for function ret use
*/
#endif
