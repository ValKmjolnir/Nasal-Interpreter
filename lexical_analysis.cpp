#include <iostream>
#include <fstream>
#include <cstring>

#define SELFDEFINE -1 //自定义标识符 
#define IDENTIFIER -2 //界符 or 运算符 
#define NUMBER -3     //数字 
#define RESERVEWORD -4//关键字 
#define FAIL   -5     //失败 
#define SCANEND -6    //扫描完成 
#define ERRORFOUND -7 //异常错误 

// \n 换行 
// \t tab
// \r 回车 
// \\ 反斜线 
// \' 单引号 
// \" 双引号 
std::string ReserveWord[27]=
{
	"for","foreach","forindex","while",
	"var","func","break","continue","return",
	"if","else","elsif","nil","and","or",
	"print","cmp","append","setsize","subvec","pop",
	"sort","contains","delete","keys","typeof","id"
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
char ResourcePrograme[16384];

int isReserveWord(std::string &p)
{
	for(int i=0;i<27;++i)
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
	return ('a'<=t && t<='z' || 'A'<=t && t<='Z');
}

bool isNumber(char t)
{
	return ('0'<=t && t<='9');
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
	while(temp==' ' || temp=='\n' || temp=='\t' || temp=='\r' || temp<0 || temp>127)
	{
		++ptr;
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
			Syn=SELFDEFINE;
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
			temp=='}' || temp==',' || temp==';' || temp=='\"' || temp==':' ||
			temp=='?' || temp=='.' || temp=='`' || temp=='\'' || temp=='&' ||
			temp=='|')
	{
		token+=temp;
		++ptr;
		Syn=IDENTIFIER;
	}
	else if(temp=='=' || temp=='+' || temp=='-' || temp=='*' || temp=='!' || temp=='/' || temp=='<' || temp=='>' || temp=='~')
	{
		Syn=IDENTIFIER;
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
		/*
		"+","-","*","/","=","+=","-=","*=","/=",
		"\n","\t","\r","\\","\'","\"",".",
		"<","<=",">",">=","==","!=","!","~",
		",",";","(",")","[","]","{","}","#"
		*/
		Syn=IDENTIFIER;
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
		Syn=FAIL;
		std::cout<<"[Error] Unexpected error occurred: "<<temp<<std::endl;
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

void help()
{
	std::cout<<">> exit: exit the programe."<<std::endl;
	std::cout<<">> clear: clean the screen."<<std::endl;
	std::cout<<">> help: find help."<<std::endl;
	std::cout<<">> input the file name to scan."<<std::endl;
}

int main()
{
	help();
	while(1)
	{
		int Syn=0;//token type
		int Ptr=0;//pointer to one char in ResourcePrograme
		std::string token;
		std::string FileNameOrCommand;
		std::cout<<">> ";
		std::cin>>FileNameOrCommand;
		
		if(FileNameOrCommand=="exit")
			break;
		else if(FileNameOrCommand=="clear")
		{
			system("cls");
			continue;
		}
		else if(FileNameOrCommand=="help")
		{
			help();
			continue;
		}
		//std::ofstream fout("Data.txt");
		InputFile(FileNameOrCommand);
		while(Syn!=SCANEND && Syn!=ERRORFOUND)
		{
			Scanner(Syn,ResourcePrograme,token,Ptr);
			if(Syn==IDENTIFIER)
				std::cout<<"( "<<token<<" , ---- )"<<std::endl;
			else if(Syn==SELFDEFINE)
				std::cout<<"( 标识符 , "<<token<<" )"<<std::endl;
			else if(Syn==NUMBER)
				std::cout<<"( Number , "<<token<<" )"<<std::endl;
			else if(Syn==RESERVEWORD)
				std::cout<<"( Reserve Word , "<<token<<" )"<<std::endl;
		}
		std::cout<<">> Complete scanning \""<<FileNameOrCommand<<"\"."<<std::endl;
		//fout.close();
	}
	return 0;
}
