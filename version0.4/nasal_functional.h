#ifndef __NASAL_FUNCTIONAL_H__
#define __NASAL_FUNCTIONAL_H__

bool is_float(std::string &str)
{
	for(int i=0;i<(int)str.length();++i)
		if(str[i]=='.')
			return true;
	return false;
}
long long int int_str2num(std::string &str)
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
double double_str2num(std::string &str)
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

#endif
