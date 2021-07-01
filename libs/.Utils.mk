.Utils.o: Utils.cpp headers/Utils.hpp headers/Macros.hpp
	$(if $(wildcard .Utils.mk.2),rm .Utils.mk.2,.extra/depgen Utils.cpp)
	gcc -std=c++2a -o .Utils.o -O3 -Wall -Wextra -c -fPIC Utils.cpp
