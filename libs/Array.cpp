#include "headers/Array.hpp"
#include <iostream>
namespace CPPExtensions {
	template <>
	void arrayput(std::ostream& os, const char* val, Utils::size_t n) {
		return static_cast<void>(os.write(val, val[n-1] ? n : n-1));
	}
}
