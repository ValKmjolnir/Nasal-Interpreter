#include <iostream>
#include "var.h"
using namespace nasal;

int main()
{
	PrintString("This is a testing programme. \n");
	NasalHash m;
	int a=1;
	std::string k="hello world!";
	PrintString(k);
	m.Append("first",&a,"int");
	m.Append("second",&a,"int");
	m.Append("third",&k,"string");
	m.PrintHash();
	std::cout<<std::endl;
	PrintVar(m);
	std::cout<<std::endl;
	
	NasalList n;
	n=m.Keys();
	n.PrintList();
	std::cout<<std::endl;
	PrintVar(n);
	std::cout<<std::endl;
	
	m.Delete("fifth");
	m.Delete("second");
	m.PrintHash();
	
	n=m.Keys();
	std::cout<<std::endl;
	n.PrintList();
	std::cout<<std::endl;
	
	//n.Append(&n,"array","n");//cause infinite loop
	n.Append(&n,"array");
	n.Append(&n,"array");
	n.PrintList();
	std::cout<<std::endl;
	
	return 0;
}
