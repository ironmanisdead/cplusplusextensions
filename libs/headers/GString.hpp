#pragma once
#include ".hidden/GString-part.hpp"
#include "String.hpp"
namespace CustomUtils {
	template <class T>
	const String& GString::runtype(const T& val) {
		return demangle(typeid(val).name());
	}
}
