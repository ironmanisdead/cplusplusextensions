#include <CPPExtensions/Fraction.hpp>
#include <CPPExtensions/GString.hpp>
#include <CPPExtensions/system.hpp>

using namespace CPPExtensions;

constexpr Fraction f1 = /*9.5*/ { 20, 7 };
constexpr auto string_frac = [] (const Fraction& ratio) noexcept {
	return GString::strcat(GString::numstr(ratio.num()).data,
			"/", GString::numstr(ratio.den()).data);
};

int main() {
	Utils::puts(Utils::std_out, string_frac(f1).data);
}
