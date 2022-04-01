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

libmd5.so: md5.cpp
	clang++ -c -O3 md5.cpp -fPIC -o md5.o
	clang++ -shared -o libmd5.so md5.o
	rm md5.o
libmd5.dll: md5.cpp
	g++ -c -O3 md5.cpp -fPIC -o md5.o -static
	g++ -shared -o libmd5.dll md5.o -static

clean:
	rm *.o *.so *.dll *.dylib
all: libfib.so libkey.so libmd5.so
	@ echo "build done"
mingw-all: libfib.dll libkey.dll libmd5.dll
	@ echo "build done"