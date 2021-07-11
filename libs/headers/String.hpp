#pragma once
#include "Utils.hpp"
#include ".hide/StringView.hpp"
namespace CPPExtensions {
	class StringView;
	class String;
	template <class>
		class Vector;
	template <class T>
		void viewput(T&, const StringView*);
	class String {
		private:
			Utils::size trulen;
			StringView view;
			bool allocate(Utils::size) noexcept;
			bool resize(Utils::size) noexcept;
			void finalize() noexcept;
			template <bool reset>
				using setby = Utils::switch_if<reset, bool, void>;
			template <bool reset = true>
				setby<reset> byray(const char*, Utils::size) noexcept;
			template <bool reset = true>
				setby<reset> byval(const String&) noexcept;
			template <bool reset = true>
				setby<reset> byval(const char*) noexcept;
			template <bool reset = true>
				setby<reset> byval(Utils::size) noexcept;
			template <bool reset = true>
				setby<reset> byval(signed) noexcept;
			template <bool reset = true>
				setby<reset> byval(const StringView&) noexcept;
			Utils::strongcmp_t valcmp(const StringView&) const noexcept;
			void byval(String&&) noexcept;
			void addray(const char*, Utils::size) noexcept;
			void addval(const String&) noexcept;
			void addval(const char*) noexcept;
			void addval(Utils::size) noexcept;
			void addval(signed) noexcept;
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
			String() noexcept;
			inline String(const String& val) : trulen(0), view(nullptr) {
				byval(val);
			}
			inline String(String&& val) noexcept : trulen(0), view(nullptr) {
				byval(Utils::forward<String>(val));
			}
			template <class T, class... V, bool isn = (sizeof...(V) == 0)>
				String(T&& val, const V&... rest) noexcept;
			constexpr Utils::size getlen() const noexcept {
				if (trulen > 0)
					return view.len;
				else
					return 0;
			}
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
			Utils::strongcmp_t operator <=>(const String&) const noexcept;
			template <class T>
			Utils::strongcmp_t operator <=>(const T& val) const noexcept {
				using ray_m = Utils::array_prop<T>;
				using ray_r = Utils::array_util<T>;
				Utils::size siz = ray_m::value ? ray_m::len : (ray_r::value ? ray_r::len : 0);
				if constexpr (ray_m::value) {
					return valcmp(StringView(val, val[siz-1] ? siz : siz - 1));
				} else if constexpr (ray_r::value) {
					return valcmp(StringView(val, val[siz-1] ? siz : siz - 1));
				} else
					return valcmp(val);
			}
			template <class T, class... V>
			String& append(const T& val, const V&... rest) noexcept {
				adder(val, rest...);
				return *this;
			}
			constexpr operator const char*() const noexcept {
				if (trulen > 0)
					return view.read();
				else
					return nullptr;
			}
			~String();
			template <class T>
			friend T& operator <<(T& os, const String& val);
			template <class T>
				friend T& operator <<(T& os, const String* val);
			friend class Vector<char>;
	};
}
#include ".hide/GString.hpp"
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
	 trulen(0), view(nullptr) {
		if constexpr (sizeof...(V) == 0) {
			setup(Utils::forward<T>(val));
		} else {
			Utils::size total = GString::strlen(val) + (GString::strlen(rest) + ... + 1);
			if (!allocate(total))
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
		viewput(os, &val.view);
		return os;
	}
	template <class T>
	T& operator <<(T& os, const String* val) {
		viewput(os, val ? &val->view : nullptr);
		return os;
	}
}
