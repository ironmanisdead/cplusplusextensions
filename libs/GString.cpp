#include "headers/String.hpp"
#include "headers/GString.hpp"
#include "headers/StringView.hpp"
#include "headers/Vector.hpp"
#include "headers/Function.hpp"
#include "headers/BinMap.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cxxabi.h>
namespace CPPExtensions {
	namespace GString {
		[[noreturn]] void overflow(size_t len, size_t idx) {
			throw std::overflow_error(String("Array of size ", len, " cannot hold string of size ", idx));
		}
		static BinMap<StringView, String> nametype = {};
		const String& demangle(const char* val) {
			StringView view = val;
			if (!nametype.find(view)) {
				int status;
				Object<char*> obj = { [&obj] () { std::free(obj.value); }, abi::__cxa_demangle(val, 0, 0, &status) };
				nametype.insert(view, obj.value);
			}
			return *nametype.find(view);
		}
		String hyperlink(const char* site, const char* display) noexcept {
			return String("\x1b]8;;", site, "\x1b\\", display, "\x1b]8;;\x1b\\");
		}
		Utils::size_t _strlen(const String& val) noexcept {
			return val.getlen();
		}
		Utils::size_t _strlen(const Vector<char>& val) noexcept {
			return val.getlen();
		}
	}
}
