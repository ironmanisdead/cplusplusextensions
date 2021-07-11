#include <CustomUtils/GString.hpp>
#include <CustomUtils/String.hpp>
#include <iostream>
using namespace CustomUtils;
using namespace GString::codes;

int main() {
	std::cout << bold << "this is bold" << text_reset << std::endl;
	std::cout << italics << "this is in italics" << text_reset << std::endl;
	std::cout << strike << "this is striked through" << text_reset << std::endl;
	std::cout << bg_red << "this has a red background" << text_reset << std::endl;
	std::cout << GString::gen_color(true, 51) << "gen_color test: cyan" << text_reset << std::endl;
	std::cout << GString::truecolor(true, 40, 177, 249).data << "trucolor test: blue" << text_reset << std::endl;
}
