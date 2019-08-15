#ifndef __NASAL_FUNC_CPP__
#define __NASAL_FUNC_CPP__

#include "nasal_func.h"

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
	parameter_head->var_name="__null_var";
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
		parameter_temp=parameter_temp->next;
		parameter_temp->param_var=temp_param->param_var;
		parameter_temp->var_name=temp_param->var_name;
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
	token_unit *statement_temp=statement_head->next;
	token_unit *temp_state=temp.statement_head;
	parameter *parameter_temp=parameter_head->next;
	parameter *temp_param=temp.parameter_head;
	
	token_unit *statement_last=NULL;
	parameter *parameter_last=NULL;
	if(statement_temp)
	{
		while(statement_temp->next)
		{
			statement_last=statement_temp;
			statement_temp=statement_temp->next;
			delete statement_last;
		}
		delete statement_temp;
	}
	if(parameter_temp)
	{
		while(parameter_temp->next)
		{
			parameter_last=parameter_temp;
			parameter_temp=parameter_temp->next;
			delete parameter_last;
		}
		delete parameter_temp;
	}
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
		parameter_temp=parameter_temp->next;
		parameter_temp->param_var=temp_param->param_var;
		parameter_temp->var_name=temp_param->var_name;
		parameter_temp->next=NULL;
	}
	return *this;
}
void func::append_var(std::string& varia_name,var& p)
{
	parameter *parameter_temp=parameter_head;
	while(parameter_temp->next)
	{
		parameter_temp=parameter_temp->next;
		if(parameter_temp->var_name==varia_name)
		{
			std::cout<<"[Error] Redefinition of var \""<<varia_name<<"\" ."<<std::endl;
			return;
		}
	}
	parameter_temp->next=new parameter;
	parameter_temp=parameter_temp->next;
	parameter_temp->param_var=p;
	parameter_temp->var_name=varia_name;
	parameter_temp->next=NULL;
	return;
}
void func::append_token(const int _line,const int _type,std::string &_content)
{
	token_unit *statement_temp=statement_head;
	while(statement_temp->next)
		statement_temp=statement_temp->next;
	statement_temp->next=new token_unit;
	statement_temp=statement_temp->next;
	statement_temp->line=_line;
	statement_temp->type=_type;
	statement_temp->content=_content;
	statement_temp->next=NULL;
	return;
}
void func::print_info()
{
	parameter *para_temp=parameter_head;
	token_unit *token_temp=statement_head;
	std::cout<<"\n\t[parameter] :";
	while(para_temp->next)
	{
		para_temp=para_temp->next;
		std::cout<<para_temp->var_name<<" ";
	}
	std::cout<<"\n\t[statement] :";
	while(token_temp->next)
	{
		token_temp=token_temp->next;
		std::cout<<token_temp->content<<" ";
	}
	std::cout<<std::endl;
	return;
}

#endif
