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
	token_unit *statement_temp=statement_head;
	token_unit *temp_state=temp.statement_head;
	parameter *parameter_temp=parameter_head;
	parameter *temp_param=temp.parameter_head;
	while(temp_state->next)
	{
		temp_state=temp_state->next;
		statement_temp->next=new token_unit;
		statement_temp=statement_temp->next;
		statement_temp->line=temp_state->line;
		statement_temp->content=temp_state->content;
		statement_temp->type=temp_state->type;
		statement_temp->next=NULL;
	}
	while(temp_param->next)
	{
		temp_param=temp_param->next;
		parameter_temp->next=new parameter;
		parameter_temp->param_var=temp_param->param_var;
		parameter_temp->next=NULL;
	}
}
func::~func()
{
	token_unit *statement_temp=statement_head;
	token_unit *statement_last_unit=NULL;
	parameter *parameter_temp=parameter_head;
	parameter *parameter_last_unit=NULL;
	
	while(statement_temp->next)
	{
		statement_last_unit=statement_temp;
		statement_temp=statement_temp->next;
		delete statement_last_unit;
	}
	delete statement_temp;
	while(parameter_temp->next)
	{
		parameter_last_unit=parameter_temp;
		parameter_temp=parameter_temp->next;
		delete parameter_last_unit;
	}
	delete parameter_temp;
}
func& func::operator=(const func& temp)
{
	token_unit *statement_temp=statement_head;
	token_unit *temp_state=temp.statement_head;
	parameter *parameter_temp=parameter_head;
	parameter *temp_param=temp.parameter_head;
	
	token_unit *statement_last=NULL;
	parameter *parameter_last=NULL;
	while(statement_temp->next)
	{
		statement_last=statement_temp;
		statement_temp=statement_temp->next;
		delete statement_last;
	}
	delete statement_temp;
	while(parameter_temp->next)
	{
		parameter_last=parameter_temp;
		parameter_temp=parameter_temp->next;
		delete parameter_last;
	}
	delete parameter_temp;
	statement_head->next=NULL;
	parameter_head->next=NULL;
	statement_temp=statement_head;
	parameter_temp=parameter_head;
	while(temp_state->next)
	{
		temp_state=temp_state->next;
		statement_temp->next=new token_unit;
		statement_temp=statement_temp->next;
		statement_temp->line=temp_state->line;
		statement_temp->content=temp_state->content;
		statement_temp->type=temp_state->type;
		statement_temp->next=NULL;
	}
	while(temp_param->next)
	{
		temp_param=temp_param->next;
		parameter_temp->next=new parameter;
		parameter_temp->param_var=temp_param->param_var;
		parameter_temp->next=NULL;
	}
	return *this;
}

#endif
