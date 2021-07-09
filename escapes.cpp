#include <CustomUtils/GString.hpp>
#include <CustomUtils/String.hpp>
#include <iostream>
using namespace CustomUtils;
using namespace GString;
using namespace GString::codes;

int main() {
	std::cout << escape<bold> << "hey" << escape<reset_all> << std::endl;
	std::cout << escape<italics> << "this is in italics" << escape<reset_all> << std::endl;
}
