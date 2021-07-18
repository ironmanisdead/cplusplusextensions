#pragma once
#include "types.hpp"
#include "GString.hpp"
//#include <typeinfo>
namespace CPPExtensions {
	template <class>
		class Function;
	class Functions {
		protected:
			Functions() = default;
			Functions(const Functions&) = default;
			Functions(Functions&&) = default;
		public:
			enum State {
				NO_ERROR,
				MEM_ERROR,
				INIT_ERROR,
				NULL_ERROR,
				CALL_ERROR
			};
	};
	template <class Ret, class... Args>
	class Function<Ret(Args...)> : public Functions {
		private:
			mutable State _status;
			Utils::size_t size;
			struct Proxy {
				virtual Ret call(Utils::add_reference<Args>... args) = 0;
				virtual void copy(void*) const = 0;
				virtual void move(void*) = 0;
				virtual Utils::size_t size() const noexcept = 0;
				virtual ~Proxy() {}
			};
			template <class T>
			class Binding final : public Proxy {
				private:
					T callable;
					using ref = Utils::add_reference<T>;
					using raw = Utils::raw_type<T>;
					static constexpr bool copy_except =
						noexcept(T(Utils::declval<ref>()));
					static constexpr bool mov_if = Utils::is_constructible<raw, const raw&>;
					using retype = Utils::switch_it<mov_if, raw, T>;
				public:
					Binding(ref val) noexcept(copy_except) : callable(val) {}
					Ret call(Utils::add_reference<Args>... args) override {
						return callable(args...);
					}
					Utils::size_t size() const noexcept override {
						return sizeof(Binding<retype>);
					}
					void copy(void* val) const override {
						new (val) Binding<T>(callable);
					}
					void move(void* val) override {
						new (val) Binding<retype>(RECAST(retype&&, callable));
					}
					~Binding() override = default;
			};
			Proxy* caller;
		public:
			Function() noexcept : _status(NO_ERROR), caller(nullptr) {}
			Function(const Function& val) : _status(NO_ERROR) {
				if (val.caller) {
					caller = downcast<Proxy*>(::operator new(size = val.size,
								std::nothrow_t {}));
					if (!caller) {
						_status = MEM_ERROR;
						return;
					}
					try {
						val.caller->copy(caller);
					} catch (...) {
						::operator delete(caller);
						throw;
					}
				} else
					caller = nullptr;
			}
			Function(Function&& val) : _status(NO_ERROR) {
				if (val.caller) {
					caller = downcast<Proxy*>(::operator new(size = val.caller->size(),
								std::nothrow_t {}));
					if (!caller) {
						_status = MEM_ERROR;
						return;
					}
					try {
						val.caller->move(caller);
					} catch (...) {
						::operator delete(caller);
						throw;
					}
				} else
					caller = nullptr;
			}
			template <class T, class raw = Utils::raw_type<T>,
				 bool null = Utils::is_same<raw, Utils::nullpt>>
			Function(T&& val) noexcept(null) : _status(NO_ERROR) {
				if constexpr (null)
					caller = nullptr;
				else if constexpr (Utils::is_same<raw, Function<Ret(Args...)>>) {
					if (val.caller) {
						if constexpr (Utils::is_lvalue_reference<T>) {
							caller = downcast<Proxy*>(
									::operator new(size = val.size,
										std::nothrow_t{}));
							if (!caller) {
								_status = MEM_ERROR;
								return;
							}
							try {
								val.caller->copy(caller);
							} catch (...) {
								::operator delete(caller);
								throw;
							}
						} else {
							caller = downcast<Proxy*>(
									::operator new(size = val.caller->size(),
										std::nothrow_t {}));
							if (!caller) {
								_status = MEM_ERROR;
								return;
							}
							try {
								val.caller->move(caller);
							} catch (...) {
								::operator delete(caller);
								throw;
							}
						}
					} else
						caller = nullptr;
				} else {
					constexpr bool icon = Utils::is_lvalue_reference<T>;
					constexpr bool addr = Utils::is_pointer<T>;
					using retype = Utils::switch_it<icon && !addr, T, raw>;
					void* temp = ::operator new(sizeof(Binding<retype>), std::nothrow_t {});
					if (!temp) {
						_status = MEM_ERROR;
						return;
					}
					try {
						caller = new (temp) Binding<retype>(val);
					} catch (...) {
						_status = INIT_ERROR;
						::operator delete(temp);
						throw;
					}
					size = sizeof(Binding<retype>);
				}
			}
			bool valid() const noexcept {
				if (caller)
					return true;
				else
					return false;
			}
			State getStatus() const noexcept { return _status; }
			Ret operator ()(Utils::add_reference<Args>... args) const {
				_status = NO_ERROR;
				if (caller)
					try {
						return caller->call(args...);
					} catch (...) {
						_status = CALL_ERROR;
						throw;
					}
				else {
					_status = NULL_ERROR;
					constexpr Array typ = GString::typestr<Function<Ret(Args...)>>;
					constexpr Array err = GString::raycat(typ.data, " error: could not return as no"
							" callable value was assigned");
					Utils::RunError(err.data);
				}
			}
			template <class T, class raw = Utils::raw_type<T>,
			bool null = Utils::is_same<raw, Utils::nullpt>>
			Function& operator =(T&& val) noexcept(null) {
				_status = NO_ERROR;
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
							temp = ::operator new (val.size, std::nothrow_t{});
							if (!temp) {
								_status = MEM_ERROR;
								return *this;
							}
							val.caller->copy(temp);
						} else {
							temp = ::operator new (val.callable->size(),
									std::nothrow_t {});
							if (!temp) {
								_status = MEM_ERROR;
								return *this;
							}
							val.caller->move(temp);
						}
					} catch (...) {
						_status = INIT_ERROR;
						::operator delete (temp);
						throw;
					}
					caller = temp;
					size = val.size;
				} else {
					constexpr bool icon = Utils::is_lvalue_reference<T>;
					constexpr bool addr = Utils::is_pointer<T>;
					using retype = Utils::switch_it<icon && !addr, T, raw>;
					void* temp = ::operator new(sizeof(Binding<retype>),
							std::nothrow_t {});
					if (!temp) {
						_status = MEM_ERROR;
						return *this;
					}
					try {
						caller = new (temp) Binding<retype>(val);
					} catch (...) {
						_status = INIT_ERROR;
						::operator delete (temp);
						throw;
					}
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
	class Constructor : public Functions {
		private:
			Function<T()> caller;
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
			template <class... V>
			void set(V&&...args) {
				caller = [&args...] () { return T { Utils::forward<V>(args)... }; };
			}
			void unset() noexcept { caller = nullptr; }
			State getStatus() const noexcept {
				return caller.getStatus();
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
