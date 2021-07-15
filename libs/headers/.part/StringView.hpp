#pragma once
#include "../Utils.hpp"
#include "../GString.hpp"
namespace CPPExtensions {
	class String;
	template <class>
		class Vector;
	class StringView;
	template <class T>
		void _viewput(T&, const StringView*);
	class StringView {
		public:
			Utils::size len;
		private:
			struct Window {
				bool modify;
				union {
					char* edit;
					const char* view;
				};
			} buffer;
			constexpr static auto viewer =
			[] (const Window& val) noexcept -> const char* {
				if (val.modify)
					return val.edit;
				else
					return val.view;
			};
		public:
			constexpr StringView(const StringView& val) noexcept : len(val.len),
				  buffer(false, {.view = {viewer(val.buffer)}}) {}
			constexpr StringView(Utils::nullpt) noexcept : len(0),
				  buffer(false, {nullptr}) {}
			constexpr StringView(StringView& val) noexcept : len(val.len),
				  buffer(val.buffer) {}
			constexpr StringView(const char* val) noexcept : len(GString::strlen(val)),
				  buffer(false, {.view = {val}}) {}
			constexpr StringView(const char* val, Utils::size ln) noexcept : len(ln),
				  buffer(false, {.view = {val}}) {}
			constexpr StringView(char* val) noexcept : len(GString::strlen(val)),
				  buffer(true, {.edit = {val}}) {}
			constexpr StringView(char* val, Utils::size ln) noexcept : len(ln),
				  buffer(true, {.edit = {val}}) {}
			constexpr StringView(const String& val) noexcept;
			constexpr const char* read() const noexcept { return viewer(buffer); }
			constexpr char* edit() noexcept {
				if (buffer.modify)
					return buffer.edit;
				else
					return nullptr;
			}
			constexpr StringView& operator =(const char* val) noexcept {
				len = GString::strlen(val);
				buffer.modify = false;
				buffer.view = val;
				return *this;
			}
			constexpr StringView& operator =(char* val) noexcept {
				len = GString::strlen(val);
				buffer.modify = true;
				buffer.edit = val;
				return *this;
			}
			constexpr StringView& operator =(const StringView& val) noexcept {
				len = val.len;
				buffer.modify = false;
				buffer.view = viewer(val.buffer);
				return *this;
			}
			constexpr StringView& operator =(StringView& val) noexcept {
				len = val.len;
				buffer = val.buffer;
				return *this;
			}
			constexpr StringView& operator =(const String& val) noexcept;
			constexpr void set(const char* val, Utils::size ln) noexcept {
				buffer.modify = false;
				buffer.view = val;
				len = ln;
			}
			constexpr void set(char* val, Utils::size ln) noexcept {
				buffer.modify = true;
				buffer.edit = val;
				len = ln;
			}
			constexpr Utils::strongcmp_t operator <=>(const StringView& val) const noexcept {
				const char* see = read();
				const char* valsee = val.read();
				if ((len == 0) || (val.len == 0))
					return (len <=> val.len);
				if (!(see && valsee))
					return (see <=> valsee);
				Utils::size shrt = len;
				if (val.len < shrt)
					shrt = val.len;
				for (Utils::size i = 0; i < shrt; i++) {
					char bufi = see[i], bufv = valsee[i];
					if (bufi != bufv)
						return (bufi <=> bufv);
				}
				return (len <=> val.len);
			}
			constexpr Utils::strongcmp_t operator <=>(const String& val) const noexcept;
	};
}
