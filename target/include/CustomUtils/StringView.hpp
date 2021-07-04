#pragma once
#include "Utils.hpp"
#include "GString.hpp"
#include "String.hpp"
namespace CustomUtils {
	class StringView {
		private:
			Utils::size len;
			const char* buffer;
		public:
			constexpr StringView(const char* val) noexcept : len(GString::strlen(val)), buffer(val) {}
			constexpr StringView(const char* val, Utils::size ln) noexcept : len(ln), buffer(val) {
				if (buffer[ln - 1] == '\0')
					--len;
			}
			constexpr StringView(const String& val) noexcept : len(0), buffer(nullptr) {
				if (val.getlen() > 0) {
					len = val.getlen();
					buffer = val.data();
				}
			}
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
			constexpr Utils::strongcmp_t operator <=>(const String& val) const noexcept {
				return (*this <=> StringView(val.data(), val.getlen()));
			}
	};
}
