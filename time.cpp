#include <CPPExtensions/system.hpp>
#include <CPPExtensions/String.hpp>
#include <CPPExtensions/GString.hpp>
using namespace CPPExtensions;

int main() {
	Utils::puts(Utils::std_out, GString::numstr(Utils::epoch()));
	Utils::puts(Utils::std_out, GString::numstr(Utils::uptime()));
	Utils::usleep(1000);
	Utils::puts(Utils::std_out, GString::numstr(Utils::uptime()));
}
