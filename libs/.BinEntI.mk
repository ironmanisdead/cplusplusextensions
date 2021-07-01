.BinEntI.o: BinEntI.cpp headers/BinEntI.hpp headers/Macros.hpp \
 headers/BinTree.hpp headers/Utils.hpp headers/Find.hpp
	$(if $(wildcard .BinEntI.mk.2),rm .BinEntI.mk.2,.extra/depgen BinEntI.cpp)
	gcc -std=c++2a -o .BinEntI.o -O3 -Wall -Wextra -c -fPIC BinEntI.cpp
