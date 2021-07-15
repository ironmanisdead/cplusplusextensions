#include <CPPExtensions/String.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() {
	String str;
	str.allocate(100);
	std::cout << str.hasErr() << std::endl;
	str = "this String uses an unnessecary amount of memory";
	std::cout << str << std::endl;
}
