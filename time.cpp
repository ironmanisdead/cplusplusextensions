#include <CPPExtensions/system.hpp>
#include <CPPExtensions/String.hpp>
using namespace CPPExtensions;

int main() {
	String epoch = { "epoch: ", Utils::epoch(), '\n' };
	Utils::print(Utils::std_out, epoch);
	String uptime = { "uptime: ", Utils::uptime(), '\n' };
	Utils::print(Utils::std_out, uptime);
	Utils::usleep(1000);
	uptime = String { "uptime: ", Utils::uptime(), '\n' };
	Utils::print(Utils::std_out, uptime);
}
