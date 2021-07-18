#pragma once
#include "Macros.hpp"
#include ".part/internals.hpp"
namespace CPPExtensions {
	namespace Utils {
#ifdef _MSC_VER
		using desc = void*;
#else
		using desc = int;
#endif
		extern const desc instream;
		extern const desc outstream;
		extern const desc errstream;
		int geterr() noexcept;
		const char* strerror(int) noexcept;
		volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
		unsigned epoch() noexcept; //high_resolution_clock since epoch
		int rand() noexcept; //random integer
		void memcpy(void*, const void*, size_t) noexcept;
		void memmove(void*, const void*, size_t) noexcept;
		ssize_t print(desc, const char*) noexcept;
		ssize_t write(desc, const char*, size_t) noexcept;
		int uncaught() noexcept; //number of uncaught exceptions
		void* malloc(size_t) noexcept; //calls operator new with nothrow_t
		void free(void*) noexcept; //calls operator delete
		[[noreturn]] void RunError(const char*);
		[[noreturn]] void RangeError(const char*);
	}
}
