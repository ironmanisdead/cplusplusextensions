#pragma once
#include ".hidden/GString-part.hpp"
#include "String.hpp"
namespace CustomUtils {
	namespace GString {
		template <class T>
		const String& runtype(const T& val) {
			return demangle(typeid(val).name());
		}
	}
}
