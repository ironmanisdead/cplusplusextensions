#include <CPPExtensions/Fraction.hpp>
#include <CPPExtensions/GString.hpp>
#include <CPPExtensions/system.hpp>

using namespace CPPExtensions;

constexpr auto string_frac = [] (const Fraction& ratio) noexcept {
	return GString::strcat(GString::numstr(ratio.num()).data,
			"/", GString::numstr(ratio.den()).data);
};
constexpr Fraction f1 = { 20, 7 };
constexpr Fraction f2 = 9.5;
constexpr Fraction f3 = -8.7;
constexpr Fraction f4 = -2.3;

int main() {
	Utils::puts_alloc(23);
	Utils::puts(Utils::std_out, string_frac(f1).data);
	Utils::puts(Utils::std_out, string_frac(f2).data);
	Utils::puts(Utils::std_out, string_frac(f3).data);
	Utils::puts(Utils::std_out, string_frac(f4).data);
}
