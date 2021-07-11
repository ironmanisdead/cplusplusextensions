#pragma once
#include "Macros.hpp"
#include <compare>
namespace CustomUtils {
	template <class, decltype(sizeof 0)>
		class Array;
	class Utils {
		private:
			template <class T>
				static auto try_lvalue_reference(int) -> T;
			template <class T>
				static auto try_lvalue_reference(...) -> T&;
			template <class T>
				static auto try_rvalue_reference(int) -> T;
			template <class T>
				static auto try_rvalue_reference(...) -> T&&;
		public:
			static volatile void* ignore(volatile void*) noexcept; //no-op for avoiding optimizations
			static int epoch() noexcept; //high_resolution_clock since epoch
			static int rand() noexcept; //random integer
			template <class T>
			static constexpr T* addressof(T& val) noexcept {
				const char* ref = &RECAST(const char&, val);
				return const_cast<T*>(RECAST(const T*, ref));
			}
			template <class T>
				using add_rvalue_reference = decltype(try_rvalue_reference<T>(0));
			template <class T>
				using add_lvalue_reference = decltype(try_lvalue_reference<T>(0));
			using size = decltype(sizeof(0));
			static void memcpy(void*, const void*, size) noexcept;
			static void memmove(void*, const void*, size) noexcept;
			[[noreturn]] static void RunError(const char*);
			[[noreturn]] static void RangeError(const char*);
			template <class...>
				using none = void;
			template <class T, class...>
				using typerep_t = T;
			template <class T, auto...>
				using typerep_v = T;
			template <auto V, class...>
				static constexpr decltype(V) valrep_t = V;
			template <auto V, auto...>
				static constexpr decltype(V) valrep_v = V;
			template <class T>
				static add_rvalue_reference<T> declval() noexcept;
			using nullpt = decltype(nullptr);
			using strongcmp_t = decltype(0 <=> 0);
			using weakcmp_t = decltype(0.0 <=> 0.0);
			static constexpr auto null = nullptr;
			template <class T, size N>
				using array = T[N];
			template <class T>
				struct identity { using type = T; };
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
		private:
			template <class, class>
				struct test_returnable : lie {};
			template <class T>
				struct test_returnable<none<T(*)()>, T> : truth {};
			template <class, class, class>
				struct test_convertible : lie {};
			template <class T, class F>
				struct test_convertible<none<
					decltype(declval<void(&)(F)>()(declval<T>()))>, T, F> : truth {};
			template <class, class, class...>
				struct test_constructible : lie {};
			template <class T, class... args>
				struct test_constructible<none<decltype(T(declval<args>()...))>, T, args...> : truth {};
			template <class, class, class...>
				struct test_noexcept_constructible : lie {};
			template <class T, class... args>
				struct test_noexcept_constructible<
				enable_if<test_constructible<void, T, args...>::value, void>, T, args...> :
					literal_constant<noexcept(T(declval<args>()...))> {};
			template <class, class, class>
				struct test_assign : lie {};
			template <class T, class V>
				struct test_assign<none<decltype(declval<T>() = declval<V>())>, T, V> : truth {};
			template <class, class, class>
				struct test_noexcept_assign : lie {};
			template <class T, class V>
				struct test_noexcept_assign<
				typename enable_if<test_assign<void, T, V>::value, void>::type, T, V> : truth {};
			template <class, class>
				struct test_destructible : lie {};
			template <class T>
				struct test_destructible<decltype(declval<T>().~T()), T> : truth {};
			template <class, class>
				struct test_noexcept_destructible : lie {};
			template <class T>
				struct test_noexcept_destructible<
				typename enable_if<test_destructible<void, T>::value, void>::type, T> : truth {};
			template <class>
				struct test_const : lie {};
			template <class T>
				struct test_const<T const> : truth {};
			template <class>
				struct test_volatile : lie {};
			template <class T>
				struct test_volatile<T volatile> : truth {};
			template <class>
				struct test_cv : lie {};
			template <class T>
				struct test_cv<T const> : truth {};
			template <class T>
				struct test_cv<T volatile> : truth {};
			template <class>
				struct test_reference : lie {};
			template <class T>
				struct test_reference<T&> : truth {};
			template <class T>
				struct test_reference<T&&> : truth {};
			template <class>
				struct test_lvalue_reference : lie {};
			template <class T>
				struct test_lvalue_reference<T&> : truth {};
			template <class>
				struct test_rvalue_reference : lie {};
			template <class T>
				struct test_rvalue_reference<T&&> : truth {};
			template <class>
				struct test_pointer : lie {};
			template <class T>
				struct test_pointer<T*> : truth {};
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
			template <class T, size N>
			struct _array_p<T[N]> {
				static constexpr bool value = true;
				using type = T;
				static constexpr size len = N;
			};
			template <class>
			struct _array_v {
				static constexpr bool value = false;
				using type = void;
			};
			template <class T, size N>
			struct _array_v<Array<T, N>> {
				static constexpr bool value = true;
				using type = T;
				static constexpr size len = N;
			};
			template <class, class, class...>
				struct test_same : lie {};
			template <class T>
				struct test_same<T, T> : truth {};
			template <class T, class... args>
				struct test_same<T, T, args...> : test_same<T, args...> {};
			template <class, class, class...>
				struct find_common : identity<void> {};
			template <class T, class V>
				struct find_common<none<decltype(true ? declval<T>() : declval<V>())>, T, V> :
					identity<decltype(true ? declval<T>() : declval<V>())> {};
			template <class T, class... V>
				struct find_common<void, T, V...> :
					find_common<void, T, class find_common<void, V...>::type> {};
			template <class T>
				struct disqualify : identity<T> {};
			template <class T>
				struct disqualify<T const> : identity<T> {};
			template <class T>
				struct disqualify<T volatile> : identity<T> {};
			template <class T>
				struct disqualify<T const volatile> : identity<T> {};
			template <class T>
				struct disarray : identity<T> {};
			template <class T>
				struct disarray<T[]> : identity<T*> {};
			template <class T, size N>
				struct disarray<T[N]> : identity<T*> {};
			template <class T>
				struct unreference : identity<T> {};
			template <class T>
				struct unreference<T&> : identity<T> {};
			template <class T>
				struct unreference<T&&> : identity<T> {};
			template <class T>
				struct unreference_l : identity<T> {};
			template <class T>
				struct unreference_l<T&> : identity<T> {};
			template <class T>
				struct unreference_r : identity<T> {};
			template <class T>
				struct unreference_r<T&&> : identity<T> {};
			template <class T>
				struct dereference : identity<T> {};
			template <class T>
				struct dereference<T*> : identity<T> {};
			template <class T>
				struct dereference_all : identity<T> {};
			template <class T>
				struct dereference_all<T*> : dereference_all<T> {};
			template <bool dir, class T, class V>
				struct alt_if : identity<V> {};
			template <class T, class V>
				struct alt_if<true, T, V> : identity<T> {};
			template <auto base, size idx, auto... items>
				struct genfull : genfull<base, idx-1, items..., base> {};
			template <auto base, auto... items>
				struct genfull<base, 0, items...> : identity<list<items...>> {};
			template <size idx, size... nums>
				struct genqueue : genqueue<idx-1, idx-1, nums...> {};
			template <size... nums>
				struct genqueue<0, nums...> : identity<list<nums...>> {};
			template <size idx, size... nums>
				struct genstack : genstack<idx-1, nums..., idx-1> {};
			template <size... nums>
				struct genstack<0, nums...> : identity<list<nums...>> {};
			template <size base, size idx, size cur, size... nums>
				struct genexp : genexp<base, idx-1, cur*10, nums..., cur> {};
			template <size base, size cur, size... nums>
				struct genexp<base, 0, cur, nums...> : identity<list<nums...>> {};
			template <size base, size idx, size cur, size... nums>
				struct gendec : gendec<base, idx-1, cur*10, cur, nums...> {};
			template <size base, size cur, size... nums>
				struct gendec<base, 0, cur, nums...> : identity<list<nums...>> {};
			template <class... list>
			static constexpr bool addtest() {
				if constexpr (sizeof...(list) > 0)
					return noexcept( (declval<list>() + ...) );
				else
					return true;
			};
		public:
			template <class... list>
			static constexpr size sum(list... val) noexcept(addtest<list...>()) {
				if constexpr (sizeof...(list) > 0)
					return (val + ...);
				else
					return 0;
			}
			template <class T, class U>
			static constexpr T&& forward(U&& val) noexcept {
				return RECAST(T&&, val);
			}
			template <class T>
			static constexpr typename unreference<T>::type&& move(T& val) noexcept {
				return reinterpret_cast<typename unreference<T>::type&&>(val);
			}
			template <class T, class V, class... args>
				static constexpr bool is_same = test_same<T, V, args...>::value;
			template <class T, class... args>
				static constexpr bool is_void = test_same<void, T, args...>::value;
			template <class T, class V>
				static constexpr bool is_convertible = test_convertible<void, T, V>::value;
			template <class T, class... args>
				static constexpr bool is_constructible = test_constructible<void, T, args...>::value;
			template <class T, class... args>
				static constexpr bool is_noexcept_constructible =
					test_noexcept_constructible<void, T, args...> {};
			template <class T, class V>
				static constexpr bool is_assignable = test_assign<void, T, V>::value;
			template <class T>
				static constexpr bool is_destructible = test_destructible<void, T>::value;
			template <class T>
				static constexpr bool is_volatile = test_volatile<T>::value;
			template <class T>
				static constexpr bool is_const = test_const<T>::value;
			template <class T>
				static constexpr bool is_cv = test_cv<T>::value;
			template <class T>
				static constexpr bool is_reference = test_reference<T>::value;
			template <class T>
				static constexpr bool is_lvalue_reference = test_lvalue_reference<T>::value;
			template <class T>
				static constexpr bool is_rvalue_reference = test_rvalue_reference<T>::value;
			template <class T, class... V>
				using common_type = typename find_common<void, T, V...>::type;
			template <class T>
				using remove_cv = typename disqualify<T>::type;
			template <class T>
				using remove_reference = typename unreference<T>::type;
			template <class T>
				using remove_lvalue_reference = typename unreference_l<T>::type;
			template <class T>
				using remove_rvalue_reference = typename unreference_r<T>::type;
			template <class T>
				using array_prop = _array_p<remove_reference<T>>;
			template <class T>
				using array_util = _array_v<remove_reference<T>>;
			template <class T>
				using raw_type = remove_cv<remove_reference<T>>;
			template <class T>
				static constexpr bool is_pointer = test_pointer<raw_type<T>>::value;
			template <class T, class V>
				static constexpr bool is_pointer_convertible =
					is_pointer<T> && is_pointer<V> &&
					is_convertible<raw_type<T>, raw_type<V>>;
			template <class T>
				using decay = typename disarray<remove_reference<T>>::type;
			template <class T>
				using remove_pointer = typename dereference<raw_type<T>>::type;
			template <class T>
				using strip_pointer = typename dereference_all<raw_type<T>>::type;
			template <bool enable, class T = void>
				using enable_it = typename enable_if<enable, T>::type;
			template <bool dir, class T, class V = void>
				using switch_if = typename alt_if<dir, T, V>::type;
			template <size num>
				using queue = typename genqueue<num>::type;
			template <size num>
				using stack = typename genstack<num>::type;
			template <size base, size num>
				using queue_exp = typename genexp<base, num, 1>::type;
			template <size base, size num>
				using stack_exp = typename gendec<base, num, 1>::type;
			template <auto base, size num>
				using fill_set = typename genfull<base, num>::type;
			template <class T>
				using add_reference = switch_if<is_reference<T>, T, const T&>;
			template <class T, class U>
			static constexpr T upcast(U&& val) noexcept {
				static_assert(is_pointer_convertible<U, T>
						|| is_convertible<U, T>, "cannot upcast types");
				static_assert(is_pointer<T> || is_reference<T>,
						"cannot recast if type is neither pointer nor reference");
				return RECAST(T, val);
			}
			template <class T, class U>
			static constexpr T downcast(U&& val) noexcept {
				static_assert(is_pointer_convertible<T, U>
						|| is_convertible<T, U>, "cannot downcast types");
				static_assert(is_pointer<T> || is_reference<T>,
						"cannot recast if type is neither pointer nor reference");
				return RECAST(T, val);
			}
			template <class T, class R = T>
			static constexpr bool wrap_less(const T& val, const R& oth)
			noexcept(noexcept(val < oth)) {
				return val < oth;
			}
			template <class T, bool assert = false>
			static enable_it<is_destructible<T>, void> wrap_destruct(void* src) 
			noexcept(assert || noexcept(declval<T>().~T())) { //this prevents additional noexcept-casting errors if check is true
				static_assert(!assert || noexcept(declval<T>().~T()),
						"wrap_destruct error: destructor is noexcept(false)");
				downcast<T*>(src)->~T();
			}
			template <class T, bool assert = false>
			static enable_it<is_destructible<T>, void> full_destruct(void* src)
			noexcept(assert || noexcept(declval<T>().~T())) {
				reinterpret_cast<T*>(src)->~T();
				::operator delete(src);
			}
			template <class T, class Y, class Z = remove_reference<Y>>
			static void wrap_construct(void* dest, switch_if<is_const<Z>, const void*, void*> src)
			noexcept(noexcept(T(declval<Y>()))) {
				return static_cast<void>(new (dest)
						T(forward<Y>(*reinterpret_cast<Z*>(src))));
			}
			template <class T, class Y, class Z = remove_reference<Y>>
			static T* full_construct(switch_if<is_const<Z>, const void*, void*> src) {
				return new (::operator new(sizeof(T))) T(*reinterpret_cast<Z*>(src));
			}
			template <class T, class Y, class Z = remove_reference<Y>>
			static void wrap_assign(void* dest, switch_if<is_const<Z>, const void*, void*> src)
			noexcept(noexcept(declval<T&>() = declval<Y>())) {
				return static_cast<void>(*reinterpret_cast<T*>(dest) =
						forward<Y>(*reinterpret_cast<Z*>(src)));
			}
	};
}
#ifndef UNCONST
 #define UNCONST(exp) const_cast<::CustomUtils::Utils::remove_cv<decltype(exp)>&&>(exp)
#endif
