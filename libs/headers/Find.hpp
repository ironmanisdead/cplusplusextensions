#pragma once
#include <compare>
namespace CPPExtensions {
	template <class T>
	struct Find {
		using stronk = decltype(0 <=> 0);
		stronk found;
		T value;
		template <class... V>
		constexpr Find(stronk res, V&&... list)
		noexcept(noexcept(T(reinterpret_cast<V&&>(list)...))) :
			found(res), value(reinterpret_cast<V&&>(list)...) {}
		template <class Y>
		constexpr Find(const Find<Y>& other)
		noexcept(noexcept(T(other.value))) :
			found(other.found), value(other.value) {}
	};
}
