#pragma once
#include "types.hpp"
DLL_HIDE
namespace CPPExtensions {
	template <class T>
		DLL_PUBLIC void arrayput(T&, const char*, Utils::size_t); //internal function for printing array
	template <class T, Utils::size_t N>
	class Array {
		public:
			T data[N]; //used to access the data raw
		private:
			template <class O>
			static constexpr bool specifier(Utils::size_t num) noexcept {
				if (num == 0)
					return noexcept(T());
				else if (num < N)
					return noexcept(T(), T(Utils::declval<const O&>()));
				else
					return noexcept(T(Utils::declval<const O&>()));
			}
			template <class O>
			static constexpr bool assignspec(Utils::size_t num) noexcept {
				if (num == 0)
					return true;
				else
					return noexcept(Utils::declval<T&>() =
							Utils::declval<const O&>());
			}
			template <class O, Utils::size_t Z, Utils::size_t... nums>
				constexpr Array(const O (&val)[Z], Utils::list<nums...>)
					noexcept(specifier<O>(Z)) : data { val[nums]... } {}
			template <Utils::nullpt... filler, class... ts>
				constexpr Array(Utils::list<filler...>, ts&&... args)
					noexcept(noexcept(T(Utils::forward<ts>(args)...))) :
						data { (static_cast<void>(filler), T { Utils::forward<ts>(args)... })... } {}
			template <class O, Utils::size_t Z>
				constexpr void setter(const O (&val)[Z])
					noexcept(assignspec<O>(Z)) {
						for (Utils::size_t i = 0; i < N; i++)
							data[i] = val[i];
					}
		public:
			constexpr Array() noexcept : Array(Utils::fill_set<Utils::null, N> {}) {} //fills with default
			template <class... ts>
			constexpr Array(Utils::nullpt, ts&&... args) noexcept(noexcept(T(Utils::forward<ts>(args)...))) :
				Array(Utils::fill_set<Utils::null, N> {}, Utils::forward<ts>(args)...) {}
			template <class O, Utils::size_t Z>
			constexpr Array(const O (&val)[Z])
				noexcept(noexcept(Array(val, Utils::queue<N> {}))) :
					Array(val, Utils::queue<N> {}) {}
			template <class O, Utils::size_t Z>
			constexpr Array(const Array<O, Z>& val)
				noexcept(noexcept(Array(val.data, Utils::queue<N> {}))) :
					Array(val.data, Utils::queue<N> {}) {}
			constexpr operator Utils::array<T, N>&() noexcept {
				return data;
			}
			constexpr operator Utils::array<const T, N>&() const noexcept {
				return data;
			}
			template <class O, Utils::size_t Z>
				constexpr Array& operator =(const Array<O, Z>& val)
				noexcept(assignspec<O>(Z)) {
					setter(val.data);
					return *this;
				}
			template <class O, Utils::size_t Z>
				constexpr Array& operator =(const O(&val)[Z])
				noexcept(assignspec<O>(Z)) {
					setter(val);
					return *this;
				}
			template <class V>
				constexpr bool operator ==(const Array<V, N>& val)
					noexcept(noexcept(Utils::declval<T&> == Utils::declval<const V&>)) {
						for (Utils::size_t i = 0; i < N; i++)
							if (!(data[i] == val[i]))
								return false;
						return true;
					}
			template <class V>
				constexpr bool operator ==(V&& val)
					noexcept(noexcept(Utils::declval<T&> == Utils::declval<V>)) {
						for (Utils::size_t i = 0; i < N; i++)
							if (!(data[i] == Utils::forward<V>(val)))
								return false;
						return true;
					}
			template <class V>
				constexpr bool operator !=(const Array<V, N>& val)
					noexcept(noexcept(Utils::declval<T&> != Utils::declval<const V&>)) {
						for (Utils::size_t i = 0; i < N; i++)
							if (data[i] != val[i])
								return true;
						return false;
					}
			template <class V>
				constexpr bool operator !=(V&& val)
					noexcept(noexcept(Utils::declval<T&> == Utils::declval<V>)) {
						for (Utils::size_t i = 0; i < N; i++)
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
	template <class T, Utils::size_t N>
		Array(const Array<T, N>&) -> Array<T, N>;
	template <class T, Utils::size_t N>
		Array(const T(&)[N]) -> Array<T, N>;
}
DLL_RESTORE
