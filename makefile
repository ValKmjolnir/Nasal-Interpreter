.PHONY:test clean

SRC=\
	main.cpp\
	nasal_ast.h\
	nasal_err.h\
	nasal_builtin.h\
	nasal_opcode.h\
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

nasal:$(SRC) nasal_new_ast.o
	$(CXX) -std=$(STD) -O3 main.cpp -o nasal -fno-exceptions -ldl -Wshadow -Wall
nasal.exe:$(SRC)
	$(CXX) -std=$(STD) -O3 main.cpp -o nasal.exe -fno-exceptions -Wshadow -Wall -static

nasal_new_ast.o: ast/nasal_new_ast.h ast/nasal_new_ast.cpp nasal.h
	$(CXX) -std=$(STD) -c -O3 ast/nasal_new_ast.cpp -fno-exceptions -fPIC -o nasal_new_ast.o -I .

stable-release:$(SRC)
	$(CXX) -std=$(STD) -O2 main.cpp -o nasal -fno-exceptions -ldl -Wshadow -Wall
stable-release-mingw:$(SRC)
	$(CXX) -std=$(STD) -O2 main.cpp -o nasal.exe -fno-exceptions -Wshadow -Wall -static

clean:
	@ echo "[clean] nasal" && if [ -e nasal ]; then rm nasal; fi
	@ echo "[clean] nasal.exe" && if [ -e nasal.exe ]; then rm nasal.exe; fi

test:nasal
	@ ./nasal -e test/ascii-art.nas
	@ ./nasal -t -d test/bfs.nas
	@ ./nasal -t test/bigloop.nas
	@ ./nasal -t test/bp.nas
	@ ./nasal -d test/calc.nas
	@ ./nasal -e test/choice.nas
	@ ./nasal -e test/class.nas
	@ ./nasal -t -d test/console3D.nas 20
	@ ./nasal -e test/coroutine.nas
	@ ./nasal -t -d test/datalog.nas
	@ ./nasal -e test/diff.nas
	@ ./nasal -e test/donuts.nas 15
	-@ ./nasal -d test/exception.nas
	@ ./nasal -t -d test/fib.nas
	@ ./nasal -e test/filesystem.nas
	@ ./nasal -d test/hexdump.nas
	@ ./nasal -e test/json.nas
	@ ./nasal -e test/leetcode1319.nas
	@ ./nasal -d test/lexer.nas
	@ ./nasal -d test/life.nas
	@ ./nasal -t test/loop.nas
	@ ./nasal -t test/mandelbrot.nas
	@ ./nasal -t test/md5.nas
	@ ./nasal -t -d test/md5compare.nas
	@ ./nasal -d test/module_test.nas
	@ ./nasal -e test/nasal_test.nas
	@ ./nasal -t -d test/occupation.nas 2
	@ ./nasal -t -d test/pi.nas
	@ ./nasal -t -d test/prime.nas
	@ ./nasal -e test/qrcode.nas
	@ ./nasal -t -d test/quick_sort.nas
	@ ./nasal -e test/scalar.nas hello world
	@ ./nasal -e test/trait.nas
	@ ./nasal -t -d test/turingmachine.nas
	@ ./nasal -d test/wavecollapse.nas
	@ ./nasal test/word_collector.nas test/md5compare.nas
	@ ./nasal -t -d test/ycombinator.nas