#include <CPPExtensions/Fraction.hpp>
#include <CPPExtensions/String.hpp>
#include <CPPExtensions/system.hpp>

using namespace CPPExtensions;

int main() {
	Fraction ratio = 0;
	String frac = { ratio.num(), '/', ratio.den(), '\n' };
	Utils::print(Utils::std_out, frac);
}
