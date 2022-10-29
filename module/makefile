.PHONY=clean all mingw-all

dynamic_libs=libfib.so libkey.so libnasock.so
dynamic_libs_dll=libfib.dll libkey.dll libnasock.dll

STD=14

libfib.so: fib.cpp
	@ echo "[build] libfib.so"
	@ $(CXX) -std=c++$(STD) -c -O3 fib.cpp -fPIC -o fib.o
	@ $(CXX) -shared -o libfib.so fib.o
	@ rm fib.o
libfib.dll: fib.cpp
	@ echo [build] libfib.dll
	@ $(CXX) -std=c++$(STD) -c -O3 fib.cpp -fPIC -o fib.o
	@ $(CXX) -shared -o libfib.dll fib.o
	@ del fib.o

libkey.so: keyboard.cpp
	@ echo "[build] libkey.so"
	@ $(CXX) -std=c++$(STD) -c -O3 keyboard.cpp -fPIC -o keyboard.o
	@ $(CXX) -shared -o libkey.so keyboard.o
	@ rm keyboard.o
libkey.dll: keyboard.cpp
	@ echo [build] libkey.dll
	@ $(CXX) -std=c++$(STD) -c -O3 keyboard.cpp -fPIC -o keyboard.o -static
	@ $(CXX) -shared -o libkey.dll keyboard.o -static
	@ del keyboard.o

libnasock.so: nasocket.cpp
	@ echo "[build] libnasock.so"
	@ $(CXX) -std=c++$(STD) -c -O3 nasocket.cpp -fPIC -o nasocket.o
	@ $(CXX) -shared -o libnasock.so nasocket.o
	@ rm nasocket.o
libnasock.dll: nasocket.cpp
	@ echo [build] libnasock.dll
	@ $(CXX) -std=c++$(STD) -c -O3 nasocket.cpp -fPIC -o nasocket.o -lwsock32 -static
	@ $(CXX) -shared -o libnasock.dll nasocket.o -lwsock32 -static
	@ del nasocket.o

clean:
	@ echo "[clean] so"
	-@ rm $(dynamic_libs)
	@ echo "[clean] dll"
	-@ rm $(dynamic_libs_dll)

all: libfib.so libkey.so libnasock.so
	@ echo "[build] done"
mingw-all: libfib.dll libkey.dll libnasock.dll
	@ echo [build] done
