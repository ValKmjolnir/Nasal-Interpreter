.PHONY=clean all mingw-all

dynamic_libs_so=libfib.so libkey.so libnasock.so libmat.so
dynamic_libs_dll=libfib.dll libkey.dll libnasock.dll libmat.dll

STD=c++14

libfib.so: fib.cpp
	@ echo "[Compiling] libfib.so"
	@ $(CXX) -std=$(STD) -c -O3 fib.cpp -fPIC -o fib.o
	@ $(CXX) -shared -o libfib.so fib.o
	@ rm fib.o
libfib.dll: fib.cpp
	@ echo [Compiling] libfib.dll
	@ $(CXX) -std=$(STD) -c -O3 fib.cpp -fPIC -o fib.o
	@ $(CXX) -shared -o libfib.dll fib.o
	@ del fib.o

libkey.so: keyboard.cpp
	@ echo "[Compiling] libkey.so"
	@ $(CXX) -std=$(STD) -c -O3 keyboard.cpp -fPIC -o keyboard.o
	@ $(CXX) -shared -o libkey.so keyboard.o
	@ rm keyboard.o
libkey.dll: keyboard.cpp
	@ echo [Compiling] libkey.dll
	@ $(CXX) -std=$(STD) -c -O3 keyboard.cpp -fPIC -o keyboard.o -static
	@ $(CXX) -shared -o libkey.dll keyboard.o -static
	@ del keyboard.o

libnasock.so: nasocket.cpp
	@ echo "[Compiling] libnasock.so"
	@ $(CXX) -std=$(STD) -c -O3 nasocket.cpp -fPIC -o nasocket.o
	@ $(CXX) -shared -o libnasock.so nasocket.o
	@ rm nasocket.o
libnasock.dll: nasocket.cpp
	@ echo [Compiling] libnasock.dll
	@ $(CXX) -std=$(STD) -c -O3 nasocket.cpp -fPIC -o nasocket.o -lwsock32 -static
	@ $(CXX) -shared -o libnasock.dll nasocket.o -lwsock32 -static
	@ del nasocket.o

libmat.so: matrix.cpp
	@ echo "[Compiling] libmat.so"
	@ $(CXX) -std=$(STD) -c -O3 matrix.cpp -fPIC -o matrix.o
	@ $(CXX) -shared -o libmat.so matrix.o
	@ rm matrix.o
libmat.dll: matrix.cpp
	@ echo [Compiling] libmat.dll
	@ $(CXX) -std=$(STD) -c -O3 matrix.cpp -fPIC -o matrix.o -static
	@ $(CXX) -shared -o libmat.dll matrix.o -static
	@ del matrix.o

clean:
	@ echo "[clean] so"
	-@ rm $(dynamic_libs_so)
	@ echo "[clean] dll"
	-@ rm $(dynamic_libs_dll)

all: $(dynamic_libs_so)
	@ echo "[Compiling] done"
winall: $(dynamic_libs_dll)
	@ echo [Compiling] done
