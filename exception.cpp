#include <CPPExtensions/system.hpp>
using namespace CPPExtensions;

int main() {
	Utils::print(Utils::std_out, "hey there\n");
	try {
		Utils::RunError("test");
	} catch (...) {
		Utils::print(Utils::std_out, "error caught\n");
	}
}
