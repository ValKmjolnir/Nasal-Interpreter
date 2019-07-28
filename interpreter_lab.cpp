#include <iostream>
#include "process_stack.h"
#include "lexer.h"
using namespace std;
using namespace nasal;

int main()
{
	
	_unit.line=1;
	_unit.type="function";
	_unit.Name="__main()";
	_unit.unitdata.Type="string";
	_unit.unitdata.data=new std::string;
	*((std::string *)_unit.unitdata.data)="in function main.";
	_unit.global=true;
	_test.stack_append(_unit);
	
	_unit.line=2;
	_unit.type="int";
	_unit.Name="NewInt";
	_unit.global=true;
	_unit.unitdata.Type="int";
	_unit.unitdata.data=new int;
	*((int *)_unit.unitdata.data)=2147483647;
	_test.stack_append(_unit);
	
	_unit.line=3;
	_unit.type="int";
	_unit.Name="ptr";
	_unit.global=true;
	_unit.unitdata.Type="int";
	*((int *)_unit.unitdata.data)=1073741824;
	_test.stack_append(_unit);
	//_test.stack_print(true);
	_test.stack_print(false);
	
	
	CommandProcess();
	return 0;
}
