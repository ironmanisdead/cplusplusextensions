#pragma once
#include "Macros.hpp"
#include ".part/internals.hpp"
namespace CPPExtensions {
	template <class, decltype(sizeof 0)>
		class Array;
	namespace Utils {
		template <class T>
			auto _try_lvalue_reference(int) -> T;
		template <class T>
			auto _try_lvalue_reference(...) -> T&;
		template <class T>
			auto _try_rvalue_reference(int) -> T;
		template <class T>
			auto _try_rvalue_reference(...) -> T&&;
		template <class T>
		constexpr T* addressof(T& val) noexcept {
			const char* ref = &RECAST(const char&, val);
			return const_cast<T*>(RECAST(const T*, ref));
		}
		template <class T>
			using add_rvalue_reference = decltype(_try_rvalue_reference<T>(0));
		template <class T>
			using add_lvalue_reference = decltype(_try_lvalue_reference<T>(0));
		template <class...>
			using none = void;
		template <class T, class...>
			using typerep_t = T;
		template <class T, auto...>
			using typerep_v = T;
		template <auto V, class...>
			constexpr decltype(V) valrep_t = V;
		template <auto V, auto...>
			constexpr decltype(V) valrep_v = V;
		template <class T>
			add_rvalue_reference<T> declval() noexcept;
		using nullpt = decltype(nullptr);
		constexpr auto null = nullptr;
		template <class T, size_t N>
			using array = T[N];
		template <auto... items>
			struct list {};
		template <auto v>
			struct literal_constant {
				using type = decltype(v);
				static constexpr type value = v;
				constexpr operator type() {
					return v;
				}
			};
		using truth = literal_constant<true>;
		using lie = literal_constant<false>;
		template <bool enable, class T>
			struct enable_if {};
		template <class T>
			struct enable_if<true, T> : identity<T> {};
		template <bool dir, class T, class F>
			struct switch_if : identity<F> {};
		template <class T, class F>
			struct switch_if<true, T, F> : identity<T> {};
		template <class, class>
			struct _test_returnable : lie {};
		template <class T>
			struct _test_returnable<none<T(*)()>, T> : truth {};
		template <class, class, class>
			struct _test_convertible : lie {};
		template <class T, class F>
			struct _test_convertible<none<
				decltype(declval<void(&)(F)>()(declval<T>()))>, T, F> : truth {};
		template <class, class, class...>
			struct _test_constructible : lie {};
		template <class T, class... args>
			struct _test_constructible<none<decltype(T(declval<args>()...))>, T, args...> : truth {};
		template <class, class, class...>
			struct _test_noexcept_constructible : lie {};
		template <class T, class... args>
			struct _test_noexcept_constructible<
			enable_if<_test_constructible<void, T, args...>::value, void>, T, args...> :
				literal_constant<noexcept(T(declval<args>()...))> {};
		template <class, class, class>
			struct _test_assign : lie {};
		template <class T, class V>
			struct _test_assign<none<decltype(declval<T>() = declval<V>())>, T, V> : truth {};
		template <class, class, class>
			struct _test_noexcept_assign : lie {};
		template <class T, class V>
			struct _test_noexcept_assign<
			typename enable_if<_test_assign<void, T, V>::value, void>::type, T, V> : truth {};
		template <class, class>
			struct _test_destructible : lie {};
		template <class T>
			struct _test_destructible<decltype(declval<T>().~T()), T> : truth {};
		template <class, class>
			struct _test_noexcept_destructible : lie {};
		template <class T>
			struct _test_noexcept_destructible<
			typename enable_if<_test_destructible<void, T>::value, void>::type, T> : truth {};
		template <class>
			struct _test_const : lie {};
		template <class T>
			struct _test_const<T const> : truth {};
		template <class>
			struct _test_volatile : lie {};
		template <class T>
			struct _test_volatile<T volatile> : truth {};
		template <class>
			struct _test_cv : lie {};
		template <class T>
			struct _test_cv<T const> : truth {};
		template <class T>
			struct _test_cv<T volatile> : truth {};
		template <class>
			struct _test_reference : lie {};
		template <class T>
			struct _test_reference<T&> : truth {};
		template <class T>
			struct _test_reference<T&&> : truth {};
		template <class>
			struct _test_lvalue_reference : lie {};
		template <class T>
			struct _test_lvalue_reference<T&> : truth {};
		template <class>
			struct _test_rvalue_reference : lie {};
		template <class T>
			struct _test_rvalue_reference<T&&> : truth {};
		template <class>
			struct _test_pointer : lie {};
		template <class T>
			struct _test_pointer<T*> : truth {};
		template <class>
		struct _array_p {
			static constexpr bool value = false;
			using type = void;
		};
		template <class T>
		struct _array_p<T[]> {
			static constexpr bool value = false;
			using type = T;
		};
		template <class T, size_t N>
		struct _array_p<T[N]> {
			static constexpr bool value = true;
			using type = T;
			static constexpr size_t len = N;
		};
		template <class>
		struct _array_v {
			static constexpr bool value = false;
			using type = void;
		};
		template <class T, size_t N>
		struct _array_v<Array<T, N>> {
			static constexpr bool value = true;
			using type = T;
			static constexpr size_t len = N;
		};
		template <class, class, class...>
			struct _test_same : lie {};
		template <class T>
			struct _test_same<T, T> : truth {};
		template <class T, class... args>
			struct _test_same<T, T, args...> : _test_same<T, args...> {};
		template <class, class, class...>
			struct _find_common : identity<void> {};
		template <class T, class V>
			struct _find_common<none<decltype(true ? declval<T>() : declval<V>())>, T, V> :
				identity<decltype(true ? declval<T>() : declval<V>())> {};
		template <class T, class... V>
			struct _find_common<void, T, V...> :
				_find_common<void, T, class _find_common<void, V...>::type> {};
		template <class T>
			struct _disqualify : identity<T> {};
		template <class T>
			struct _disqualify<T const> : identity<T> {};
		template <class T>
			struct _disqualify<T volatile> : identity<T> {};
		template <class T>
			struct _disqualify<T const volatile> : identity<T> {};
		template <class T>
			struct _disarray : identity<T> {};
		template <class T>
			struct _disarray<T[]> : identity<T*> {};
		template <class T, size_t N>
			struct _disarray<T[N]> : identity<T*> {};
		template <class T>
			struct _unreference : identity<T> {};
		template <class T>
			struct _unreference<T&> : identity<T> {};
		template <class T>
			struct _unreference<T&&> : identity<T> {};
		template <class T>
			struct _unreference_l : identity<T> {};
		template <class T>
			struct _unreference_l<T&> : identity<T> {};
		template <class T>
			struct _unreference_r : identity<T> {};
		template <class T>
			struct _unreference_r<T&&> : identity<T> {};
		template <class T>
			struct _dereference : identity<T> {};
		template <class T>
			struct _dereference<T*> : identity<T> {};
		template <class T>
			struct _dereference_all : identity<T> {};
		template <class T>
			struct _dereference_all<T*> : _dereference_all<T> {};
		template <bool dir, auto T, auto F>
			struct _alt_v : literal_constant<F> {};
		template <auto T, auto F>
			struct _alt_v<true, T, F> : literal_constant<T> {};
		template <auto base, size_t idx, auto... items>
			struct _genfull : _genfull<base, idx-1, items..., base> {};
		template <auto base, auto... items>
			struct _genfull<base, 0, items...> : identity<list<items...>> {};
		template <size_t idx, size_t... nums>
			struct _genqueue : _genqueue<idx-1, idx-1, nums...> {};
		template <size_t... nums>
			struct _genqueue<0, nums...> : identity<list<nums...>> {};
		template <size_t idx, size_t... nums>
			struct _genstack : _genstack<idx-1, nums..., idx-1> {};
		template <size_t... nums>
			struct _genstack<0, nums...> : identity<list<nums...>> {};
		template <size_t base, size_t idx, size_t cur, size_t... nums>
			struct _genexp : _genexp<base, idx-1, cur*10, nums..., cur> {};
		template <size_t base, size_t cur, size_t... nums>
			struct _genexp<base, 0, cur, nums...> : identity<list<nums...>> {};
		template <size_t base, size_t idx, size_t cur, size_t... nums>
			struct _gendec : _gendec<base, idx-1, cur*10, cur, nums...> {};
		template <size_t base, size_t cur, size_t... nums>
			struct _gendec<base, 0, cur, nums...> : identity<list<nums...>> {};
		template <class... list>
		constexpr bool addtest() {
			if constexpr (sizeof...(list) > 0)
				return noexcept( (declval<list>() + ...) );
			else
				return true;
		};
		template <class... list>
		constexpr size_t sum(list... val) noexcept(addtest<list...>()) {
			if constexpr (sizeof...(list) > 0)
				return (val + ...);
			else
				return 0;
		}
		template <class T, class U>
		constexpr T&& forward(U&& val) noexcept {
			return RECAST(T&&, val);
		}
		template <class T>
		constexpr typename _unreference<T>::type&& move(T& val) noexcept {
			return reinterpret_cast<typename _unreference<T>::type&&>(val);
		}
		template <class T, class V, class... args>
			constexpr bool is_same = _test_same<T, V, args...>::value;
		template <class T, class... args>
			constexpr bool is_void = _test_same<void, T, args...>::value;
		template <class T, class V>
			constexpr bool is_convertible = _test_convertible<void, T, V>::value;
		template <class T, class... args>
			constexpr bool is_constructible = _test_constructible<void, T, args...>::value;
		template <class T, class... args>
			constexpr bool is_noexcept_constructible =
				_test_noexcept_constructible<void, T, args...> {};
		template <class T, class V>
			constexpr bool is_assignable = _test_assign<void, T, V>::value;
		template <class T>
			constexpr bool is_destructible = _test_destructible<void, T>::value;
		template <class T>
			constexpr bool is_volatile = _test_volatile<T>::value;
		template <class T>
			constexpr bool is_const = _test_const<T>::value;
		template <class T>
			constexpr bool is_cv = _test_cv<T>::value;
		template <class T>
			constexpr bool is_reference = _test_reference<T>::value;
		template <class T>
			constexpr bool is_lvalue_reference = _test_lvalue_reference<T>::value;
		template <class T>
			constexpr bool is_rvalue_reference = _test_rvalue_reference<T>::value;
		template <class T, class... V>
			using common_type = typename _find_common<void, T, V...>::type;
		template <class T>
			using remove_cv = typename _disqualify<T>::type;
		template <class T>
			using remove_reference = typename _unreference<T>::type;
		template <class T>
			using remove_lvalue_reference = typename _unreference_l<T>::type;
		template <class T>
			using remove_rvalue_reference = typename _unreference_r<T>::type;
		template <class T>
			using array_prop = _array_p<remove_reference<T>>;
		template <class T>
			using array_util = _array_v<remove_reference<T>>;
		template <class T>
			using raw_type = remove_cv<remove_reference<T>>;
		template <class T>
			constexpr bool is_pointer = _test_pointer<raw_type<T>>::value;
		template <class T, class V>
			constexpr bool is_pointer_convertible =
				is_pointer<T> && is_pointer<V> &&
				is_convertible<raw_type<T>, raw_type<V>>;
		template <class T>
			using decay = typename _disarray<remove_reference<T>>::type;
		template <class T>
			using remove_pointer = typename _dereference<raw_type<T>>::type;
		template <class T>
			using strip_pointer = typename _dereference_all<raw_type<T>>::type;
		template <bool enable, class T = void>
			using enable_it = typename enable_if<enable, T>::type;
		template <bool dir, class T, class F = void>
			using switch_it = typename switch_if<dir, T, F>::type;
		template <bool dir, auto T, auto F = nullptr>
			constexpr typename 
				_alt_v<dir, T, F>::type switch_v = _alt_v<dir, T, F>::value;
		template <size_t num>
			using queue = typename _genqueue<num>::type;
		template <size_t num>
			using stack = typename _genstack<num>::type;
		template <size_t base, size_t num>
			using queue_exp = typename _genexp<base, num, 1>::type;
		template <size_t base, size_t num>
			using stack_exp = typename _gendec<base, num, 1>::type;
		template <auto base, size_t num>
			using fill_set = typename _genfull<base, num>::type;
		template <class T>
			using add_reference = switch_it<is_reference<T>, T, const T&>;
		template <class T, class U>
		constexpr T upcast(U&& val) noexcept {
			static_assert(is_pointer_convertible<U, T>
					|| is_convertible<U, T>, "cannot upcast types");
			static_assert(is_pointer<T> || is_reference<T>,
					"cannot recast if type is neither pointer nor reference");
			return RECAST(T, val);
		}
		template <class T, class U>
		constexpr T downcast(U&& val) noexcept {
			static_assert(is_pointer_convertible<T, U>
					|| is_convertible<T, U>, "cannot downcast types");
			static_assert(is_pointer<T> || is_reference<T>,
					"cannot recast if type is neither pointer nor reference");
			return RECAST(T, val);
		}
		template <class T, class R = T>
		constexpr bool wrap_less(const T& val, const R& oth)
		noexcept(noexcept(val < oth)) {
			return val < oth;
		}
	}
}
