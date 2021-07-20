#include "headers/String.hpp"
#include "headers/GString.hpp"
#include "headers/StringView.hpp"
#include "headers/Vector.hpp"
#include "headers/Function.hpp"
#include "headers/BinMap.hpp"
#include <stdexcept>
#include <cstdlib>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
namespace CPPExtensions {
	namespace GString {
		[[noreturn]] void overflow(size_t len, size_t idx) {
			using namespace Utils;
			String msg = { "Array of size ", len, " cannot hold string of size ", idx };
			throw std::overflow_error(msg);
		}
		inline String _demangle(const char* src) noexcept {
#ifdef _MSC_VER
			return String(src);
#else
			int status;
			Object<char*> obj = { [&obj] () { std::free(obj.value); },
				abi::__cxa_demangle(src, 0, 0, &status) };
			return String(obj.value);
#endif
		}
		const String* demangle(const char* val) {
			static BinMap<StringView, String> nametype = {};
			StringView view = val;
			String* find = nametype.find(view);
			if (find) {
				return find;
			} else {
				String str = _demangle(val);
				auto ptr = nametype.insert(view, str);
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
