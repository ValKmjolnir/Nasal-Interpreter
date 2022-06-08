.PHONY=clean all mingw-all
libfib.so: fib.cpp
	clang++ -c -O3 fib.cpp -fPIC -o fib.o
	clang++ -shared -o libfib.so fib.o
	rm fib.o
libfib.dll: fib.cpp
	g++ -c -O3 fib.cpp -fPIC -o fib.o
	g++ -shared -o libfib.dll fib.o

libkey.so: keyboard.cpp
	clang++ -c -O3 keyboard.cpp -fPIC -o keyboard.o
	clang++ -shared -o libkey.so keyboard.o
	rm keyboard.o
libkey.dll: keyboard.cpp
	g++ -c -O3 keyboard.cpp -fPIC -o keyboard.o -static
	g++ -shared -o libkey.dll keyboard.o -static

libnasock.so: nasocket.cpp
	clang++ -c -O3 nasocket.cpp -fPIC -o nasocket.o
	clang++ -shared -o libnasock.so nasocket.o
	rm nasocket.o
libnasock.dll: nasocket.cpp
	g++ -c -O3 nasocket.cpp -fPIC -o nasocket.o -lwsock32 -static
	g++ -shared -o libnasock.dll nasocket.o -lwsock32 -static

clean:
	rm *.o *.so *.dll *.dylib
all: libfib.so libkey.so libnasock.so
	@ echo "build done"
mingw-all: libfib.dll libkey.dll libnasock.dll
	@ echo "build done"