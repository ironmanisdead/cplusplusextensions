#include "headers/String.hpp"
#include "headers/GString.hpp"
#include "headers/StringView.hpp"
#include <new>
#include <iostream>
#define BOOL_EXTERN(fun, ...) template String::setby<true> fun<true>(__VA_ARGS__); \
	template String::setby<false> fun<false>(__VA_ARGS__)
namespace CPPExtensions {
	String::String() noexcept : trulen(0), view(nullptr) {}
	void String::finalize() noexcept {
		if (trulen > 0)
			::operator delete(view.edit());
		errbit = false;
		trulen = 0;
		view.len = 0;
	}
	String::~String() {
		finalize();
	}
	bool String::_allocate(Utils::size n1) noexcept {
		finalize();
		if (n1 == 0)
			return true;
		char* temp = Utils::downcast<char*>(::operator new(n1, std::nothrow_t{}));
		if (!temp) {
			return !(errbit = true);
		}
		view.set(temp, view.len);
		trulen = n1;
		return true;
	}
	bool String::resize(Utils::size n1) noexcept {
		if (trulen == 0)
			return _allocate(n1);
		else if (n1 == 0)
			return finalize(), true;
		else if (n1 < trulen)
			return true;
		errbit = false;
		Utils::size ntru = trulen;
		while (ntru < n1)
			ntru *= 2;
		char* temp = Utils::downcast<char*>(::operator new (ntru, std::nothrow_t{}));
		if (!temp)
			return !(errbit = true);
		Utils::memcpy(temp, view.read(), view.len);
		::operator delete(view.edit());
		view.set(temp, view.len);
		trulen = ntru;
		view.edit()[view.len] = '\0';
		return true;
	}
	template <bool reset>
	String::setby<reset> String::byray(const char* val, Utils::size siz) noexcept {
		errbit = false;
		Utils::size ntru, nlen;
		if (val[siz - 1] == '\0') {
			nlen = siz - 1;
			ntru = siz;
		} else {
			nlen = siz;
			ntru = siz + 1;
		}
		if constexpr (reset)
			if (!resize(ntru))
				return false;
		view.len = nlen;
		Utils::memcpy(view.edit(), val, siz);
		view.edit()[view.len] = '\0';
		if constexpr (reset)
			return true;
	}
	void String::byval(String&& val) noexcept {
		finalize();
		view = val.view;
		val.trulen = 0;
	}
	template <bool reset>
	String::setby<reset> String::byval(const String& val) noexcept {
		if (val.trulen == 0) {
			finalize();
			if constexpr (reset)
				return true;
			else
				return;
		}
		if constexpr (reset)
			if (!resize(val.view.len + 1))
				return false;
		Utils::memcpy(view.edit(), val.view.read(), val.view.len);
		view.len = val.view.len;
		view.edit()[view.len] = '\0';
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(const char* val) noexcept {
		if (val == nullptr) {
			finalize();
			if constexpr (reset)
				return true;
			else
				return;
		}
		Utils::size siz = GString::strlen(val);
		if constexpr (reset)
			if (!resize(siz + 1))
				return false;
		Utils::memcpy(view.edit(), val, trulen);
		view.len = siz;
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(Utils::size str) noexcept {
		if constexpr (reset)
			if (!resize(GString::strlen(str) + 1))
				return false;
		addval(str);
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(signed str) noexcept {
		if constexpr (reset)
			if (!resize(GString::strlen(str) + 1))
				return false;
		addval(str);
		if constexpr (reset)
			return true;
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
	void String::addval(char ch) noexcept { view.edit()[view.len++] = ch; }
	bool String::allocate(Utils::size nsize) noexcept {
		errbit = false;
		if (trulen == 0)
			return _allocate(nsize);
		else if (nsize == 0)
			return finalize(), true;
		else if (nsize < trulen)
			return true;
		char* temp = Utils::downcast<char*>(::operator new (nsize, std::nothrow_t{}));
		if (!temp)
			return !(errbit = true);
		Utils::memcpy(temp, view.read(), view.len);
		::operator delete(view.edit());
		view.set(temp, view.len);
		trulen = nsize;
		view.edit()[view.len] = '\0';
		return true;
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
	void _viewput(std::ostream& os, const StringView* val) {
		if (val && *val)
			return static_cast<void>(os.write(val->read(), val->len));
	}
	BOOL_EXTERN(String::byval, const String&);
	BOOL_EXTERN(String::byval, const char*);
	BOOL_EXTERN(String::byval, Utils::size);
	BOOL_EXTERN(String::byval, signed);
	BOOL_EXTERN(String::byray, const char*, Utils::size);
}
