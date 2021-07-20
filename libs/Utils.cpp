#include "headers/Macros.hpp"
#include "headers/system.hpp"
#include "headers/types.hpp"
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <cerrno>
#include <cstring>
#include "headers/GString.hpp"
DLL_HIDE
namespace CPPExtensions {
	namespace Utils {
		int geterr() noexcept { return errno; }
		const char* strerror(int err) noexcept {
			return std::strerror(err);
		}
		volatile void* ignore(volatile void*) noexcept {
			return nullptr;
		}
		unsigned epoch() noexcept {
			using namespace std::chrono;
			auto time = system_clock::now().time_since_epoch();
			return duration_cast<milliseconds>(time).count();
		}
		int rand() noexcept {
			static bool set = false;
			if (!set) {
				std::srand(Utils::epoch());
				set = true;
			}
			return std::rand();
		}
		void memcpy(void* dest, const void* src, size_t len) noexcept {
			char* cdest = downcast<char*>(dest);
			const char* csrc = downcast<const char*>(src);
			for (size_t i = 0; i < len; i++)
				cdest[i] = csrc[i];
		}
		void memmove(void* dest, const void* src, size_t len) noexcept {
			char* cdest = downcast<char*>(dest);
			const char* csrc = downcast<const char*>(src);
			if (cdest < csrc)
				memcpy(dest, src, len);
			else
				for (size_t i = len; i > 0; i--)
					cdest[i-1] = csrc[i-1];
		}
	}
}
DLL_RESTORE
#ifdef _MSC_VER
 #include <windows.h>
 #include <fileapi.h>
 #include <winbase.h>
 #include <sysinfoapi.h>
#else
 #include <fstream>
 #include <unistd.h>
 #include <fcntl.h>
#endif
DLL_HIDE
namespace CPPExtensions {
	namespace Utils {
#ifdef _MSC_VER
		DLL_PUBLIC const desc std_in = GetStdHandle(STD_INPUT_HANDLE);
		DLL_PUBLIC const desc std_out = GetStdHandle(STD_OUTPUT_HANDLE);
		DLL_PUBLIC const desc std_err = GetStdHandle(STD_ERROR_HANDLE);
		DLL_PUBLIC const desc errdesc = HFILE_ERROR;
#endif
		DLL_PUBLIC ssize_t write(desc fd, const char* str, size_t len) noexcept {
#ifdef _MSC_VER
			int written = 0;
			HANDLE file = RECAST(HANDLE, file);
			if (::WriteFile(file, str, len, &written, nullptr))
				return written;
			else
				return -1;
#else
			return ::write(fd, str, len);
#endif
		}
		DLL_PUBLIC ssize_t writestr(desc fd, const String& val) noexcept {
			if (val.gettlen() == 0)
				return -1;
			else
				return write(fd, val.data(), val.getlen());
		}
		DLL_PUBLIC ssize_t print(desc fd, const char* str) noexcept {
			auto numbytes = GString::_strlen(str);
			return write(fd, str, numbytes);
		}
		DLL_PUBLIC bool putchar(desc fd, char ch) noexcept {
			static char ray[1];
			ray[0] = ch;
			if (write(fd, ray, 1) > 0) {
				return true;
			} else {
				return false;
			}
		}
		DLL_PUBLIC ssize_t puts(desc fd, const char* str) noexcept {
			auto numbytes = GString::_strlen(str);
			auto written = write(fd, str, numbytes);
			if (written != -1) {
				if (putchar(fd, '\n')) {
					return written + 1;
				} else {
					return written;
				}
			} else {
				return -1;
			}
		}
		DLL_PUBLIC ssize_t read(desc fd, char* str, size_t len) noexcept {
#ifdef _MSC_VER
			int written = 0;
			HANDLE file = RECAST(HANDLE, file);
			if (::ReadFile(file, str, len, &written, nullptr))
				return written;
			else
				return -1;
#else
			return ::read(fd, str, len);
#endif
		}
		DLL_PUBLIC desc open(const chtype* path, Flags flags) noexcept {
#ifdef _MSC_VER
			int sysflags = 0;
			int creator = OPEN_EXISTING;
			if (flags & F_CREATE)
				creator = OPEN_ALWAYS;
			if (flags & F_READ)
				sysflags = GENERIC_READ;
			if (flags & F_WRITE)
				sysflags |= GENERIC_WRITE;
			return ::CreateFileA(path, sysflags,
					FILE_SHARE_READ, nullptr,
					sysflags, FILE_ATTRIBUTE_NORMAL, nullptr);
#else
			int sysflags = 0;
			if ((flags & F_READ) && (flags & F_WRITE)) {
				sysflags = O_RDWR;
			} else if (flags & F_READ) {
				sysflags = O_RDONLY;
			} else if (flags & F_WRITE) {
				sysflags = O_WRONLY;
			}
			if (flags & F_CREATE)
				sysflags |= O_CREAT;
			mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP;
			return ::open(path, sysflags, mode);
#endif
		}
		DLL_PUBLIC bool close(desc fd) noexcept {
#ifdef _MSC_VER
			return ::CloseHandle(RECAST(HANDLE, file));
#else
			if (::close(fd) == 0)
				return true;
			else
				return false;
#endif
		}
		DLL_PUBLIC ssize_t seek(desc fd, ssize_t pos, SeekFlag method) noexcept {
#ifdef _MSC_VER
			auto result = ::SetFilePointer(fd, pos, nullptr, method);
			if (result == INVALID_SET_FILE_POINTER)
				return -1;
			else
				return result;
#else
			auto result = ::lseek(fd, pos, method);
			if (result == -1)
				return -1;
			else
				return result;
#endif
		}
		DLL_PUBLIC bool unlink(const chtype* name) noexcept {
#ifdef _MSC_VER
			return ::DeleteFile(name);
#else
			if (::unlink(name) == 0)
				return true;
			else
				return false;
#endif
		}
		DLL_PUBLIC u64 uptime() noexcept {
#ifdef _MSC_VER
			return GetTickCount64();
#else
			static desc file = open("/proc/uptime", F_READ);
			static char array[16];
			static StringView view = { array, 16 };
			if (file == errdesc)
				return 0;
			seek(file, 0, S_RES);
			if (read(file, array, 16) != -1) {
				return view.convert_double() * 1000;
			} else {
				return 0;
			}
#endif
		}
		DLL_PUBLIC void usleep(ulong time) noexcept {
			using namespace std::this_thread;
			using namespace std::chrono;
			sleep_for(milliseconds(time));
		}
		DLL_PUBLIC int uncaught() noexcept {
			return std::uncaught_exceptions();
		}
		DLL_PUBLIC void* malloc(size_t size) noexcept {
			return ::operator new(size, std::nothrow_t {});
		}
		DLL_PUBLIC void free(void* ptr) noexcept { ::operator delete(ptr); }
		[[noreturn]] DLL_PUBLIC void RunError(const char* str) {
			throw std::runtime_error(str);
		}
		[[noreturn]] DLL_PUBLIC void RangeError(const char* str) {
			throw std::out_of_range(str);
		}
	}
}
DLL_RESTORE
