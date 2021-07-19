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

int main() {
	String linktext = { fg_blue, display, text_reset };
	String link = String(GString::hyperlink(addr, linktext).data(), '\n');
	String boldtext = { bold, "this is bold", text_reset, '\n' };
	Utils::print(Utils::outstream, boldtext);
	String italictext = { italics, "this is in italics", text_reset, '\n' };
	Utils::print(Utils::outstream, italictext);
	String strikedtext = { strike, "this is striked through", text_reset, '\n' };
	Utils::print(Utils::outstream, strikedtext);
	String redfgtext = { fg_red, "this has a red foreground", text_reset, '\n' };
	Utils::print(Utils::outstream, redfgtext);
	String redbgtext = { bg_red, "this has a red background", text_reset, '\n' };
	Utils::print(Utils::outstream, redbgtext);
	String cyantext = { gencyan.data, "gen_color test: cyan", text_reset, '\n' };
	Utils::print(Utils::outstream, cyantext);
	String bluetext = { trublue.data, "trucolor test: blue", text_reset, '\n' };
	Utils::print(Utils::outstream, bluetext);
	Utils::print(Utils::outstream, link);
}
