#pragma once
#include "Utils.hpp"
#include <type_traits>
namespace CustomUtils {
	template <class T>
		void arrayfeed(T&, const char*, Utils::size);
	template <class T, Utils::size N>
	class Array : Utils {
		public:
			T data[N];
		private:
			template <size... nums>
				constexpr Array(const T (&val)[N], list<nums...>) noexcept : data { val[nums]... } {}
			template <nullptr_t... filler, class... ts>
				constexpr Array(list<filler...>, ts&&... args) noexcept(noexcept(T(forward<ts>(args)...))) :
					data { (static_cast<void>(filler), T { forward<ts>(args)... })... } {}
		public:
			constexpr Array() noexcept : Array(fill_set<nullptr, N> {}) {}
			template <class... ts>
			constexpr Array(nullptr_t, ts&&... args) noexcept(noexcept(T(forward<ts>(args)...))) :
				Array(fill_set<nullptr, N> {}, forward<ts>(args)...) {}
			constexpr Array(const T (&val)[N])
				noexcept(noexcept(Array(val, queue<N> {}))) :
					Array(val, queue<N> {}) {}
			constexpr Array(const Array& val)
				noexcept(noexcept(Array(val.data, queue<N> {}))) :
					Array(val.data, queue<N> {}) {}
			constexpr operator array<T, N>&() noexcept {
				return data;
			}
			constexpr operator array<const T, N>&() const noexcept {
				return data;
			}
			template <class V>
				constexpr bool operator ==(const Array<V, N>& val)
					noexcept(noexcept(declval<T&> == declval<const V&>)) {
						for (size i = 0; i < N; i++)
							if (!(data[i] == val[i]))
								return false;
						return true;
					}
			template <class V>
				constexpr bool operator ==(V&& val)
					noexcept(noexcept(declval<T&> == declval<V>)) {
						for (size i = 0; i < N; i++)
							if (!(data[i] == forward<V>(val)))
								return false;
						return true;
					}
			template <class V>
				constexpr bool operator !=(const Array<V, N>& val)
					noexcept(noexcept(declval<T&> != declval<const V&>)) {
						for (size i = 0; i < N; i++)
							if (data[i] != val[i])
								return true;
						return false;
					}
			template <class V>
				constexpr bool operator !=(V&& val)
					noexcept(noexcept(declval<T&> == declval<V>)) {
						for (size i = 0; i < N; i++)
							if (data[i] != forward<V>(val))
								return true;
						return false;
					}
			template <class V>
				friend enable_it<is_same<remove_cv<T>, char>, V&>
					operator <<(V& os, const Array& val) {
						arrayfeed(os, val.data, N);
						return os;
					}
	};
	template <class T, Utils::size N>
		Array(const Array<T, N>&) -> Array<T, N>;
	template <class T, Utils::size N>
		Array(const T(&)[N]) -> Array<T, N>;
}
