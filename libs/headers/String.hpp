#pragma once
#include "Utils.hpp"
namespace CustomUtils {
	class StringView;
	class String;
	template <class>
		class Vector;
	template <class T>
		void stringput(T&, const String*);
	class String {
		private:
			Utils::size trulen;
			Utils::size len;
			char* buffer;
			void allocate(Utils::size);
			void resize(Utils::size);
			void finalize() noexcept;
			template <bool reset = true>
				void byray(const char*, Utils::size) noexcept(!reset);
			template <bool reset = true>
				void byval(const String&) noexcept(!reset);
			template <bool reset = true>
				void byval(const char*) noexcept(!reset);
			template <bool reset = true>
				void byval(Utils::size) noexcept(!reset);
			template <bool reset = true>
				void byval(signed) noexcept(!reset);
			template <bool reset = true>
				void byval(const StringView&) noexcept(!reset);
			Utils::strongcmp_t valcmp(const StringView&) const noexcept;
			void byval(String&&) noexcept;
			void addray(const char*, Utils::size) noexcept;
			void addval(const String&) noexcept;
			void addval(const char*) noexcept;
			void addval(Utils::size) noexcept;
			void addval(signed) noexcept;
			template <bool reset = true, class T, class... Ts>
			void adder(const T&, const Ts&...) noexcept(!reset);
			template <class T>
			void setup(T&& val) noexcept(Utils::is_same<T, String>) {
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
			String() noexcept;
			inline String(const String& val) : trulen(0), len(0) {
				byval(val);
			}
			inline String(String&& val) noexcept : trulen(0), len(0) {
				byval(Utils::forward<String>(val));
			}
			template <class T, class... V, bool isn = (sizeof...(V) == 0)>
				String(T&& val, const V&... rest) noexcept(isn && Utils::is_same<T, String>);
			constexpr Utils::size getlen() const noexcept {
				if (trulen > 0)
					return len;
				else
					return 0;
			}
			constexpr const char* data() const noexcept {
				if (trulen > 0)
					return buffer;
				else
					return nullptr;
			}
			template <class T>
			String operator +(const T& val) const {
				return String(*this, val);
			}
			template <class T>
			String& operator +=(const T& val) {
				adder(val);
				return *this;
			}
			template <class T>
			String& operator =(T&& val) noexcept(Utils::is_same<T, String>) {
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
			String& operator =(const String& val) {
				byval(val);
				return *this;
			}
			String& operator =(String&& val) noexcept {
				byval(Utils::forward<String>(val));
				return *this;
			}
			Utils::strongcmp_t operator <=>(const String&) const noexcept;
			template <class T>
			Utils::strongcmp_t operator <=>(const T& val) const noexcept {
				using ray_m = Utils::array_prop<T>;
				using ray_r = Utils::array_util<T>;
				if constexpr (ray_m::value)
					return valcmp(StringView(val, ray_m::len));
				else if constexpr (ray_r::value)
					return valcmp(StringView(val, ray_m::len));
				else
					return valcmp(val);
			}
			template <class T, class... V>
			String& append(const T& val, const V&... rest) {
				adder(val, rest...);
				return *this;
			}
			constexpr operator const char*() const noexcept {
				if (trulen > 0)
					return buffer;
				else
					return nullptr;
			}
			~String();
			template <class T>
			friend T& operator <<(T& os, const String& val) {
				stringput(os, &val);
				return os;
			}
			template <class T>
			friend T& operator <<(T& os, const String* val) {
				stringput(os, val);
				return os;
			}
			template <class T>
			friend void stringput(T&, const String*);
			friend class Vector<char>;
	};
}
#include "GString.hpp"
namespace CustomUtils {
	template <bool reset, class T, class... V>
	void String::adder(const T& val, const V&... rest) noexcept(!reset) {
		if constexpr (reset)
			resize(len + GString::strlen(val) + (GString::strlen(rest) + ... + 1));
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
	}
	template <class T, class... V, bool isn>
	String::String(T&& val, const V&... rest) noexcept(isn && Utils::is_same<T, String>) :
	 trulen(0), len(0) {
		if constexpr (isn) {
			setup(Utils::forward<T>(val));
		} else {
			Utils::size total = GString::strlen(val) + (GString::strlen(rest) + ... + 1);
			allocate(total);
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
}
