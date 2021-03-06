#include "headers/predefs.hpp"
#include "headers/system_internals.hpp"
#include "headers/types.hpp"
#include "headers/GString.hpp"
#include <signal.h>
#include <setjmp.h>
DLL_HIDE
namespace CPPExtensions {
	namespace Utils {
		DLL_PUBLIC volatile void* ignore(volatile void*) noexcept {
			return nullptr;
		}
		static volatile void* _ignore(char) noexcept {
			return nullptr;
		}
		static thread_local jmp_buf savepoint; //keeps track of state
		static void _unBlockSegV() noexcept { //removes SIGSEGV block
#if DLL_OS_unix
			sigset_t signal_set;
			sigemptyset(&signal_set);
			sigaddset(&signal_set, SIGSEGV);
			sigprocmask(SIG_UNBLOCK, &signal_set, nullptr);
#endif
		}
		static void _onError(int) noexcept {
			longjmp(savepoint, 2);
		}
		DLL_PUBLIC bool isvalid(const volatile void* read) noexcept {
			if (read == nullptr)
				return false;
			switch (setjmp(savepoint)) {
				case 0:
					_unBlockSegV();
					signal(SIGSEGV, _onError);
					_ignore(*reinterpret_cast<const volatile char*>(read));
					longjmp(savepoint, 1); //longjmp should (theoretically) restore the signal handler
				case 1:
					return true;
				case 2:
					return false;
			}
			_abort();
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
#ifdef DLL_OS_windows
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
#ifdef DLL_OS_windows
		DLL_PUBLIC const f_desc std_in = GetStdHandle(STD_INPUT_HANDLE);
		DLL_PUBLIC const f_desc std_out = GetStdHandle(STD_OUTPUT_HANDLE);
		DLL_PUBLIC const f_desc std_err = GetStdHandle(STD_ERROR_HANDLE);
		DLL_PUBLIC const f_desc errdesc = HFILE_ERROR;
#endif
		DLL_PUBLIC ssize_t write(f_desc fd, const char* str, size_t len) noexcept {
			_libErr = _noerr;
			if (str == nullptr) {
				_libErr = _nullerr;
				return -1;
			}
#ifdef DLL_OS_windows
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
		DLL_PUBLIC ssize_t print(f_desc fd, const char* str) noexcept {
			auto numbytes = GString::_strlen(str);
			return write(fd, str, numbytes);
		}
		DLL_PUBLIC bool putchar(f_desc fd, char ch) noexcept {
			static thread_local char ray[1];
			ray[0] = ch;
			if (write(fd, ray, 1) > 0) {
				return true;
			} else {
				return false;
			}
		}
		static String _putstr = {};
		DLL_PUBLIC ssize_t puts(f_desc fd, const char* str) noexcept {
			_putstr.set(str, '\n');
			if (_putstr.gettlen() > 0)
				return write(fd, _putstr.data(), _putstr.getlen());
			else
				return -1;
		}
		DLL_LOCAL ssize_t _puts(f_desc fd, const char* str, Utils::size_t len) noexcept {
			_putstr.set(StringView(str, len), '\n');
			if (_putstr.gettlen() > 0)
				return write(fd, _putstr.data(), _putstr.getlen());
			else
				return -1;
		}
		DLL_PUBLIC bool puts_alloc(size_t siz) noexcept {
			return _putstr.allocate(siz);
		}
		DLL_PUBLIC void puts_free() noexcept { _putstr = nullptr; }
		DLL_PUBLIC ssize_t read(f_desc fd, char* str, size_t len) noexcept {
#ifdef DLL_OS_windows
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
		DLL_PUBLIC f_desc open(const char* path, OpenFlags flags) noexcept {
			_sysFlags sysflags = _sys_oflags(flags);
#ifdef DLL_OS_windows
			return ::CreateFileA(path, sysflags.access,
					FILE_SHARE_READ, nullptr,
					sysflags.disp, FILE_ATTRIBUTE_NORMAL, nullptr);
#else
			mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP;
			return ::open(path, sysflags.access, mode);
#endif
		}
		DLL_PUBLIC f_desc open(const char* path, OpenFlags flags, ModeFlags mode) noexcept {
			_sysFlags sysflags = _sys_oflags(flags);
			S_WORD sysmode = _sys_mflags(mode);
#ifdef DLL_OS_windows
			return ::CreateFileA(path, sysflags.access,
					FILE_SHARE_READ, nullptr,
					sysflags.creation, sysmode, nullptr);
#else
			return ::open(path, sysflags.access, sysmode);
#endif
		}
		DLL_PUBLIC bool close(f_desc fd) noexcept {
#ifdef DLL_OS_windows
			return ::CloseHandle(RECAST(HANDLE, file));
#else
			if (::close(fd) == 0)
				return true;
			else
				return false;
#endif
		}
		DLL_PUBLIC ssize_t seek(f_desc fd, ssize_t pos, SeekFlag method) noexcept {
			_libErr = _noerr;
			S_WORD flags = _sys_seek(method);
			if (_libErr != _noerr)
				return -1;
#ifdef DLL_OS_windows
			auto result = ::SetFilePointer(fd, pos, nullptr, flags);
			if (result == INVALID_SET_FILE_POINTER)
				return -1;
			else
				return result;
#else
			auto result = ::lseek(fd, pos, flags);
			if (result == -1)
				return -1;
			else
				return result;
#endif
		}
		DLL_PUBLIC bool unlink(const char* name) noexcept {
#ifdef DLL_OS_windows
			return ::DeleteFile(name);
#else
			if (::unlink(name) == 0)
				return true;
			else
				return false;
#endif
		}
		DLL_PUBLIC u64 uptime() noexcept {
#ifdef DLL_OS_windows
			return GetTickCount64();
#else
			static f_desc file = open("/proc/uptime", _readflag);
			static thread_local char array[16];
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
		DLL_PUBLIC void sleep(ulong time) noexcept {
#ifdef DLL_OS_windows
			Sleep(time);
#else
			usleep(time * 1000);
#endif
		}
		DLL_PUBLIC int uncaught() noexcept {
			return std::uncaught_exceptions();
		}
		DLL_PUBLIC void* malloc(size_t size) noexcept {
			_libErr = _noerr;
			void* alloc = ::operator new(size, std::nothrow_t {});
			if (!alloc)
				_libErr = _memerr;
			return alloc;
		}
		DLL_PUBLIC bool free(void* ptr) noexcept { 
			if (ptr) {
				::operator delete(ptr);
				return true;
			} else {
				return false;
			}
		}
	}
}
DLL_RESTORE
