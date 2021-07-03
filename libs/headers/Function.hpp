#pragma once
#include "Utils.hpp"
#include "GString.hpp"
#include <typeinfo>
namespace CustomUtils {
	template <class>
		class Function;
	template <class Ret, class... Args>
	class Function<Ret(Args...)> {
		private:
			Utils::size size;
			struct Proxy {
				virtual Ret call(Utils::add_reference<Args>... args) = 0;
				virtual void copy(void*) const = 0;
				virtual void move(void*) = 0;
				virtual Utils::size size() const noexcept = 0;
				virtual ~Proxy() {}
			};
			template <class T>
			class Binding final : public Proxy {
				private:
					T callable;
					using raw = Utils::raw_type<T>;
					static constexpr bool mov_if = Utils::is_constructible<raw, const raw&>;
					using retype = Utils::switch_if<mov_if, raw, T>;
				public:
					Binding(Utils::add_reference<T> val) : callable(val) {}
					Ret call(Utils::add_reference<Args>... args) override {
						return callable(args...);
					}
					Utils::size size() const noexcept override {
						return sizeof(Binding<retype>);
					}
					void copy(void* val) const override {
						new (val) Binding<T>(callable);
					}
					void move(void* val) override {
						new (val) Binding<retype>(reinterpret_cast<retype&&>(callable));
					}
					~Binding() override = default;
			};
			Proxy* caller;
		public:
			Function() noexcept : caller(nullptr) {}
			Function(const Function& val) {
				if (val.caller) {
					caller = downcast<Proxy*>(::operator new(size = val.size));
					val.caller->copy(caller);
				} else
					caller = nullptr;
			}
			Function(Function&& val) {
				if (val.caller) {
					caller = downcast<Proxy*>(::operator new(size = val.caller->size()));
					val.caller->move(caller);
				} else
					caller = nullptr;
			}
			template <class T, class raw = Utils::raw_type<T>,
				 bool null = Utils::is_same<raw, Utils::nullptr_t>>
			Function(T&& val) noexcept(null) {
				if constexpr (null)
					caller = nullptr;
				else if constexpr (Utils::is_same<raw, Function<Ret(Args...)>>) {
					if (val.caller) {
						if constexpr (Utils::is_lvalue_reference<T>) {
							caller = downcast<Proxy*>(::operator new(size = val.size));
							val.caller->copy(caller);
						} else {
							caller = downcast<Proxy*>(::operator new(size = val.caller->size()));
							val.caller->move(caller);
						}
					} else
						caller = nullptr;
				} else {
					constexpr bool icon = Utils::is_lvalue_reference<T>;
					constexpr bool addr = Utils::is_pointer<T>;
					using retype = Utils::switch_if<icon && !addr, T, raw>;
					caller = new Binding<retype>(val);
					size = sizeof(Binding<retype>);
				}
			}
			bool valid() const noexcept {
				if (caller)
					return true;
				else
					return false;
			}
			Ret operator ()(Utils::add_reference<Args>... args) const {
				if (caller)
					return caller->call(args...);
				else {
					constexpr Array typ = GString::typestr<Function<Ret(Args...)>>;
					constexpr Array err = GString::raycat(typ.data, " error: not assigned a callable value");
					Utils::RunError(err.data);
				}
			}
			template <class T, class raw = Utils::raw_type<T>,
			bool null = Utils::is_same<raw, Utils::nullptr_t>>
			Function& operator =(T&& val) noexcept(null) {
				if constexpr (null) {
					if (caller) {
						delete caller;
						caller = nullptr;
					}
					return *this;
				} else if constexpr (Utils::is_same<raw, Function<Ret(Args...)>>) {
					if (!val.callable) {
						delete caller;
						caller = nullptr;
						return *this;
					}
					void* temp;
					try {
						if constexpr (Utils::is_lvalue_reference<T>) {
							temp = ::operator new (val.size);
							val.caller->copy(temp);
						} else {
							temp = ::operator new (val.callable->size());
							val.caller->move(temp);
						}
					} catch (...) {
						::operator delete (temp);
						throw;
					}
					caller = temp;
					size = val.size;
				} else {
					constexpr bool icon = Utils::is_lvalue_reference<T>;
					constexpr bool addr = Utils::is_pointer<T>;
					using retype = Utils::switch_if<icon && !addr, T, raw>;
					caller = new Binding<retype>(val);
					size = sizeof(Binding<retype>);
				}
				return *this;
			}
			~Function() {
				if (caller)
					delete caller;
			}
	};
	template <class Ret, class... Args>
		Function(const Function<Ret(Args...)>&) -> Function<Ret(Args...)>;
	template <class T>
	class Constructor {
		private:
			const Function<T()> caller;
		public:
			Constructor(const Constructor& val) : caller(val.caller) {}
			Constructor(Constructor& val) : caller(val.caller) {}
			Constructor(Constructor&&) = delete;
			template <class... V>
			Constructor(V&&... args) :
				caller([&args...] () { return T { Utils::forward<V>(args)... }; }) {}
			T operator()() const {
				return caller();
			}
	};
	template <class T>
	class Object {
		private:
			Function<void ()> destruct;
		public:
			T value;
			template <class Z, class... V>
			constexpr Object(Z&& dest, V&&... args) :
				destruct(Utils::forward<Z>(dest)), value(Utils::forward<V>(args)...) {}
			~Object() { destruct(); }
	};
}
