#pragma once
#include ".part/GString.hpp"
#include "String.hpp"
#include <typeinfo>
namespace CPPExtensions {
	namespace GString {
		template <class T>
		const String& runtype(const T& val) {
			return demangle(typeid(val).name());
		}
	}
}
