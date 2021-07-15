#pragma once
#include "types.hpp"
namespace CPPExtensions {
	template <class T>
		void arrayput(T&, const char*, Utils::size);
	template <class T, Utils::size N>
	class Array {
		public:
			T data[N];
		private:
			template <Utils::size Z, Utils::size... nums>
				constexpr Array(const T (&val)[Z], Utils::list<nums...>)
				noexcept : data { val[nums]... } {
					static_assert(Z >= N, "assigning to Array from array of smaller size");
				}
			template <Utils::nullpt... filler, class... ts>
				constexpr Array(Utils::list<filler...>, ts&&... args)
				noexcept(noexcept(T(Utils::forward<ts>(args)...))) :
					data { (static_cast<void>(filler), T { Utils::forward<ts>(args)... })... } {}
		public:
			constexpr Array() noexcept : Array(Utils::fill_set<Utils::null, N> {}) {}
			template <class... ts>
			constexpr Array(Utils::nullpt, ts&&... args) noexcept(noexcept(T(Utils::forward<ts>(args)...))) :
				Array(Utils::fill_set<Utils::null, N> {}, Utils::forward<ts>(args)...) {}
			template <Utils::size Z>
			constexpr Array(const T (&val)[Z])
				noexcept(noexcept(Array(val, Utils::queue<N> {}))) :
					Array(val, Utils::queue<N> {}) {}
			template <Utils::size Z>
			constexpr Array(const Array<T, Z>& val)
				noexcept(noexcept(Array(val.data, Utils::queue<N> {}))) :
					Array(val.data, Utils::queue<N> {}) {}
			constexpr operator Utils::array<T, N>&() noexcept {
				return data;
			}
			constexpr operator Utils::array<const T, N>&() const noexcept {
				return data;
			}
			template <class V>
				constexpr bool operator ==(const Array<V, N>& val)
					noexcept(noexcept(Utils::declval<T&> == Utils::declval<const V&>)) {
						for (Utils::size i = 0; i < N; i++)
							if (!(data[i] == val[i]))
								return false;
						return true;
					}
			template <class V>
				constexpr bool operator ==(V&& val)
					noexcept(noexcept(Utils::declval<T&> == Utils::declval<V>)) {
						for (Utils::size i = 0; i < N; i++)
							if (!(data[i] == Utils::forward<V>(val)))
								return false;
						return true;
					}
			template <class V>
				constexpr bool operator !=(const Array<V, N>& val)
					noexcept(noexcept(Utils::declval<T&> != Utils::declval<const V&>)) {
						for (Utils::size i = 0; i < N; i++)
							if (data[i] != val[i])
								return true;
						return false;
					}
			template <class V>
				constexpr bool operator !=(V&& val)
					noexcept(noexcept(Utils::declval<T&> == Utils::declval<V>)) {
						for (Utils::size i = 0; i < N; i++)
							if (data[i] != Utils::forward<V>(val))
								return true;
						return false;
					}
			template <class V>
				friend Utils::enable_it<Utils::is_same<Utils::remove_cv<T>, char>, V&>
					operator <<(V& os, const Array& val) {
						arrayput(os, val.data, N);
						return os;
					}
	};
	template <class T, Utils::size N>
		Array(const Array<T, N>&) -> Array<T, N>;
	template <class T, Utils::size N>
		Array(const T(&)[N]) -> Array<T, N>;
}
