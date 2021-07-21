#include <CPPExtensions/Fraction.hpp>
#include <CPPExtensions/GString.hpp>
#include <CPPExtensions/system.hpp>

using namespace CPPExtensions;

constexpr Fraction ratio = 0.4;
constexpr auto frac = GString::strcat(GString::numstr(ratio.num()).data,
		"/", GString::numstr(ratio.den()).data);

int main() {
	Utils::puts(Utils::std_out, frac.data);
}
