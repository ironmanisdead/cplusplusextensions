#pragma once
#include "../Utils.hpp"
#include "../Array.hpp"
#include "../Tuple.hpp"
namespace CPPExtensions {
	class String;
	template <class>
		class Vector;
	namespace GString {
			using size = Utils::size;
			template <auto... vals>
				using list = Utils::list<vals...>;
			template <class T, size N>
				using array = Utils::array<T, N>;
			template <size N>
				using queue = Utils::queue<N>;
			constexpr size _strlen(const char* str) noexcept {
				for (size i = 0; true; i++)
					if (str[i] == '\0')
						return i;
			}
			constexpr size _strlen(size str) noexcept {
				size len = 1;
				while ((str /= 10) > 0)
					len++;
				return len;
			}
			constexpr size _strlen(signed str) noexcept {
				size len = 1;
				if (str < 0) {
					str = -str;
					len = 2;
				}
				while ((str /= 10) > 0)
					len++;
				return len;
			}
			size _strlen(const String&) noexcept;
			size _strlen(const Vector<char>&) noexcept;
			const String& demangle(const char*);
			template <class T>
			const String& runtype(const T&);
			template <class T>
			constexpr size strlen(T&& str) noexcept {
				using ray_t = Utils::array_prop<T>;
				using ray_r = Utils::array_util<T>;
				if constexpr (ray_t::value)
					return ray_t::len;
				else if constexpr (ray_r::value)
					return ray_r::len;
				else if (Utils::is_same<Utils::raw_type<T>, char>)
					return 1;
				else
					return _strlen(str);
			}
			constexpr auto find(const char* str, const char* cmp) {
				const size len1 = strlen(str);
				const size len2 = strlen(cmp);
				if (len2 > len1)
					return Tuple { (size)0, (size)0, false };
				size mat = 0;
				bool ready = false;
				size pos = 0;
				bool found = false;
				for (size idx = 0; idx < len1; ++idx) {
					if (str[idx] == cmp[mat]) {
						if (!ready) {
							ready = true;
							pos = idx;
						}
						if (++mat >= len2) {
							found = true;
							break;
						}
					} else {
						ready = false;
						mat = 0;
					}
				}
				return Tuple { pos, len2, found };
			}
			constexpr auto rfind(const char* str, const char* cmp) {
				const size len1 = strlen(str);
				const size len2 = strlen(cmp);
				if (len2 > len1)
					return Tuple { (size)0, (size)0, false };
				bool found = false;
				size mat = len2 - 1;
				size idx = 0;
				for (idx = len1 - 1;; --idx) {
					if (str[idx] == cmp[mat]) {
						if (mat < 2) {
							found = true;
							break;
						}
						mat--;
					} else
						mat = len2 - 1;
					if (idx == 0)
						break;
				}
				return Tuple { idx, len2, found };
			}
			[[noreturn]] void _overflow(size, size);
			template <size n, char... str>
				struct _numberify : _numberify<n / 10, (n % 10) + '0', str...> {};
			template <char... str>
				struct _numberify<0, str...> {
					static constexpr size len = sizeof...(str) + 1;
					static constexpr const char value[len] = { str..., '\0' };
				};
			template <size n1, size n2, size... l1, size... l2>
				constexpr Array<char, (l1, ...) + (l2, ...) + 2>
				_concatter(const char (&val1)[n1], const char (&val2)[n2], list<l1...>, list<l2...>) noexcept {
					return Array { { val1[l1]..., val2[l2]... } };
				}
			template <size n1, size n2, char... items, size raylen = n1 + n2 - 1>
				constexpr Array<char, raylen>
				_string_cat(const char (&val1)[n1], const char (&val2)[n2], list<items...>) noexcept {
					char result[raylen] = { items... };
					size idx = 0;
					for (idx = 0; idx < n1; idx++)
						if (val1[idx] == '\0')
							break;
						else
							result[idx] = val1[idx];
					//
					for (size id2 = 0; id2 < n2; id2++)
						if (val2[id2] == '\0')
							break;
						else
							result[idx + id2] = val2[id2];
					//
					return Array { result };
				}
			constexpr size _s_len = sizeof(_numberify<(size)0 - (size)1>::value);
			constexpr auto _getint(const char* src, const char* cmp, const char* suf) noexcept {
				Tuple start = find(src, cmp);
				Tuple end = rfind(src, suf);
				size init = start.get<0>() + start.get<1>();
				size fin = end.get<0>();
				return Tuple { init, fin - init, start.get<2>() };
			}
		#if defined(_CUSTOMUTILS_recognized)
			template <class T>
				constexpr auto _gettype() {
					#if defined(__clang__)
					 constexpr const char prefix[] = "[T = ";
					#elif defined(__GNUC__)
					 constexpr const char prefix[] = "[with T = ";
					#elif defined(_MSC_VER)
					 constexpr const char prefix[] = "type_name_array<";
					 constexpr const char suffix[] = ">(void)";
					 constexpr const char* name = __FUNCSIG__;
					#endif
					#if !defined(_MSC_VER)
					 constexpr const char suffix[] = "]";
					 constexpr const char* name = __PRETTY_FUNCTION__;
					#endif
					constexpr Tuple info = _getint(name, prefix, suffix);
					static_assert(info.get<2>(), "search failed, (possible macro interference)");
					constexpr size len = info.get<1>(), beg = info.get<0>();
					Array<char, (size)(len + 1)> ray;
					for (size idx = 0; idx < len; idx++)
						ray[idx] = name[idx + beg];
					return ray;
				}
			template <class T>
				constexpr auto typestr = _gettype<T>();
		#endif
			constexpr Array<char, _s_len> numstr(size num) noexcept {
				size len = 0;
				for (size idx = num; idx > 0; len++)
					idx /= 10;
				Array<char, _s_len> result;
				size idx = len;
				for (size cop = num; cop > 0; cop /= 10)
					result[--idx] = (cop % 10) + '0';
				return result;
			}
			template <size n>
				constexpr auto& stringify = _numberify<n / 10, (n % 10) + '0'>::value;
			template <size n1, size n2, size... nums>
				constexpr Array<char, Utils::sum(n1, n2, nums...)> 
				concat(const char (&val1)[n1], const char (&val2)[n2], const array<char, nums>&... args) noexcept {
					if constexpr (sizeof...(nums) > 0)
						return concat(val1, concat(val2, args...).data);
					else
						return _concatter(val1, val2, queue<n1> {}, queue<n2> {});
				}
			template <size n1, size n2, size... nums, size raylen = (Utils::sum(n1, n2, nums...) - sizeof...(nums) - 1)>
				constexpr Array<char, raylen>
				raycat(const char (&val1)[n1], const char (&val2)[n2], const array<char, nums>&... args) noexcept {
					if constexpr (sizeof...(nums) > 0)
						return raycat(val1, raycat(val2, args...).data);
					else if constexpr (n1 < 2)
						return Array { val2 };
					else if constexpr (n2 < 2)
						return Array { val1 };
					else
						return _concatter(val1, val2, queue<n1-1> {}, queue<n2> {});
				}
			template <size n1, size n2, size... nums, size raylen = (Utils::sum(n1, n2, nums...) - sizeof...(nums) - 1)>
				constexpr Array<char, raylen>
				strcat(const char (&val1)[n1], const char (&val2)[n2], const array<char, nums>&... args) noexcept {
					if constexpr (sizeof...(nums) > 0)
						return strcat(val1, strcat(val2, args...).data);
					else if constexpr (n1 < 2)
						return Array { val2 };
					else if constexpr (n2 < 2)
						return Array { val1 };
					else
						return _string_cat(val1, val2, Utils::fill_set<'\0', raylen> {});
				}
			constexpr char esc = '\x1b';
#pragma push_macro("ESCAPE")
#define ESCAPE(name, code) constexpr const char name[] = "\x1b[" #code "m"
			namespace codes {
				ESCAPE(text_reset, 0);
				//main font settings
				ESCAPE(bold, 1);
				ESCAPE(faint, 2);
				ESCAPE(intensity_reset, 22);
				ESCAPE(italics, 3);
				ESCAPE(italics_reset, 23);
				ESCAPE(underline, 4);
				ESCAPE(underline_reset, 24);
				ESCAPE(blink_slow, 5);
				ESCAPE(blink_fast, 6);
				ESCAPE(blink_reset, 25);
				ESCAPE(strike, 9);
				ESCAPE(strike_reset, 29);
				ESCAPE(font_reset, 10);
				//foreground colors
				ESCAPE(fg_black, 30);
				ESCAPE(fg_red, 31);
				ESCAPE(fg_green, 32);
				ESCAPE(fg_yellow, 33);
				ESCAPE(fg_blue, 34);
				ESCAPE(fd_magenta, 35);
				ESCAPE(fg_cyan, 36);
				ESCAPE(fg_white, 37);
				ESCAPE(fg_reset, 39);
				//background colors
				ESCAPE(bg_black, 40);
				ESCAPE(bg_red, 41);
				ESCAPE(bg_green, 42);
				ESCAPE(bg_yellow, 43);
				ESCAPE(bg_blue, 44);
				ESCAPE(bd_magenta, 45);
				ESCAPE(bg_cyan, 46);
				ESCAPE(bg_white, 47);
				ESCAPE(bg_reset, 49);
			};
#pragma pop_macro("ESCAPE")
			String hyperlink(const char* site, const char* display) noexcept;
			using uchar = unsigned char;
			constexpr auto gen_color(bool isfg, uchar col) noexcept {
				Array<char, 5> code;
				uchar colcop = col;
				short len = _strlen(Utils::size(col));
				for (short i = 1; i <= len; i++) {
					code[len - i] = (colcop % 10) + '0';
					colcop /= 10;
				}
				code[len] = 'm';
				auto result = strcat("\x1b[48;5;", code.data);
				if (isfg)
					result[2] = '3';
				return result;
			}
			constexpr auto truecolor(bool isfg, uchar r, uchar g, uchar b) noexcept {
				auto fillcode = 
					[] (auto& ray, uchar col) {
						uchar colcop = col;
						short len = _strlen(Utils::size(col));
						for (short i = 1; i <= len; i++) {
							ray[len - i] = (colcop % 10) + '0';
							colcop /= 10;
						}
						ray[len] = ';';
						return len;
					};
				Array<char, 5> rcode, gcode, bcode;
				fillcode(rcode, r);
				fillcode(gcode, g);
				short len = fillcode(bcode, b);
				bcode[len] = 'm';
				auto result = strcat("\x1b[48;2;",
						rcode.data, gcode.data, bcode.data);
				if (isfg)
					result[2] = '3';
				return result;
			}
	};
}
