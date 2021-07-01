.Array.o: Array.cpp headers/Array.hpp headers/Utils.hpp headers/Macros.hpp
	$(if $(wildcard .Array.mk.2),rm .Array.mk.2,.extra/depgen Array.cpp)
	gcc -std=c++2a -o .Array.o -O3 -Wall -Wextra -c -fPIC Array.cpp
