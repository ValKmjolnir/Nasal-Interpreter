#ifndef __NASAL_MISC_H__
#define __NASAL_MISC_H__

/*
	check if a string can be converted to a number
	
	strings like these below is correct:
	'0.00012'
	'12314.234'
	'1234'
	'0xdeadbeef'
	'0xDEADBEEF'
	'0o71230'
	'1e23'
	'1E-123'
	'1.34E10'

	if this string cannot be converted to a number,it will return nan
*/
inline double hex_to_double(std::string str,int len)
{
	double ret=0,num_pow=1;
	for(int i=len-1;i>1;--i)
	{
		if('0'<=str[i] && str[i]<='9')
			ret+=num_pow*(str[i]-'0');
		else if('a'<=str[i] && str[i]<='f')
			ret+=num_pow*(str[i]-'a'+10);
		else if('A'<=str[i] && str[i]<='F')
			ret+=num_pow*(str[i]-'A'+10);
		else
			return (1/0.0)+(-1/0.0);
		num_pow*=16;
	}
	return ret;
}
inline double oct_to_double(std::string str,int len)
{
	double ret=0,num_pow=1;
	for(int i=len-1;i>1;--i)
	{
		if('0'<=str[i] && str[i]<='8')
			ret+=num_pow*(str[i]-'0');
		else
			return (1/0.0)+(-1/0.0);
		num_pow*=8;
	}
	return ret;
}
inline double dec_to_double(std::string str,int len)
{
	double ret=0;
	int i=0;
	while('0'<=str[i] && str[i]<='9' && i<len)
	{
		ret=ret*10+(str[i]-'0');
		++i;
	}
	if(i==len) return ret;
	if(str[i]!='.' && str[i]!='e' && str[i]!='E')
		return (1/0.0)+(-1/0.0);
	if(str[i]=='.')
	{
		++i;
		if(i==len)
			return (1/0.0)+(-1/0.0);
		double num_pow=0.1;
		while('0'<=str[i] && str[i]<='9' && i<len)
		{
			ret+=num_pow*(str[i]-'0');
			num_pow*=0.1;
			++i;
		}
	}
	if(i==len) return ret;
	if(str[i]!='e' && str[i]!='E')
		return (1/0.0)+(-1/0.0);
	if(str[i]=='e' || str[i]=='E')
	{
		++i;
		if(i==len)
			return (1/0.0)+(-1/0.0);
		double negative=(str[i]=='-'? -1:1);
		if(str[i]=='-' || str[i]=='+')
			++i;
		if(i==len)
			return (1/0.0)+(-1/0.0);
		double num_pow=0;
		for(;i<len;++i)
		{
			if('0'<=str[i] && str[i]<='9')
				num_pow=num_pow*10+(str[i]-'0');
			else
				return (1/0.0)+(-1/0.0);
		}
		num_pow=std::pow(10,negative*num_pow);
		ret*=num_pow;
	}
	return ret;
}
double trans_string_to_number(std::string str)
{
	bool is_negative=false;
	int len=str.length();
	double ret_num=0;
	if(!len)
		return (1/0.0)+(-1/0.0);
	if(str[0]=='-' || str[0]=='+')
	{
		is_negative=(str[0]=='-');
		std::string tmp="";
		for(int i=1;i<len;++i)
			tmp.push_back(str[i]);
		str=tmp;
		--len;
		if(!len)
			return (1/0.0)+(-1/0.0);
	}
	if(len>2 && str[0]=='0' && str[1]=='x')
		ret_num=hex_to_double(str,len);
	else if(len>2 && str[0]=='0' && str[1]=='o')
		ret_num=oct_to_double(str,len);
	else
		ret_num=dec_to_double(str,len);
	return is_negative?-ret_num:ret_num;
}

/*
	trans_number_to_string:
	convert number to string
*/
std::string trans_number_to_string(double number)
{
	std::string trans_num_string="";
	if(number<0)
	{
		trans_num_string+='-';
		number=-number;
	}
	if(number==0)
		return "0";
	double integer_bit=1;
	while(number>=integer_bit)
		integer_bit*=10;
	integer_bit/=10;
	if(integer_bit==0.1)
		trans_num_string+='0';
	while(integer_bit!=0.1)
	{
		trans_num_string+=(char)('0'+(int(number/integer_bit)));
		number-=(double)(int(number/integer_bit))*integer_bit;
		integer_bit/=10;
	}
	if(number>0.000000001)
		trans_num_string+='.';
	while(number>0.000000001)
	{
		trans_num_string+=(char)('0'+int(number*10));
		number*=10;
		number-=(double)(int(number));
	}
	return trans_num_string;
}

/*
	prt_hex:
	transform int to hex format and print it out (std::cout)
*/
void prt_hex(const int ptr)
{
	char hex[9];
	hex[8]=0;
	int tmp_plc=ptr;
	if(tmp_plc<0)
	{
		tmp_plc=-tmp_plc;
		std::cout<<"-0x";
	}
	else
		std::cout<<"0x";
	/*
		int: 00000000 00000000 00000000 00000000
		int: 0x00 00 00 00
		example:
			a=0x13 57 9b df
			a=00010011 01010111 10011011 11011111
			a & 0x00 00 00 0f:
				00010011 01010111 10011011 11011111
			and	00000000 00000000 00000000 00001111
			---------------------------------------
				00000000 00000000 00000000 00001111
			a>>=4:
				00000001 00110101 01111001 10111101
			a & 0x00 00 00 0f
				00000001 00110101 01111001 10111101
			and 00000000 00000000 00000000 00001111
			---------------------------------------
				00000000 00000000 00000000 00001101
			then convert 0~15 to 0~9 a~f
	*/
	for(int j=7;j>=0;--j)
	{
		int tmp=(tmp_plc & 0x0000000f);
		hex[j]=tmp<10? (char)('0'+tmp):(char)('a'+tmp-10);
		tmp_plc>>=4;
	}
	std::cout<<hex;
	return;
}

#endif
