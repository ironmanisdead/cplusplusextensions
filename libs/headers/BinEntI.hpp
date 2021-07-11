#pragma once
#include "Macros.hpp"
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
			BinEntI& operator ++() noexcept;
			BinEntI& operator --() noexcept;
			BinEntI operator ++(int) noexcept;
			BinEntI operator --(int) noexcept;
			BinEntI next() noexcept;
			BinEntI prev() noexcept;
			BinEntC next() const noexcept;
			BinEntC prev() const noexcept;
			bool operator ==(const BinEntI&) const noexcept;
			bool operator ==(const BinEntC&) const noexcept;
			void erase() noexcept;
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
			BinEntC& operator ++() noexcept;
			BinEntC& operator --() noexcept;
			BinEntC operator ++(int) noexcept;
			BinEntC operator --(int) noexcept;
			BinEntC next() const noexcept;
			BinEntC prev() const noexcept;
			bool operator ==(const BinEntC&) const noexcept;
			bool operator ==(const BinEntI&) const noexcept;
			constexpr const BinEnt* operator ->() const noexcept {
				return current;
			}
			constexpr const BinEnt& operator *() const noexcept {
				return *current;
			}
			friend class BinEntI;
	};
}
