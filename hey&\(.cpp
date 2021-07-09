#include <iostream>
#include <CustomUtils/String.hpp>

using namespace CustomUtils;

int main() {
	std::cout << String("this file is weirdly named, but doesn't violate the rules") << std::endl;
	std::cout << String("tabs, spaces, and newlines aren't allowed in test file names,\n"
			"as I cannot properly escape them for make") << std::endl;
}
