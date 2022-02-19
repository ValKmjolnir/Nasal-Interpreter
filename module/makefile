.PHONY=clean
libfib.so: fib.cpp
	clang++ -c -O3 fib.cpp -fPIC -o fib.o
	clang++ -shared -o libfib.so fib.o
libfib.dll: fib.cpp
	g++ -c -O3 fib.cpp -fPIC -o fib.o
	g++ -shared -o libfib.dll fib.o
libkey.so: keyboard.cpp
	clang++ -c -O3 keyboard.cpp -fPIC -o keyboard.o
	clang++ -shared -o libkey.so keyboard.o
libkey.dll: keyboard.cpp
	g++ -c -O3 keyboard.cpp -fPIC -o keyboard.o
	g++ -shared -o libkey.dll keyboard.o
clean:
	rm *.o *.so *.dll *.dylib