.PHONY=clean all winall

dynamic_libs_so=libfib.so libkey.so libnasock.so libmat.so
dynamic_libs_dll=libfib.dll libkey.dll libnasock.dll libmat.dll

used_header= ../src/nasal_new_header.h ../src/nasal_new_gc.h

STD=c++17

all: $(dynamic_libs_so)
	@ echo "[Compiling] done"
winall: $(dynamic_libs_dll)
	@ echo [Compiling] done

libfib.so: fib.cpp $(used_header)
	@ echo "[Compiling] libfib.so"
	@ $(CXX) -std=$(STD) -c -O3 fib.cpp -fPIC -o fib.o
	@ $(CXX) -shared -o libfib.so fib.o
	@ rm fib.o
libfib.dll: fib.cpp $(used_header)
	@ echo [Compiling] libfib.dll
	@ $(CXX) -std=$(STD) -c -O3 fib.cpp -fPIC -o fib.o
	@ $(CXX) -shared -o libfib.dll fib.o
	@ del fib.o

libkey.so: keyboard.cpp $(used_header)
	@ echo "[Compiling] libkey.so"
	@ $(CXX) -std=$(STD) -c -O3 keyboard.cpp -fPIC -o keyboard.o
	@ $(CXX) -shared -o libkey.so keyboard.o
	@ rm keyboard.o
libkey.dll: keyboard.cpp $(used_header)
	@ echo [Compiling] libkey.dll
	@ $(CXX) -std=$(STD) -c -O3 keyboard.cpp -fPIC -o keyboard.o -static
	@ $(CXX) -shared -o libkey.dll keyboard.o -static
	@ del keyboard.o

libnasock.so: nasocket.cpp $(used_header)
	@ echo "[Compiling] libnasock.so"
	@ $(CXX) -std=$(STD) -c -O3 nasocket.cpp -fPIC -o nasocket.o
	@ $(CXX) -shared -o libnasock.so nasocket.o
	@ rm nasocket.o
libnasock.dll: nasocket.cpp $(used_header)
	@ echo [Compiling] libnasock.dll
	@ $(CXX) -std=$(STD) -c -O3 nasocket.cpp -fPIC -o nasocket.o -lwsock32 -static
	@ $(CXX) -shared -o libnasock.dll nasocket.o -lwsock32 -static
	@ del nasocket.o

libmat.so: matrix.cpp $(used_header)
	@ echo "[Compiling] libmat.so"
	@ $(CXX) -std=$(STD) -c -O3 matrix.cpp -fPIC -o matrix.o
	@ $(CXX) -shared -o libmat.so matrix.o
	@ rm matrix.o
libmat.dll: matrix.cpp $(used_header)
	@ echo [Compiling] libmat.dll
	@ $(CXX) -std=$(STD) -c -O3 matrix.cpp -fPIC -o matrix.o -static
	@ $(CXX) -shared -o libmat.dll matrix.o -static
	@ del matrix.o

clean:
	@ echo "[clean] libfib.so" && if [ -e libfib.so ]; then rm libfib.so; fi
	@ echo "[clean] libkey.so" && if [ -e libkey.so ]; then rm libkey.so; fi
	@ echo "[clean] libnasock.so" && if [ -e libnasock.so ]; then rm libnasock.so; fi
	@ echo "[clean] libmat.so" && if [ -e libmat.so ]; then rm libmat.so; fi
	@ echo "[clean] libfib.dll" &&if [ -e libfib.dll ]; then rm libfib.dll; fi
	@ echo "[clean] libkey.dll" &&if [ -e libkey.dll ]; then rm libkey.dll; fi
	@ echo "[clean] libnasock.dll" &&if [ -e libnasock.dll ]; then rm libnasock.dll; fi
	@ echo "[clean] libmat.dll" &&if [ -e libmat.dll ]; then rm libmat.dll; fi