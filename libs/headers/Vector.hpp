#pragma once
#include "UVector.hpp"
#include "String.hpp"
namespace CPPExtensions {
	class String;
	template <class>
		class Vector;
	template <class T>
	class Vector : public UVector {
		private:
			#include ".part/Vector-info.hpp"
			static constexpr bool self_char = Utils::is_same<T, char>;
			void create(Utils::enable_it<self_char, String&&> val) noexcept;
			[[nodiscard]] bool create(Utils::enable_it<self_char, const String&> val) noexcept;
			template <class V>
			[[nodiscard]] bool create(const Vector<V>& val) noexcept {
				return UVector::copy(val, &Utils::wrap_construct<T, const V&>);
			}
			void create(Vector&& val) noexcept { UVector::save(val); }
			[[nodiscard]] bool create(const UVector& val) { return UVector::copy(val); }
			bool create(UVector&& val) { return UVector::save(val); }
		public:
			Vector() noexcept : UVector(&_typeinfo) { trulen = 0; }
			Vector(const Vector& val) noexcept : UVector(&_typeinfo) { (void)create(val); }
			Vector(Vector&& val) noexcept : UVector(&_typeinfo) { save(val); }
			template <class V, typename... L, bool sin_arg = (sizeof...(L) == 0),
			 bool str_cons = sin_arg && Utils::is_same<V, String>>
			Vector(V&& ini, L&&... rest)
			noexcept(str_cons || Utils::is_same<V, Vector>) : UVector(&_typeinfo) {
				if constexpr (Utils::is_same<Utils::raw_type<V>, Vector> && sin_arg)
					(void)create(Utils::forward<V>(ini));
				else if constexpr (Utils::is_same<V, UVector> && sin_arg)
					(void)create(Utils::forward<V>(ini));
				else if constexpr (Utils::is_same<T, char>)
					(void)create(String(Utils::forward<V>(ini), Utils::forward<L>(rest)...));
			}
			template <class... Ts>
			bool insert(Utils::size n1, Ts&&... vals) noexcept {
				if (n1 > len)
					return false;
				char alloc[sizeof(T)];
				if (!resize( (len + 1) * sizeof(T) ))
					return false;
				new (alloc) T(Utils::forward<Ts>(vals)...);
				place(n1, alloc);
				return true;
			}
			template <class... Ts>
			bool append(Ts&&... vals) noexcept {
				char alloc[sizeof(T)];
				if (!resize( (len + 1) * sizeof(T) ))
					return false;
				new (alloc) T(Utils::forward<Ts>(vals)...);
				place(len, alloc);
				return true;
			}
			Vector& operator =(const UVector& val) {
				static_assert(Utils::is_constructible<T, const T&>, "type has no copy constructor");
				(void)UVector::copy(val);
				return *this;
			}
			template <typename V>
			Vector& operator =(const Vector<V>& val) {
				static_assert(Utils::is_constructible<T, const V&>, "cannot convert types");
				(void)copy(val, &Utils::wrap_construct<T, const V&>);
				return *this;
			}
			Vector& operator =(Vector&& val) noexcept {
				static_assert(Utils::is_assignable<T&, T&&>, "type is not move-constructible");
				(void)UVector::save(val, &Utils::wrap_assign<T&, T&&>);
				return *this;
			}
			template <typename Z>
			void foreach(Z&& call) {
				if (trulen > 0)
					for (Utils::size n = 0; n < len; n++)
						call(*Utils::downcast<T*>(&raw[sizeof(T) * n]));
			}
			T* operator [](Utils::size n) {
				return Utils::downcast<T*>(&raw[sizeof(T) * n]);
			}
			const T* operator [](Utils::size n) const {
				return Utils::downcast<const T*>(&raw[sizeof(T) * n]);
			}
			template <class V>
			friend V& operator <<(V& os, const Vector<char>& vec) {
				vecput(os, &vec);
				return os;
			}
			template <class V>
			friend V& operator <<(V& os, const Vector<char>* vec) {
				vecput(os, vec);
				return os;
			}
			friend class String;
	};
	template <class T>
	UVector::UVector(const Vector<T>& val) : typeinfo(val.typeinfo) {
		copy(val);
	}
	template <class T>
	UVector::UVector(Vector<T>&& val) noexcept : typeinfo(val.typeinfo) {
		save(val);
	}
}
