#include <CPPExtensions/TestFunc.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() {
	Function<int()> two = FuncTest();
	std::cout << two() << std::endl;
}
