#include <CustomUtils/Vector.hpp>
#include <iostream>
using namespace CustomUtils;

int main() noexcept {
	Vector<char> arr = "hey";
	std::cout << arr << std::endl;
	arr.append('l');
	std::cout << arr << std::endl;
}
