#pragma once
#include ".hidden/StringView-part.hpp"
#include "String.hpp"
namespace CustomUtils {
	constexpr StringView::StringView(const String& val) noexcept : len(0), buffer(nullptr) {
		if (val.getlen() > 0) {
			len = val.getlen();
			buffer = val.data();
		}
	}
	constexpr Utils::strongcmp_t StringView::operator <=>(const String& val) const noexcept {
		return (*this <=> StringView(val.data(), val.getlen()));
	}
}
