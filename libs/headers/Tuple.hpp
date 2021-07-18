#pragma once
#include "types.hpp"
namespace CPPExtensions {
	template <class, class...>
		class Tuple;
	//
	class Tuples {
		public:
			template <class T, class... V>
				struct Worth {
					static constexpr bool tup = false;
				};
			template <class T, class... V>
				struct Worth<Tuple<T, V...>> {
					static constexpr bool tup = true;
				};
		private:
			struct mapper {};
			struct hidden {
				constexpr hidden(const hidden&) = default;
				template <class... ts>
					constexpr hidden(mapper, const ts&...) {}
				template <class T, class... ts>
					constexpr hidden(T, ts...) {
						static_assert(sizeof...(ts) < 0, "Tuple is given too many parameters");
					}
				constexpr hidden() {}
			};
			template <class, class... V>
				struct GenTuple : Utils::identity<Tuple<V...>> {};
			template <class T>
				struct GenTuple<T> : Utils::identity<hidden> {};
			template <class, class...>
				friend class Tuple;
	};
	//
	template <class T, class... V>
	class Tuple {
		private:
			template <class, class...>
				friend class Tuple;
			static constexpr Utils::size_t amount = sizeof...(V);
			static constexpr bool isgg = (amount > 0);
			static constexpr bool isll = (amount == 0);
			static constexpr bool curnone = Utils::is_noexcept_constructible<T>;
			static constexpr bool iscur = noexcept(T(Utils::declval<const T&>()));
			template <class Z, class... L>
			static constexpr bool iscons = noexcept(Z(Utils::declval<L>()...));
			T current;
			typename Tuples::GenTuple<void, V...>::type other;
		public:
			template <Utils::size_t N>
				constexpr auto& get() noexcept {
					constexpr bool valid = (N <= amount);
					static_assert(valid, "index out of bounds");
					if constexpr (N == 0)
						return current;
					else if constexpr (valid)
						return other.template get<N-1>();
					else
						return other.template get<amount>();
				}
			template <Utils::size_t N>
				constexpr const auto& get() const noexcept {
					constexpr bool valid = (N <= amount);
					static_assert(valid, "index out of bounds");
					if constexpr (N == 0)
						return current;
					else if constexpr (valid)
						return other.template get<N-1>();
					else
						return other.template get<amount>();
				}
		private:
			template <class F, class tup, Utils::size_t... ns, Utils::size_t idx>
			constexpr Tuple(Tuples::mapper, const tup& val, Utils::list<ns...>, Utils::list<idx>, const F& func) 
				noexcept(noexcept((func(get<ns>()), ...))) : current(func(val.current)),
					other(Tuples::mapper {}, val.other, Utils::queue<idx> {}, Utils::list<idx-1> {}, func) {}
			//
			template <class F, class tup, Utils::size_t... ns, Utils::size_t idx>
			constexpr Tuple(Tuples::mapper, tup& val, Utils::list<ns...>, Utils::list<idx>, F& func)
				noexcept(noexcept((func(get<ns>()), ...))) : current(func(val.current)),
					other(Tuples::mapper {}, val.other, Utils::queue<idx> {}, Utils::list<idx-1> {}, func) {}
		public:
			template <class Z, class... L>
			constexpr Tuple(const Z& val, const L&... args)
				noexcept(iscons<T, Z> && iscons<decltype(other), L...>) : current(val), other(args...) {}
			//
			constexpr Tuple(const Tuple& val) noexcept(iscur && iscons<decltype(other), const decltype(other)&>) :
				current(val.current), other(val.other) {}
			//
			constexpr Tuple(Tuple&& val) noexcept(iscons<T, T&&> && iscons<decltype(other), decltype(other)&&>) :
				current(Utils::move(val.current)), other(Utils::move(val.other)) {}
		private:
			template <class F, Utils::size_t... ns>
			constexpr void ForEach(const F& func, Utils::list<ns...>) noexcept(noexcept( (func(get<ns>()), ...) )) {
				(func(get<ns>()), ...);
			}
			template <class F, Utils::size_t... ns>
			constexpr void ForEach(const F& func, Utils::list<ns...>) const noexcept( noexcept( (func(get<ns>()), ...) )) {
				(func(get<ns>()), ...);
			}
			template <class F, Utils::size_t... ns>
			constexpr auto MakeMap(F& func, Utils::list<ns...> lis) noexcept(noexcept( (func(get<ns>()), ...) )) {
				using type = Tuple<Utils::decay<decltype(func(get<ns>()))>...>;
				return type { Tuples::mapper {}, *this, lis, Utils::list<amount> {}, func };
			}
			template <class F, Utils::size_t... ns>
			constexpr auto MakeMap(const F& func, Utils::list<ns...> lis) const noexcept(noexcept( (func(get<ns>()), ...) )) {
				using type = Tuple<decltype(func(get<ns>()))...>;
				return type { Tuples::mapper {}, *this, lis, Utils::list<amount> {}, func };
			}
		public:
			template <class F>
			constexpr void foreach(const F& func) noexcept(noexcept(ForEach(func, Utils::queue<1 + amount> {}))) {
				ForEach(func, Utils::queue<1 + amount> {});
			}
			template <class F>
			constexpr void foreach(const F& func) const noexcept(noexcept(ForEach(func, Utils::queue<1 + amount> {}))) {
				ForEach(func, Utils::queue<1 + amount> {});
			}
			template <class F>
			constexpr auto map(F& func) noexcept(noexcept(MakeMap(func, Utils::queue<1 + amount> {}))) {
				return MakeMap(func, Utils::queue<1 + amount> {});
			}
			template <class F>
			constexpr auto map(const F& func) const noexcept(noexcept(MakeMap(func, Utils::queue<1 + amount> {}))) {
				return MakeMap(func, Utils::queue<1 + amount> {});
			}
	};
	template <class T, class... list>
		Tuple(T, list...) ->
			Tuple<Utils::enable_it<!Tuples::Worth<T, list...>::tup, Utils::decay<T>>, Utils::decay<list>...>;
	template <class T, class... list>
		Tuple(const Tuple<T, list...>&) ->
			Tuple<Utils::enable_it<Tuples::Worth<T, list...>::tup, Utils::decay<T>>, Utils::decay<list>...>;
}
