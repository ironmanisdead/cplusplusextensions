#include <CPPExtensions/system.hpp>
#include <CPPExtensions/String.hpp>
using namespace CPPExtensions;
constexpr Utils::Flags flags = Utils::F_RDWR | Utils::F_CREATE;

int main() {
	Utils::desc fd = Utils::open("test.txt", flags);
	String text = { "fd: ", fd, '\n' };
	Utils::print(Utils::std_out, text);
	Utils::close(fd);
}
