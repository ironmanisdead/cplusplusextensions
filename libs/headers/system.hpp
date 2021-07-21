#pragma once
#include "predefs.hpp"
DLL_HIDE
namespace CPPExtensions {
	class String;
	namespace Utils {
		//chtype is the type of character that the OS supports in filenames
		//desc is the OS-defined file descriptor or handle
		//std_in is the standard input descriptor/handle
		//std_out is the standard output descriptor/handle
		//std_err is the standard error descriptor/handle
#ifdef _MSC_VER
		using chtype = wchar_t;
		using desc = void*;
		DLL_PUBLIC extern const desc std_in;
		DLL_PUBLIC extern const desc std_out;
		DLL_PUBLIC extern const desc std_err;
		DLL_PUBLIC extern const desc errdesc;
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
		DLL_PUBLIC int geterr() noexcept; //return errno
		DLL_PUBLIC const char* strerror(int) noexcept; //gets string from error code
		DLL_PUBLIC volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
		DLL_PUBLIC unsigned epoch() noexcept; //high_resolution_clock since epoch
		DLL_PUBLIC u64 uptime() noexcept; //gets the uptime of the system in milliseconds
		DLL_PUBLIC int rand() noexcept; //random integer
		DLL_PUBLIC void memcpy(void*, const void*, size_t) noexcept; //copies memory from one location to another
		DLL_PUBLIC void memmove(void*, const void*, size_t) noexcept; //safer way to copy memory, checks for overlap
		DLL_PUBLIC ssize_t writestr(desc, const String&) noexcept; //writes out String object to file descriptor/handle
		DLL_PUBLIC ssize_t write(desc, const char*, size_t) noexcept; //prints out string of specific length to stream
		DLL_PUBLIC bool putchar(desc, char) noexcept; //prints character to stream
		DLL_PUBLIC ssize_t print(desc, const char*) noexcept; //prints out string to stream
		DLL_PUBLIC ssize_t puts(desc, const char*) noexcept; //prints out string and newline
		DLL_PUBLIC bool puts_alloc(size_t) noexcept; //allocates specific amount of memory beforehand for puts
		DLL_PUBLIC void puts_free() noexcept; //frees all memory used by puts
		DLL_PUBLIC desc open(const chtype*, Flags) noexcept; //opens file under descriptor with flags
		DLL_PUBLIC bool close(desc) noexcept; //closes open file descriptor
		DLL_PUBLIC ssize_t seek(desc, ssize_t, SeekFlag) noexcept; //seeks with method
		DLL_PUBLIC bool unlink(const chtype*) noexcept; //deletes (unlinks) a file
		DLL_PUBLIC void usleep(ulong) noexcept; //sleeps for a specific number of milliseconds
		DLL_PUBLIC int uncaught() noexcept; //number of uncaught exceptions
		DLL_PUBLIC void* malloc(size_t) noexcept; //calls operator new with nothrow_t
		DLL_PUBLIC void free(void*) noexcept; //calls operator delete
		[[noreturn]] DLL_PUBLIC void RunError(const char*); //throws std::runtime_error
		[[noreturn]] DLL_PUBLIC void RangeError(const char*); //throws std::out_of_range error
	}
}
DLL_RESTORE
