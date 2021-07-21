#pragma once
#include "../predefs.hpp"
DLL_HIDE
namespace CPPExtensions {
	class BinEnt;
	class BinPtrI;
	class BinPtrC;
	class BinEntV {
		protected:
			struct Window {
				bool modify;
				union {
					const BinEnt* view;
					BinEnt* edit;
				};
			} buffer;
			constexpr auto viewer =
			[] (const Window& val) -> const BinEnt* {
				if (val.modify)
					return val.edit;
				else
					return val.view;
			};
			constexpr BinEntV(Window& val) noexcept : buffer(val) {}
			constexpr BinEntV(const Window& val) noexcept : buffer(val) {}
		public:
			constexpr BinEntV() noexcept : buffer(false, { .view = {nullptr} }) {}
			constexpr BinEntV(BinEnt* val) noexcept : buffer(true, { .edit = {val} }) {}
			constexpr BinEntV(const BinEnt* val) noexcept : buffer(false, { .view = {val} }) {}
			constexpr BinEntV(BinEntV& val) noexcept : buffer(val.buffer) {}
			constexpr BinEntV(const BinEntV& val) noexcept :
				buffer(false, { .view = { viewer(val.buffer) } }) {}
			BinEntV(BinEntV&& val) noexcept : buffer(val.buffer) {}
			constexpr BinEntV& operator =(BinEntV& val) noexcept {
				current = val.current;
				return *this;
			}
			constexpr BinEntV& operator =(BinEntV&& val) noexcept {
				current = val.current;
				return *this;
			}
			constexpr operator bool() const noexcept {
				return current ? true : false;
			}
			constexpr BinEntV& operator ++() noexcept {
			}
			constexpr BinEntV& operator --() noexcept;
			constexpr BinEntV operator ++(int) noexcept;
			constexpr BinEntV operator --(int) noexcept;
			constexpr BinEntV next() noexcept;
			constexpr BinEntV prev() noexcept;
			constexpr BinEntV next() const noexcept;
			constexpr BinEntV prev() const noexcept;
			constexpr BinPtrI operator *() noexcept;
			constexpr BinPtrC operator *() const noexcept;
			constexpr bool operator ==(const BinEntV& val) const noexcept {
				return viewer(buffer) == viewer(val.buffer);
			}
			DLL_PUBLIC void erase() noexcept;
	};
	class BinPtrI {
		private:
			struct Window {
				bool modify;
				union {
					const BinEnt* view;
					BinEnt* edit;
				};
			} buffer;
		public:
			constexpr const void* _key() const noexcept;
			constexpr void* _value() noexcept;
	};
}
DLL_RESTORE
