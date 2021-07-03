#include "headers/String.hpp"
#include "headers/GString.hpp"
#include "headers/StringView.hpp"
#include <iostream>
#define BOOL_EXTERN(fun, ...) template fun<true>(__VA_ARGS__); template fun<false>(__VA_ARGS__)
namespace CustomUtils {
	String::String() noexcept : trulen(0), len(0) {}
	void String::finalize() noexcept {
		if (trulen > 0)
			::operator delete(buffer);
		trulen = 0;
		len = 0;
	}
	String::~String() {
		finalize();
	}
	void String::allocate(Utils::size n1) {
		finalize();
		if (n1 == 0)
			return;
		char* temp = Utils::downcast<char*>(::operator new(n1));
		buffer = temp;
		trulen = n1;
		len = 0;
	}
	void String::resize(Utils::size n1) {
		if (trulen == 0)
			return allocate(n1);
		else if (n1 == 0)
			return finalize();
		else if (n1 < trulen)
			return;
		Utils::size ntru = trulen;
		while (ntru < n1)
			ntru *= 2;
		char* temp = Utils::downcast<char*>(::operator new(ntru));
		Utils::memcpy(temp, buffer, len);
		buffer = temp;
		trulen = ntru;
		buffer[len] = '\0';
	}
	template <bool reset>
	void String::byray(const char* val, Utils::size siz) noexcept(!reset) {
		Utils::size ntru, nlen;
		if (val[siz - 1] == '\0') {
			nlen = siz - 1;
			ntru = siz;
		} else {
			nlen = siz;
			ntru = siz + 1;
		}
		if constexpr (reset)
			resize(ntru);
		len = nlen;
		Utils::memcpy(buffer, val, siz);
		buffer[len] = '\0';
	}
	void String::byval(String&& val) noexcept {
		finalize();
		len = val.len;
		trulen = val.trulen;
		buffer = val.buffer;
		val.trulen = 0;
	}
	template <bool reset>
	void String::byval(const String& val) noexcept(!reset) {
		if (val.trulen == 0) {
			return finalize();
		}
		if constexpr (reset)
			resize(val.len + 1);
		Utils::memcpy(buffer, val.buffer, val.len);
		len = val.len;
		buffer[len] = '\0';
	}
	template <bool reset>
	void String::byval(const char* val) noexcept(!reset) {
		if (val == nullptr) {
			if constexpr (reset)
				return finalize();
			else
				return;
		}
		Utils::size siz = GString::strlen(val);
		if constexpr (reset)
			resize(siz + 1);
		Utils::memcpy(buffer, val, trulen);
		len = siz;
	}
	template <bool reset>
	void String::byval(Utils::size str) noexcept(!reset) {
		if constexpr (reset)
			resize(GString::strlen(str) + 1);
		addval(str);
	}
	template <bool reset>
	void String::byval(signed str) noexcept(!reset) {
		if constexpr (reset)
			resize(GString::strlen(str) + 1);
		addval(str);
	}
	void String::addray(const char* val, Utils::size siz) noexcept {
		Utils::size nlen;
		if (val[siz - 1] == '\0')
			nlen = siz - 1;
		else
			nlen = siz;
		Utils::memcpy(&buffer[len], val, siz);
		len += nlen;
		buffer[len] = '\0';
	}
	void String::addval(const char* val) noexcept {
		Utils::size siz = GString::strlen(val);
		Utils::memcpy(&buffer[len], val, siz);
		len += siz;
	}
	void String::addval(const String& val) noexcept {
		if (val.trulen > 0) {
			Utils::memcpy(&buffer[len], val.buffer, val.len);
			len += val.len;
		}
	}
	void String::addval(Utils::size str) noexcept {
		buffer[len] = '0';
		Utils::size siz = GString::strlen(str);
		for (Utils::size i = 0; i < siz; i++) {
			buffer[len + siz - i] = (str % 10);
			str /= 10;
		}
		len += siz;
	}
	void String::addval(signed str) noexcept {
		if (str < 0) {
			buffer[len++] = '-';
			str = -str;
		}
		buffer[len] = '0';
		Utils::size siz = GString::strlen(str);
		for (Utils::size i = 1; i <= siz; i++) {
			buffer[len + siz - i] = (str % 10) + '0';
			str /= 10;
		}
		len += siz;
	}
	Utils::strongcmp_t String::operator <=>(const String& val) const noexcept {
		if ((trulen == 0) || (val.trulen == 0))
			return (trulen <=> val.trulen);
		return (StringView(buffer, len) <=> StringView(val.buffer, val.len));
	}
	Utils::strongcmp_t String::valcmp(const StringView& val) const noexcept {
		return StringView(*this) <=> val;
	}
	template <>
	void stringput(std::ostream& os, const String& val) {
		if (val.trulen > 0)
			return static_cast<void>(os.write(val.buffer, val.len));
	}
	BOOL_EXTERN(void String::byval, const String&);
	BOOL_EXTERN(void String::byval, const char*);
	BOOL_EXTERN(void String::byval, Utils::size);
	BOOL_EXTERN(void String::byval, signed);
	BOOL_EXTERN(void String::byray, const char*, Utils::size);
}
