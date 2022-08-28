.PHONY:test

SRC=\
	main.cpp\
	nasal_ast.h\
	nasal_err.h\
	nasal_builtin.h\
	nasal_opt.h\
	nasal_codegen.h\
	nasal_gc.h\
	nasal_import.h\
	nasal_lexer.h\
	nasal_parse.h\
	nasal_vm.h\
	nasal_dbg.h\
	nasal.h

STD=-std=c++11

nasal:$(SRC)
	$(CXX) $(STD) -O3 main.cpp -o nasal -fno-exceptions -ldl -Wshadow -Wall
nasal.exe:$(SRC)
	$(CXX) $(STD) -O3 main.cpp -o nasal.exe -fno-exceptions -Wshadow -Wall -static

stable-release:$(SRC)
	$(CXX) $(STD) -O2 main.cpp -o nasal -fno-exceptions -ldl -Wshadow -Wall
stable-release-mingw:$(SRC)
	$(CXX) $(STD) -O2 main.cpp -o nasal.exe -fno-exceptions -Wshadow -Wall -static

test:nasal
	@ ./nasal -o -e test/ascii-art.nas
	@ ./nasal -o -c test/auto_crash.nas
	@ ./nasal -o -a -c test/bf.nas
	@ ./nasal -o -a -c test/bfcolored.nas
	@ ./nasal -o -a -c test/bfconvertor.nas
	@ ./nasal -o -e -d test/bfs.nas
	@ ./nasal -o -t test/bigloop.nas
	@ ./nasal -o -e test/bp.nas
	@ ./nasal -o -e -d test/calc.nas
	@ ./nasal -o -e test/choice.nas
	@ ./nasal -o -e test/class.nas
	@ ./nasal -o -d test/coroutine.nas
	@ ./nasal -o -e test/diff.nas
	-@ ./nasal -o -d test/exception.nas
	@ ./nasal -o -t -d test/fib.nas
	@ ./nasal -o -e test/filesystem.nas
	@ ./nasal -o -e -d test/hexdump.nas
	@ ./nasal -o -c test/httptest.nas
	@ ./nasal -o -e test/json.nas
	@ ./nasal -o -e test/leetcode1319.nas
	@ ./nasal -o -e -d test/lexer.nas
	@ ./nasal -o -e -d test/life.nas
	@ ./nasal -o -t test/loop.nas
	@ ./nasal -o -t -d test/mandel.nas
	@ ./nasal -o -t -d test/mandelbrot.nas
	@ ./nasal -o -t -d test/md5.nas
	@ ./nasal -o -t -d test/md5compare.nas
	-@ ./nasal -o -d test/module_test.nas
	@ ./nasal -o -e test/nasal_test.nas
	@ ./nasal -o -c test/occupation.nas
	@ ./nasal -o -t -d test/pi.nas
	@ ./nasal -o -c test/ppmgen.nas
	@ ./nasal -o -t -d test/prime.nas
	@ ./nasal -o -e test/qrcode.nas
	@ ./nasal -o -t -d test/quick_sort.nas
	@ ./nasal -o -e test/scalar.nas hello world
	-@ ./nasal -o -c -t test/snake.nas
	@ ./nasal -o -c -e test/trait.nas
	-@ ./nasal -o -c -t test/tetris.nas
	@ ./nasal -o -c -t -d test/turingmachine.nas
	@ ./nasal -o -c -t -d test/ycombinator.nas
	@ ./nasal -o -d test/wavecollapse.nas