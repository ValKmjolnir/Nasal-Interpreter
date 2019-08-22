#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#include "nasal_functional.h"

#define FAIL        -1 // ß∞‹ 
#define SCANEND     -2 //…®√ËÕÍ≥… 
#define ERRORFOUND  -3 //“Ï≥£¥ÌŒÛ 

std::string ReserveWord[15]=
{
	"for","foreach","forindex","while",
	"var","func","break","continue","return",
	"if","else","elsif","nil","and","or"
};

std::string OperatorOrDelimiter[40]=
{
	"+","-","*","/","=","+=","-=","*=","/=",
	"\n","\t","\r","\\","\'","\"",".",
	"<","<=",">",">=","==","!=","~=","!","~",
	",",";","(",")","[","]","{","}","#","?",":",
	"&","|","%","^"
};

std::string IdentifierTable[1000]={""};
char ResourceProgram[16777216];

int isReserveWord(std::string &p)
{
	for(int i=0;i<15;++i)
		if(ReserveWord[i]==p)
			return i+1;
	return FAIL;
}

int isOperatorOrDelimiter(std::string &p)
{
	for(int i=0;i<40;++i)
		if(OperatorOrDelimiter[i]==p)
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

void InputFile(std::string &FileName)
{
	std::ifstream fin(FileName);
	int i=0;
	bool FindNote=false;
	while(!fin.eof())
	{
		ResourceProgram[i]=fin.get();
		if(ResourceProgram[i]=='\n')
			FindNote=false;
		if(ResourceProgram[i]!='#' && !FindNote)
			++i;
		else if(ResourceProgram[i]=='#')
		{
			FindNote=true;
		}
		if(fin.eof())
			break;
	}
	ResourceProgram[i]=0;

	fin.close();
	return;
}

void PrintSourceFile()
{
	int line=1;
	std::cout<<line<<"	";
	for(int i=0;i<16777216;++i)
	{
		if(!ResourceProgram[i])
			break;
		std::cout<<ResourceProgram[i];
		if(ResourceProgram[i]=='\n')
		{
			++line;
			std::cout<<line<<"	";
		}
	}
	std::cout<<std::endl;
}

void Scanner(int &Syn,const char Source[],std::string &token,int &ptr,int &line)
{
	char temp;
	temp=Source[ptr];
	while(temp==' ' || temp=='\n' || temp=='\t' || temp=='\r' || temp<0 || temp>127)
	{
		++ptr;
		if(temp=='\n')
			++line;
		temp=Source[ptr];
	}
	
	token="";
	if(isLetter(temp) || temp=='_')
	{
		token+=temp;
		++ptr;
		temp=Source[ptr];
		while(isLetter(temp) || isNumber(temp) || temp=='_')
		{
			token+=temp;
			++ptr;
			temp=Source[ptr];
		}
		Syn=isReserveWord(token);
		if(Syn==FAIL)
			Syn=IDENTIFIER;
		else
			Syn=RESERVEWORD;
	}
	else if(isNumber(temp))
	{
		int PointCnt=0;
		while(isNumber(temp))
		{
			token+=temp;
			++ptr;
			temp=Source[ptr];
			if(temp=='.' && !PointCnt)
			{
				++PointCnt;
				token+=temp;
				++ptr;
				temp=Source[ptr];
			}
		}
		Syn=NUMBER;
	}
	else if(temp=='(' || temp==')' || temp=='[' || temp==']' || temp=='{' ||
			temp=='}' || temp==',' || temp==';' || temp=='|' || temp==':' ||
			temp=='?' || temp=='.' || temp=='`' || temp=='\'' || temp=='&'||
			temp=='%' || temp=='$' || temp=='^')
	{
		token+=temp;
		++ptr;
		Syn=OPERATOR;
	}
	else if(temp=='=' || temp=='+' || temp=='-' || temp=='*' || temp=='!' || temp=='/' || temp=='<' || temp=='>' || temp=='~')
	{
		Syn=OPERATOR;
		token+=temp;
		++ptr;
		temp=Source[ptr];
		if(temp=='=')
		{
			token+=temp;
			++ptr;
		}
	}
	else if(temp=='\\')
	{
		Syn=OPERATOR;
		token+=temp;
		++ptr;
		temp=Source[ptr];
		if(temp=='=' || temp=='n' || temp=='t' || temp=='r' || temp=='\\' || temp=='\'' || temp=='\"')
		{
			token+=temp;
			++ptr;
		}
	}
	else if(temp=='\"')
	{
		Syn=STRING;
		token+=temp;
		++ptr;
		temp=Source[ptr];
		while(temp!='\"')
		{
			if(temp=='\\')
			{
				token+=temp;
				
				++ptr;
				temp=Source[ptr];
				token+=temp;
				
				++ptr;
				temp=Source[ptr];
			}
			else
			{
				token+=temp;
				++ptr;
				temp=Source[ptr];
			}
			if(temp==0 || temp=='\n')
				break;
		}
		//add the last char \"
		if(temp=='\"')
		{
			token+=temp;
			++ptr;
		}
		else
			token+=" __missing_end_of_string";
	}
	else if(temp==0)
	{
		Syn=SCANEND;
		return;
	}
	else
	{
		Syn=FAIL;
		std::cout<<"[Error] Unexpected error occurred: "<<temp<<std::endl;
		system("pause");
		++ptr;
		return;
	}
	if(token=="")
	{
		Syn=ERRORFOUND;
		std::cout<<"[Error] Cannot identify "<<std::endl;
	}
	return;
}

void RunProcess(std::string &FileName)
{
	int Syn=0;//token type
	int Ptr=0;//pointer to one char in ResourcePrograme
	int line=1;
	std::string token;
	std::ifstream fin(FileName);
	if(fin.fail())
	{
		std::cout<<">>[Error] Failed to load file: "<<FileName<<std::endl;
		fin.close();
		return;
	}
	else
		fin.close();
	ResourceProgram[0]=0;
	nasal_lexer.delete_all();
	nasal_var_stack.delete_all();
	nasal_func_stack.delete_all();
	InputFile(FileName);
	while(Syn!=SCANEND && Syn!=ERRORFOUND)
	{
		Scanner(Syn,ResourceProgram,token,Ptr,line);
		if(Syn>0)//all Syn type is larger than zero
			nasal_lexer.append(line,Syn,token);
	}
	//nasal_lexer.print(); //for debug mode
	std::cout<<">>[Lexer] Complete scanning \""<<FileName<<"\"."<<std::endl;
	return;
}

#endif
