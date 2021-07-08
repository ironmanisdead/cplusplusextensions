#pragma once
#include "../Utils.hpp"
#include "../GString.hpp"
namespace CustomUtils {
	class String;
	template <class>
		class Vector;
	class StringView;
	template <class T>
		void viewput(T&, const StringView*);
	class StringView {
		private:
			Utils::size len;
			const char* buffer;
		public:
			constexpr StringView(const char* val) noexcept : len(GString::strlen(val)), buffer(val) {}
			constexpr StringView(const char* val, Utils::size ln) noexcept : len(ln), buffer(val) {}
			constexpr StringView(const String& val) noexcept;
			StringView(String&&) = delete;
			constexpr Utils::size getlen() const noexcept {
				return len;
			}
			constexpr const char* data() const noexcept {
				if (len > 0)
					return buffer;
				else
					return nullptr;
			}
			constexpr Utils::strongcmp_t operator <=>(const StringView& val) const noexcept {
				if ((len == 0) || (val.len == 0))
					return (len <=> val.len);
				if (!(buffer && val.buffer))
					return (buffer <=> val.buffer);
				Utils::size shrt = len;
				if (val.len < shrt)
					shrt = val.len;
				for (Utils::size i = 0; i < shrt; i++) {
					char bufi = buffer[i], bufv = val.buffer[i];
					if (bufi != bufv)
						return (bufi <=> bufv);
				}
				return (len <=> val.len);
			}
			constexpr Utils::strongcmp_t operator <=>(const String& val) const noexcept;
			template <class T>
				friend void viewput(T&, const StringView*);
			friend class String;
			friend class Vector<char>;
	};
}
