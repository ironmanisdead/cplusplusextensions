#pragma once
#include ".hide/GString.hpp"
#include "String.hpp"
#include <typeinfo>
namespace CustomUtils {
	namespace GString {
		template <class T>
		const String& runtype(const T& val) {
			return demangle(typeid(val).name());
		}
	}
}
