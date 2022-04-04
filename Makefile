libluastubs.so: luastubs.cpp
	g++ -c -fPIC -o luastubs.o -std=c++17 luastubs.cpp -I/usr/include/lua5.3 -I/home/philip/Documents/prolog/souffle/src/include -fopenmp
	g++ -shared -o libluastubs.so luastubs.o -llua5.3 -fopenmp

all: libluastubs.so

clean:
	rm libluastubs.so luastubs.o