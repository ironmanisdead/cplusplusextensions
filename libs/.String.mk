.String.o: String.cpp headers/String.hpp headers/Utils.hpp \
 headers/Macros.hpp headers/GString.hpp headers/Array.hpp \
 headers/Tuple.hpp headers/StringView.hpp
	$(if $(wildcard .String.mk.2),rm .String.mk.2,.extra/depgen String.cpp)
	gcc -std=c++2a -o .String.o -O3 -Wall -Wextra -c -fPIC String.cpp
