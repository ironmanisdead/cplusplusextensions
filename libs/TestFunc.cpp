#include "headers/TestFunc.hpp"
DLL_HIDE
namespace CPPExtensions {
	DLL_PUBLIC Function<int()> FuncTest() noexcept {
		int i = 0;
		return [i] () mutable { return i++; };
	}
}
DLL_RESTORE
