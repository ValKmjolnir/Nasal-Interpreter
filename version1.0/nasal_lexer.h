#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#include <iostream>
#include <fstream>
#include <list>
#include <cstring>

#define OPERATOR     1  // operator
#define IDENTIFIER   2  // id
#define NUMBER       3  // number
#define RESERVEWORD  4  // reserve word
#define STRING       5  // string 
#define DYNAMIC_ID   6 // id...
#define FAIL        -1  //fail
#define SCANEND     -2  //complete scanning
#define ERRORFOUND  -3  //error occurred

std::string reserve_word[15]=
{
	"for","foreach","forindex","while",
	"var","func","break","continue","return",
	"if","else","elsif","nil","and","or"
};

int isReserveWord(std::string &p)
{
	for(int i=0;i<15;++i)
		if(reserve_word[i]==p)
			return i+1;
	return FAIL;
}

bool isLetter(char t)
{
	return (('a'<=t) && (t<='z') || ('A'<=t) && (t<='Z'));
}

bool isNumber(char t)
{
	return (('0'<=t) && (t<='9'));
}

bool isHex(char t)
{
	return ((('0'<=t) && (t<='9')) || (('a'<=t) && (t<='f')));
}

bool isOct(char t)
{
	return (('0'<=t) && (t<='7'));
}

class resource_programme_process
{
	private:
		char *resource;
	public:
		resource_programme_process()
		{
			resource=NULL;
			resource=new char[16777216];
		}
		~resource_programme_process()
		{
			if(resource)
				delete []resource;
		}
		char* use_file()
		{
			return resource;
		}
		void input_file(std::string& filename)
		{
			std::ifstream fin(filename);
			if(fin.fail())
			{
				std::cout<<">>[Error] Cannot load file: "<<filename<<" ."<<std::endl;
				fin.close();
				return;
			}
			memset(resource,0,sizeof(char));
			int i=0;
			bool findnote=false;// to find the note with # at the head of line.
			while(!fin.eof())
			{
				resource[i]=fin.get();
				if(resource[i]=='\n')
					findnote=false;
				//when meeting '\n' the findnote is set to false then the next statement can be executed.
				if(resource[i]!='#' && !findnote)
					++i;
				else if(resource[i]=='#')
					findnote=true;
				if(fin.eof())
					break;
			}
			resource[i]='\0';
			fin.close();
			return;
		}
		void print_file()
		{
			if(!resource[0])
			{
				std::cout<<"0	null"<<std::endl;
				return;
			}
			int line=1;
			std::cout<<line<<"	";
			for(int i=0;i<16777216;++i)
			{
				if(!resource[i])
					break;
				std::cout<<resource[i];
				if(resource[i]=='\n')
				{
					++line;
					std::cout<<line<<"	";
				}
			}
			std::cout<<std::endl;
			return;
		}
		void del_file()
		{
			memset(resource,0,sizeof(char));
			return;
		}
};

struct token
{
	int line;
	int type;
	std::string content;
};

class nasal_lexer
{
	private:
		std::list<token> lexer;
	public:
		void scanner(int &syn,const char* source,std::string &__token,int &ptr,int &line)
		{
			char temp;
			temp=source[ptr];
			while(temp==' ' || temp=='\n' || temp=='\t' || temp=='\r' || temp<0 || temp>127)
			{
				++ptr;
				if(temp=='\n')
					++line;
				temp=source[ptr];
			}
			__token="";
			if(isLetter(temp) || temp=='_')
			{
				__token+=temp;
				++ptr;
				temp=source[ptr];
				while(isLetter(temp) || isNumber(temp) || temp=='_')
				{
					__token+=temp;
					++ptr;
					temp=source[ptr];
				}
				syn=isReserveWord(__token);
				if(syn==FAIL)
					syn=IDENTIFIER;
				else
					syn=RESERVEWORD;
				if((syn==IDENTIFIER) && source[ptr]=='.' && source[ptr+1]=='.' && source[ptr+2]=='.')
				{
					__token+="...";
					syn=DYNAMIC_ID;
					ptr+=3;
				}
			}
			else if(isNumber(temp))
			{
				if((source[ptr]=='0') && (source[ptr+1]=='x'))
				{
					__token+=source[ptr];
					__token+=source[ptr+1];
					ptr+=2;
					temp=source[ptr];
					while(isNumber(temp) || isHex(temp))
					{
						__token+=temp;
						++ptr;
						temp=source[ptr];
					}
				}
				else if((source[ptr]=='0') && (source[ptr+1]=='o'))
				{
					__token+=source[ptr];
					__token+=source[ptr+1];
					ptr+=2;
					temp=source[ptr];
					while(isNumber(temp) || isOct(temp))
					{
						__token+=temp;
						++ptr;
						temp=source[ptr];
					}
				}
				else
				{
					int PointCnt=0;
					while(isNumber(temp))
					{
						__token+=temp;
						++ptr;
						temp=source[ptr];
						if(temp=='.' && !PointCnt)
						{
							++PointCnt;
							__token+=temp;
							++ptr;
							temp=source[ptr];
						}
					}
				}
				syn=NUMBER;
			}
			else if(temp=='(' || temp==')' || temp=='[' || temp==']' || temp=='{' ||
					temp=='}' || temp==',' || temp==';' || temp=='|' || temp==':' ||
					temp=='?' || temp=='.' || temp=='`' || temp=='&' || temp=='@' ||
					temp=='%' || temp=='$' || temp=='^')
			{
				__token+=temp;
				++ptr;
				syn=OPERATOR;
			}
			else if(temp=='\'')
			{
				syn=STRING;
				__token+=temp;
				++ptr;
				temp=source[ptr];
				while(temp!='\'')
				{
					if(temp=='\\')
					{
						__token+=temp;
						
						++ptr;
						temp=source[ptr];
						__token+=temp;
						
						++ptr;
						temp=source[ptr];
					}
					else
					{
						__token+=temp;
						++ptr;
						temp=source[ptr];
					}
					if(temp==0 || temp=='\n')
						break;
				}
				//add the last char \"
				if(temp=='\'')
				{
					__token+=temp;
					++ptr;
				}
				else
					__token+=" __missing_end_of_string";
			}
			else if(temp=='=' || temp=='+' || temp=='-' || temp=='*' || temp=='!' || temp=='/' || temp=='<' || temp=='>' || temp=='~')
			{
				syn=OPERATOR;
				__token+=temp;
				++ptr;
				temp=source[ptr];
				if(temp=='=')
				{
					__token+=temp;
					++ptr;
				}
			}
			else if(temp=='\\')
			{
				syn=OPERATOR;
				__token+=temp;
				++ptr;
				temp=source[ptr];
				if(temp=='=' || temp=='n' || temp=='t' || temp=='r' || temp=='\\' || temp=='\'' || temp=='\"')
				{
					__token+=temp;
					++ptr;
				}
			}
			else if(temp=='\"')
			{
				syn=STRING;
				__token+=temp;
				++ptr;
				temp=source[ptr];
				while(temp!='\"')
				{
					if(temp=='\\')
					{
						__token+=temp;
						
						++ptr;
						temp=source[ptr];
						__token+=temp;
						
						++ptr;
						temp=source[ptr];
					}
					else
					{
						__token+=temp;
						++ptr;
						temp=source[ptr];
					}
					if(temp==0 || temp=='\n')
						break;
				}
				//add the last char \"
				if(temp=='\"')
				{
					__token+=temp;
					++ptr;
				}
				else
					__token+=" __missing_end_of_string";
			}
			else if(temp==0)
			{
				syn=SCANEND;
				return;
			}
			else
			{
				syn=FAIL;
				std::cout<<">>[Error] unexpected error occurred: "<<temp<<std::endl;
				system("pause");
				++ptr;
				return;
			}
			if(__token=="")
			{
				syn=ERRORFOUND;
				std::cout<<">>[Error] cannot identify "<<std::endl;
			}
			return;
		}
		void lexer_process(const char *source)
		{
			std::cout<<">>[Lexer] max size: "<<lexer.max_size()<<" ."<<std::endl;
			lexer.clear();
			int syn=0;//token type
			int ptr=0;//pointer to one char in ResourcePrograme
			int line=1;
			std::string __token;
			token temp;
			while(syn!=SCANEND && syn!=ERRORFOUND)
			{
				scanner(syn,source,__token,ptr,line);
				if(syn>0)//all Syn type is larger than zero
				{
					temp.line=line;
					temp.type=syn;
					temp.content=__token;
					lexer.push_back(temp);
				}
			}
			std::cout<<">>[Lexer] complete scanning."<<std::endl;
			return;
		}
		void print_lexer()
		{
			token temp;
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				temp=*i;
				std::cout<<"line "<<temp.line<<": ";
				if(temp.type==OPERATOR)
					std::cout<<"( Operator       | ";
				else if(temp.type==IDENTIFIER)
					std::cout<<"( Identifier     | ";
				else if(temp.type==NUMBER)
					std::cout<<"( Number         | ";
				else if(temp.type==RESERVEWORD)
					std::cout<<"( ReserveWord    | ";
				else if(temp.type==STRING)
					std::cout<<"( String         | ";
				else if(temp.type==DYNAMIC_ID)
					std::cout<<"( Identifier     | ";
				std::cout<<temp.content<<" )"<<std::endl;
			}
			return;
		}
		void token_list_type_detail_edit()
		{
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				if((*i).type==RESERVEWORD)
				{
					if((*i).content=="var")
						(*i).type=__var;
					else if((*i).content=="func")
						(*i).type=__func;
					else if((*i).content=="return")
						(*i).type=__return;
					else if((*i).content=="nil")
						(*i).type=__number;
					else if((*i).content=="continue")
						(*i).type=__continue;
					else if((*i).content=="break")
						(*i).type=__break;
					else if((*i).content=="and")
						(*i).type=__and_operator;
					else if((*i).content=="or")
						(*i).type=__or_operator;
					else if((*i).content=="for")
						(*i).type=__for;
					else if((*i).content=="forindex")
						(*i).type=__forindex;
					else if((*i).content=="foreach")
						(*i).type=__foreach;
					else if((*i).content=="while")
						(*i).type=__while;
					else if((*i).content=="if")
						(*i).type=__if;
					else if((*i).content=="else")
						(*i).type=__else;
					else if((*i).content=="elsif")
						(*i).type=__elsif;
				}
				else if(((*i).content=="==") || ((*i).content=="!=") || ((*i).content==">") || ((*i).content==">=") || ((*i).content=="<") || ((*i).content=="<="))
				{
					if((*i).content=="==")
						(*i).type=__cmp_equal;
					else if((*i).content=="!=")
						(*i).type=__cmp_not_equal;
					else if((*i).content==">")
						(*i).type=__cmp_more;
					else if((*i).content==">=")
						(*i).type=__cmp_more_or_equal;
					else if((*i).content=="<")
						(*i).type=__cmp_less;
					else if((*i).content=="<=")
						(*i).type=__cmp_less_or_equal;
				}
				else if(((*i).content==";") || ((*i).content==",") || ((*i).content=="=") || ((*i).content==":") || ((*i).content==".") || ((*i).content=="?") || ((*i).content=="%") || ((*i).content=="$") || ((*i).content=="`") || ((*i).content=="^") || ((*i).content=="@"))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case ';':(*i).type=__semi;break;
						case ',':(*i).type=__comma;break;
						case '=':(*i).type=__equal;break;
						case ':':(*i).type=__colon;break;
						case '.':(*i).type=__dot;break;
						default:(*i).type=__unknown_operator;break;
					}
				}
				else if(((*i).type==NUMBER) || ((*i).type==STRING) || ((*i).type==IDENTIFIER) || ((*i).type==DYNAMIC_ID))
				{
					int t=(*i).type;
					switch(t)
					{
						case NUMBER:(*i).type=__number;break;
						case STRING:(*i).type=__string;break;
						case IDENTIFIER:(*i).type=__id;break;
						case DYNAMIC_ID:(*i).type=__dynamic_id;break;
					}
				}
				else if(((*i).content=="+") || ((*i).content=="-") || ((*i).content=="*") || ((*i).content=="/") || ((*i).content=="~") || ((*i).content=="!"))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case '+':(*i).type=__add_operator;break;
						case '-':(*i).type=__sub_operator;break;
						case '*':(*i).type=__mul_operator;break;
						case '/':(*i).type=__div_operator;break;
						case '~':(*i).type=__link_operator;break;
						case '!':(*i).type=__nor_operator;break;
					}
				}
				else if(((*i).content=="+=") || ((*i).content=="-=") || ((*i).content=="*=") || ((*i).content=="/=") || ((*i).content=="~="))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case '+':(*i).type=__add_equal;break;
						case '-':(*i).type=__sub_equal;break;
						case '*':(*i).type=__mul_equal;break;
						case '/':(*i).type=__div_equal;break;
						case '~':(*i).type=__link_equal;break;
					}
				}
				else if(((*i).content=="(") || ((*i).content==")") || ((*i).content=="[") || ((*i).content=="]") || ((*i).content=="{") || ((*i).content=="}"))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case '(':(*i).type=__left_curve;break;
						case ')':(*i).type=__right_curve;break;
						case '[':(*i).type=__left_bracket;break;
						case ']':(*i).type=__right_bracket;break;
						case '{':(*i).type=__left_brace;break;
						case '}':(*i).type=__right_brace;break;
					}
				}
			}
			return;
		}
		std::list<token>& return_list()
		{
			return lexer;
		}
};
		

		

#endif
