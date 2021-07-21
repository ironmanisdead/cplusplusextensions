#include <CPPExtensions/system.hpp>
#include <CPPExtensions/String.hpp>
#include <CPPExtensions/GString.hpp>
using namespace CPPExtensions;

int main() {
	String prnt;
	prnt.allocate(79);
	prnt.set("epoch: ", Utils::epoch(), '\n');
	Utils::writestr(Utils::std_out, prnt);
	prnt.set("uptime: ", Utils::uptime(), '\n');
	Utils::writestr(Utils::std_out, prnt);
	Utils::usleep(1000);
	prnt.set("uptime: ", Utils::uptime(), '\n');
	Utils::writestr(Utils::std_out, prnt);
	Utils::puts(Utils::std_out, Utils::strlocerr(Utils::getlocerr()));
}
