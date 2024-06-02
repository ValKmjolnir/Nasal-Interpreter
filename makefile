STD = c++17

ifndef OS
	OS = $(shell uname)
endif
ifeq ($(OS), Darwin)
	CXXFLAGS = -std=$(STD) -c -O3 -fPIC -mmacosx-version-min=10.15 -I src
else
	CXXFLAGS = -std=$(STD) -c -O3 -fPIC -I src
endif

NASAL_HEADER = \
	src/ast_dumper.h\
	src/ast_visitor.h\
	src/nasal_ast.h\
	src/natives/builtin.h\
	src/nasal_codegen.h\
	src/nasal_dbg.h\
	src/nasal_err.h\
	src/nasal_gc.h\
	src/nasal_import.h\
	src/nasal_lexer.h\
	src/nasal_opcode.h\
	src/nasal_parse.h\
	src/nasal_vm.h\
	src/nasal_type.h\
	src/nasal.h\
	src/optimizer.h\
	src/symbol_finder.h\
	src/cli/cli.h\
	src/natives/fg_props.h\
	src/natives/bits_lib.h\
	src/natives/io_lib.h\
	src/natives/math_lib.h\
	src/natives/dylib_lib.h\
	src/natives/json_lib.h\
	src/natives/unix_lib.h\
	src/natives/coroutine.h\
	src/repl/repl.h\
	src/natives/regex_lib.h

NASAL_OBJECT = \
	build/nasal_err.o\
	build/nasal_ast.o\
	build/ast_visitor.o\
	build/bits_lib.o\
	build/ast_dumper.o\
	build/nasal_lexer.o\
	build/nasal_parse.o\
	build/nasal_import.o\
	build/optimizer.o\
	build/nasal_opcode.o\
	build/symbol_finder.o\
	build/nasal_codegen.o\
	build/nasal_misc.o\
	build/nasal_gc.o\
	build/builtin.o\
	build/fg_props.o\
	build/io_lib.o\
	build/math_lib.o\
	build/unix_lib.o\
	build/dylib_lib.o\
	build/json_lib.o\
	build/coroutine.o\
	build/nasal_type.o\
	build/nasal_vm.o\
	build/nasal_dbg.o\
	build/repl.o\
	build/regex_lib.o\
	build/cli.o\
	build/main.o


# for test
nasal: $(NASAL_OBJECT) | build
	@if [ OS = "Darwin" ]; then\
		$(CXX) $(NASAL_OBJECT) -O3 -o nasal -ldl -lpthread -stdlib=libc++ -static-libstdc++;\
	else\
		$(CXX) $(NASAL_OBJECT) -O3 -o nasal -ldl -lpthread;\
	fi

nasal.exe: $(NASAL_OBJECT) | build
	$(CXX) $(NASAL_OBJECT) -O3 -o nasal.exe

build:
	@ if [ ! -d build ]; then mkdir build; fi

build/main.o: $(NASAL_HEADER) src/main.cpp | build
	$(CXX) $(CXXFLAGS) src/main.cpp -o build/main.o

build/nasal_misc.o: src/nasal.h src/nasal_misc.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_misc.cpp -o build/nasal_misc.o

build/cli.o: src/cli/cli.h src/cli/cli.cpp | build
	$(CXX) $(CXXFLAGS) src/cli/cli.cpp -o build/cli.o

build/repl.o: $(NASAL_HEADER) src/repl/repl.h src/repl/repl.cpp | build
	$(CXX) $(CXXFLAGS) src/repl/repl.cpp -o build/repl.o

build/nasal_err.o: src/nasal.h src/repl/repl.h src/nasal_err.h src/nasal_err.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_err.cpp -o build/nasal_err.o

build/nasal_type.o: src/nasal.h src/nasal_type.h src/nasal_type.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_type.cpp -o build/nasal_type.o

build/nasal_gc.o: src/nasal.h src/nasal_type.h src/nasal_gc.h src/nasal_gc.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_gc.cpp -o build/nasal_gc.o

build/nasal_import.o: \
	src/nasal.h\
	src/nasal_ast.h\
	src/nasal_lexer.h\
	src/nasal_parse.h\
	src/nasal_import.h src/nasal_import.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_import.cpp -o build/nasal_import.o

build/nasal_lexer.o: \
	src/nasal.h\
	src/repl/repl.h\
	src/nasal_err.h\
	src/nasal_lexer.h src/nasal_lexer.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_lexer.cpp -o build/nasal_lexer.o

build/nasal_ast.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h src/nasal_ast.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_ast.cpp -o build/nasal_ast.o

build/builtin.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/builtin.h src/natives/builtin.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/builtin.cpp -o build/builtin.o

build/coroutine.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/coroutine.h src/natives/coroutine.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/coroutine.cpp -o build/coroutine.o

build/bits_lib.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/bits_lib.h src/natives/bits_lib.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/bits_lib.cpp -o build/bits_lib.o


build/math_lib.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/math_lib.h src/natives/math_lib.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/math_lib.cpp -o build/math_lib.o

build/io_lib.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/io_lib.h src/natives/io_lib.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/io_lib.cpp -o build/io_lib.o

build/dylib_lib.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/dylib_lib.h src/natives/dylib_lib.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/dylib_lib.cpp -o build/dylib_lib.o

build/json_lib.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/json_lib.h src/natives/json_lib.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/json_lib.cpp -o build/json_lib.o

build/unix_lib.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/unix_lib.h src/natives/unix_lib.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/unix_lib.cpp -o build/unix_lib.o

build/regex_lib.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/regex_lib.h src/natives/regex_lib.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/regex_lib.cpp -o build/regex_lib.o

build/fg_props.o: \
	src/nasal.h\
	src/nasal_type.h\
	src/nasal_gc.h\
	src/natives/fg_props.h src/natives/fg_props.cpp | build
	$(CXX) $(CXXFLAGS) src/natives/fg_props.cpp -o build/fg_props.o

build/nasal_codegen.o: $(NASAL_HEADER) src/nasal_codegen.h src/nasal_codegen.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_codegen.cpp -o build/nasal_codegen.o

build/nasal_opcode.o: \
	src/nasal.h\
	src/natives/builtin.h\
	src/nasal_opcode.h src/nasal_opcode.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_opcode.cpp -o build/nasal_opcode.o

build/nasal_parse.o: \
	src/nasal.h\
	src/nasal_ast.h\
	src/nasal_lexer.h\
	src/nasal_err.h\
	src/nasal_parse.h src/nasal_parse.cpp src/nasal_ast.h | build
	$(CXX) $(CXXFLAGS) src/nasal_parse.cpp -o build/nasal_parse.o

build/optimizer.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h\
	src/ast_visitor.h\
	src/optimizer.h src/optimizer.cpp src/nasal_ast.h | build
	$(CXX) $(CXXFLAGS) src/optimizer.cpp -o build/optimizer.o

build/symbol_finder.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h\
	src/ast_visitor.h\
	src/symbol_finder.h src/symbol_finder.cpp src/nasal_ast.h | build
	$(CXX) $(CXXFLAGS) src/symbol_finder.cpp -o build/symbol_finder.o

build/ast_visitor.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h\
	src/ast_visitor.h src/ast_visitor.cpp | build
	$(CXX) $(CXXFLAGS) src/ast_visitor.cpp -o build/ast_visitor.o

build/ast_dumper.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h\
	src/ast_visitor.h\
	src/ast_dumper.h src/ast_dumper.cpp | build
	$(CXX) $(CXXFLAGS) src/ast_dumper.cpp -o build/ast_dumper.o

build/nasal_vm.o: $(NASAL_HEADER) src/nasal_vm.h src/nasal_vm.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_vm.cpp -o build/nasal_vm.o

build/nasal_dbg.o: $(NASAL_HEADER) src/nasal_dbg.h src/nasal_dbg.cpp | build
	$(CXX) $(CXXFLAGS) src/nasal_dbg.cpp -o build/nasal_dbg.o

.PHONY: clean
clean:
	@ echo "[clean] nasal" && if [ -e nasal ]; then rm nasal; fi
	@ echo "[clean] nasal.exe" && if [ -e nasal.exe ]; then rm nasal.exe; fi
	@ rm $(NASAL_OBJECT)

.PHONY: test
test:nasal
	@ ./nasal -t -d test/andy_gc_test.nas
	@ ./nasal test/argparse_test.nas
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
	@ ./nasal -t -d test/globals_test.nas
	@ ./nasal -d test/hexdump.nas
	@ ./nasal -e test/json.nas
	@ ./nasal -e test/leetcode1319.nas
	@ ./nasal -d test/lexer.nas
	@ ./nasal -d test/life.nas
	@ ./nasal -t test/loop.nas
	@ ./nasal -t test/mandelbrot.nas
	@ ./nasal -t test/md5_self.nas
	@ ./nasal -t -d test/md5compare.nas
	@ ./nasal -d test/module_test.nas
	@ ./nasal -e test/nasal_test.nas
	@ ./nasal -t -d test/occupation.nas 2
	@ ./nasal -t -d test/pi.nas
	@ ./nasal -t -d test/prime.nas
	@ ./nasal -e test/qrcode.nas
	@ ./nasal -t -d test/quick_sort.nas
	@ ./nasal -t -d test/regex_test.nas
	@ ./nasal -e test/scalar.nas hello world
	@ ./nasal -e test/trait.nas
	@ ./nasal -t -d test/turingmachine.nas
	@ ./nasal -d test/wavecollapse.nas
	@ ./nasal -d test/wavecity.nas
	@ ./nasal test/word_collector.nas test/md5compare.nas
	@ ./nasal -t -d test/ycombinator.nas
