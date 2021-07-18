#include <CPPExtensions/system.hpp>
#include <CPPExtensions/String.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() {
	std::cout << String((unsigned long)Utils::epoch()) << std::endl;
}
