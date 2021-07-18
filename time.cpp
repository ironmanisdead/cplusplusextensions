#include <CPPExtensions/misc.hpp>
#include <CPPExtensions/String.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() {
	std::cout << String((Utils::size_t)Utils::epoch()) << std::endl;
}
