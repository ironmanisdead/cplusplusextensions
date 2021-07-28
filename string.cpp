#include <CPPExtensions/String.hpp>
#include <CPPExtensions/StringView.hpp>
using namespace CPPExtensions;
constexpr StringView cons = "The StringView now can handle literal "
	"null characters directly.\0 this text is shown.\n";

int main() {
	String test;
	test.allocate(99);
	test.set("This is a String object.");
	test += " It uses up space and can be directly allocated if needed.\n";
	test.write(Utils::std_out);
	test.set("The String object also can handle string literals with null\0 bytes in them.\n");
	test.write(Utils::std_out);
	test.set("Strings can also handle numbers directly. ", 14, ' ', 15, '\n');
	test.write(Utils::std_out);
	test.set("The String class also allows to have (indexed) substrings extracted.\n");
	test.write(Utils::std_out);
	test.substr(22, 47).write(Utils::std_out);
	StringView pattern = "The String supports X-OR encoding.\n";
	String oldstr = test;
	test.encode(pattern);
	test.encode(oldstr.viewer());
	test.substr(0, 35).write(Utils::std_out);
	cons.write(Utils::std_out);
}
