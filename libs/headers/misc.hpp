#pragma once
namespace CPPExtensions {
	namespace Utils {
		volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
		int epoch() noexcept; //high_resolution_clock since epoch
		int rand() noexcept; //random integer
		void memcpy(void*, const void*, decltype(sizeof(0))) noexcept;
		void memmove(void*, const void*, decltype(sizeof(0))) noexcept;
		[[noreturn]] void RunError(const char*);
		[[noreturn]] void RangeError(const char*);
	}
}
