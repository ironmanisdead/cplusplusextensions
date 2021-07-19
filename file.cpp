#include <CPPExtensions/system.hpp>
using namespace CPPExtensions;

int main() {
	Utils::desc file = Utils::open("tests/testfile.txt", Utils::F_CREATE | Utils::F_RDWR);
}
