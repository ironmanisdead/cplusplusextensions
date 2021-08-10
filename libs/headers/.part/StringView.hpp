#pragma once
#include "../Utils.hpp"
#include "GString.hpp"
DLL_HIDE
namespace CPPExtensions {
	class String;
	template <class>
		class Vector;
	class StringView;
	template <class T>
		void _viewput(T&, const StringView*);
	class StringView {
		public:
			Utils::size_t len;
		private:
			struct Window {
				bool modify;
				union {
					char* edit;
					const char* view;
				};
			} buffer;
			constexpr static auto viewer =
			[] (const Window& val) noexcept -> const char* {
				if (val.modify)
					return val.edit;
				else
					return val.view;
			};
			constexpr void byray(char* val, Utils::size_t ln) noexcept {
				len = ln;
				buffer.modify = true;
				buffer.edit = val;
			}
			constexpr void byray(const char* val, Utils::size_t ln) noexcept {
				len = ln;
				buffer.modify = false;
				buffer.view = val;
			}
			constexpr void byval(const char* val) noexcept {
				len = GString::strlen(val);
				buffer.modify = false;
				buffer.view = val;
			}
			constexpr void byval(char* val) noexcept {
				len = GString::strlen(val);
				buffer.modify = true;
				buffer.edit = val;
			}
			constexpr void byval(const StringView& val) noexcept {
				len = val.len;
				buffer.modify = false;
				buffer.view = viewer(val.buffer);
			}
			constexpr void byval(StringView& val) noexcept {
				len = val.len;
				buffer = val.buffer;
			}
			constexpr void byval(Utils::nullpt) noexcept {
				len = 0;
				buffer.modify = false;
				buffer.view = nullptr;
			}
			constexpr void byval(const String&) noexcept;
			void byval(String&&) = delete;
		public:
			DLL_PUBLIC Utils::ssize_t write(Utils::f_desc) const noexcept;
			DLL_PUBLIC Utils::ssize_t puts(Utils::f_desc) const noexcept;
			constexpr StringView(const StringView& val) noexcept :
				len(val.len), buffer{ false, { .view = viewer(val.buffer) } } {}
			constexpr StringView(StringView&& val) noexcept :
				len(val.len), buffer(val.buffer) {}
			constexpr StringView(StringView& val) noexcept :
				len(val.len), buffer(val.buffer) {}
			template <class T>
			constexpr StringView(T&& val) noexcept :
			len(0), buffer { false, { .view = nullptr } } {
				using prop = Utils::array_prop<T>;
				using util = Utils::array_util<T>;
				if constexpr (prop::value) {
					byray(val, prop::len);
				} else if constexpr (util::value) {
					byray(val.data, util::len);
				} else {
					byval(val);
				}
			}
			constexpr StringView(char* val, Utils::size_t ln) noexcept :
				len(ln), buffer { true, { .edit = val } } {}
			constexpr StringView(const char* val, Utils::size_t ln) noexcept :
				len(ln), buffer { false, { .view = val } } {}
			constexpr const char* read() const noexcept { return viewer(buffer); }
			constexpr char* edit() noexcept {
				if (buffer.modify)
					return buffer.edit;
				else
					return nullptr;
			}
			template <class T>
			constexpr StringView& operator =(T&& val) noexcept {
				using prop = Utils::array_prop<T>;
				using util = Utils::array_util<T>;
				if constexpr (prop::value) {
					byray(val, prop::len);
				} else if constexpr (util::value) {
					byray(val.data(), util::len);
				} else {
					byval(val);
				}
				return *this;
			}
			constexpr void set(const char* val, Utils::size_t ln) noexcept {
				buffer.modify = false;
				buffer.view = val;
				len = ln;
			}
			constexpr void set(char* val, Utils::size_t ln) noexcept {
				buffer.modify = true;
				buffer.edit = val;
				len = ln;
			}
			constexpr StringView substr(Utils::ssize_t ln) const noexcept {
				const char* str = viewer(buffer);
				if (!str)
					return StringView(nullptr);
				if (ln > 0) {
					Utils::size_t abs = ln;
					if (abs >= len)
						return StringView(nullptr);
					return StringView(&str[abs], len - abs);
				}
				Utils::size_t abs = -ln;
				if (abs >= len)
					return StringView(nullptr);
				return StringView(str, len - abs);
			}
			constexpr StringView substr(Utils::size_t n1, Utils::size_t n2) const noexcept {
				const char* str = viewer(buffer);
				if ((!n2) || (!str) || (n1 >= len))
					return StringView(nullptr);
				if (n1 + n2 > len)
					return StringView(&str[n1], len - n1);
				return StringView(&str[n1], n2);
			}
			constexpr Utils::strongcmp_t operator <=>(const StringView& val) const noexcept {
				const char* see = read();
				const char* valsee = val.read();
				if ((len == 0) || (val.len == 0))
					return (len <=> val.len);
				if (!(see && valsee))
					return (see <=> valsee);
				Utils::size_t shrt = len;
				if (val.len < shrt)
					shrt = val.len;
				for (Utils::size_t i = 0; i < shrt; i++) {
					char bufi = see[i], bufv = valsee[i];
					if (bufi != bufv)
						return (bufi <=> bufv);
				}
				return (len <=> val.len);
			}
			constexpr int convert_int() const noexcept {
				const char* str = viewer(buffer);
				int result = 0;
				for (Utils::size_t i = 0; i < len; i++) {
					char n = str[i];
					if ((n < '0') || (n > '9'))
						break;
					result *= 10;
					result += (n - '0');
				}
				return result;
			}
			constexpr double convert_double() const noexcept {
				const char* str = viewer(buffer);
				double result = 0;
				char n = '\0';
				Utils::size_t i = 0;
				for (i = 0; i < len; i++) {
					n = str[i];
					if ((n < '0') || (n > '9'))
						break;
					result *= 10;
					result += (n - '0');
				}
				if (n != '.')
					return result;
				i++;
				double base = 0.1;
				for (; i < len; i++) {
					n = str[i];
					if ((n < '0') || (n > '9'))
						break;
					result += (n - '0') * base;
					base /= 10;
				}
				return result;
			}
			constexpr Utils::strongcmp_t operator <=>(const String& val) const noexcept;
	};
}
DLL_RESTORE
