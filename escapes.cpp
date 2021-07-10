#include <CustomUtils/GString.hpp>
#include <CustomUtils/String.hpp>
#include <iostream>
using namespace CustomUtils;
using namespace GString;
using namespace GString::codes;

int main() {
	std::cout << escape<bold> << "this is bold" << escape<reset_all> << std::endl;
	std::cout << escape<italics> << "this is in italics" << escape<reset_all> << std::endl;
	std::cout << escape<strikethrough> << "this is striked through" << escape<reset_all> << std::endl;
	std::cout << escape<bg_red> << "this has a red background" << escape<reset_all> << std::endl;
}
