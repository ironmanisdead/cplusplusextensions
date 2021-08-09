#include "headers/All.hpp"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <cstdlib>
DLL_HIDE
namespace CPPExtensions {
	DLL_LOCAL [[noreturn]] void Utils::_abort() noexcept {
		std::abort();
	}
	template <>
	DLL_PUBLIC void _viewput(std::ostream& os, const StringView* val) {
		if (val && val->read() && (val->len > 0))
			return static_cast<void>(os.write(val->read(), val->len));
	}
	template <>
	DLL_PUBLIC void arrayput(std::ostream& os, const char* val, Utils::size_t n) {
		return static_cast<void>(os.write(val, val[n-1] ? n : n-1));
	}
	template <>
	DLL_PUBLIC void vecput(std::ostream& os, const Vector<char>* val) {
		if (val && (val->trulen > 0))
			return static_cast<void>(os.write(val->raw, val->len));
	}
	namespace Utils {
		DLL_PUBLIC u64 epoch() noexcept {
			using namespace std::chrono;
			return system_clock::now().time_since_epoch() / milliseconds(1);
		}
		DLL_PUBLIC int rand() noexcept {
			static bool set = false;
			if (!set) {
				std::srand(Utils::epoch());
				set = true;
			}
			return std::rand();
		}
		[[noreturn]] DLL_PUBLIC void RunError(const char* str) {
			throw std::runtime_error(str);
		}
		[[noreturn]] DLL_PUBLIC void RangeError(const char* str) {
			throw std::out_of_range(str);
		}
	}
}
DLL_RESTORE
