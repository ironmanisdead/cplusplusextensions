#pragma once
#include "Utils.hpp"
#include "Array.hpp"
#include <type_traits>
#include "GString.hpp"
#include <stdexcept>
namespace CPPExtensions {
	template <class>
		class Vector;
	template <class T>
		void vecput(T&, const Vector<char>*);
	class UVector {
		public:
			const char* id() noexcept;
			UVector(const UVector&);
			UVector(UVector&&) noexcept;
			template <class T>
				UVector(const Vector<T>&);
			template <class T>
				UVector(Vector<T>&&) noexcept;
			~UVector();
		protected:
			struct TypeText {
				const char* id;
				const char* vecid;
				const char* copy;
				const char* conv;
				const char* assign;
			};
			struct TypeData {
				Utils::size elem;
				void (*deleter)(void*) noexcept;
				void (*copier)(void*, const void*);
				void (*pheonix)(void*, void*);
				void (*assigner)(void*, const void*);
				void (*possess)(void*, void*);
			};
			struct FullType {
				const TypeData* data;
				const TypeText* text;
			};
			const FullType* const typeinfo;
			UVector(const FullType*) noexcept;
			[[noreturn]] void copy_error();
			Utils::size trulen;
			Utils::size len;
			char* raw;
			void save(UVector&) noexcept;
			void save(UVector&, void (*)(void*, void*));
			void copy(const UVector&);
			void copy(const UVector&, void (*)(void*, const void*));
			void allocate(Utils::size);
			void finalize() noexcept;
			void deinit() noexcept;
			void resize(Utils::size);
			void place(Utils::size, const char*) noexcept;
			void remove(Utils::size, Utils::size = 1);
		public:
			UVector& operator =(const UVector&);
			inline Utils::size getlen() const noexcept {
				if (trulen > 0)
					return len;
				else
					return 0;
			}
			inline Utils::size fullen() const noexcept { return trulen; }
			void reallocate(Utils::size);
			template <class V>
				friend void vecput(V&, const Vector<char>*);
	};
}
