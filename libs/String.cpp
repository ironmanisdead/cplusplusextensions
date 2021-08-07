#include "headers/String.hpp"
#include "headers/GString.hpp"
#include "headers/StringView.hpp"
#include "headers/system.hpp"
#define BOOL_EXTERN(fun, ...) template DLL_PUBLIC String::setby<true> fun<true>(__VA_ARGS__); \
	template DLL_PUBLIC String::setby<false> fun<false>(__VA_ARGS__)
DLL_HIDE
namespace CPPExtensions {
	DLL_PUBLIC String::String() noexcept : trulen(0), view(nullptr) {}
	DLL_LOCAL void String::finalize() noexcept {
		if (trulen > 0)
			Utils::free(view.edit());
		trulen = 0;
		view.len = 0;
	}
	DLL_PUBLIC String::~String() {
		finalize();
	}
	DLL_PUBLIC bool String::_allocate(Utils::size_t n1) noexcept {
		Utils::_libErr = Utils::E_NOERR;
		finalize();
		if (n1 == 0)
			return true;
		char* temp = Utils::downcast<char*>(Utils::malloc(n1 + 1));
		if (!temp) {
			return false;
		}
		view.set(temp, view.len);
		trulen = n1;
		return true;
	}
	DLL_PUBLIC bool String::resize(Utils::size_t n1) noexcept {
		Utils::_libErr = Utils::E_NOERR;
		if (trulen == 0) {
			return _allocate(n1);
		} else if (n1 == 0) {
			return finalize(), true;
		} else if (n1 < trulen) {
			return true;
		}
		Utils::size_t ntru = trulen;
		while (ntru < n1)
			ntru *= 2;
		char* temp = Utils::downcast<char*>(Utils::malloc(ntru));
		if (!temp)
			return false;
		Utils::memcpy(temp, view.read(), view.len);
		Utils::free(view.edit());
		view.set(temp, view.len);
		trulen = ntru;
		view.edit()[view.len] = '\0';
		return true;
	}
	DLL_PUBLIC StringView String::substr(Utils::size_t n1, Utils::size_t len) const noexcept {
		Utils::_libErr = Utils::E_NOERR;
		if (trulen > 0) {
			if (n1 > view.len) {
				Utils::_libErr = Utils::E_RANGE;
				return StringView(nullptr);
			}
			const Utils::size_t cap = view.len - n1;
			const Utils::size_t zlen = (len > cap) ? cap : len;
			return StringView(&view.read()[n1], zlen);
		} else {
			Utils::_libErr = Utils::E_NULL;
			return StringView(nullptr);
		}
	}
	DLL_PUBLIC StringView String::substr(Utils::ssize_t n) const noexcept {
		Utils::_libErr = Utils::E_NOERR;
		if (trulen) {
			if (n >= 0) {
				const Utils::size_t n1 = n; //avoiding compiler warnings by casting to like-signed integer
				return StringView(view.read(), (n1 > view.len) ? view.len : n1);
			} else {
				const Utils::size_t n1 = -n;
				const Utils::size_t len = view.len;
				const Utils::size_t diff = (n1 > len) ? 0 : (len - n1);
				return StringView(&view.read()[diff], len - diff);
			}
		} else {
			Utils::_libErr = Utils::E_NULL;
			return StringView(nullptr);
		}
	}
	template <bool reset>
	String::setby<reset> String::byray(const char* val, Utils::size_t siz) noexcept {
		Utils::_libErr = Utils::E_NOERR;
		Utils::size_t ntru, nlen;
		if (val[siz - 1] == '\0') {
			nlen = siz - 1;
			ntru = siz;
		} else {
			nlen = siz;
			ntru = siz + 1;
		}
		view.len = 0;
		if constexpr (reset)
			if (!resize(ntru))
				return false;
		Utils::memcpy(view.edit(), val, nlen);
		view.len = nlen;
		view.edit()[view.len] = '\0';
		if constexpr (reset)
			return true;
	}
	DLL_PUBLIC void String::byval(String&& val) noexcept {
		Utils::_libErr = Utils::E_NOERR;
		finalize();
		trulen = val.trulen;
		view = val.view;
		val.trulen = 0;
	}
	template <bool reset>
	String::setby<reset> String::byval(const StringView& val) noexcept {
		Utils::_libErr = Utils::E_NOERR;
		if (val.len == 0) {
			finalize();
			if constexpr (reset)
				return true;
			else
				return;
		}
		view.len = 0;
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
		Utils::_libErr = Utils::E_NOERR;
		if ((val.trulen == 0) || (val.view.len == 0)) {
			finalize();
			if constexpr (reset)
				return true;
			else
				return;
		}
		view.len = 0;
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
		Utils::_libErr = Utils::E_NOERR;
		if (val == nullptr) {
			finalize();
			Utils::_libErr = Utils::E_NULL;
			if constexpr (reset)
				return true;
			else
				return;
		}
		Utils::size_t siz = GString::strlen(val);
		view.len = 0;
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
		Utils::_libErr = Utils::E_NOERR;
		view.len = 0;
		if constexpr (reset) {
			if (!resize(2))
				return false;
		}
		addval(ch);
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(Utils::u64 str) noexcept {
		Utils::_libErr = Utils::E_NOERR;
		view.len = 0;
		if constexpr (reset) {
			if (!resize(GString::strlen(str) + 1))
				return false;
		}
		addval(str);
		if constexpr (reset)
			return true;
	}
	template <bool reset>
	String::setby<reset> String::byval(Utils::s64 str) noexcept {
		Utils::_libErr = Utils::E_NOERR;
		view.len = 0;
		if constexpr (reset) {
			if (!resize(GString::strlen(str) + 1))
				return false;
		}
		addval(str);
		if constexpr (reset)
			return true;
	}
	DLL_PUBLIC void String::addray(const char* val, Utils::size_t siz) noexcept {
		Utils::_libErr = Utils::E_NOERR;
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
	DLL_PUBLIC void String::addval(const StringView& val) noexcept {
		const char* str = val.read();
		if (str) {
			Utils::memcpy(&view.edit()[view.len], str, val.len);
			view.len += val.len;
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
		Utils::_libErr = Utils::E_NOERR;
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
			return false;
		Utils::memcpy(temp, view.read(), view.len);
		Utils::free(view.edit());
		view.set(temp, view.len);
		trulen = nsize;
		view.edit()[view.len] = '\0';
		return true;
	}
	DLL_PUBLIC bool String::encode(const StringView& key) noexcept {
		Utils::_libErr = Utils::E_NOERR;
		const char* str = key.read();
		if (str == nullptr) {
			Utils::_libErr = Utils::E_NULL;
			return false;
		}
		if (trulen > 0) {
			char* edit = view.edit();
			Utils::size_t r = 0; //read index
			Utils::size_t w = 0; //write index
			while (w < view.len) {
				char ch = str[r++];
				edit[w++] ^= ch;
				if (r >= key.len)
					r = 0;
			}
		} else {
			Utils::_libErr = Utils::E_NULL;
			return false;
		}
		return true;
	}
	DLL_PUBLIC Utils::ssize_t StringView::write(Utils::f_desc id) const noexcept {
		return Utils::write(id, read(), len);
	}
	DLL_PUBLIC Utils::size_t String::write(Utils::f_desc id) const noexcept {
		Utils::_libErr = Utils::E_NOERR;
		if (trulen > 0) {
			return Utils::write(id, view.read(), view.len);
		} else {
			Utils::_libErr = Utils::E_NULL;
			return -1;
		}
	}
	DLL_PUBLIC Utils::ssize_t StringView::puts(Utils::f_desc id) const noexcept {
		return Utils::_puts(id, read(), len);
	}
	DLL_PUBLIC Utils::size_t String::puts(Utils::f_desc id) const noexcept {
		Utils::_libErr = Utils::E_NOERR;
		if (trulen > 0) {
			return Utils::_puts(id, view.read(), view.len);
		} else {
			Utils::_libErr = Utils::E_NULL;
			return -1;
		}
	}
	DLL_PUBLIC Utils::ssize_t Utils::writestr(Utils::f_desc id, const String& str) noexcept {
		return str.write(id);
	}
	DLL_PUBLIC Utils::ssize_t Utils::writestr(Utils::f_desc id, const StringView& str) noexcept {
		return str.write(id);
	}
	DLL_PUBLIC Utils::strongcmp_t String::operator <=>(const String& val) const noexcept {
		if ((trulen == 0) || (val.trulen == 0))
			return (trulen <=> val.trulen);
		return (view <=> val.view);
	}
	DLL_PUBLIC Utils::strongcmp_t String::valcmp(const StringView& val) const noexcept {
		return StringView(*this) <=> val;
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
