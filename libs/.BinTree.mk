.BinTree.o: BinTree.cpp headers/BinTree.hpp headers/Utils.hpp \
 headers/Macros.hpp headers/Find.hpp headers/BinEntI.hpp \
 headers/BinNode.hpp headers/Function.hpp headers/GString.hpp \
 headers/Array.hpp headers/Tuple.hpp headers/String.hpp
	$(if $(wildcard .BinTree.mk.2),rm .BinTree.mk.2,.extra/depgen BinTree.cpp)
	gcc -std=c++2a -o .BinTree.o -O3 -Wall -Wextra -c -fPIC BinTree.cpp
