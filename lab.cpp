#include <iostream>
#include "var.h"
using namespace nasal;
using namespace std;

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
	
	n.Append(&n,"array");
	n.Append(&n,"array");
	n.PrintList();
	std::cout<<std::endl;
	
	NasalList rm;
	rm.Append(&a,"int");
	rm.Append(&a,"int");
	rm.Append(&a,"int");
	rm.PrintList();
	std::cout<<std::endl;
	m.Append("fifth",&rm,"array");
	m.Append("sixth",&m,"hash");
	m.PrintHash();
	std::cout<<std::endl;
	
	NasalHash test1(m);
	test1.PrintHash();
	NasalList test2(n);
	std::cout<<std::endl;
	test2.PrintList();
	std::cout<<std::endl;
	
	n.Append(&m,"hash");
	n.PrintList();
	std::cout<<std::endl<<std::endl;
	
	NasalList qt;
	qt.Append(&a,"int");
	a++;
	qt.Append(&a,"int");
	NasalHash qthash;
	qthash.Append("testlist",&qt,"array");
	qthash.PrintHash();
	std::cout<<std::endl;
	qthash.Append("int",&a,"int");
	qt.Append(&qthash,"hash");
	qt.PrintList();
	
	return 0;
}
