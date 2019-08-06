#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__


#include <iostream>
#include <fstream>
#include <cstring>
#include "nasal_functional.h"


namespace nasal
{

#define IDENTIFIER  -1 //自定义标识符 
#define OPERATOR    -2 //界符 or 运算符 
#define NUMBER      -3 //数字 
#define RESERVEWORD -4 //关键字 
#define STRING      -5 //字符串类型 
#define FAIL        -6 //失败 
#define SCANEND     -7 //扫描完成 
#define ERRORFOUND  -8 //异常错误 

std::string ReserveWord[26]=
{
	"for","foreach","forindex","while",
	"var","func","break","continue","return",
	"if","else","elsif","nil","and","or",
	"print","cmp","append","setsize","subvec","pop",
	"sort","contains","delete","keys","typeof"
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
char ResourcePrograme[16777216];

int isReserveWord(std::string &p)
{
	for(int i=0;i<26;++i)
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
	if(fin.fail())
	{
		std::cout<<"[Error] Failed to load file: "<<FileName<<std::endl;
		ResourcePrograme[0]='@';
		fin.close();
		return;
	}
	int i=0;
	bool FindNote=false;
	while(!fin.eof())
	{
		ResourcePrograme[i]=fin.get();
		if(ResourcePrograme[i]=='\n')
			FindNote=false;
		if(ResourcePrograme[i]!='#' && !FindNote)
			++i;
		else if(ResourcePrograme[i]=='#')
		{
			FindNote=true;
		}
		if(fin.eof())
			break;
	}
	ResourcePrograme[i]='@';
//	print source file
//	++i;
//	for(int j=0;j<i;++j)
//		std::cout<<ResourcePrograme[j];
//	std::cout<<std::endl;
	fin.close();
	return;
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
			if(temp=='@' || temp=='\n')
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
	else if(temp=='@')
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

token_list nasal_lexer;

void RunProcess(std::string &FileName)
{
	int Syn=0;//token type
	int Ptr=0;//pointer to one char in ResourcePrograme
	int line=1;
	std::string token;
	nasal_lexer.delete_all();
	
	InputFile(FileName);
	while(Syn!=SCANEND && Syn!=ERRORFOUND)
	{
		Scanner(Syn,ResourcePrograme,token,Ptr,line);
		if(Syn==OPERATOR)
			nasal_lexer.append("Operator",token,line);
		else if(Syn==IDENTIFIER)
			nasal_lexer.append("Identifier",token,line);
		else if(Syn==NUMBER)
			nasal_lexer.append("Number",token,line);
		else if(Syn==RESERVEWORD)
			nasal_lexer.append("ReserveWord",token,line);
		else if(Syn==STRING)
			nasal_lexer.append("String",token,line);
	}
	nasal_lexer.print();
	std::cout<<">> Complete scanning \""<<FileName<<"\"."<<std::endl;
	return;
}




}
#endif
