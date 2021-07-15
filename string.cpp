#include <CPPExtensions/String.hpp>
#include <CPPExtensions/StringView.hpp>
#include <iostream>
using namespace CPPExtensions;
constexpr StringView cons = "The StringView cannot handle literal "
	"null characters directly\0this text isn't shown";

int main() {
	String test = "This is a String object.";
	test += " It uses up space and can be directly allocated if needed.";
	test.allocate(test.getlen() * 7);
	std::cout << test << std::endl;
	test = "The String object also can handle string literals with null\0 bytes in them.";
	std::cout << test << std::endl;
}
