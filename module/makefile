.PHONY=clean
libfib.so: fib.cpp
	clang++ -c -O3 fib.cpp -fPIC -o fib.o
	clang++ -shared -o libfib.so fib.o
libfib.dll: fib.cpp
	g++ -c -O3 fib.cpp -fPIC -o fib.o
	g++ -shared -o libfib.dll fib.o
clean:
	rm *.o *.so *.dll *.dylib