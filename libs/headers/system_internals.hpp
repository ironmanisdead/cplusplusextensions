#pragma once
#include "predefs.hpp"
DLL_HIDE
namespace CPPExtensions {
	class String;
	class StringView;
	namespace Utils {
		//chtype is the type of character that the OS supports in filenames
		//f_desc is the OS-defined file descriptor or handle
		//std_in is the standard input descriptor/handle
		//std_out is the standard output descriptor/handle
		//std_err is the standard error descriptor/handle
		//S_WORD is the system defined default flag type (internal use)
		enum OpenFlags : unsigned; //Flags for opening file
		enum ModeFlags : unsigned; //permissions granted if file is created during open
		enum SeekFlag : unsigned; //Flags instructing seek how to traverse
		enum ErrFlag : unsigned; //Error states
		constexpr OpenFlags operator |(OpenFlags a, OpenFlags b) noexcept {
			return (OpenFlags)((unsigned)a | (unsigned)b);
		} //Allows merging file flags with or operator
		constexpr ModeFlags operator |(ModeFlags a, ModeFlags b) noexcept {
			return (ModeFlags)((unsigned)a | (unsigned)b);
		} //Allows merging mode flags with or operator
#ifdef _MSC_VER
		using S_WORD = int32_t;
		using f_desc = void*;
		DLL_PUBLIC extern const f_desc std_in;
		DLL_PUBLIC extern const f_desc std_out;
		DLL_PUBLIC extern const f_desc std_err;
		DLL_PUBLIC extern const f_desc errdesc;
#else
		using S_WORD = int;
		using f_desc = int;
		constexpr f_desc std_in = 0;
		constexpr f_desc std_out = 1;
		constexpr f_desc std_err = 2;
		constexpr f_desc errdesc = -1;
		DLL_LOCAL extern const OpenFlags _readflag; //read flag for uptime() to open "/proc/uptime"(linux)(internal)
		DLL_LOCAL extern const SeekFlag _setflag; //reset flag for uptime() to reset "/proc/uptime"(linux)(internal)
#endif
		struct _sysFlags { //system defined internal flags
			S_WORD access;
#ifdef _MSC_VER
			S_WORD disp;
#endif
		};
		struct f_type;
		DLL_LOCAL extern thread_local volatile ErrFlag _libErr; //error code storage (internal)
		DLL_PUBLIC volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
		DLL_LOCAL extern ErrFlag _noerr; //empty error code(internal use)
		DLL_LOCAL extern ErrFlag _memerr; //memory error code(internal use)
		DLL_LOCAL extern ErrFlag _nullerr; //null error code(internal use)
		DLL_LOCAL _sysFlags _sys_oflags(OpenFlags) noexcept; //converts OpenFlags into system specific flags(internal)
		DLL_LOCAL S_WORD _sys_mflags(ModeFlags) noexcept; //converts SeekFlags into system specific flags(internal)
		DLL_PUBLIC int geterrno() noexcept; //returns current errno
		DLL_PUBLIC const char* strerrno(int) noexcept; //returns string describing global(geterrno()) error code
		DLL_PUBLIC ErrFlag getlocerr() noexcept; //gets local error code
		DLL_PUBLIC const char* strlocerr(ErrFlag) noexcept; //returns string describing local(getlocerr()) error code
		DLL_PUBLIC u64 epoch() noexcept; //high_resolution_clock since epoch
		DLL_PUBLIC u64 uptime() noexcept; //gets the uptime of the system in milliseconds
		DLL_PUBLIC int rand() noexcept; //random integer
		//memcpy copies memory from one location (2nd parameter) to another location (1st parameter)
		DLL_PUBLIC void memcpy(void*, const void*, size_t) noexcept;
		DLL_PUBLIC void memmove(void*, const void*, size_t) noexcept; //safer way to copy memory, checks for overlap
		DLL_PUBLIC ssize_t write(f_desc, const char*, size_t) noexcept; //prints out string of specific length to stream
		DLL_PUBLIC DEPRECATE(ssize_t writestr(f_desc, const StringView&) noexcept,
			"Utils::writestr() is deprecated, use StringView::write() instead");
		DLL_PUBLIC DEPRECATE(ssize_t writestr(f_desc, const String&) noexcept,
			"Utils::writestr() is deprecated, use String::write() instead");
		DLL_PUBLIC bool putchar(f_desc, char) noexcept; //prints character to stream
		DLL_PUBLIC ssize_t print(f_desc, const char*) noexcept; //prints out string to stream
		DLL_PUBLIC ssize_t puts(f_desc, const char*) noexcept; //prints out string and newline
		DLL_LOCAL ssize_t _puts(f_desc, const char*, size_t) noexcept; //prints off string followed by newline(internal)
		DLL_PUBLIC bool puts_alloc(size_t) noexcept; //allocates specific amount of memory beforehand for puts
		DLL_PUBLIC void puts_free() noexcept; //frees all memory used by puts
		DLL_PUBLIC f_desc open(const char*, OpenFlags) noexcept; //opens file under descriptor with flags
		DLL_PUBLIC f_desc open(const char*, OpenFlags, ModeFlags) noexcept; //open() with settable mode flags
		DLL_PUBLIC bool close(f_desc) noexcept; //closes open file descriptor
		DLL_PUBLIC ssize_t seek(f_desc, ssize_t, SeekFlag) noexcept; //seeks with method
		DLL_PUBLIC bool unlink(const char*) noexcept; //deletes (unlinks) a file
		DLL_PUBLIC void sleep(ulong) noexcept; //sleeps for a specific number of milliseconds
		DLL_PUBLIC int uncaught() noexcept; //number of uncaught exceptions
		DLL_PUBLIC void* malloc(size_t) noexcept; //calls operator new with nothrow_t
		DLL_PUBLIC bool free(void*) noexcept; //calls operator delete
		[[noreturn]] DLL_PUBLIC void RunError(const char*); //throws std::runtime_error
		[[noreturn]] DLL_PUBLIC void RangeError(const char*); //throws std::out_of_range error
	}
}
DLL_RESTORE
