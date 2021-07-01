.UVector.o: UVector.cpp headers/UVector.hpp headers/Utils.hpp \
 headers/Macros.hpp headers/Array.hpp headers/GString.hpp \
 headers/Tuple.hpp headers/String.hpp
	$(if $(wildcard .UVector.mk.2),rm .UVector.mk.2,.extra/depgen UVector.cpp)
	gcc -std=c++2a -o .UVector.o -O3 -Wall -Wextra -c -fPIC UVector.cpp
