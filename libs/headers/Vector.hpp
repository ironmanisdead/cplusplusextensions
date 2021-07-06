#pragma once
#include "UVector.hpp"
namespace CustomUtils {
	class String;
	template <class T>
	class Vector : public UVector {
		private:
			#include ".hidden/Vector-info.hpp"
		public:
			Vector() noexcept : UVector(&_typeinfo) { trulen = 0; }
			template <class... Ts>
			void insert(Utils::size n1, Ts&&... vals) {
				if (n1 > len)
					Utils::RunError("Vector not large enough");
				char alloc[sizeof(T)];
				resize( (len + 1) * sizeof(T) );
				new (alloc) T(Utils::forward<Ts>(vals)...);
				place(n1, alloc);
			}
			template <class... Ts>
			void append(Ts&&... vals) {
				char alloc[sizeof(T)];
				resize( (len + 1) * sizeof(T) );
				new (alloc) T(Utils::forward<Ts>(vals)...);
				place(len, alloc);
			}
			Vector& operator =(const UVector& val) {
				static_assert(Utils::is_constructible<T, const T&>, "type has no copy constructor");
				UVector::copy(val);
				return *this;
			}
			template <typename V>
			Vector& operator =(const Vector<V>& val) {
				static_assert(Utils::is_constructible<T, const V&>, "cannot convert types");
				copy(val, &Utils::wrap_construct<T, const V&>);
				return *this;
			}
			Vector& operator =(Vector&& val) noexcept {
				static_assert(Utils::is_assignable<T&, T&&>, "type is not move-constructible");
				save(val, &Utils::wrap_assign<T&, T&&>);
				return *this;
			}
			template <typename Z>
			void foreach(Z&& call) {
				if (trulen > 0)
					for (Utils::size n = 0; n < len; n++)
						call(*downcast<T*>(&raw[sizeof(T) * n]));
			}
			T& operator[](Utils::size n) {
				return *downcast<T*>(&raw[sizeof(T) * n]);
			}
			const T& operator[](Utils::size n) const {
				return *downcast<const T*>(&raw[sizeof(T) * n]);
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
