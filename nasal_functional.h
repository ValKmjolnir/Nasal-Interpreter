#ifndef __NASAL_FUNCTIONAL_H__
#define __NASAL_FUNCTIONAL_H__

#include <iostream>
#include <cstring>

#include "nasal_var_stack.h"
#include "nasal_func_stack.h"

namespace nasal
{

bool isFloat(std::string &str)
{
	for(int i=0;i<(int)str.length();++i)
		if(str[i]=='.')
			return true;
	return false;
}
long long int int_Str2Num(std::string &str)
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
double double_Str2Num(std::string &str)
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

#define FUNC_OPERATOR    1
#define FUNC_IDENTIFIER  2
#define FUNC_NUMBER      3
#define FUNC_RESERVEWORD 4
#define FUNC_STRING      5

var_stack nasal_var_stack;
func_stack nasal_func_stack;
var temp_var;
func temp_func;

struct token_unit
{
	std::string type;
	std::string content;
	int line;
	token_unit *next;
};

class token_list
{
	private:
		token_unit *head;
	public:
		token_list()
		{
			head=new token_unit;
			head->type="Begin";
			head->content="__process_begin";
			head->line=0;
			head->next=NULL;
		}
		~token_list()
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
		void delete_all()
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
		void append(const char *_type,std::string &_content,const int _line)
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
		void print()
		{
			token_unit *temp=head;
			std::cout<<"line "<<temp->line<<": "<<"( ProcessBegin | "<<temp->content<<" )"<<std::endl;
			if(!head->next)
				return;
			while(temp->next)
			{
				temp=temp->next;
				std::cout<<"line "<<temp->line<<": ";
				if(temp->type=="Operator")
					std::cout<<"( Operator     | ";
				else if(temp->type=="Identifier")
					std::cout<<"( Identifier   | ";
				else if(temp->type=="Number")
					std::cout<<"( Number       | ";
				else if(temp->type=="ReserveWord")
					std::cout<<"( ReserveWord  | ";
				else if(temp->type=="String")
					std::cout<<"( String       | ";
				std::cout<<temp->content<<" )"<<std::endl;
			}
			return;
		}
		void run()
		{
			token_unit *temp=head;
			if(!head->next)
			{
				std::cout<<"Running complete."<<std::endl;
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
					;
				}
				else if(temp->type=="ReserveWord")
				{
					if(temp->content=="var")
					{
						if(temp->next)
						{
							temp=temp->next;
							std::string name_of_var;
							if(temp->type=="Identifier")
								name_of_var=temp->content;
							else
							{
								std::cout<<"line "<<temp->line<<": "<<"[Error] Missing identifier after \"var\"."<<std::endl;
								return;
							}
							if(temp->next)
							{
								temp=temp->next;
								if(temp->content=="=")
								{
									if(temp->next)
									{
										temp=temp->next;
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
												std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \"}\"."<<std::endl;
												return;
											}
										//end {
										}
										
										if(!(temp->next && temp->next->content==";"))
										{
											std::cout<<"line "<<temp->line<<": "<<"[Error] Expect a \";\" at the end of the statement."<<std::endl;
											nasal_var_stack.pop_var();
											return; 
										} 
										else
											temp=temp->next;
									}
									else
									{
										std::cout<<"line "<<temp->line<<": "<<"[Error] Missing value after operator = ."<<std::endl;
										return;
									}
								}
								else
								{
									std::cout<<"line "<<temp->line<<": "<<"[Error] Missing \"=\" after identifier."<<std::endl;
									return;
								}
							//end operator
							}
							else
							{
								std::cout<<"line "<<temp->line<<": "<<"[Error] Missing elements after identifier."<<std::endl;
								return;
							}
						//end identifier
						}
						else
						{
							std::cout<<"line "<<temp->line<<": "<<"[Error] Missing identifier after \"var\"."<<std::endl;
							return;
						}
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
							}
						}
						else
						{
							std::cout<<"line "<<temp->line<<": "<<"[Error] Expect \"(\" after function print."<<std::endl;
							return;
						}
					}
				}
				else if(temp->type=="String")
				{
					;
				}
			}
			nasal_var_stack.print_var();
			nasal_func_stack.print_function();
			nasal_var_stack.delete_all();
			nasal_func_stack.delete_all();
			std::cout<<"Running complete."<<std::endl;
			return;
		}
};

}


#endif
