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
			using namespace Utils;
			String msg = { "Array of size ", len, " cannot hold string of size ", idx };
			throw std::overflow_error(msg);
		}
		static BinMap<StringView, String> nametype = {};
		const String* demangle(const char* val) {
			StringView view = val;
			String* find = nametype.find(view);
			if (find) {
				return find;
			} else {
				int status;
				Object<char*> obj = { [&obj] () { std::free(obj.value); }, abi::__cxa_demangle(val, 0, 0, &status) };
				auto ptr = nametype.insert(view, obj.value);
				if (ptr.value) {
					return &(ptr.value->getValue());
				} else {
					return nullptr;
				}
			}
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
