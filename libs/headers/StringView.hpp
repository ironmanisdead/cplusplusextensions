#pragma once
#include ".part/StringView.hpp"
#include "String.hpp"
DLL_HIDE
namespace CPPExtensions {
	constexpr void StringView::byval(const String& val) noexcept {
		if (val.getlen() > 0) {
			len = val.getlen();
			buffer.view = val.data();
		}
	}
	constexpr Utils::strongcmp_t StringView::operator <=>(const String& val) const noexcept {
		return (*this <=> StringView(val.data(), val.getlen()));
	}
}
DLL_RESTORE
