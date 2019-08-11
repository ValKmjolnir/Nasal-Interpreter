#ifndef __NASAL_FUNCTIONAL_CPP__
#define __NASAL_FUNCTIONAL_CPP__

#include "nasal_functional.h"
#include "nasal_var_stack.h"
#include "nasal_func_stack.h"

bool is_float(std::string &str)
{
	for(int i=0;i<(int)str.length();++i)
		if(str[i]=='.')
			return true;
	return false;
}
long long int int_str2num(std::string &str)
{
	for(int i=0;i<(int)str.length();++i)
		if(!(('0'<=str[i]) && (str[i]<='9') || (str[i]=='.')))
		{
			std::cout<<"[Error] Non-numeric string."<<std::endl;
			return 0;
		}
	long long int num=0;
	long long int acc=1;
	for(int i=(int)str.length()-1;i>=0;--i)
	{
		num+=acc*((long long int)(str[i]-'0'));
		acc*=10;
	}
	return num;
}
double double_str2num(std::string &str)
{
	for(int i=0;i<(int)str.length();++i)
		if(!(('0'<=str[i]) && (str[i]<='9') || (str[i]=='.')))
		{
			std::cout<<"[Error] Non-numeric string."<<std::endl;
			return 0;
		}
	int DotPlace=0;
	for(int i=0;i<(int)str.length();++i)
		if(str[i]=='.')
		{
			DotPlace=i;
			break;
		}
	double num=0;
	double acc=1;
	double aff=0.1;
	for(int i=DotPlace+1;i<(int)str.length();++i)
	{
		num+=aff*((double)(str[i]-'0'));
		aff*=0.1;
	}
	for(int i=DotPlace-1;i>=0;--i)
	{
		num+=acc*((double)(str[i]-'0'));
		acc*=10;
	}
	return num;
}


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
		parameter_temp->param_var=temp_param->param_var;
		parameter_temp->next=NULL;
	}
	return *this;
}
void func::append_var(var& p)
{
	parameter *parameter_temp=parameter_head;
	while(parameter_temp->next)
		parameter_temp=parameter_temp->next;
	parameter_temp->next=new parameter;
	parameter_temp=parameter_temp->next;
	parameter_temp->param_var=p;
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

token_list::token_list()
{
	list_range=0;
	head=new token_unit;
	head->type=FUNC_BEGIN;
	head->content="__process_begin";
	head->line=0;
	head->next=NULL;
}
token_list::~token_list()
{
	token_unit *temp=head;
	token_unit *this_node=NULL;
	while(temp->next)
	{
		this_node=temp;
		temp=temp->next;
		delete this_node;
	}
	delete temp;
}
token_unit* token_list::get_head()
{
	return head;
}
int token_list::get_list_range()
{
	return list_range;
}
void token_list::print_line_token(const int _line)
{
	std::cout<<"line "<<_line<<": ";
	token_unit *temp=head;
	while(temp->next)
	{
		temp=temp->next;
		if(temp->line==_line)
			std::cout<<temp->content<<" ";
		else if(temp->line>_line)
			break;
	}
	std::cout<<std::endl;
	return;
}
void token_list::delete_all()
{
	if(!head->next)
		return;
	token_unit *temp=head;
	token_unit *this_node=NULL;
	temp=temp->next;
	head->next=NULL;
	while(temp->next)
	{
		this_node=temp;
		temp=temp->next;
		delete this_node;
	}
	delete temp;
	return;
}
void token_list::append(const int _line,const int _type,std::string &_content)
{
	token_unit *temp=head;
	while(temp->next)
		temp=temp->next;
	++list_range;
	temp->next=new token_unit;
	temp=temp->next;
	temp->next=NULL;
	temp->type=_type;
	temp->line=_line;
	temp->content=_content;
	return;
}
void token_list::print()
{
	token_unit *temp=head;
	std::cout<<"line "<<temp->line<<": "<<"( ProcessBegin | "<<temp->content<<" )"<<std::endl;
	if(!head->next)
		return;
	while(temp->next)
	{
		temp=temp->next;
		std::cout<<"line "<<temp->line<<": ";
		if(temp->type==OPERATOR)
			std::cout<<"( Operator     | ";
		else if(temp->type==IDENTIFIER)
			std::cout<<"( Identifier   | ";
		else if(temp->type==NUMBER)
			std::cout<<"( Number       | ";
		else if(temp->type==RESERVEWORD)
			std::cout<<"( ReserveWord  | ";
		else if(temp->type==STRING)
			std::cout<<"( String       | ";
		std::cout<<temp->content<<" )"<<std::endl;
	}
	return;
}

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
void parse::brace_check()
{
	int curve=0;
	int bracket=0;
	int brace=0;
	for(int i=1;i<=len;++i)
	{
		if(content_array[i].content=="(")
			++curve;
		else if(content_array[i].content==")")
			--curve;
		else if(content_array[i].content=="[")
			++bracket;
		else if(content_array[i].content=="]")
			--bracket;
		else if(content_array[i].content=="{")
			++brace;
		else if(content_array[i].content=="}")
			--brace;
	}
	if(curve>0)
		std::cout<<"[Error] Missing \")\" ."<<std::endl;
	else if(curve<0)
		std::cout<<"[Error] Missing \"(\" ."<<std::endl;
	if(bracket>0)
		std::cout<<"[Error] Missing \"]\" ."<<std::endl;
	else if(bracket<0)
		std::cout<<"[Error] Missing \"[\" ."<<std::endl;
	if(brace>0)
		std::cout<<"[Error] Missing \"}\" ."<<std::endl;
	else if(brace<0)
		std::cout<<"[Error] Missing \"{\" ."<<std::endl;
	return;
}
bool parse::def_function()
{
	if(statement[0].content=="var" && statement[1].type==IDENTIFIER && statement[2].content=="=" && statement[3].content=="func")
	{
		func temp_func;
		nasal_func_stack.append_function(statement[1].content,temp_func);
		return true;
	}
	return false;
}
bool parse::def_array()
{
	if(statement[0].content=="var" && statement[1].type==IDENTIFIER && statement[2].content=="=" && statement[3].content=="[" && statement[4].content=="]")
	{
		var temp_var;
		temp_var.type=VAR_LIST;
		temp_var.data=new nasal_list;
		nasal_var_stack.append_var(statement[1].content,temp_var);
		return true;
	}
	return false;
}
bool parse::def_hash()
{
	if(statement[0].content=="var" && statement[1].type==IDENTIFIER && statement[2].content=="=" && statement[3].content=="{" && statement[4].content=="}")
	{
		var temp_var;
		temp_var.type=VAR_HASH;
		temp_var.data=new nasal_hash;
		nasal_var_stack.append_var(statement[1].content,temp_var);
		return true;
	}
	return false;
}
bool parse::def_scalar()
{
	if(statement[0].content=="var" && statement[1].type==IDENTIFIER && statement[2].content=="=" && (statement[3].type==NUMBER || statement[3].type==STRING))
	{
		if(statement[3].type==NUMBER)
		{
			var temp_var;
			if(is_float(statement[3].content))
			{
				temp_var.type=VAR_DOUBLE;
				temp_var.data=new double;
				*((double *)temp_var.data)=double_str2num(statement[3].content);
			}
			else
			{
				temp_var.type=VAR_LLINT;
				temp_var.data=new long long int;
				*((long long int *)temp_var.data)=int_str2num(statement[3].content);
			}
			nasal_var_stack.append_var(statement[1].content,temp_var);
		}
		else if(statement[3].type==STRING)
		{
			var temp_var;
			temp_var.type=VAR_STRING;
			temp_var.data=new std::string;
			std::string temp_str="";
			for(int i=1;i<(int)statement[3].content.length()-1;++i)
				temp_str+=statement[3].content[i];
			*((std::string *)temp_var.data)=temp_str;
			nasal_var_stack.append_var(statement[1].content,temp_var);
		}
		return true;
	}
	return false;
}
void parse::definition()
{
	int brace=0;
	for(int i=0;i<2048;++i)
	{
		if(!statement[i].line)
		{
			std::cout<<"[Error] line "<<statement[0].line<<": Expect a \";\" after "<<statement[0].content<<"."<<std::endl;
			break;
		}
		if(statement[i].content==";" && !brace)
			break;
		if(statement[i].content=="{")
			++brace;
		if(statement[i].content=="}")
			--brace;
		if(!statement[i].type && brace)
			std::cout<<"[Error] Missing \""<<(brace>0? '}':'{')<<"\" ."<<std::endl;
	}
	if(def_function() || def_array() || def_hash() || def_scalar())
	{
		;
	}
	else
		std::cout<<"[Error] line "<<statement[0].line<<": Missing elements."<<std::endl;
	return;
}
bool parse::asi_function()
{
	return false;
}
bool parse::asi_array()
{
	return false;
}
bool parse::asi_hash()
{
	return false;
}
bool parse::asi_scalar()
{
	if(statement[0].type==IDENTIFIER && statement[1].content=="=" && statement[2].type==IDENTIFIER && statement[3].content==";")
	{
		var temp_var=nasal_var_stack.search_var(statement[2].content);
		nasal_var_stack.edit_var(statement[0].content,temp_var);
		return true;
	}
	else if(statement[0].type==IDENTIFIER && statement[1].content=="=" && statement[2].type==NUMBER && statement[3].content==";")
	{
		var temp_var;
		if(is_float(statement[2].content))
		{
			temp_var.type=VAR_DOUBLE;
			temp_var.data=new double;
			*((double *)temp_var.data)=double_str2num(statement[2].content);
		}
		else
		{
			temp_var.type=VAR_LLINT;
			temp_var.data=new long long int;
			*((long long int *)temp_var.data)=int_str2num(statement[2].content);
		}
		nasal_var_stack.edit_var(statement[0].content,temp_var);
		return true;
	}
	else if(statement[0].type==IDENTIFIER && statement[1].content=="=" && statement[2].type==STRING && statement[3].content==";")
	{
		var temp_var;
		temp_var.type=VAR_STRING;
		temp_var.data=new std::string;
		std::string temp_str="";
		for(int i=1;i<(int)statement[2].content.length()-1;++i)
			temp_str+=statement[2].content[i];
		*((std::string *)temp_var.data)=temp_str;
		nasal_var_stack.edit_var(statement[1].content,temp_var);
		return true;
	}
	return false;
}
void parse::assignment()
{
	int brace=0;
	for(int i=0;i<2048;++i)
	{
		if(statement[i].line==0)
		{
			std::cout<<"[Error] line "<<statement[0].line<<": Expect a \";\" after "<<statement[0].content<<"."<<std::endl;
			break;
		}
		if(statement[i].content==";" && !brace)
			break;
		if(statement[i].content=="{")
			++brace;
		if(statement[i].content=="}")
			--brace;
		if(!statement[i].type && brace)
			std::cout<<"[Error] Missing \""<<(brace>0? '}':'{')<<"\" ."<<std::endl;
	}
	if(asi_function() || asi_array() || asi_hash() || asi_scalar())
	{
		;
	}
	else
		std::cout<<"[Error] line "<<statement[0].line<<": Missing elements."<<std::endl;
	return;
}
void parse::total_run(token_list& p)
{
	len=0;
	token_unit *temp=p.get_head();
	while(temp->next)
	{
		temp=temp->next;
		++len;//begin from 1
		content_array[len].type=temp->type;
		content_array[len].line=temp->line;
		content_array[len].content=temp->content;
	}
	brace_check();
	for(int i=1;i<=len;++i)
	{
		if(content_array[i].type==RESERVEWORD && content_array[i].content=="var")
		{
			int in_brace=0;
			statement_set_empty();
			for(int j=i;j<=len;++j)
			{
				statement[j-i]=content_array[j];
				if(content_array[j].type==OPERATOR && content_array[j].content==";" && in_brace==0)
				{
					i=j;
					break;
				}
				if(content_array[j].type==OPERATOR && content_array[j].content=="{")
					++in_brace;
				if(content_array[j].type==OPERATOR && content_array[j].content=="}")
					--in_brace;
			}
			definition();
		}
		else if(content_array[i].type==IDENTIFIER)
		{
			int in_brace=0;
			statement_set_empty();
			for(int j=i;j<=len;++j)
			{
				statement[j-i]=content_array[j];
				if(content_array[j].type==OPERATOR && content_array[j].content==";" && in_brace==0)
				{
					i=j;
					break;
				}
				if(content_array[j].type==OPERATOR && content_array[j].content=="{")
					++in_brace;
				if(content_array[j].type==OPERATOR && content_array[j].content=="}")
					--in_brace;
			}
			assignment();
		}
	}
	nasal_var_stack.print_var();
	nasal_func_stack.print_function();
	return;
}

#endif
