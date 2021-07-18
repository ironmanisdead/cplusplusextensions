#pragma once
#include "Macros.hpp"
namespace CPPExtensions {
	namespace Utils {
#ifdef _MSC_VER
		using desc = void*;
#else
		using desc = unsigned;
#endif
		using size_t = decltype(sizeof(0));
		int geterr() noexcept;
		const char* strerror(int) noexcept;
		volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
		unsigned epoch() noexcept; //high_resolution_clock since epoch
		int rand() noexcept; //random integer
		int uncaught() noexcept;
		void memcpy(void*, const void*, size_t) noexcept;
		void memmove(void*, const void*, size_t) noexcept;
		long print(desc, const char*) noexcept;
		long write(desc, const char*, size_t) noexcept;
		[[noreturn]] void RunError(const char*);
		[[noreturn]] void RangeError(const char*);
	}
}
