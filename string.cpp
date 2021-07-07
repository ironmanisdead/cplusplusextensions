#include <CustomUtils/String.hpp>
#include <iostream>
using namespace CustomUtils;

int main() {
	String arr = "hey";
	std::cout << arr << std::endl;
	arr += " babe";
	std::cout << arr << std::endl;
	arr = "whee, and i am not sure about how i will end up";
	std::cout << arr << std::endl;
}
