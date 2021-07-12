#include <CPPExtensions/Vector.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() noexcept {
	Vector<char> arr;
	arr.allocate(20);
	arr = "hey";
	Vector<char> par = arr;
	std::cout << arr << std::endl;
	for (int i = 0; i < 10; i++)
		arr.append('l');
	std::cout << par << std::endl;
	std::cout << arr << std::endl;
}
