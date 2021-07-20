#pragma once
#include "types.hpp"
#include "Array.hpp"
//#include <type_traits>
#include "GString.hpp"
#include <stdexcept>
DLL_HIDE
namespace CPPExtensions {
	template <class>
		class Vector;
	template <class T>
		DLL_PUBLIC void vecput(T&, const Vector<char>*);
	class UVector {
		public:
			enum State { NO_ERROR, MEM_ERROR, TYPE_ERROR };
			DLL_PUBLIC const char* id() noexcept;
			DLL_PUBLIC UVector(const UVector&) noexcept;
			DLL_PUBLIC UVector(UVector&&) noexcept;
			template <class T>
				UVector(const Vector<T>&);
			template <class T>
				UVector(Vector<T>&&) noexcept;
			DLL_PUBLIC ~UVector();
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
			DLL_PUBLIC UVector(const FullType*) noexcept;
			State _status;
			Utils::size_t trulen;
			Utils::size_t len;
			char* raw;
			DLL_PUBLIC [[nodiscard]] bool save(UVector&) noexcept;
			DLL_PUBLIC [[nodiscard]] bool save(UVector&, void (*)(void*, void*));
			DLL_PUBLIC [[nodiscard]] bool save(UVector&, void (*)(void*, void*) noexcept) noexcept;
			DLL_PUBLIC [[nodiscard]] bool copy(const UVector&);
			DLL_PUBLIC [[nodiscard]] bool copy(const UVector&, void (*)(void*, const void*));
			DLL_PUBLIC [[nodiscard]] bool copy(const UVector&, void (*)(void*, const void*) noexcept) noexcept;
			DLL_PUBLIC [[nodiscard]] bool _allocate(Utils::size_t) noexcept;
			DLL_PUBLIC void finalize() noexcept;
			DLL_PUBLIC void deinit() noexcept;
			DLL_PUBLIC [[nodiscard]] bool resize(Utils::size_t) noexcept;
			DLL_PUBLIC void place(Utils::size_t, const char*) noexcept;
			DLL_PUBLIC bool remove(Utils::size_t, Utils::size_t = 1) noexcept;
		public:
			DLL_PUBLIC UVector& operator =(const UVector&);
			constexpr Utils::size_t getlen() const noexcept {
				if (trulen > 0)
					return len;
				else
					return 0;
			}
			constexpr Utils::size_t fullen() const noexcept { return trulen; }
			constexpr State getStatus() const noexcept { return _status; }
			DLL_PUBLIC bool allocate(Utils::size_t) noexcept;
			template <class V>
				friend void vecput(V&, const Vector<char>*);
	};
}
DLL_RESTORE
