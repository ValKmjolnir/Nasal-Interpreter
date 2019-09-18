#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

#include "nasal_token_type.h"

class nasal_parser
{
	private:
		ast_tree_node root;
		std::stack<token> parse;
		std::stack<ast_tree_node> node_cache;
		token this_token;
		bool error;
	public:
		nasal_parser()
		{
			error=false;
			this_token.type=0;
		}
		void get_token()
		{
			if(parse.empty())
			{
				this_token.type=0;
				return;
			}
			this_token=parse.top();
			parse.pop();
			return;
		}
		void number_expr_process()
		{
			get_token();
			if(this_token.type==__semi)
				return;
			return;
		}
		void string_expr_process()
		{
			get_token();
			if(this_token.type==__semi)
				return;
			return;
		}
		void identifier_call()
		{
			get_token();
			if(this_token.type==__left_curve)
			{
				return;
			}
			else if(this_token.type==__left_brace)
			{
				return;
			}
			else if(this_token.type==__dot)
			{
				return;
			}
			else if(this_token.type==__semi)
				return;
			return;
		}
		void definition()
		{
			get_token();
			if(this_token.type!=__id)
			{
				std::cout<<">>[Error] parse error in line "<<this_token.line<<": incorrect token behind the declarator."<<std::endl;
				error=true;
				return;
			}
			get_token();
			if(this_token.type!=__equal && this_token.type!=__semi)
			{
				std::cout<<">>[Error] parse error in line "<<this_token.line<<": expect \'=\' behind the identifier."<<std::endl;
				error=true;
				return;
			}
			else if(this_token.type==__semi)
			{
				std::cout<<">>[Error] [-Warning] in line "<<this_token.line<<": new variable without initialization."<<std::endl;
				error=true;
				return;
			}
			/*
			
			
			
			*/
			get_token();
			if(this_token.type!=__number && this_token.type!=__string)
			{
				std::cout<<">>[Error] parse error in line "<<this_token.line<<": missing element after \'=\'."<<std::endl;
				error=true;
				return;
			}
			get_token();
			if(this_token.type!=__semi)
			{
				std::cout<<">>[Error] parse error in line "<<this_token.line<<": expect \';\' at the end of the statement."<<std::endl;
				error=true;
				return;
			}
			return;
		}
		void parse_main_work()
		{
			while(!parse.empty())
			{
				get_token();
				switch(this_token.type)
				{
					case __var:definition();break;
					case __id:identifier_call();break;
					case __number:number_expr_process();break;
					case __string:string_expr_process();break;
					case __if:break;
					case __while:break;
					case __for:break;
					case __foreach:break;
					case __forindex:break;
					case __semi:break;
					default:
						std::cout<<">>[Error] parse error in line "<<this_token.line<<": ";
						print_token(this_token.type);
						std::cout<<" in an incorrect place."<<std::endl;
						error=true;
						break;
				}
			}
			return;
		}
		void print_parser_stack()
		{
			if(parse.empty())
				return;
			int line=0;
			std::stack<token> temp;
			while(!parse.empty())
			{
				temp.push(parse.top());
				if(line!=temp.top().line)
				{
					if(line+1==temp.top().line)
					{
						line=temp.top().line;
						if(line==1)
							std::cout<<"line "<<line<<"    ";
						else
							std::cout<<std::endl<<"line "<<line<<"    ";
					}
					else
					{
						while(line!=temp.top().line)
						{
							++line;
							std::cout<<std::endl<<"line "<<line<<"    ";
						}
					}
				}
				std::cout<<"<";
				print_token(temp.top().type);
				std::cout<<"> ";
				parse.pop();
			}
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			std::cout<<std::endl;
			return;
		}
		void parse_process(std::list<token>& lexer)
		{
			while(!parse.empty())
				parse.pop();
			std::stack<token> temp;
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				token temp_parse;
				temp_parse=*i;
				temp.push(temp_parse);//push this into stack
			}
			if(temp.empty())
			{
				std::cout<<">>[Parse] [-Warning] Empty lexer list."<<std::endl;
				return;
			}
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			return;
		}
		
		
		
		
		
		
		
		
		
		
		
		
		// for building abstract syntax tree
		void number_gen()
		{
			number_expr t;
			t.set_line(parse.top().line);
			t.set_number(parse.top().content);
			if(node_cache.empty())
				node_cache.push(t);
			else if((node_cache.top().return_type()==__add_operator || node_cache.top().return_type()==__sub_operator || node_cache.top().return_type()==__mul_operator || node_cache.top().return_type()==__div_operator)
					&& (node_cache.top().child_num()==1))
				node_cache.top().add_child(t);
			else
			{
				std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing \';\' at this place."<<std::endl;
				error=true;
			}
			return;
		}
		void add_sub_operator_gen()
		{
			operator_expr opt;
			opt.set_line(parse.top().line);
			opt.set_operator_type(parse.top().type);
			if(node_cache.empty())
			{
				number_expr t;
				t.set_line(parse.top().line);
				std::string temp_str="0";
				t.set_number(temp_str);
				opt.add_child(t);
				node_cache.push(opt);
			}
			else if(node_cache.top().return_type()==__number
					|| ((node_cache.top().return_type()==__add_operator
					|| node_cache.top().return_type()==__sub_operator
					|| node_cache.top().return_type()==__mul_operator
					|| node_cache.top().return_type()==__div_operator)
					&& node_cache.top().child_num()==2))
			{
				opt.add_child(node_cache.top());
				node_cache.pop();
				node_cache.push(opt);
			}
			else
			{
				std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number/calculation at this place."<<std::endl;
				error=true;
			}
			return;
		}
		void mul_div_operator_gen()
		{
			operator_expr opt;
			opt.set_line(parse.top().line);
			opt.set_operator_type(parse.top().type);
			if(node_cache.empty())
			{
				std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number at this place(you cannot use * or / as the begin of calculation)."<<std::endl;
				error=true;
				return;
			}
				
			if(node_cache.top().return_type()==__number)
			{
				// add new leaf node number to operator
				opt.add_child(node_cache.top());
				node_cache.pop();
				node_cache.push(opt);
			}
			else if(node_cache.top().return_type()==__add_operator || node_cache.top().return_type()==__sub_operator)
			{
				if(node_cache.top().child_num()!=2)
				{
					std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number at this place."<<std::endl;
					error=true;
				}
				else
				{
					// calcu * and / first so pick a number from +-'s children nodes
					opt.add_child(node_cache.top().return_last_child());
					parse.pop();
					// add the next number to fix the operator node's children list
					if(!parse.empty() && parse.top().type==__number)
					{
						number_expr t;
						t.set_line(parse.top().line);
						t.set_number(parse.top().content);
						opt.add_child(t);
					}
					else
					{
						std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number at this place."<<std::endl;
						error=true;
						return;
					}
					node_cache.top().return_last_child()=opt;
				}
			}
			else if(node_cache.top().return_type()==__mul_operator || node_cache.top().return_type()==__div_operator)
			{
				if(node_cache.top().child_num()!=2)
				{
					std::cout<<">>[Error] parse error in line "<<parse.top().line<<":missing number at this place."<<std::endl;
					error=true;
				}
				else
				{
					opt.add_child(node_cache.top());
					node_cache.pop();
					node_cache.push(opt);
				}
			}
			return;
		}
		void gen_main_prog()
		{
			error=false;
			root.clear_tree();
			// initializing
			
			while(!node_cache.empty())
				node_cache.pop();
			while(!parse.empty())
			{
				int type=parse.top().type;
				bool is_correct=false;
				switch(type)
				{
					case __number:number_gen();break;
					case __add_operator:add_sub_operator_gen();break;
					case __sub_operator:add_sub_operator_gen();break;
					case __mul_operator:mul_div_operator_gen();break;
					case __div_operator:mul_div_operator_gen();break;
					case __semi:
						if(!node_cache.empty())
						{
							root.add_child(node_cache.top());
							node_cache.pop();
						}
						break;
					default:
						std::cout<<">>[Error] parse error in line "<<parse.top().line<<":";
						print_token(parse.top().type);
						std::cout<<" at an incorrect place."<<std::endl;
						error=true;
						break;
				}
				parse.pop();
			}
			if(error)
			{
				std::cout<<">>[Parse] Error(s) occurred."<<std::endl;
				std::cout<<">>[Parse] Complete parsing."<<std::endl;
			}
			else
			{
				std::cout<<">>[Parse] 0 error."<<std::endl;
				std::cout<<">>[Parse] Complete parsing."<<std::endl;
			}
			return;
		}
		void print_ast()
		{
			std::cout<<">>[Abstract-syntax-tree]"<<std::endl;
			root.print(0);
			return;
		}
		void run()
		{
			if(!error)
				root.run();
			else
				std::cout<<">>[Parse] Error(s) occurred,stop."<<std::endl;
			return;
		}
};

#endif
