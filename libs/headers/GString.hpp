#pragma once
#include ".part/GString.hpp"
#include "String.hpp"
#include <typeinfo>
namespace CPPExtensions {
	namespace GString {
		template <class T>
		const String* runtype(const T& val) noexcept {
			return demangle(typeid(val).name());
		}
	}
	constexpr Utils::size_t GString::escsize(const char* str) noexcept {
		if (str[0] != '\x1b')
			return 0;
		if (str[1] != '[')
			return 0;
		int i = 2;
		for (char ch = str[i]; ch; ch = str[++i]) {
			if ((ch >= 'A') && (ch <= 'Z'))
				return i;
			if ((ch >= 'a') && (ch <= 'z'))
				return i;
		}
		return 0;
	}
	constexpr Utils::ssize_t GString::nextesc(StringView view) noexcept {
		if (!view.read())
			return -1;
		const char* str = view.read();
		for (size_t i = 0; i < view.len; i++) {
			if (str[i] == '\x1b')
				return i;
		}
		return -1;
	}
}
