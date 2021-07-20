#pragma once
#include "Macros.hpp"
#include ".part/internals.hpp"
namespace CPPExtensions {
	namespace Utils {
		//chtype is the type of character that the OS supports in filenames
		//desc is the OS-defined file descriptor or handle
		//std_in is the standard input descriptor/handle
		//std_out is the standard output descriptor/handle
		//std_err is the standard error descriptor/handle
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
			F_CREATE = 1, //create file if non-existant
			F_READ = 2, //open file in read mode
			F_WRITE = 4, //open file in write mode
			F_RDWR = 6 //F_RDRW = F_READ | F_WRITE
		};
		enum SeekFlag { //Seek flags are for seek 
			S_RES, //Seeks from beginning
			S_CUR, //Seeks from current position
			S_END //Seeks from end
		};
		constexpr Flags operator |(Flags a, Flags b) noexcept {
			return (Flags)((int)a | (int) b);
		} //Allows merging file flags with or operator
		int geterr() noexcept; //return errno
		const char* strerror(int) noexcept; //gets string from error code
		volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
		unsigned epoch() noexcept; //high_resolution_clock since epoch
		u64 uptime() noexcept; //gets the uptime of the system in milliseconds
		int rand() noexcept; //random integer
		void memcpy(void*, const void*, size_t) noexcept; //copies memory from one location to another
		void memmove(void*, const void*, size_t) noexcept; //safer way to copy memory, checks for overlap
		ssize_t write(desc, const char*, size_t) noexcept; //prints out string of specific length to stream
		bool putchar(desc, char) noexcept; //prints character to stream
		ssize_t print(desc, const char*) noexcept; //prints out string to stream
		ssize_t puts(desc, const char*) noexcept; //prints out string and newline
		desc open(const chtype*, Flags) noexcept; //opens file under descriptor with flags
		bool close(desc) noexcept; //closes open file descriptor
		ssize_t seek(desc, ssize_t, SeekFlag) noexcept; //seeks with method
		bool unlink(const chtype*) noexcept; //deletes (unlinks) a file
		void usleep(ulong) noexcept; //sleeps for a specific number of milliseconds
		int uncaught() noexcept; //number of uncaught exceptions
		void* malloc(size_t) noexcept; //calls operator new with nothrow_t
		void free(void*) noexcept; //calls operator delete
		[[noreturn]] void RunError(const char*); //throws std::runtime_error
		[[noreturn]] void RangeError(const char*); //throws std::out_of_range error
	}
}
