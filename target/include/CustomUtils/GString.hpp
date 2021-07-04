#pragma once
#include "Utils.hpp"
#include "Array.hpp"
#include "Tuple.hpp"
//#include "Vector.hpp"
#include <typeinfo>
namespace CustomUtils {
	class String;
	template <class>
		class Vector;
	class GString : Utils {
		private:
			static constexpr size _strlen(const char* str) noexcept {
				for (size i = 0; true; i++)
					if (str[i] == '\0')
						return i;
			}
			static constexpr size _strlen(Utils::size str) noexcept {
				Utils::size len = 1;
				while ((str /= 10) > 0)
					len++;
				return len;
			}
			static constexpr size _strlen(signed str) noexcept {
				Utils::size len = 1;
				if (str < 0) {
					str = -str;
					len = 2;
				}
				while ((str /= 10) > 0)
					len++;
				return len;
			}
			static size _strlen(const String&) noexcept;
			static size _strlen(const Vector<char>&) noexcept;
		public:
			static const String& demangle(const char*);
			template <class T>
			static const String& runtype(const T&);
			template <class T>
			static constexpr size strlen(T&& str) noexcept {
				using ray_t = array_prop<T>;
				using ray_r = array_util<T>;
				if constexpr (ray_t::value)
					return ray_t::len;
				else if constexpr (ray_r::value)
					return ray_r::len;
				else
					return _strlen(str);
			}
			static constexpr auto find(const char* str, const char* cmp) {
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
			static constexpr auto rfind(const char* str, const char* cmp) {
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
		private:
			[[noreturn]] static void overflow(size, size);
			template <size n, char... str>
				struct numberify : numberify<n / 10, (n % 10) + '0', str...> {};
			template <char... str>
				struct numberify<0, str...> {
					static constexpr size len = sizeof...(str) + 1;
					static constexpr const char value[len] = { str..., '\0' };
				};
			template <size n1, size n2, size... l1, size... l2>
				static constexpr Array<char, (l1, ...) + (l2, ...) + 2>
				concatter(const char (&val1)[n1], const char (&val2)[n2], list<l1...>, list<l2...>) noexcept {
					return Array { { val1[l1]..., val2[l2]... } };
				}
			template <size n1, size n2, char... items, size raylen = n1 + n2 - 1>
				static constexpr Array<char, raylen>
				string_cat(const char (&val1)[n1], const char (&val2)[n2], list<items...>) noexcept {
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
			static constexpr size s_len = sizeof(numberify<(Utils::size)0 - (Utils::size)1>::value);
			static constexpr auto _getint(const char* src, const char* cmp, const char* suf) noexcept {
				Tuple start = find(src, cmp);
				Tuple end = rfind(src, suf);
				size init = start.get<0>() + start.get<1>();
				size fin = end.get<0>();
				return Tuple { init, fin - init, start.get<2>() };
			}
		#if defined(_CUSTOMUTILS_recognized)
			template <class T>
				static constexpr auto _gettype() {
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
		public:
			template <class T>
				static constexpr auto typestr = _gettype<T>();
		#endif
		public:
			static constexpr Array<char, s_len> numstr(size num) noexcept {
				size len = 0;
				for (size idx = num; idx > 0; len++)
					idx /= 10;
				Array<char, s_len> result;
				size idx = len;
				for (size cop = num; cop > 0; cop /= 10)
					result[--idx] = (cop % 10) + '0';
				return result;
			}
			template <size n>
				static constexpr auto& stringify = numberify<n / 10, (n % 10) + '0'>::value;
			template <size n1, size n2, size... nums>
				static constexpr Array<char, sum(n1, n2, nums...)> 
				concat(const char (&val1)[n1], const char (&val2)[n2], const array<char, nums>&... args) noexcept {
					if constexpr (sizeof...(nums) > 0)
						return concat(val1, concat(val2, args...).data);
					else
						return concatter(val1, val2, queue<n1> {}, queue<n2> {});
				}
			template <size n1, size n2, size... nums, size raylen = (sum(n1, n2, nums...) - sizeof...(nums) - 1)>
				static constexpr Array<char, raylen>
				raycat(const char (&val1)[n1], const char (&val2)[n2], const array<char, nums>&... args) noexcept {
					if constexpr (sizeof...(nums) > 0)
						return raycat(val1, raycat(val2, args...).data);
					else if constexpr (n1 < 2)
						return Array { val2 };
					else if constexpr (n2 < 2)
						return Array { val1 };
					else
						return concatter(val1, val2, queue<n1-1> {}, queue<n2> {});
				}
			template <size n1, size n2, size... nums, size raylen = (sum(n1, n2, nums...) - sizeof...(nums) - 1)>
				static constexpr Array<char, raylen>
				strcat(const char (&val1)[n1], const char (&val2)[n2], const array<char, nums>&... args) noexcept {
					if constexpr (sizeof...(nums) > 0)
						return strcat(val1, strcat(val2, args...).data);
					else if constexpr (n1 < 2)
						return Array { val2 };
					else if constexpr (n2 < 2)
						return Array { val1 };
					else
						return string_cat(val1, val2, fill_set<'\0', raylen> {});
				}
	};
}
#include "String.hpp"
namespace CustomUtils {
	template <class T>
	const String& GString::runtype(const T& val) {
		return demangle(typeid(val).name());
	}
}
