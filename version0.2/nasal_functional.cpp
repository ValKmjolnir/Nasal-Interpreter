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
		if(temp->type==FUNC_OPERATOR)
			std::cout<<"( Operator     | ";
		else if(temp->type==FUNC_IDENTIFIER)
			std::cout<<"( Identifier   | ";
		else if(temp->type==FUNC_NUMBER)
			std::cout<<"( Number       | ";
		else if(temp->type==FUNC_RESERVEWORD)
			std::cout<<"( ReserveWord  | ";
		else if(temp->type==FUNC_STRING)
			std::cout<<"( String       | ";
		std::cout<<temp->content<<" )"<<std::endl;
	}
	return;
}
/*
void token_list::run()
{
	nasal_var_stack.delete_all();
	nasal_func_stack.delete_all();
	token_unit *temp=head;
	if(!head->next)
	{
		std::cout<<">> Running complete."<<std::endl;
		return;
	}
	
	while(temp->next)
	{
		temp=temp->next;
		if(temp->type=="Operator")
		{
			;
		}
		else if(temp->type=="Identifier")
		{
			;
		}
		else if(temp->type=="Number")
		{
			print_line_token(temp->line);
			std::cout<<"line "<<temp->line<<": "<<"[Error] Number without any tokens."<<std::endl;
			return;
		}
		else if(temp->type=="ReserveWord")
		{
			if(temp->content=="var")
			{
				if(temp->next)
					temp=temp->next;
				else
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Missing elements after \"var\"."<<std::endl;
					return;
				}
				//end identifier
				std::string name_of_var;
				if(temp->type=="Identifier")
					name_of_var=temp->content;
				else
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Missing identifier after \"var\"."<<std::endl;
					return;
				}
				if(temp->next)
					temp=temp->next;
				else
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Missing operator \"=\" after identifier."<<std::endl;
					return;
				}
				if(temp->content!="=")
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Missing operator \"=\" after identifier."<<std::endl;
					return;
				}
				if(temp->next)
					temp=temp->next;
				else
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Missing value after operator \"=\" ."<<std::endl;
					return;
				}
				if(temp->type=="Number")
				{
					temp_var.isGlobal=true;
					if(isFloat(temp->content))
					{
						temp_var.Type="double";
						temp_var.data=new double;
						*((double *)temp_var.data)=double_Str2Num(temp->content);
						nasal_var_stack.append_var(name_of_var,temp_var);
						delete (double *)temp_var.data;
						temp_var.data=NULL;
					}
					else
					{
						temp_var.Type="long long int";
						temp_var.data=new long long int;
						*((long long int *)temp_var.data)=int_Str2Num(temp->content);
						nasal_var_stack.append_var(name_of_var,temp_var);
						delete (long long int *)temp_var.data;
						temp_var.data=NULL;
					}
				}
				else if(temp->type=="String")
				{
					temp_var.isGlobal=true;
					temp_var.Type="string";
					temp_var.data=new std::string;
					std::string temp_string="";
					for(int i=1;i<(int)temp->content.length()-1;++i)
						temp_string+=temp->content[i];
					*((std::string *)temp_var.data)=temp_string;
					nasal_var_stack.append_var(name_of_var,temp_var);
					delete (std::string *)temp_var.data;
					temp_var.data=NULL;
				}
				else if(temp->type=="Operator" && temp->content=="{")
				{
					bool make_pair=false;
					int cnt=1;
					while(temp->next)
					{
						temp=temp->next;
						if(temp->type=="Operator" && temp->content=="}")
						{
							--cnt;
							if(!cnt)
							{
								make_pair=true;
								break;
							}
						}
						else if(temp->type=="Operator" && temp->content=="{")
							++cnt;
					}
					if(!make_pair)
					{
						print_line_token(temp->line);
						std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \"}\"."<<std::endl;
						return;
					}
				}
				else if(temp->type=="Operator" && temp->content=="[")
				{
					bool make_pair=false;
					int cnt=1;
					while(temp->next)
					{
						temp=temp->next;
						if(temp->type=="Operator" && temp->content=="]")
						{
							--cnt;
							if(!cnt)
							{
								make_pair=true;
								break;
							}
						}
						else if(temp->type=="Operator" && temp->content=="[")
							++cnt;
					}
					if(!make_pair)
					{
						print_line_token(temp->line);
						std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \"]\"."<<std::endl;
						return;
					}
				}
				else
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Missing value after operator \"=\" ."<<std::endl;
					return;
				}
						
				if(!(temp->next && temp->next->content==";"))
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \";\" at the end of the statement."<<std::endl;
					nasal_var_stack.pop_var();
					return; 
				} 
				else
					temp=temp->next;
			//end var
			}
			else if(temp->content=="print")
			{
				if(temp->next && temp->next->content=="(")
				{
					temp=temp->next;
					while(temp->next)
					{
						temp=temp->next;
						if(temp->type=="String")
						{
							std::string temp_string="";
							for(int i=1;i<(int)temp->content.length()-1;++i)
								temp_string+=temp->content[i];
							PrintString(temp_string);
						}
						else if(temp->type=="Identifier")
							PrintVar(nasal_var_stack.SearchVar(temp->content));
						else if(temp->type=="Operator" && temp->content==")")
						{
							if(!temp->next)
							{
								print_line_token(temp->line);
								std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \";\" at the end of the statement."<<std::endl;
								return;
							}
							break;
						}
						else if(temp->type=="Operator" && temp->content==";")
						{
							print_line_token(temp->line);
							std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \")\" at the end of print."<<std::endl;
							return;
						}
					}
				}
				else
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \"(\" after function print."<<std::endl;
					return;
				}
				if(!(temp->next && temp->next->content==";"))
				{
					print_line_token(temp->line);
					std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \";\" at the end of the statement."<<std::endl;
					return;
				}
			}
		}
		else if(temp->type=="String")
		{
			print_line_token(temp->line);
			std::cout<<"line "<<temp->line<<": "<<"[Error] String without any tokens."<<std::endl;
			return;
		}
	}
	//nasal_var_stack.print_var(); //for debug mode
	//nasal_func_stack.print_function(); //for debug mode
	nasal_var_stack.delete_all();
	nasal_func_stack.delete_all();
	std::cout<<">> Running complete."<<std::endl;
	return;
}*/

#endif
