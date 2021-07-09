#include <iostream>
#include <CustomUtils/String.hpp>

using namespace CustomUtils;

int main() {
	std::cout << String("this file is weirdly named") << std::endl;
	std::cout << String("tabs, spaces, and newlines aren't allowed in test file generations") << std::endl;
}
