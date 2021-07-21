#include "headers/system.hpp"
#if defined(DLL_OS_windows)
 #include <windows.h> 
#else
 #include <fcntl.h>
#endif
namespace CPPExtensions {
	namespace Utils {
		const FileFlags _readflag = F_READ;
		const SeekFlag _setflag = S_RES;
		DLL_LOCAL thread_local ErrFlag _locerr = NO_ERROR;
		DLL_LOCAL int _sysflags(FileFlags flags) noexcept {
#ifdef DLL_OS_windows
			int sysflags = OPEN_EXISTING;
			if (flags & F_CREATE)
				sysflags = OPEN_EXISTING;
			if (flags & F_READ)
				sysflags |= GENERIC_READ;
			if (flags & F_WRITE)
				sysflags |= GENERIC_WRITE;
			return sysflags;
#else
			int sysflags = 0;
			if ((flags & F_READ) && (flags & F_WRITE)) {
				sysflags = O_RDWR;
			} else if (flags & F_READ) {
				sysflags = O_RDONLY;
			} else if (flags & F_WRITE) {
				sysflags = O_WRONLY;
			}
			return sysflags;
#endif
		}
		DLL_PUBLIC ErrFlag getlocerr() noexcept { return _locerr; }
		DLL_PUBLIC const char* strlocerr(ErrFlag flag) noexcept {
			switch (flag) {
				case NO_ERROR:
					return "Success.";
				case MEM_ERROR:
					return "Allocation failed.";
				case NULL_ERROR:
					return "Null pointer was passed to function.";
				default:
					return "Undefined Error";
			}
		}
		DLL_LOCAL void _clrerr() noexcept {
			_locerr = NO_ERROR;
		}
		DLL_LOCAL void _memerr() noexcept {
			_locerr = MEM_ERROR;
		}
		DLL_LOCAL void _nullerr() noexcept {
			_locerr = NULL_ERROR;
		}
	}
}
