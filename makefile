.PHONY:test clean

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

STD=c++14

nasal:$(SRC)
	$(CXX) -std=$(STD) -O3 main.cpp -o nasal -fno-exceptions -ldl -Wshadow -Wall
nasal.exe:$(SRC)
	$(CXX) -std=$(STD) -O3 main.cpp -o nasal.exe -fno-exceptions -Wshadow -Wall -static

stable-release:$(SRC)
	$(CXX) -std=$(STD) -O2 main.cpp -o nasal -fno-exceptions -ldl -Wshadow -Wall
stable-release-mingw:$(SRC)
	$(CXX) -std=$(STD) -O2 main.cpp -o nasal.exe -fno-exceptions -Wshadow -Wall -static

clean:
	-@ rm ./nasal
	-@ rm ./nasal.exe

test:nasal
	@ ./nasal -e test/ascii-art.nas
	@ ./nasal -c test/auto_crash.nas
	@ ./nasal -a -c test/bf.nas
	@ ./nasal -a -c test/bfconvertor.nas
	@ ./nasal -d test/bfs.nas
	@ ./nasal -t test/bigloop.nas
	@ ./nasal -t test/bp.nas
	@ ./nasal -d test/calc.nas
	@ ./nasal -e test/choice.nas
	@ ./nasal -e test/class.nas
	@ ./nasal -t -d test/console3D.nas 50
	@ ./nasal -e test/coroutine.nas
	@ ./nasal -t -d test/datalog.nas
	@ ./nasal -e test/diff.nas
	@ ./nasal -e test/donuts.nas 100
	-@ ./nasal -d test/exception.nas
	@ ./nasal -t -d test/fib.nas
	@ ./nasal -e test/filesystem.nas
	@ ./nasal -d test/hexdump.nas
	@ ./nasal -c test/httptest.nas
	@ ./nasal -e test/json.nas
	@ ./nasal -e test/leetcode1319.nas
	@ ./nasal -d test/lexer.nas
	@ ./nasal -d test/life.nas
	@ ./nasal -t test/loop.nas
	@ ./nasal -t -d test/mandel.nas
	@ ./nasal -t test/mandelbrot.nas
	@ ./nasal -t test/md5.nas
	@ ./nasal -t -d test/md5compare.nas
	-@ ./nasal -d test/module_test.nas
	@ ./nasal -e test/nasal_test.nas
	@ ./nasal test/occupation.nas 10
	@ ./nasal -t -d test/pi.nas
	@ ./nasal -c test/ppmgen.nas
	@ ./nasal -t -d test/prime.nas
	@ ./nasal -e test/qrcode.nas
	@ ./nasal -t -d test/quick_sort.nas
	@ ./nasal -e test/scalar.nas hello world
	-@ ./nasal -t test/snake.nas --skip
	@ ./nasal -e test/trait.nas
	-@ ./nasal -t test/tetris.nas --skip
	@ ./nasal -t -d test/turingmachine.nas
	@ ./nasal -d test/wavecollapse.nas
	@ ./nasal test/word_collector.nas test/md5compare.nas
	@ ./nasal -t -d test/ycombinator.nas