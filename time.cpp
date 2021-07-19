#include <CPPExtensions/system.hpp>
#include <CPPExtensions/String.hpp>
using namespace CPPExtensions;

int main() {
	String epoch = { Utils::epoch(), '\n' };
	Utils::print(Utils::outstream, epoch);
}
