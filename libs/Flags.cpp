#include "headers/system.hpp"
#if defined(DLL_OS_windows)
 #include <windows.h> 
#else
 #include <fcntl.h>
#endif
namespace CPPExtensions {
	namespace Utils {
		const OpenFlags _readflag = F_READ;
		const SeekFlag _setflag = S_RES;
		DLL_LOCAL thread_local volatile ErrFlag _libErrInt = E_NOERR; //where _libErr is initialized
		DLL_LOCAL volatile ErrFlag& _getLibErr() noexcept { //support for funky compilers
			return _libErrInt;
		}
		DLL_LOCAL _sysFlags _sys_oflags(OpenFlags flags) noexcept {
#ifdef DLL_OS_windows
			_sysFlags sysflags = { OPEN_EXISTING, 0 };
			if (flags & F_CREATE) {
				if (flags & F_TRUNC)
					sysflags.disp = CREATE_ALWAYS;
				else
					sysflags.disp = OPEN_ALWAYS;
			}
			if (flags & F_READ)
				sysflags.access = GENERIC_READ;
			if (flags & F_WRITE)
				sysflags.access |= GENERIC_WRITE;
			return sysflags;
#else
			_sysFlags sysflags = { 0 };
			if ((flags & F_READ) && (flags & F_WRITE)) {
				sysflags.access = O_RDWR;
			} else if (flags & F_READ) {
				sysflags.access = O_RDONLY;
			} else if (flags & F_WRITE) {
				sysflags.access = O_WRONLY;
			}
			if (flags & F_CREATE)
				sysflags.access |= O_CREAT;
			if (flags & F_APPEND)
				sysflags.access |= O_APPEND;
			if (flags & F_TRUNC)
				sysflags.access |= O_TRUNC;
			return sysflags;
#endif
		}
		DLL_LOCAL S_WORD _sys_mflags(ModeFlags flags) noexcept {
#ifdef _MSC_WIN
			if (flags & M_WRITE) {
				return FILE_ATTRIBUTE_NORMAL;
			} else
				return FILE_ATTRIBUTE_READONLY;
#else
			S_WORD mode = S_IRUSR | S_IWUSR;
			if (flags & M_READ) {
				mode |= (S_IRGRP | S_IROTH);
			}
			if (flags & M_WRITE) {
				mode |= (S_IWGRP | S_IWOTH);
			}
			if (flags & M_EXEC) {
				mode |= (S_IXUSR | S_IXGRP | S_IXOTH);
			}
			return mode;
#endif
		}
		DLL_PUBLIC ErrFlag getlocerr() noexcept { return _libErrInt; }
		DLL_PUBLIC const char* strlocerr(ErrFlag flag) noexcept {
			switch (flag) {
				case E_NOERR:
					return "Success.";
				case E_MEM:
					return "Allocation failed.";
				case E_NULL:
					return "Null pointer was passed to function.";
				case E_TYPE:
					return "Undefined type error occured.";
				case E_CONV:
					return "Type conversion error occured.";
				case E_ATTR:
					return "Type had invalid properties.";
				case E_UNKNOWN:
					[[fallthrough]];
				default:
					return "Unknown/undefined Error.";
			}
		}
		DLL_LOCAL ErrFlag _noerr = E_NOERR;
		DLL_LOCAL ErrFlag _memerr = E_MEM;
		DLL_LOCAL ErrFlag _nullerr = E_NULL;
	}
}
