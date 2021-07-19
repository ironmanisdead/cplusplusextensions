#include <CPPExtensions/system.hpp>
using namespace CPPExtensions;

int main() {
	Utils::print(Utils::outstream, "hey there\n");
	try {
		Utils::RunError("slash");
	} catch (...) {
		Utils::print(Utils::outstream, "error caught\n");
	}
}
