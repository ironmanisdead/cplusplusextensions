#pragma once
#include "Macros.hpp"
#include ".part/internals.hpp"
namespace CPPExtensions {
	namespace Utils {
		//desc is a file descriptor or handle
#ifdef _MSC_VER
		using chtype = wchar_t;
		using desc = void*;
		extern const desc std_in;
		extern const desc std_out;
		extern const desc std_err;
		extern const desc errdesc;
#else
		using chtype = char;
		using desc = int;
		constexpr desc std_in = 0;
		constexpr desc std_out = 1;
		constexpr desc std_err = 2;
		constexpr desc errdesc = -1;
#endif
		enum Flags { //flags are for opening files
			F_CREATE = 1,
			F_READ = 2,
			F_WRITE = 4,
			F_RDWR = 6
		};
		constexpr Flags operator |(Flags a, Flags b) noexcept {
			return (Flags)((int)a | (int) b);
		}
		int geterr() noexcept;
		const char* strerror(int) noexcept;
		volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
		unsigned epoch() noexcept; //high_resolution_clock since epoch
		u64 uptime() noexcept; //gets the uptime of the system in milliseconds
		int rand() noexcept; //random integer
		void memcpy(void*, const void*, size_t) noexcept; //copies memory from one location to another
		void memmove(void*, const void*, size_t) noexcept; //safer way to copy memory, checks for overlap
		ssize_t print(desc, const char*) noexcept; //prints out string to stream
		ssize_t write(desc, const char*, size_t) noexcept; //prints out string of specific length to stream
		desc open(const chtype*, Flags) noexcept; //opens file under descriptor with flags
		bool close(desc) noexcept; //closes open file descriptor
		bool unlink(const chtype*) noexcept; //deletes (unlinks) a file
		void usleep(ulong) noexcept; //sleeps for a specific number of milliseconds
		int uncaught() noexcept; //number of uncaught exceptions
		void* malloc(size_t) noexcept; //calls operator new with nothrow_t
		void free(void*) noexcept; //calls operator delete
		[[noreturn]] void RunError(const char*);
		[[noreturn]] void RangeError(const char*);
	}
}
