#include <CPPExtensions/Vector.hpp>
#include <iostream>
using namespace CPPExtensions;

int main() noexcept {
	Vector<char> arr;
	arr = "this string repeats.";
	std::cout << arr << std::endl;
	for (int i = 0; i < 70; i++) {
		char n = *arr[i]; //capture in variable before reallocation
		arr.append(n); //potential reallocation
	}
	std::cout << arr << std::endl;
}
