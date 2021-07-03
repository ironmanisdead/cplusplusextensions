#include "headers/Utils.hpp"
#include <stdexcept>
#include <cstdlib>
#include <chrono>
namespace CustomUtils {
	volatile void* Utils::ignore(volatile void*) noexcept {
		return nullptr;
	}
	int Utils::epoch() noexcept {
		using namespace std::chrono;
		auto time = system_clock::now().time_since_epoch();
		return duration_cast<milliseconds>(time).count();
	}
	int Utils::rand() noexcept {
		std::srand(Utils::epoch());
		return std::rand();
	}
	void Utils::memcpy(void* dest, const void* src, size len) noexcept {
		char* cdest = downcast<char*>(dest);
		const char* csrc = downcast<const char*>(src);
		for (size i = 0; i < len; i++)
			cdest[i] = csrc[i];
	}
	void Utils::memmove(void* dest, const void* src, size len) noexcept {
		char* cdest = downcast<char*>(dest);
		const char* csrc = downcast<const char*>(src);
		if (cdest < csrc)
			memcpy(dest, src, len);
		else
			for (size i = len; i > 0; i--)
				cdest[i-1] = csrc[i-1];
	}
	[[noreturn]] void Utils::RunError(const char* str) {
		throw std::runtime_error(str);
	}
	[[noreturn]] void Utils::RangeError(const char* str) {
		throw std::out_of_range(str);
	}
}
