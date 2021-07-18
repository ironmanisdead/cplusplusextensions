#include "headers/Macros.hpp"
#include "headers/misc.hpp"
#include "headers/types.hpp"
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <cerrno>
#include <cstring>
namespace CPPExtensions {
	int Utils::geterr() noexcept { return errno; }
	const char* strerror(int err) noexcept {
		return std::strerror(err);
	}
	volatile void* Utils::ignore(volatile void*) noexcept {
		return nullptr;
	}
	unsigned Utils::epoch() noexcept {
		using namespace std::chrono;
		auto time = system_clock::now().time_since_epoch();
		return duration_cast<milliseconds>(time).count();
	}
	int Utils::rand() noexcept {
		static bool set = false;
		if (!set) {
			std::srand(Utils::epoch());
			set = true;
		}
		return std::rand();
	}
	void Utils::memcpy(void* dest, const void* src, size_t len) noexcept {
		char* cdest = downcast<char*>(dest);
		const char* csrc = downcast<const char*>(src);
		for (size_t i = 0; i < len; i++)
			cdest[i] = csrc[i];
	}
	void Utils::memmove(void* dest, const void* src, size_t len) noexcept {
		char* cdest = downcast<char*>(dest);
		const char* csrc = downcast<const char*>(src);
		if (cdest < csrc)
			memcpy(dest, src, len);
		else
			for (size_t i = len; i > 0; i--)
				cdest[i-1] = csrc[i-1];
	}
}
#ifdef _MSC_VER
 #include <fileapi.h>
#else
 #include <unistd.h>
#endif
#include "headers/.part/GString.hpp"
namespace CPPExtensions {
	namespace Utils {
#ifdef _MSC_VER
		const desc instream = GetStdHandle(STD_INPUT_HANDLE);
		const desc outstream = GetStdHandle(STD_OUTPUT_HANDLE);
		const desc errstream = GetStdHandle(STD_ERROR_HANDLE);
#else
		const desc instream = 0;
		const desc outstream = 1;
		const desc errstream = 2;
#endif
		size_t print(desc fd, const char* str) noexcept {
			auto numbytes = GString::_strlen(str);
#ifdef _MSC_VER
			int written = 0;
			HANDLE file = RECAST(HANDLE, file);
			::WriteFile(file, str, numbytes, &written, nullptr);
			return written;
#else
			return ::write(fd, str, numbytes);
#endif
		}
		size_t write(desc fd, const char* str, size_t len) noexcept {
#ifdef _MSC_VER
			int written = 0;
			HANDLE file = RECAST(HANDLE, file);
			::WriteFile(file, str, len, &written, nullptr);
			return written;
#else
			return ::write(fd, str, len);
#endif
		}
		int uncaught() noexcept {
			return std::uncaught_exceptions();
		}
		void* malloc(size_t size) noexcept {
			return ::operator new(size, std::nothrow_t {});
		}
		void free(void* ptr) noexcept { ::operator delete(ptr); }
		[[noreturn]] void RunError(const char* str) {
			throw std::runtime_error(str);
		}
		[[noreturn]] void RangeError(const char* str) {
			throw std::out_of_range(str);
		}
	}
}
