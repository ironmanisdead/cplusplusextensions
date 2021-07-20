#include <CPPExtensions/Array.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() {
	std::cout << Array<char, 5> { "hello" } << std::endl;
}
