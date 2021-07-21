#include "headers/String.hpp"
#include "headers/GString.hpp"
#include "headers/StringView.hpp"
#include "headers/system_internals.hpp"
#include <iostream>
#define BOOL_EXTERN(fun, ...) template DLL_PUBLIC String::setby<true> fun<true>(__VA_ARGS__); \
	template DLL_PUBLIC String::setby<false> fun<false>(__VA_ARGS__)
DLL_HIDE
namespace CPPExtensions {
	DLL_PUBLIC String::String() noexcept : trulen(0), view(nullptr) {}
	DLL_LOCAL void String::finalize() noexcept {
		if (trulen > 0)
			Utils::free(view.edit());
		errbit = false;
		trulen = 0;
		view.len = 0;
	}
	DLL_PUBLIC String::~String() {
		finalize();
	}
	DLL_PUBLIC bool String::_allocate(Utils::size_t n1) noexcept {
		finalize();
		if (n1 == 0)
			return true;
		char* temp = Utils::downcast<char*>(Utils::malloc(n1));
		if (!temp) {
			return !(errbit = true);
		}
		view.set(temp, view.len);
		trulen = n1;
		return true;
	}
	DLL_PUBLIC bool String::resize(Utils::size_t n1) noexcept {
		if (trulen == 0)
			return _allocate(n1);
		else if (n1 == 0)
			return finalize(), true;
		else if (n1 < trulen)
			return true;
		errbit = false;
		Utils::size_t ntru = trulen;
		while (ntru < n1)
			ntru *= 2;
		char* temp = Utils::downcast<char*>(Utils::malloc(ntru));
		if (!temp)
			return !(errbit = true);
		Utils::memcpy(temp, view.read(), view.len);
		Utils::free(view.edit());
		view.set(temp, view.len);
		trulen = ntru;
		view.edit()[view.len] = '\0';
		return true;
	}
	DLL_PUBLIC StringView String::substr(Utils::size_t n1, Utils::size_t len) const noexcept {
		if ((trulen > 0) && (n1 < view.len)) {
			const Utils::size_t cap = view.len - n1;
			const Utils::size_t zlen = (len > cap) ? cap : len;
			return StringView(&view.read()[n1], zlen);
		} else {
			return StringView(nullptr);
		}
	}
	DLL_PUBLIC StringView String::substr(Utils::size_t n1) const noexcept {
		if ((trulen > 0) && (n1 < view.len)) {
			return StringView(&view.read()[n1], view.len - n1);
		} else {
			return StringView(nullptr);
		}
	}
	template <bool reset>
	String::setby<reset> String::byray(const char* val, Utils::size_t siz) noexcept {
		errbit = false;
		Utils::size_t ntru, nlen;
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
	DLL_PUBLIC void String::byval(String&& val) noexcept {
		finalize();
		trulen = val.trulen;
		view = val.view;
		val.trulen = 0;
	}
	template <bool reset>
	String::setby<reset> String::byval(const StringView& val) noexcept {
		if (val.len == 0) {
			finalize();
			if constexpr (reset)
				return true;
			else
				return;
		}
		if constexpr (reset)
			if (!resize(val.len + 1))
				return false;
		Utils::memcpy(view.edit(), val.read(), val.len);
		view.len = val.len;
		view.edit()[view.len] = '\0';
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(const String& val) noexcept {
		if ((val.trulen == 0) || (val.view.len == 0)) {
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
		Utils::size_t siz = GString::strlen(val);
		if constexpr (reset)
			if (!resize(siz + 1))
				return false;
		Utils::memcpy(view.edit(), val, trulen);
		view.len = siz;
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(char ch) noexcept {
		if constexpr (reset)
			if (!resize(2))
				return false;
		addval(ch);
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(Utils::u64 str) noexcept {
		if constexpr (reset)
			if (!resize(GString::strlen(str) + 1))
				return false;
		addval(str);
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(Utils::s64 str) noexcept {
		if constexpr (reset)
			if (!resize(GString::strlen(str) + 1))
				return false;
		addval(str);
		if constexpr (reset)
			return true;
	}
	DLL_PUBLIC void String::addray(const char* val, Utils::size_t siz) noexcept {
		Utils::size_t nlen;
		if (val[siz - 1] == '\0')
			nlen = siz - 1;
		else
			nlen = siz;
		Utils::memcpy(&view.edit()[view.len], val, siz);
		view.len += nlen;
		view.edit()[view.len] = '\0';
	}
	DLL_PUBLIC void String::addval(const char* val) noexcept {
		Utils::size_t siz = GString::strlen(val);
		Utils::memcpy(&view.edit()[view.len], val, siz);
		view.len += siz;
	}
	DLL_PUBLIC void String::addval(const String& val) noexcept {
		if (val.trulen > 0) {
			Utils::memcpy(&view.edit()[view.len], val.view.read(), val.view.len);
			view.len += val.view.len;
		}
	}
	DLL_PUBLIC void String::addval(Utils::u64 str) noexcept {
		view.edit()[view.len] = '0';
		Utils::size_t siz = GString::strlen(str);
		for (Utils::size_t i = 0; i < siz; i++) {
			view.edit()[view.len + siz - i - 1] = (str % 10) + '0';
			str /= 10;
		}
		view.len += siz;
		view.edit()[view.len] = '\0';
	}
	DLL_PUBLIC void String::addval(Utils::s64 str) noexcept {
		if (str < 0) {
			view.edit()[view.len++] = '-';
			str = -str;
		}
		addval(Utils::u64(str));
	}
	DLL_PUBLIC void String::addval(char ch) noexcept { 
		view.edit()[view.len++] = ch;
		view.edit()[view.len] = '\0';
	}
	DLL_PUBLIC bool String::allocate(Utils::size_t nsize) noexcept {
		errbit = false;
		if (nsize > 0)
			nsize++;
		if (trulen == 0)
			return _allocate(nsize);
		else if (nsize == 0)
			return finalize(), true;
		else if (nsize < trulen)
			return true;
		char* temp = Utils::downcast<char*>(Utils::malloc(nsize));
		if (!temp)
			return !(errbit = true);
		Utils::memcpy(temp, view.read(), view.len);
		Utils::free(view.edit());
		view.set(temp, view.len);
		trulen = nsize;
		view.edit()[view.len] = '\0';
		return true;
	}
	DLL_PUBLIC Utils::strongcmp_t String::operator <=>(const String& val) const noexcept {
		if ((trulen == 0) || (val.trulen == 0))
			return (trulen <=> val.trulen);
		return (view <=> val.view);
	}
	DLL_PUBLIC Utils::strongcmp_t String::valcmp(const StringView& val) const noexcept {
		return StringView(*this) <=> val;
	}
	template <>
	DLL_PUBLIC void _viewput(std::ostream& os, const StringView* val) {
		if (val && val->read() && (val->len > 0))
			return static_cast<void>(os.write(val->read(), val->len));
	}
	BOOL_EXTERN(String::byval, const StringView&);
	BOOL_EXTERN(String::byval, const String&);
	BOOL_EXTERN(String::byval, const char*);
	BOOL_EXTERN(String::byval, Utils::u64);
	BOOL_EXTERN(String::byval, Utils::s64);
	BOOL_EXTERN(String::byval, char);
	BOOL_EXTERN(String::byray, const char*, Utils::size_t);
}
DLL_RESTORE
