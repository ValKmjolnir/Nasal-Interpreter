.PHONY:test clean

STD=c++17

NASAL_OBJECT=\
	nasal_new_err.o\
	nasal_new_ast.o\
	ast_visitor.o\
	ast_dumper.o\
	nasal_new_lexer.o\
	nasal_new_parse.o\
	nasal_new_import.o\
	optimizer.o\
	nasal_new_opcode.o\
	symbol_finder.o\
	nasal_new_codegen.o\
	nasal_new_misc.o\
	nasal_new_gc.o\
	nasal_new_builtin.o\
	nasal_new_vm.o\
	nasal_new_dbg.o\
	nasal_new_main.o

# for test
nasal: $(NASAL_OBJECT)
	$(CXX) $(NASAL_OBJECT) -O3 -o nasal -ldl

nasal.exe: $(NASAL_OBJECT)
	$(CXX) $(NASAL_OBJECT) -O3 -o nasal.exe

nasal_new_main.o: src/nasal_new_main.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_main.cpp -fno-exceptions -fPIC -o nasal_new_main.o -I .

nasal_new_misc.o: src/nasal_new_header.h src/nasal_new_misc.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_misc.cpp -fno-exceptions -fPIC -o nasal_new_misc.o -I .

nasal_new_err.o: src/nasal_new_err.h src/nasal_new_err.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_err.cpp -fno-exceptions -fPIC -o nasal_new_err.o -I .

nasal_new_gc.o: src/nasal_new_gc.h src/nasal_new_gc.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_gc.cpp -fno-exceptions -fPIC -o nasal_new_gc.o -I . 

nasal_new_import.o: src/nasal_new_import.h src/nasal_new_import.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_import.cpp -fno-exceptions -fPIC -o nasal_new_import.o -I .

nasal_new_lexer.o: src/nasal_new_lexer.h src/nasal_new_lexer.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_lexer.cpp -fno-exceptions -fPIC -o nasal_new_lexer.o -I .

nasal_new_ast.o: src/nasal_new_ast.h src/nasal_new_ast.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_ast.cpp -fno-exceptions -fPIC -o nasal_new_ast.o -I .

nasal_new_builtin.o: src/nasal_new_builtin.h src/nasal_new_builtin.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_builtin.cpp -fno-exceptions -fPIC -o nasal_new_builtin.o -I .

nasal_new_codegen.o: src/nasal_new_codegen.h src/nasal_new_codegen.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_codegen.cpp -fno-exceptions -fPIC -o nasal_new_codegen.o -I .

nasal_new_opcode.o: src/nasal_new_opcode.h src/nasal_new_opcode.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_opcode.cpp -fno-exceptions -fPIC -o nasal_new_opcode.o -I .

nasal_new_parse.o: src/nasal_new_parse.h src/nasal_new_parse.cpp src/nasal_new_ast.h
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_parse.cpp -fno-exceptions -fPIC -o nasal_new_parse.o -I .

optimizer.o: src/optimizer.h src/optimizer.cpp src/nasal_new_ast.h
	$(CXX) -std=$(STD) -c -O3 src/optimizer.cpp -fno-exceptions -fPIC -o optimizer.o -I .

symbol_finder.o: src/symbol_finder.h src/symbol_finder.cpp src/nasal_new_ast.h
	$(CXX) -std=$(STD) -c -O3 src/symbol_finder.cpp -fno-exceptions -fPIC -o symbol_finder.o -I .

ast_visitor.o: src/nasal_new_ast.h src/ast_visitor.h src/ast_visitor.cpp
	$(CXX) -std=$(STD) -c -O3 src/ast_visitor.cpp -fno-exceptions -fPIC -o ast_visitor.o -I .

ast_dumper.o: src/nasal_new_ast.h src/ast_visitor.h src/ast_dumper.h src/ast_dumper.cpp
	$(CXX) -std=$(STD) -c -O3 src/ast_dumper.cpp -fno-exceptions -fPIC -o ast_dumper.o -I .

nasal_new_vm.o: src/nasal_new_vm.h src/nasal_new_vm.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_vm.cpp -fno-exceptions -fPIC -o nasal_new_vm.o -I .

nasal_new_dbg.o: src/nasal_new_dbg.h src/nasal_new_dbg.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_new_dbg.cpp -fno-exceptions -fPIC -o nasal_new_dbg.o -I .

.PHONY: nasal_new_clean
nasal_new_clean:
	rm $(NASAL_OBJECT)

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

nasal_old:$(SRC)
	$(CXX) -std=$(STD) -O3 main.cpp -o nasal_old -fno-exceptions -ldl -Wshadow -Wall

nasal_old.exe:$(SRC)
	$(CXX) -std=$(STD) -O3 main.cpp -o nasal_old.exe -fno-exceptions -Wshadow -Wall -static

stable-release:$(SRC)
	$(CXX) -std=$(STD) -O2 main.cpp -o nasal_old -fno-exceptions -ldl -Wshadow -Wall

stable-release-mingw:$(SRC)
	$(CXX) -std=$(STD) -O2 main.cpp -o nasal_old.exe -fno-exceptions -Wshadow -Wall -static

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