#pragma once
#include "Macros.hpp"
DLL_HIDE
namespace CPPExtensions {
	class BinEnt;
	class BinTree;
	class BinEntC;
	class BinEntI {
		protected:
			BinEnt* current;
		public:
			constexpr BinEntI() noexcept : current(nullptr) {}
			constexpr BinEntI(BinEnt* val) noexcept : current(val) {}
			BinEntI(BinEntI&) = default;
			BinEntI(BinEntI&&) = default;
			constexpr BinEntI& operator =(BinEntI& val) noexcept {
				current = val.current;
				return *this;
			}
			constexpr BinEntI& operator =(BinEntI&& val) noexcept {
				current = val.current;
				return *this;
			}
			constexpr operator bool() const noexcept {
				return current ? true : false;
			}
			DLL_PUBLIC BinEntI& operator ++() noexcept;
			DLL_PUBLIC BinEntI& operator --() noexcept;
			DLL_PUBLIC BinEntI operator ++(int) noexcept;
			DLL_PUBLIC BinEntI operator --(int) noexcept;
			DLL_PUBLIC BinEntI next() noexcept;
			DLL_PUBLIC BinEntI prev() noexcept;
			DLL_PUBLIC BinEntC next() const noexcept;
			DLL_PUBLIC BinEntC prev() const noexcept;
			DLL_PUBLIC bool operator ==(const BinEntI&) const noexcept;
			DLL_PUBLIC bool operator ==(const BinEntC&) const noexcept;
			DLL_PUBLIC void erase() noexcept;
			constexpr BinEnt* operator ->() noexcept {
				return current;
			}
			constexpr const BinEnt* operator ->() const noexcept {
				return current;
			}
			constexpr BinEnt& operator *() noexcept {
				return *current;
			}
			constexpr const BinEnt& operator *() const noexcept {
				return *current;
			}
			friend class BinEntC;
	};
	class BinEntC {
		protected:
			const BinEnt* current;
		public:
			constexpr BinEntC() noexcept : current(nullptr) {}
			constexpr BinEntC(const BinEnt* val) noexcept : current(val) {}
			constexpr BinEntC(const BinEntI& val) noexcept : current(val.current) {}
			BinEntC(const BinEntC&) = default;
			constexpr BinEntC& operator =(const BinEntC& val) noexcept {
				current = val.current;
				return *this;
			}
			constexpr operator bool() const noexcept {
				return current ? true : false;
			}
			DLL_PUBLIC BinEntC& operator ++() noexcept;
			DLL_PUBLIC BinEntC& operator --() noexcept;
			DLL_PUBLIC BinEntC operator ++(int) noexcept;
			DLL_PUBLIC BinEntC operator --(int) noexcept;
			DLL_PUBLIC BinEntC next() const noexcept;
			DLL_PUBLIC BinEntC prev() const noexcept;
			DLL_PUBLIC bool operator ==(const BinEntC&) const noexcept;
			DLL_PUBLIC bool operator ==(const BinEntI&) const noexcept;
			constexpr const BinEnt* operator ->() const noexcept {
				return current;
			}
			constexpr const BinEnt& operator *() const noexcept {
				return *current;
			}
			friend class BinEntI;
	};
}
DLL_RESTORE
