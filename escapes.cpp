#include <CPPExtensions/GString.hpp>
#include <CPPExtensions/String.hpp>
#include <iostream>
using namespace CPPExtensions;
using namespace GString::codes;

constexpr auto gencyan = GString::strcat(fg_black,
		GString::gen_color(false, 51).data);

constexpr auto trublue = GString::truecolor(true, 40, 177, 249);

int main() {
	std::cout << bold << "this is bold" << text_reset << std::endl;
	std::cout << italics << "this is in italics" << text_reset << std::endl;
	std::cout << strike << "this is striked through" << text_reset << std::endl;
	std::cout << bg_red << "this has a red background" << text_reset << std::endl;
	std::cout << gencyan << "gen_color test: cyan" << text_reset << std::endl;
	std::cout << trublue << "trucolor test: blue" << text_reset << std::endl;
}
