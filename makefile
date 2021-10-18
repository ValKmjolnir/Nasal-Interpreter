.PHONY=test
nasal:main.cpp nasal_ast.h nasal_builtin.h nasal_codegen.h nasal_gc.h nasal_import.h nasal_lexer.h nasal_parse.h nasal_vm.h nasal.h
	clang++ -std=c++11 -O3 main.cpp -o nasal -fno-exceptions
test:nasal
	./nasal test/ascii-art.nas
	./nasal test/bp.nas
	./nasal -t test/bigloop.nas
	./nasal -t test/fib.nas
	./nasal test/class.nas
	./nasal test/lexer.nas
	./nasal -t test/mandelbrot.nas
	./nasal -t test/pi.nas
	./nasal -t test/ycombinator.nas
	./nasal test/exception.nas