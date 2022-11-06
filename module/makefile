.PHONY=clean all mingw-all

dynamic_libs=libfib.so libkey.so libnasock.so
dynamic_libs_dll=libfib.dll libkey.dll libnasock.dll

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

clean:
	@ echo "[clean] so"
	-@ rm $(dynamic_libs)
	@ echo "[clean] dll"
	-@ rm $(dynamic_libs_dll)

all: libfib.so libkey.so libnasock.so
	@ echo "[Compiling] done"
winall: libfib.dll libkey.dll libnasock.dll
	@ echo [Compiling] done
