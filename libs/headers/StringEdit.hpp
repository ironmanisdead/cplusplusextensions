#pragma once
#include "Utils.hpp"
#include "GString.hpp"
namespace CustomUtils {
	class String;
	template <class>
		class Vector;
	class StringEdit;
	template <class T>
		void viewput(T&, const StringEdit*);
	class StringEdit {
		private:
			Utils::size len;
			char* buffer;
		public:
			constexpr StringEdit(char* val) noexcept : len(GString::strlen(val)), buffer(val) {}
			constexpr StringEdit(char* val, Utils::size ln) noexcept : len(ln), buffer(val) {}
			constexpr StringEdit(String& val) noexcept;
			StringEdit(String&&) = delete;
			constexpr Utils::size getlen() const noexcept {
				return len;
			}
			constexpr const char* data() const noexcept {
				if (len > 0)
					return buffer;
				else
					return nullptr;
			}
			constexpr char* data() noexcept {
				if (len > 0)
					return buffer;
				else
					return nullptr;
			}
			constexpr Utils::strongcmp_t operator <=>(const StringEdit& val) const noexcept {
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
			template <class T>
				friend void viewput(T&, const StringEdit*);
			friend class String;
			friend class Vector<char>;
	};
}
