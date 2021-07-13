#pragma once
#include ".part/StringView.hpp"
#include "String.hpp"
namespace CPPExtensions {
	constexpr StringView::StringView(const String& val) noexcept : len(0),
	 buffer(false, {nullptr}) {
		if (val.getlen() > 0) {
			len = val.getlen();
			buffer.view = val.data();
		}
	}
	constexpr StringView& StringView::operator =(const String& val) noexcept {
		buffer.modify = false;
		len = val.getlen();
		buffer.view = val.data();
		return *this;
	}
	constexpr Utils::strongcmp_t StringView::operator <=>(const String& val) const noexcept {
		return (*this <=> StringView(val.data(), val.getlen()));
	}
}
