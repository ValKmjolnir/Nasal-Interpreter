#ifndef __NASAL_FUNCTIONAL_CPP__
#define __NASAL_FUNCTIONAL_CPP__

#include "nasal_functional.h"

func::func()
{
	statement_head=new token_unit;
	statement_head->line=0;
	statement_head->type=FUNC_BEGIN;
	statement_head->content="__func_begin";
	statement_head->next=NULL;
	
	parameter_head=new parameter;
	parameter_head->param_var.type=VAR_NONE;
	parameter_head->param_var.data=NULL;
	parameter_head->next=NULL;
}
func::func(const func& temp)
{
	;
}
func::~func()
{
	;
}
func& func::operator=(const func& temp)
{
	return *this;
}

#endif
