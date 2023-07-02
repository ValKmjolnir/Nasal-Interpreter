STD=c++17

NASAL_OBJECT=\
	nasal_err.o\
	nasal_ast.o\
	ast_visitor.o\
	ast_dumper.o\
	nasal_lexer.o\
	nasal_parse.o\
	nasal_import.o\
	optimizer.o\
	nasal_opcode.o\
	symbol_finder.o\
	nasal_codegen.o\
	nasal_misc.o\
	nasal_gc.o\
	nasal_builtin.o\
	nasal_vm.o\
	nasal_dbg.o\
	main.o

# for test
nasal: $(NASAL_OBJECT)
	$(CXX) $(NASAL_OBJECT) -O3 -o nasal -ldl

nasal.exe: $(NASAL_OBJECT)
	$(CXX) $(NASAL_OBJECT) -O3 -o nasal.exe

main.o: src/main.cpp
	$(CXX) -std=$(STD) -c -O3 src/main.cpp -fno-exceptions -fPIC -o main.o -I .

nasal_misc.o: src/nasal.h src/nasal_misc.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_misc.cpp -fno-exceptions -fPIC -o nasal_misc.o -I .

nasal_err.o: src/nasal_err.h src/nasal_err.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_err.cpp -fno-exceptions -fPIC -o nasal_err.o -I .

nasal_gc.o: src/nasal_gc.h src/nasal_gc.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_gc.cpp -fno-exceptions -fPIC -o nasal_gc.o -I . 

nasal_import.o: src/nasal_import.h src/nasal_import.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_import.cpp -fno-exceptions -fPIC -o nasal_import.o -I .

nasal_lexer.o: src/nasal_lexer.h src/nasal_lexer.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_lexer.cpp -fno-exceptions -fPIC -o nasal_lexer.o -I .

nasal_ast.o: src/nasal_ast.h src/nasal_ast.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_ast.cpp -fno-exceptions -fPIC -o nasal_ast.o -I .

nasal_builtin.o: src/nasal_builtin.h src/nasal_builtin.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_builtin.cpp -fno-exceptions -fPIC -o nasal_builtin.o -I .

nasal_codegen.o: src/nasal_codegen.h src/nasal_codegen.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_codegen.cpp -fno-exceptions -fPIC -o nasal_codegen.o -I .

nasal_opcode.o: src/nasal_opcode.h src/nasal_opcode.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_opcode.cpp -fno-exceptions -fPIC -o nasal_opcode.o -I .

nasal_parse.o: src/nasal_parse.h src/nasal_parse.cpp src/nasal_ast.h
	$(CXX) -std=$(STD) -c -O3 src/nasal_parse.cpp -fno-exceptions -fPIC -o nasal_parse.o -I .

optimizer.o: src/optimizer.h src/optimizer.cpp src/nasal_ast.h
	$(CXX) -std=$(STD) -c -O3 src/optimizer.cpp -fno-exceptions -fPIC -o optimizer.o -I .

symbol_finder.o: src/symbol_finder.h src/symbol_finder.cpp src/nasal_ast.h
	$(CXX) -std=$(STD) -c -O3 src/symbol_finder.cpp -fno-exceptions -fPIC -o symbol_finder.o -I .

ast_visitor.o: src/nasal_ast.h src/ast_visitor.h src/ast_visitor.cpp
	$(CXX) -std=$(STD) -c -O3 src/ast_visitor.cpp -fno-exceptions -fPIC -o ast_visitor.o -I .

ast_dumper.o: src/nasal_ast.h src/ast_visitor.h src/ast_dumper.h src/ast_dumper.cpp
	$(CXX) -std=$(STD) -c -O3 src/ast_dumper.cpp -fno-exceptions -fPIC -o ast_dumper.o -I .

nasal_vm.o: src/nasal_vm.h src/nasal_vm.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_vm.cpp -fno-exceptions -fPIC -o nasal_vm.o -I .

nasal_dbg.o: src/nasal_dbg.h src/nasal_dbg.cpp
	$(CXX) -std=$(STD) -c -O3 src/nasal_dbg.cpp -fno-exceptions -fPIC -o nasal_dbg.o -I .

.PHONY: clean
clean:
	@ echo "[clean] nasal" && if [ -e nasal ]; then rm nasal; fi
	@ echo "[clean] nasal.exe" && if [ -e nasal.exe ]; then rm nasal.exe; fi
	@ rm $(NASAL_OBJECT)

.PHONY: test
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