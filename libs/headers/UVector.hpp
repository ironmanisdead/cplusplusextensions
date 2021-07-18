#pragma once
#include "types.hpp"
#include "Array.hpp"
//#include <type_traits>
#include "GString.hpp"
#include <stdexcept>
namespace CPPExtensions {
	template <class>
		class Vector;
	template <class T>
		void vecput(T&, const Vector<char>*);
	class UVector {
		public:
			enum State { NO_ERROR, MEM_ERROR, TYPE_ERROR };
			const char* id() noexcept;
			UVector(const UVector&) noexcept;
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
				Utils::size_t elem;
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
			State _status;
			Utils::size_t trulen;
			Utils::size_t len;
			char* raw;
			[[nodiscard]] bool save(UVector&) noexcept;
			[[nodiscard]] bool save(UVector&, void (*)(void*, void*));
			[[nodiscard]] bool save(UVector&, void (*)(void*, void*) noexcept) noexcept;
			[[nodiscard]] bool copy(const UVector&);
			[[nodiscard]] bool copy(const UVector&, void (*)(void*, const void*));
			[[nodiscard]] bool copy(const UVector&, void (*)(void*, const void*) noexcept) noexcept;
			[[nodiscard]] bool _allocate(Utils::size_t) noexcept;
			void finalize() noexcept;
			void deinit() noexcept;
			[[nodiscard]] bool resize(Utils::size_t) noexcept;
			void place(Utils::size_t, const char*) noexcept;
			bool remove(Utils::size_t, Utils::size_t = 1) noexcept;
		public:
			UVector& operator =(const UVector&);
			constexpr Utils::size_t getlen() const noexcept {
				if (trulen > 0)
					return len;
				else
					return 0;
			}
			constexpr Utils::size_t fullen() const noexcept { return trulen; }
			constexpr State getStatus() const noexcept { return _status; }
			bool allocate(Utils::size_t) noexcept;
			template <class V>
				friend void vecput(V&, const Vector<char>*);
	};
}
