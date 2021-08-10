#include <CPPExtensions/GString.hpp>
#include <CPPExtensions/String.hpp>
#include <CPPExtensions/system.hpp>
using namespace CPPExtensions;
using namespace GString::codes;

constexpr auto gencyan = GString::raycat(fg_black,
		GString::gen_color_temp<false, 51>.data);

constexpr auto trublue = GString::truecolor_temp<false, 40, 177, 249>;
constexpr const char addr[] = "https://github.com/ironmanisdead/cplusplusextensions";
constexpr const char display[] = "this is a link";
constexpr auto linkcolor = GString::strcat(fg_magenta, display, text_reset);

int main() {
	String print;
	print.allocate(89);
	GString::hyperlink(addr, linkcolor.data, print);
	print.puts(Utils::std_out);
	print.set(bold, "this is bold", text_reset);
	print.puts(Utils::std_out);
	print.insert(-9, "not ");
	print.remove_esc();
	print.puts(Utils::std_out);
	print.set(italics, "this is in italics", text_reset);
	print.puts(Utils::std_out);
	print.set(strike, "this is striked through", text_reset);
	print.puts(Utils::std_out);
	print.set(fg_red, "this has a red foreground", text_reset);
	print.puts(Utils::std_out);
	print.set(bg_red, "this has a red background", text_reset);
	print.puts(Utils::std_out);
	print.set(gencyan.data, "gen_color test: cyan", text_reset);
	print.puts(Utils::std_out);
	print.set(trublue.data, "trucolor test: blue", text_reset);
	print.puts(Utils::std_out);
}
