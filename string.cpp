#include <CPPExtensions/String.hpp>
#include <CPPExtensions/StringView.hpp>
using namespace CPPExtensions;
constexpr StringView cons = "The StringView now can handle literal "
	"null characters directly.\0 this text is shown.";

int main() {
	String test = "This is a String object.";
	test += " It uses up space and can be directly allocated if needed.";
	test.allocate(test.getlen() * 7);
	test.puts(Utils::std_out);
	test.set("The String object also can handle string literals with null\0 bytes in them.\n");
	test.write(Utils::std_out);
	test.set("Strings can also handle numbers directly. ", 14, ' ', 15, '\n');
	test.write(Utils::std_out);
	test = "Strings can have substrings extracted";
	test.puts(Utils::std_out);
	test.substr(6, 8).puts(Utils::std_out);
	StringView pattern = "A pattern can be encoded atop the string.";
	String oldstr = test;
	test.encode(pattern);
	test.encode(oldstr.viewer());
	test.puts(Utils::std_out);
	cons.puts(Utils::std_out);
}
