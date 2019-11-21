#include "balloon.h"

resource_file prog;
balloon_lexer lex;
balloon_parse pas;

int main()
{
	std::string command;
	std::cout<<">> Balloon interpreter by ValKmjolnir"<<std::endl;
	std::cout<<">> Input [help] to find help."<<std::endl;
	while(1)
	{
		std::cout<<">> ";
		std::getline(std::cin,command);
		if(command=="help")
		{
			std::cout<<">> Balloon interpreter by ValKmjolnir"<<std::endl;
			std::cout<<">> 1. [      ] |input file name to load the file."<<std::endl;
			std::cout<<">> 2. [cls   ] |clear the screen."<<std::endl;
			std::cout<<">> 3. [exit  ] |shut down the interpreter."<<std::endl;
			std::cout<<">> 4. [lexer ] |run and show the lexer. (-lexer)"<<std::endl;
			std::cout<<">> 5. [parser] |run parser and see parse stack & parse result(s). (-parser)"<<std::endl;
			std::cout<<">> 6. [ast   ] |print the abstract syntax tree."<<std::endl;
			std::cout<<">> 7. [del   ] |delete program in memory."<<std::endl;
			std::cout<<">> 8. [run   ] |run the programme in stack. (-lexer -parser)"<<std::endl;
			std::cout<<">> 9. [rs    ] |check the source program."<<std::endl;
			std::cout<<">>10. [total ] |check code with lib."<<std::endl;
			std::cout<<">>11. [sound ] |make noise(?)."<<std::endl;
		}
		else if(command=="cls")
		{
			system("cls");
			// Windows system("cls");
			// Linux   system("clear");
			// MacOS   system("clear");
		}
		else if(command=="rs")
			prog.print_file(false);// false means print resource code without lib
		else if(command=="exit")
			break;
		else if(command=="total")
		{
			prog.clear_lib_code();// clear lib code
			prog.add_lib();       // reload lib code to avoid redefinition
			prog.print_file(true);// true means print resource code with lib
		}
		else if(command=="lexer")
		{
			prog.clear_lib_code(); // avoid print lib code
			lex.scanner(prog.get_resource());
			lex.print_token_list();
		}
		else if(command=="del")
		{
			prog.set_clear();
			lex.set_clear();
			std::cout<<">>[Delete] Complete."<<std::endl;
		}
		else if(command=="parser")
		{
			prog.clear_lib_code(); // avoid print lib code
			lex.scanner(prog.get_resource());
			lex.generate_detail_token();
			if(!lex.get_error())
			{
				pas.get_detail_token_stream(lex.get_detail_token());
				pas.print_parse_stack();
				pas.parse_main();
			}
			else
			{
				alert_sound();
				std::cout<<">>[Lexer] error(s) found,stop."<<std::endl;
			}
		}
		else if(command=="ast")
		{
			prog.clear_lib_code(); // avoid print lib code
			lex.scanner(prog.get_resource());
			lex.generate_detail_token();
			if(!lex.get_error())
			{
				pas.get_detail_token_stream(lex.get_detail_token());
				pas.parse_main();
				if(!pas.get_error())
					pas.print_generated_tree();
				else
				{
					alert_sound();
					std::cout<<">>[Parse] error(s) found,stop."<<std::endl;
				}
			}
			else
			{
				alert_sound();
				std::cout<<">>[Lexer] error(s) found,stop."<<std::endl;
			}
		}
		else if(command=="run")
		{
			prog.add_lib(); // load lib code
			lex.scanner(prog.get_resource());
			lex.generate_detail_token();
			if(!lex.get_error())
			{
				pas.get_detail_token_stream(lex.get_detail_token());
				pas.parse_main();
				if(!pas.get_error())
					pas.run_tree();
				else
				{
					alert_sound();
					std::cout<<">>[Parse] error(s) found,stop."<<std::endl;
				}
			}
			else
			{
				alert_sound();
				std::cout<<">>[Lexer] error(s) found,stop."<<std::endl;
			}
		}
		else if(command=="sound")
			alert_sound();// check if the sound works
		else
			prog.input_file(command);
	}
	return 0;
}
