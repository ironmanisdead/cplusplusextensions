#include <CPPExtensions/Vector.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() noexcept {
	Vector<char> arr;
	arr.allocate(20);
	arr = "hey";
	std::cout << arr << std::endl;
	for (int i = 0; i < 10; i++)
		arr.append('l');
	std::cout << arr << std::endl;
}
