#include <iostream>
#include "nasal.h"
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
	std::cout<<std::endl;
	
	NasalList _l;
	_l.Append(&a,"int");
	_l.SetSize(10);
	_l.PrintList();
	std::cout<<std::endl;
	_l.SetSize(3);
	_l.PrintList();
	std::cout<<std::endl;
	
	_l.SetSize(0);
	_l.PrintList();
	std::cout<<std::endl;
	_l.Append(&a,"int");
	_l.Append(&a,"int");
	_l.Append(&a,"int");
	_l.Append(&a,"int");
	_l.Append(&a,"int");
	_l.Append(&a,"int");
	NasalList _k;
	_l.PrintList();
	std::cout<<std::endl;
	_k=_l.SubVec(9,4);
	_k.PrintList();
	std::cout<<std::endl;
	_k=_l.SubVec(1,10);
	_k.PrintList();
	std::cout<<std::endl;
	
	NasalList _m;
	var _temp=_k.Pop();
	_m.Append(&_temp,"var");
	_k.PrintList();
	std::cout<<std::endl;
	_m.PrintList();
	std::cout<<std::endl;
	qthash.Append("var",&_temp,"var");
	qthash.PrintHash();
	std::cout<<std::endl;
	
	NasalList _test_list;
	int _test_int=10;
	_test_list.Append(&_test_int,"int");
	_test_int--;
	_test_list.Append(&_test_int,"int");
	_test_int--;
	_test_list.Append(&_test_int,"int");
	_test_int--;
	_test_list.Append(&_test_int,"float");
	NasalList _test_temp_list;
	_test_temp_list=_test_list.Sort(SORT_INT,1);
	_test_temp_list.PrintList();
	std::cout<<std::endl;
	_test_list.Pop();
	_test_temp_list=_test_list.Sort(SORT_INT,-1);
	_test_temp_list.PrintList();
	std::cout<<std::endl;
	
	std::string _test_str;
	_test_list.SetSize(0);
	_test_str=".20.";
	_test_list.Append(&_test_str,"string");
	_test_str=".01.";
	_test_list.Append(&_test_str,"string");
	_test_str=".1";
	_test_list.Append(&_test_str,"string");
	_test_str=".3.";
	_test_list.Append(&_test_str,"string");
	_test_list.Append(&_test_int,"int");
	_test_temp_list=_test_list.Sort(SORT_STRING,1);
	_test_temp_list.PrintList();
	std::cout<<std::endl;
	_test_list.Pop();
	_test_temp_list=_test_list.Sort(SORT_STRING,1);
	_test_temp_list.PrintList();
	std::cout<<std::endl;
	return 0;
}
