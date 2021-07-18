#pragma once
#include "Macros.hpp"
namespace CPPExtensions {
	namespace Utils {
#ifdef _MSC_VER
		using desc = void*;
#else
		using desc = unsigned;
#endif
		extern const desc instream;
		extern const desc outstream;
		extern const desc errstream;
		using size_t = decltype(sizeof(0));
		int geterr() noexcept;
		const char* strerror(int) noexcept;
		volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
		unsigned epoch() noexcept; //high_resolution_clock since epoch
		int rand() noexcept; //random integer
		void memcpy(void*, const void*, size_t) noexcept;
		void memmove(void*, const void*, size_t) noexcept;
		size_t print(desc, const char*) noexcept;
		size_t write(desc, const char*, size_t) noexcept;
		int uncaught() noexcept; //number of uncaught exceptions
		void* malloc(size_t) noexcept; //calls operator new with nothrow_t
		void free(void*) noexcept; //calls operator delete
		[[noreturn]] void RunError(const char*);
		[[noreturn]] void RangeError(const char*);
	}
}
