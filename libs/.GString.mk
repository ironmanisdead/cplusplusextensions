.GString.o: GString.cpp headers/String.hpp headers/Utils.hpp \
 headers/Macros.hpp headers/GString.hpp headers/Array.hpp \
 headers/Tuple.hpp headers/StringView.hpp headers/Vector.hpp \
 headers/UVector.hpp headers/.hidden/Vector-info.hpp headers/Function.hpp \
 headers/BinMap.hpp headers/BinNode.hpp headers/BinTree.hpp \
 headers/Find.hpp headers/BinEntI.hpp headers/BinNodeI.hpp
	$(if $(wildcard .GString.mk.2),rm .GString.mk.2,.extra/depgen GString.cpp)
	gcc -std=c++2a -o .GString.o -O3 -Wall -Wextra -c -fPIC GString.cpp
