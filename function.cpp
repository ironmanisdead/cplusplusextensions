#include <CPPExtensions/Function.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() {
	Function<void()> hey = [] () { std::cout << "hey" << std::endl; };
	hey();
	hey = nullptr;
	hey();
}
