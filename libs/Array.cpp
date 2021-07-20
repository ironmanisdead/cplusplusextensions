#include "headers/Array.hpp"
#include <iostream>
DLL_HIDE
namespace CPPExtensions {
	template <>
	DLL_PUBLIC void arrayput(std::ostream& os, const char* val, Utils::size_t n) {
		return static_cast<void>(os.write(val, val[n-1] ? n : n-1));
	}
}
DLL_RESTORE
