.PHONY=test
nasal:main.cpp nasal_ast.h nasal_err.h nasal_builtin.h nasal_codegen.h nasal_gc.h nasal_import.h nasal_lexer.h nasal_parse.h nasal_vm.h nasal_dbg.h nasal.h
	clang++ -std=c++11 -O3 main.cpp -o nasal -fno-exceptions -ldl -Wshadow -Wall
test:nasal
	./nasal test/ascii-art.nas
	./nasal -c test/bf.nas
	./nasal -c test/bfconvertor.nas
	./nasal test/bfs.nas
	./nasal -t test/bigloop.nas
	./nasal test/bp.nas
	./nasal test/calc.nas
	./nasal test/choice.nas
	./nasal test/class.nas
	./nasal -c test/exception.nas
	./nasal -t test/fib.nas
	./nasal test/filesystem.nas
	./nasal test/hexdump.nas
	./nasal test/json.nas
	./nasal test/leetcode1319.nas
	./nasal test/lexer.nas
	./nasal test/life.nas
	./nasal -t test/loop.nas
	./nasal -c test/mandel.nas
	./nasal -t test/mandelbrot.nas
	./nasal -c test/module_test.nas
	./nasal test/nasal_test.nas
	./nasal -t test/pi.nas
	./nasal -t test/prime.nas
	./nasal -t test/quick_sort.nas
	./nasal test/scalar.nas
	./nasal test/trait.nas
	./nasal -t test/turingmachine.nas
	./nasal -t test/ycombinator.nas
	