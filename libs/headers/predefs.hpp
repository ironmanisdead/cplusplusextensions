#pragma once
namespace CPPExtensions {
	namespace Utils {
		namespace {
			using uint = unsigned;
			using ulong = unsigned long;
			using u64 = unsigned long long;
			using s64 = long long;
			using ushort = unsigned short;
			using schar = signed char;
		}
		template <typename T>
		struct identity {
			using type = T;
		};
		using size_t = decltype(sizeof(0));
		template <class>
			struct _make_unsigned;
		template <>
			struct _make_unsigned<int> : identity<uint> {};
		template <>
			struct _make_unsigned<uint> : identity<uint> {};
		template <>
			struct _make_unsigned<long> : identity<ulong> {};
		template <>
			struct _make_unsigned<ulong> : identity<ulong> {};
		template <>
			struct _make_unsigned<s64> : identity<u64> {};
		template <>
			struct _make_unsigned<u64> : identity<u64> {};
		template <>
			struct _make_unsigned<short> : identity<ushort> {};
		template <>
			struct _make_unsigned<ushort> : identity<ushort> {};
		template <>
			struct _make_unsigned<schar> : identity<char> {};
		template <>
			struct _make_unsigned<char> : identity<char> {};
		template <class>
			struct _make_signed;
		template <>
			struct _make_signed<int> : identity<int> {};
		template <>
			struct _make_signed<uint> : identity<int> {};
		template <>
			struct _make_signed<long> : identity<long> {};
		template <>
			struct _make_signed<ulong> : identity<long> {};
		template <>
			struct _make_signed<s64> : identity<s64> {};
		template <>
			struct _make_signed<u64> : identity<s64> {};
		template <>
			struct _make_signed<short> : identity<short> {};
		template <>
			struct _make_signed<ushort> : identity<short> {};
		template <>
			struct _make_signed<schar> : identity<schar> {};
		template <>
			struct _make_signed<char> : identity<schar> {};
		template <class T>
			using make_unsigned = typename _make_unsigned<T>::type;
		template <class T>
			using make_signed = typename _make_signed<T>::type;
		using ssize_t = make_signed<size_t>;
	}
}
#undef DLL_EXPORTED
#define DLL_EXPORTED DLL_EXPORT_LIB_cppextensions
#include ".part/internal-macros.hpp"
