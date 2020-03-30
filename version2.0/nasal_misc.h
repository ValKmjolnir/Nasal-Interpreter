#ifndef __NASAL_MISC_H__
#define __NASAL_MISC_H__

/*
	check_numerable_string:
	check if a string can be converted to a number
	
	strings like these below is correct:
	'0.00012'
	'12314.234'
	'1234'
	'0xdeadbeef'
	'0xDEADBEEF'
	'0o71230'
*/
bool check_numerable_string(std::string str)
{
	if(str.length()>1 && str[0]=='-')
	{
		// in lexer this statements are useless
		// because lexer judge a number that begins with 0~9 (or 0x for hex & 0o for oct)
		std::string temp="";
		int str_len=str.length();
		for(int i=1;i<str_len;++i)
			temp+=str[i];
		str=temp;
	}
	if(str.length()==1 && '0'<=str[0] && str[0]<='9')
		return true;
	else if(str.length()>2 && str[0]=='0' && str[1]=='x')
	{
		int str_len=str.length();
		for(int i=2;i<str_len;++i)
			if(!(('0'<=str[i] && str[i]<='9') || ('a'<=str[i] && str[i]<='f') || ('A'<=str[i] && str[i]<='F')))
				return false;
		return true;
	}
	else if(str.length()>2 && str[0]=='0' && str[1]=='o')
	{
		int str_len=str.length();
		for(int i=2;i<str_len;++i)
			if(!('0'<=str[i] && str[i]<='7'))
				return false;
		return true;
	}
	else
	{
		int str_len=str.length();
		int float_dot_cnt=0;
		for(int i=0;i<str_len;++i)
		{
			if(str[i]=='.')
				++float_dot_cnt;
			else if(!('0'<=str[i] && str[i]<='9'))
				return false;
		}
		if((float_dot_cnt>1) || (str[0]=='.') || (!float_dot_cnt && str[0]=='0'))
			return false;
		return true;
	}
	return false;
}

/*
	trans_string_to_number:
	convert string to number
*/
double trans_string_to_number(std::string str)
{
	bool is_negative=false;
	if(str.length()>1 && str[0]=='-')
	{
		// in parse this statements are useless
		// because lexer recognizes a number that begins with a '0'~'9' char
		std::string temp="";
		int str_len=str.length();
		for(int i=1;i<str_len;++i)
			temp+=str[i];
		str=temp;
		is_negative=true;
	}
	double trans_str_number=0;
	if(str.length()==1)
		trans_str_number=(double)(str[0]-'0');
	else if(str[0]=='0' && str[1]=='x')
	{
		long long int hex_number=0,bit_pow=1;
		for(int i=str.length()-1;i>1;--i)
		{
			if('0'<=str[i] && str[i]<='9')
				hex_number+=bit_pow*(str[i]-'0');
			else if('a'<=str[i] && str[i]<='f')
				hex_number+=bit_pow*(10+str[i]-'a');
			else
				hex_number+=bit_pow*(10+str[i]-'A');
			bit_pow<<=4;
		}
		trans_str_number=(double)hex_number;
	}
	else if(str[0]=='0' && str[1]=='o')
	{
		long long int oct_number=0,bit_pow=1;
		for(int i=str.length()-1;i>1;--i)
		{
			oct_number+=bit_pow*(str[i]-'0');
			bit_pow<<=3;
		}
		trans_str_number=(double)oct_number;
	}
	else
	{
		int float_dot_place=str.length();
		int str_len=str.length();
		double pow;
		for(int i=0;i<str_len;++i)
			if(str[i]=='.')
			{
				float_dot_place=i;
				break;
			}
		pow=0.1;
		for(int i=float_dot_place+1;i<str_len;++i)
		{
			trans_str_number+=pow*(double(str[i]-'0'));
			pow/=10;
		}
		pow=1;
		for(int i=float_dot_place-1;i>=0;--i)
		{
			trans_str_number+=pow*(double(str[i]-'0'));
			pow*=10;
		}
	}
	if(is_negative)
		trans_str_number*=-1;
	return trans_str_number;
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
		number*=-1;
	}
	double integer_bit=1;
	while(number>=integer_bit)
		integer_bit*=10;
	integer_bit/=10;
	while(integer_bit!=0.1)
	{
		trans_num_string+=(char)('0'+(int(number/integer_bit)));
		number-=(double)(int(number/integer_bit))*integer_bit;
		integer_bit/=10;
	}
	if(number!=0)
		trans_num_string+='.';
	while(number!=0)
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
