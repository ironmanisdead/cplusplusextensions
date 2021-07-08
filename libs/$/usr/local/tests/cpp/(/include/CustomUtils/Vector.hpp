#pragma once
#include "UVector.hpp"
#include "String.hpp"
namespace CustomUtils {
	class String;
	template <class>
		class Vector;
	template <class T>
	class Vector : public UVector {
		private:
			#include ".hidden/Vector-info.hpp"
			static constexpr bool self_char = Utils::is_same<T, char>;
			void create(Utils::enable_it<self_char, String&&> val) noexcept;
			void create(Utils::enable_it<self_char, const String&> val);
			template <class V>
			void create(const Vector<V>& val) {
				UVector::copy(val, &Utils::wrap_construct);
			}
			void create(Vector&& val) noexcept { UVector::save(val); }
			void create(const UVector& val) { UVector::copy(val); }
			void create(UVector&& val) { UVector::save(val); }
		public:
			Vector() noexcept : UVector(&_typeinfo) { trulen = 0; }
			Vector(const Vector& val) : UVector(&_typeinfo) { create(val); }
			Vector(Vector&& val) noexcept : UVector(&_typeinfo) { save(val); }
			template <class V, typename... L, bool sin_arg = (sizeof...(L) == 0),
			 bool str_cons = sin_arg && Utils::is_same<V, String>>
			Vector(V&& ini, L&&... rest)
			noexcept(str_cons || Utils::is_same<V, Vector>) : UVector(&_typeinfo) {
				if constexpr (Utils::is_same<Utils::raw_type<V>, Vector> && sin_arg)
					create(Utils::forward<V>(ini));
				else if constexpr (Utils::is_same<V, UVector> && sin_arg)
					create(Utils::forward<V>(ini));
				else if constexpr (Utils::is_same<T, char>)
					create(String(Utils::forward<V>(ini), Utils::forward<L>(rest)...));
			}
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
