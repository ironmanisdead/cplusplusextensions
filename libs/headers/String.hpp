#pragma once
#include "types.hpp"
#include ".part/StringView.hpp"
DLL_HIDE
namespace CPPExtensions {
	class String;
	template <class>
		class Vector;
	template <class T>
		void _viewput(T&, const StringView*);
	class String {
		private:
			Utils::size_t trulen;
			StringView view;
			bool errbit;
			DLL_PUBLIC bool _allocate(Utils::size_t) noexcept;
			DLL_PUBLIC bool resize(Utils::size_t) noexcept;
			DLL_LOCAL void finalize() noexcept;
			template <bool reset>
				using setby = Utils::switch_it<reset, bool, void>;
			template <bool reset = true>
				DLL_PUBLIC setby<reset> byray(const char*, Utils::size_t) noexcept;
			template <bool reset = true>
				DLL_PUBLIC setby<reset> byval(const String&) noexcept;
			template <bool reset = true>
				DLL_PUBLIC setby<reset> byval(const char*) noexcept;
			template <bool reset = true>
				DLL_PUBLIC setby<reset> byval(Utils::u64) noexcept;
			template <bool reset = true>
				setby<reset> byval(Utils::ulong val) noexcept {
					return byval<reset>(Utils::u64(val));
				}
			template <bool reset = true>
				setby<reset> byval(Utils::uint val) noexcept {
					return byval<reset>(Utils::u64(val));
				}
			template <bool reset = true>
				setby<reset> byval(Utils::ushort val) noexcept {
					return byval<reset>(Utils::u64(val));
				}
			template <bool reset = true>
				DLL_PUBLIC setby<reset> byval(Utils::s64) noexcept;
			template <bool reset = true>
				setby<reset> byval(long val) noexcept {
					return byval<reset>(Utils::s64(val));
				}
			template <bool reset = true>
				setby<reset> byval(int val) noexcept {
					return byval<reset>(Utils::s64(val));
				}
			template <bool reset = true>
				setby<reset> byval(short val) noexcept {
					return byval<reset>(Utils::s64(val));
				}
			template <bool reset = true>
				DLL_PUBLIC setby<reset> byval(const StringView&) noexcept;
			template <bool reset = true>
				DLL_PUBLIC setby<reset> byval(char) noexcept;
			DLL_PUBLIC Utils::strongcmp_t valcmp(const StringView&) const noexcept;
			DLL_PUBLIC void byval(String&&) noexcept;
			DLL_PUBLIC void addray(const char*, Utils::size_t) noexcept;
			DLL_PUBLIC void addval(const String&) noexcept;
			DLL_PUBLIC void addval(const char*) noexcept;
			DLL_PUBLIC void addval(Utils::u64) noexcept;
			inline void addval(Utils::ulong val) noexcept {
				return addval(Utils::u64(val));
			}
			inline void addval(Utils::uint val) noexcept {
				return addval(Utils::u64(val));
			}
			inline void addval(Utils::ushort val) noexcept {
				return addval(Utils::u64(val));
			}
			DLL_PUBLIC void addval(Utils::s64) noexcept;
			inline void addval(long val) noexcept {
				return addval(Utils::s64(val));
			}
			inline void addval(int val) noexcept {
				return addval(Utils::s64(val));
			}
			inline void addval(short val) noexcept {
				return addval(Utils::s64(val));
			}
			DLL_PUBLIC void addval(char) noexcept;
			template <bool reset = true, class T, class... Ts>
				setby<reset> adder(const T&, const Ts&...) noexcept;
			template <class T>
			void setup(T&& val) noexcept {
				using ray_m = Utils::array_prop<T>;
				using ray_r = Utils::array_util<T>;
				if constexpr (ray_m::value)
					byray(val, ray_m::len);
				else if constexpr (ray_r::value)
					byray(val.data, ray_r::len);
				else
					byval(Utils::forward<T>(val));
			}
		public:
			DLL_PUBLIC String() noexcept;
			inline String(const String& val) noexcept : trulen(0), view(nullptr), errbit(false) {
				byval(val);
			}
			inline String(String&& val) noexcept : trulen(0), view(nullptr), errbit(false) {
				byval(Utils::forward<String>(val));
			}
			template <class T, class... V, bool isn = (sizeof...(V) == 0)>
				String(T&& val, const V&... rest) noexcept;
			constexpr Utils::size_t getlen() const noexcept {
				if (trulen > 0)
					return view.len;
				else
					return 0;
			}
			DLL_PUBLIC StringView substr(Utils::size_t, Utils::size_t) const noexcept;
			DLL_PUBLIC StringView substr(Utils::size_t) const noexcept;
			constexpr Utils::size_t gettlen() const noexcept { return trulen; }
			constexpr bool hasErr() const noexcept { return errbit; }
			constexpr const char* data() const noexcept {
				if (trulen > 0)
					return view.read();
				else
					return nullptr;
			}
			template <class T>
			String operator +(const T& val) const noexcept {
				return String(*this, val);
			}
			template <class T>
			String& operator +=(const T& val) noexcept {
				adder(val);
				return *this;
			}
			template <class T>
			String& operator =(T&& val) noexcept {
				using ray_m = Utils::array_prop<T>;
				using ray_r = Utils::array_util<T>;
				if constexpr (ray_m::value)
					byray(val, ray_m::len);
				else if constexpr (ray_r::value)
					byray(val.data, ray_r::len);
				else
					byval(Utils::forward<T>(val));
				return *this;
			}
			String& operator =(const String& val) noexcept {
				byval(val);
				return *this;
			}
			String& operator =(String&& val) noexcept {
				byval(Utils::forward<String>(val));
				return *this;
			}
			DLL_PUBLIC Utils::strongcmp_t operator <=>(const String&) const noexcept;
			template <class T>
			Utils::strongcmp_t operator <=>(const T& val) const noexcept {
				using ray_m = Utils::array_prop<T>;
				using ray_r = Utils::array_util<T>;
				Utils::size_t siz = ray_m::value ? ray_m::len : (ray_r::value ? ray_r::len : 0);
				if constexpr (ray_m::value) {
					return valcmp(StringView(val, val[siz-1] ? siz : siz - 1));
				} else if constexpr (ray_r::value) {
					return valcmp(StringView(val, val[siz-1] ? siz : siz - 1));
				} else
					return valcmp(val);
			}
			template <class T, class... V>
			String& set(const T& val, const V&... rest) noexcept {
				resize(GString::strlen(val) + (GString::strlen(rest) + ... + 1));
				*this = val;
				if constexpr (sizeof...(rest) > 0)
					adder<false>(rest...);
				return *this;
			}
			constexpr operator const char*() const& noexcept {
				if (trulen > 0)
					return view.read();
				else
					return nullptr;
			}
			operator const char*() && = delete;
			DLL_PUBLIC bool allocate(Utils::size_t) noexcept;
			DLL_PUBLIC ~String();
			template <class T>
			friend T& operator <<(T& os, const String& val);
			template <class T>
				friend T& operator <<(T& os, const String* val);
			friend class Vector<char>;
	};
}
#include ".part/GString.hpp"
namespace CPPExtensions {
	template <bool reset, class T, class... V>
	String::setby<reset> String::adder(const T& val, const V&... rest) noexcept {
		if constexpr (reset) {
			const bool has =
				resize(view.len + GString::strlen(val) +
						(GString::strlen(rest) + ... + 1));
			if (!has)
				return false;
		}
		using ray_m = Utils::array_prop<T>;
		using ray_r = Utils::array_util<T>;
		if constexpr (ray_m::value)
			addray(val, ray_m::len);
		else if constexpr (ray_r::value)
			addray(val.data, ray_r::len);
		else
			addval(val);
		if constexpr (sizeof...(V) > 0)
			adder<false>(rest...);
		if constexpr (reset)
			return true;
	}
	template <class T, class... V, bool isn>
	String::String(T&& val, const V&... rest) noexcept :
	 trulen(0), view(nullptr), errbit(false) {
		if constexpr (sizeof...(V) == 0) {
			setup(Utils::forward<T>(val));
		} else {
			Utils::size_t total = GString::strlen(val) + (GString::strlen(rest) + ... + 1);
			if (!_allocate(total))
				return;
			using ray_m = Utils::array_prop<T>;
			using ray_r = Utils::array_util<T>;
			if constexpr (ray_m::value)
				byray<false>(val, ray_m::len);
			else if constexpr (ray_r::value)
				byray<false>(val.data, ray_r::len);
			else
				byval<false>(val);
			adder<false>(rest...);
		}
	}
	template <class T>
	T& operator <<(T& os, const String& val) {
		_viewput(os, &val.view);
		return os;
	}
	template <class T>
	T& operator <<(T& os, const String* val) {
		_viewput(os, val ? &val->view : nullptr);
		return os;
	}
}
DLL_RESTORE
