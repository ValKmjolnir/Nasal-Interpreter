.PHONY=clean
nasal:main.cpp nasal_ast.h nasal_builtin.h nasal_codegen.h nasal_gc.h nasal_import.h nasal_lexer.h nasal_parse.h nasal_vm.h nasal.h
	-@ clang++ -std=c++11 -O3 main.cpp -o nasal -fno-exceptions
clean:
	-@ rm nasal