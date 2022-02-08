.PHONY=test
nasal:main.cpp nasal_ast.h nasal_err.h nasal_builtin.h nasal_opt.h nasal_codegen.h nasal_gc.h nasal_import.h nasal_lexer.h nasal_parse.h nasal_vm.h nasal_dbg.h nasal.h
	clang++ -std=c++11 -O3 main.cpp -o nasal -fno-exceptions -ldl -Wshadow -Wall
test:nasal
	./nasal -op -e test/ascii-art.nas
	./nasal -op -c test/bf.nas
	./nasal -op -c test/bfconvertor.nas
	./nasal -op -e -d test/bfs.nas
	./nasal -op -t test/bigloop.nas
	./nasal -op -e test/bp.nas
	./nasal -op -e -d test/calc.nas
	./nasal -op -e test/choice.nas
	./nasal -op -e test/class.nas
	./nasal -op -c test/exception.nas
	./nasal -op -t -d test/fib.nas
	./nasal -op -e test/filesystem.nas
	./nasal -op -e -d test/hexdump.nas
	./nasal -op -e test/json.nas
	./nasal -op -e test/leetcode1319.nas
	./nasal -op -e -d test/lexer.nas
	./nasal -op -e -d test/life.nas
	./nasal -op -t test/loop.nas
	./nasal -op -c test/mandel.nas
	./nasal -op -t -d test/mandelbrot.nas
	./nasal -op -c test/module_test.nas
	./nasal -op -e test/nasal_test.nas
	./nasal -op -t -d test/pi.nas
	./nasal -op -t -d test/prime.nas
	./nasal -op -t -d test/quick_sort.nas
	./nasal -op -e test/scalar.nas
	./nasal -op -e test/trait.nas
	./nasal -op -t -d test/turingmachine.nas
	./nasal -op -t -d test/ycombinator.nas
	