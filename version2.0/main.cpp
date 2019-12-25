#include "nasal.h"

resource_file resource;
nasal_lexer lexer;
nasal_parse parser;

std::string command;

int main()
{
	std::cout<<">> Nasal interpreter by github:ValKmjolnir"<<std::endl;
	std::cout<<">> Input \"help\" to get help ."<<std::endl;
	while(1)
	{
		std::cout<<">> ";
		std::cin>>command;
		if(command=="help")
		{
			std::cout<<">> [\'file\'] input a file."<<std::endl;
			std::cout<<">> [cls   ] clear the screen."<<std::endl;
			std::cout<<">> [del   ] clear the resource code."<<std::endl;
			std::cout<<">> [lib   ] add lib file."<<std::endl;
			std::cout<<">> [rs    ] print resource code."<<std::endl;
			std::cout<<">> [total ] print resource code with lib code."<<std::endl;
			std::cout<<">> [lexer ] turn code into tokens."<<std::endl;
			std::cout<<">> [parser] turn tokens into abstract syntax tree."<<std::endl;
			std::cout<<">> [ast   ] check the abstract syntax tree."<<std::endl;
			std::cout<<">> [exit  ] quit nasal interpreter."<<std::endl;
		}
		else if(command=="cls")
		{
			system("cls");
			// windows system("cls");
			// linux   system("clear");
			// macOS   system("clear");
		}
		else if(command=="del")
		{
			resource.delete_all_source();
			std::cout<<">>[Delete] complete."<<std::endl;
		}
		else if(command=="lib")
		{
			resource.load_lib_file();
			std::cout<<">>[Lib] loaded."<<std::endl;
		}
		else if(command=="rs")
		{
			resource.print_resource(false);
		}
		else if(command=="total")
		{
			resource.print_resource(true);
		}
		else if(command=="lexer")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
				lexer.print_token_list();
			else
				std::cout<<">>[Lexer] error occurred,stop."<<std::endl;
		}
		else if(command=="parser")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.print_detail_token();
				parser.main_generate();
			}
			else
				std::cout<<">>[Lexer] error occurred,stop."<<std::endl;
		}
		else if(command=="ast")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.main_generate();
				if(!parser.get_error())
					parser.get_root().print_tree(1);
				else
					std::cout<<">>[Parse] error occurred,stop."<<std::endl;
			}
			else
				std::cout<<">>[Lexer] error occurred,stop."<<std::endl;
		}
		else if(command=="exit")
			break;
		else
			resource.input_file(command);
	}
	return 0;
}
