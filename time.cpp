#include <CPPExtensions/system.hpp>
#include <CPPExtensions/String.hpp>
#include <CPPExtensions/GString.hpp>
using namespace CPPExtensions;

int main() {
	String prnt;
	prnt.allocate(79);
	prnt.set("epoch: ", Utils::epoch(), '\n');
	prnt.write(Utils::std_out);
	prnt.set("uptime: ", Utils::uptime(), '\n');
	prnt.write(Utils::std_out);
	Utils::sleep(1000);
	prnt.set("uptime: ", Utils::uptime(), '\n');
	prnt.write(Utils::std_out);
	Utils::puts(Utils::std_out, Utils::strlocerr(Utils::getlocerr()));
}
