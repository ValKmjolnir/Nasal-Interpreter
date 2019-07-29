#include <iostream>
#include <fstream>
#include <cstring>

#define IDENTR -1 //界符 or 运算符 
#define NUMBER -2 //数字 
#define RESERV -3 //关键字 
#define FAIL   -4
#define SCANEND -5 
#define ERROROCC -6
#define SELFD -7 //自定义标识符 
// \n 换行 
// \t tab
// \r 回车 
// \\ 反斜线 
// \' 单引号 
// \" 双引号 
std::string ReserveWord[25]=
{
	"for","foreach","forindex","while",
	"var","func","break","continue","return",
	"if","else","elsif","nil","and","or",
	"print","cmp","append","setsize","subvec","pop",
	"sort","contains","delete","keys"
};

std::string OperatorOrDelimiter[33]=
{
	"+","-","*","/","=","+=","-=","*=","/=",
	"\n","\t","\r","\\","\'","\"",".",
	"<","<=",">",">=","==","!=","!","~",
	",",";","(",")","[","]","{","}","#"
};

std::string IdentifierTable[1000]={""};
char ResourcePrograme[16384];

int isReserveWord(std::string &p)
{
	for(int i=0;i<25;++i)
		if(ReserveWord[i]==p)
			return i+1;
	return FAIL;
}

int isOperatorOrDelimiter(std::string &p)
{
	for(int i=0;i<33;++i)
		if(OperatorOrDelimiter[i]==p)
			return i+1;
	return FAIL;
}

bool isLetter(char t)
{
	return ('a'<=t && t<='z' || 'A'<=t && t<='Z');
}

bool isNumber(char t)
{
	return ('0'<=t && t<='9');
}

void InputFile(const char *FileName)
{
	std::ifstream fin(FileName);
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
	++i;
	for(int j=0;j<i;++j)
		std::cout<<ResourcePrograme[j];
	std::cout<<std::endl;
	fin.close();
	return;
}

void Scanner(int &Syn,const char Source[],std::string &token,int &ptr)
{
	char temp;
	temp=Source[ptr];
	while(temp==' ' || temp=='\n')
	{
		++ptr;
		temp=Source[ptr];
	}
	
	token="";
	if(isLetter(temp))
	{
		token+=temp;
		++ptr;
		temp=Source[ptr];
		while(isLetter(temp) || isNumber(temp) || temp==':' || temp=='?' || temp=='`' || temp=='.')
		{
			token+=temp;
			++ptr;
			temp=Source[ptr];
		}
		Syn=isReserveWord(token);
		if(Syn==FAIL)
			Syn=SELFD;
		else
			Syn=RESERV;
	}
	else if(isNumber(temp))
	{
		int PointCnt=0;
		token+=temp;
		++ptr;
		temp=Source[ptr];
		while(isNumber(temp))
		{
			token+=temp;
			++ptr;
			temp=Source[ptr];
			if(temp=='.')
				++PointCnt;
			else if(temp!='.' && !isNumber(temp))
				break;
			if(PointCnt>1)
				break;
		}
		Syn=NUMBER;
	}
	else if(temp=='(' || temp==')' || temp=='[' || temp==']' || temp=='{' || temp=='}' || temp=='~' || temp==',' || temp==';' || temp=='\"')
	{
		token+=temp;
		++ptr;
		Syn=IDENTR;
	}
	else if(temp=='=' || temp=='+' || temp=='-' || temp=='*' || temp=='!' ||
			temp=='/' || temp=='=' || temp=='\\' || temp=='.' || temp=='<' ||
			temp=='>'
			)
	{
		/*
		"+","-","*","/","=","+=","-=","*=","/=",
		"\n","\t","\r","\\","\'","\"",".",
		"<","<=",">",">=","==","!=","!","~",
		",",";","(",")","[","]","{","}","#"
		*/
		Syn=IDENTR;
		token+=temp;
		++ptr;
		temp=Source[ptr];
		if(temp=='=' || temp=='n' || temp=='t' || temp=='r' || temp=='\\' || temp=='\'' || temp=='\"')
		{
			token+=temp;
			++ptr;
		}
	}
	else if(temp=='@')
	{
		Syn=SCANEND;
		return;
	}
	else
	{
		Syn=SCANEND;
		return;
	}
	
	if(token=="")
	{
		Syn=ERROROCC;
		std::cout<<"[Error] Cannot identify special id."<<std::endl;
	}
	return;
}

int main()
{
	int Syn=0,Ptr=0;
	std::string token;
	InputFile("source.nas");
	while(Syn!=SCANEND && Syn!=ERROROCC)
	{
		Scanner(Syn,ResourcePrograme,token,Ptr);
		if(Syn==IDENTR)
			std::cout<<"( "<<token<<" , ---- )"<<std::endl;
		else if(Syn==SELFD)
			std::cout<<"( 标识符 , "<<token<<" )"<<std::endl;
		else if(Syn==NUMBER)
			std::cout<<"( Number , "<<token<<" )"<<std::endl;
		else if(Syn==RESERV)
			std::cout<<"( Reserve Word , "<<token<<" )"<<std::endl;
	}
	
	return 0;
}
