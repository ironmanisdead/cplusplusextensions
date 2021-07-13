#include <CPPExtensions/String.hpp>
#include <iostream>
#include <climits>
using namespace CPPExtensions;

int main() {
	String test = { "hey man ", "are we cool." };
	std::cout << test.allocate(100) << std::endl;
	std::cout << test.haserr() << std::endl;
	test += ' ';
	test += test += test;
	std::cout << test << std::endl;
}
