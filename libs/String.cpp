#include "headers/String.hpp"
#include "headers/GString.hpp"
#include "headers/StringView.hpp"
#include <iostream>
#define BOOL_EXTERN(fun, ...) template fun<true>(__VA_ARGS__); template fun<false>(__VA_ARGS__)
namespace CustomUtils {
	String::String() noexcept : trulen(0), view(nullptr) {}
	void String::finalize() noexcept {
		if (trulen > 0)
			::operator delete(view.edit());
		trulen = 0;
		view.len = 0;
	}
	String::~String() {
		finalize();
	}
	void String::allocate(Utils::size n1) {
		finalize();
		if (n1 == 0)
			return;
		char* temp = Utils::downcast<char*>(::operator new(n1));
		view = temp;
		trulen = n1;
		view.len = 0;
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
		Utils::memcpy(temp, view.read(), view.len);
		::operator delete(view.edit());
		view = temp;
		trulen = ntru;
		view.edit()[view.len] = '\0';
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
		view.len = nlen;
		Utils::memcpy(view.edit(), val, siz);
		view.edit()[view.len] = '\0';
	}
	void String::byval(String&& val) noexcept {
		finalize();
		view = val.view;
		val.trulen = 0;
	}
	template <bool reset>
	void String::byval(const String& val) noexcept(!reset) {
		if (val.trulen == 0) {
			return finalize();
		}
		if constexpr (reset)
			resize(val.view.len + 1);
		Utils::memcpy(view.edit(), val.view.read(), val.view.len);
		view.len = val.view.len;
		view.edit()[view.len] = '\0';
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
		Utils::memcpy(view.edit(), val, trulen);
		view.len = siz;
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
		Utils::memcpy(&view.edit()[view.len], val, siz);
		view.len += nlen;
		view.edit()[view.len] = '\0';
	}
	void String::addval(const char* val) noexcept {
		Utils::size siz = GString::strlen(val);
		Utils::memcpy(&view.edit()[view.len], val, siz);
		view.len += siz;
	}
	void String::addval(const String& val) noexcept {
		if (val.trulen > 0) {
			Utils::memcpy(&view.edit()[view.len], val.view.read(), val.view.len);
			view.len += val.view.len;
		}
	}
	void String::addval(Utils::size str) noexcept {
		view.edit()[view.len] = '0';
		Utils::size siz = GString::strlen(str);
		for (Utils::size i = 0; i < siz; i++) {
			view.edit()[view.len + siz - i] = (str % 10);
			str /= 10;
		}
		view.len += siz;
	}
	void String::addval(signed str) noexcept {
		if (str < 0) {
			view.edit()[view.len++] = '-';
			str = -str;
		}
		view.edit()[view.len] = '0';
		Utils::size siz = GString::strlen(str);
		for (Utils::size i = 1; i <= siz; i++) {
			view.edit()[view.len + siz - i] = (str % 10) + '0';
			str /= 10;
		}
		view.len += siz;
	}
	Utils::strongcmp_t String::operator <=>(const String& val) const noexcept {
		if ((trulen == 0) || (val.trulen == 0))
			return (trulen <=> val.trulen);
		return (view <=> val.view);
	}
	Utils::strongcmp_t String::valcmp(const StringView& val) const noexcept {
		return StringView(*this) <=> val;
	}
	template <>
	void viewput(std::ostream& os, const StringView* val) {
		if (val && *val)
			return static_cast<void>(os.write(val->read(), val->len));
	}
	BOOL_EXTERN(void String::byval, const String&);
	BOOL_EXTERN(void String::byval, const char*);
	BOOL_EXTERN(void String::byval, Utils::size);
	BOOL_EXTERN(void String::byval, signed);
	BOOL_EXTERN(void String::byray, const char*, Utils::size);
}
