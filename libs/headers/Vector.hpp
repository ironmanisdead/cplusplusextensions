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
			T& operator[](Utils::size n) {
				return RECAST(T&, raw[sizeof(T) * n]);
			}
			const T& operator[](Utils::size n) const {
				return RECAST(const T&, raw[sizeof(T) * n]);
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
