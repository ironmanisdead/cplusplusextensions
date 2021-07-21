#pragma once
#include "types.hpp"
#include "system.hpp"
#include <compare>
namespace CPPExtensions {
	namespace Utils {
		using strongcmp_t = decltype(0 <=> 0);
		using weakcmp_t = decltype(0.0 <=> 0.0);
		template <class T, bool assert = false>
		enable_it<is_destructible<T>, void> wrap_destruct(void* src) 
		noexcept(assert || noexcept(declval<T>().~T())) { //this prevents additional noexcept-casting errors if check is true
			static_assert(!assert || noexcept(declval<T>().~T()),
					"wrap_destruct error: destructor is noexcept(false)");
			downcast<T*>(src)->~T();
		}
		template <class T, bool assert = false>
		enable_it<is_destructible<T>, void> full_destruct(void* src)
		noexcept(assert || noexcept(declval<T>().~T())) {
			reinterpret_cast<T*>(src)->~T();
			Utils::free(src);
		}
		template <class T, class Y, class Z = remove_reference<Y>>
		void wrap_construct(void* dest, switch_it<is_const<Z>, const void*, void*> src)
		noexcept(noexcept(T(declval<Y>()))) {
			return static_cast<void>(new (dest)
					T(forward<Y>(*reinterpret_cast<Z*>(src))));
		}
		template <class T, class Y, class Z = remove_reference<Y>>
		T* full_construct(switch_it<is_const<Z>, const void*, void*> src)
		noexcept(noexcept(T(declval<Y>()))) {
			void* store = downcast<T*>(Utils::malloc(sizeof(T)));
			if (store)
				new (store) T(forward<Y>(*downcast<Z*>(src)));
			return store;
		}
		template <class T, class Y, class U = remove_reference<T>, class Z = remove_reference<Y>>
		void wrap_assign(void* dest, switch_it<is_const<Z>, const void*, void*> src)
		noexcept(noexcept(declval<T&>() = declval<Y>())) {
			return static_cast<void>(forward<T>(*reinterpret_cast<U*>(dest)) =
					forward<Y>(*reinterpret_cast<Z*>(src)));
		}
	};
}
#ifndef UNCONST
 #define UNCONST(exp) const_cast<::CPPExtensions::Utils::remove_cv<decltype(exp)>&&>(exp)
#endif
