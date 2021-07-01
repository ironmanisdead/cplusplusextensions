#include "headers/Array.hpp"
#include <iostream>
namespace CustomUtils {
	template <>
	void arrayfeed(std::ostream& os, const char* val, Utils::size n) {
		return static_cast<void>(os.write(val, val[n-1] ? n : n-1));
	}
}
