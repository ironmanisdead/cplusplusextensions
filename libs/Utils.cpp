#include "headers/predefs.hpp"
#include "headers/system_internals.hpp"
#include "headers/types.hpp"
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "headers/GString.hpp"
DLL_HIDE
namespace CPPExtensions {
	namespace Utils {
		DLL_PUBLIC volatile void* ignore(volatile void*) noexcept {
			return nullptr;
		}
		DLL_PUBLIC unsigned epoch() noexcept {
			using namespace std::chrono;
			auto time = system_clock::now().time_since_epoch();
			return duration_cast<milliseconds>(time).count();
		}
		DLL_PUBLIC int rand() noexcept {
			static bool set = false;
			if (!set) {
				std::srand(Utils::epoch());
				set = true;
			}
			return std::rand();
		}
		DLL_PUBLIC void memcpy(void* dest, const void* src, size_t len) noexcept {
			char* cdest = downcast<char*>(dest);
			const char* csrc = downcast<const char*>(src);
			for (size_t i = 0; i < len; i++)
				cdest[i] = csrc[i];
		}
		DLL_PUBLIC void memmove(void* dest, const void* src, size_t len) noexcept {
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
#include <cerrno>
#include <cstring>
#include "headers/String.hpp"
DLL_HIDE
namespace CPPExtensions {
	namespace Utils {
		DLL_PUBLIC int geterrno() noexcept { return errno; }
		DLL_PUBLIC const char* strerrno(int err) noexcept {
			return std::strerror(err);
		}
#ifdef _MSC_VER
		DLL_PUBLIC const desc std_in = GetStdHandle(STD_INPUT_HANDLE);
		DLL_PUBLIC const desc std_out = GetStdHandle(STD_OUTPUT_HANDLE);
		DLL_PUBLIC const desc std_err = GetStdHandle(STD_ERROR_HANDLE);
		DLL_PUBLIC const desc errdesc = HFILE_ERROR;
#endif
		DLL_PUBLIC ssize_t write(desc fd, const char* str, size_t len) noexcept {
			_clrerr();
			if (str == nullptr) {
				_nullerr();
				return -1;
			}
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
		static String _putstr = {};
		DLL_PUBLIC ssize_t puts(desc fd, const char* str) noexcept {
			_putstr.set(str, '\n');
			if (_putstr.gettlen() > 0)
				return write(fd, _putstr.data(), _putstr.getlen());
			else
				return -1;
		}
		DLL_PUBLIC bool puts_alloc(size_t siz) noexcept {
			return _putstr.allocate(siz);
		}
		DLL_PUBLIC void puts_free() noexcept { _putstr = nullptr; }
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
		DLL_PUBLIC desc open(const chtype* path, FileFlags flags) noexcept {
			int sysflags = _sysflags(flags);
#ifdef _MSC_VER
			return ::CreateFileA(path, sysflags,
					FILE_SHARE_READ, nullptr,
					sysflags, FILE_ATTRIBUTE_NORMAL, nullptr);
#else
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
			static desc file = open("/proc/uptime", _readflag);
			static char array[16];
			static StringView view = { array, 16 };
			if (file == errdesc)
				return 0;
			seek(file, 0, _setflag);
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
			_clrerr();
			void* alloc = ::operator new(size, std::nothrow_t {});
			if (!alloc)
				_memerr();
			return alloc;
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
