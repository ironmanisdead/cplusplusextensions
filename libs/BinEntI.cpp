#include "headers/BinEntI.hpp"
#include "headers/BinTree.hpp"
DLL_HIDE
namespace CPPExtensions {
	DLL_PUBLIC void BinEntI::erase() noexcept {
		if (!current)
			return;
		current->detach();
		delete current;
		current = nullptr;
	}
	DLL_PUBLIC BinEntI& BinEntI::operator ++() noexcept {
		if (current)
			current = current->_next();
		return *this;
	}
	DLL_PUBLIC BinEntI& BinEntI::operator --() noexcept {
		if (current)
			current = current->_prev();
		return *this;
	}
	DLL_PUBLIC BinEntI BinEntI::operator ++(int) noexcept {
		BinEnt* prev = current;
		BinEntI::operator++();
		return prev;
	}
	DLL_PUBLIC BinEntI BinEntI::operator --(int) noexcept {
		BinEnt* prev = current;
		BinEntI::operator--();
		return prev;
	}
	DLL_PUBLIC BinEntC& BinEntC::operator ++() noexcept {
		if (current)
			current = current->_next();
		return *this;
	}
	DLL_PUBLIC BinEntC& BinEntC::operator --() noexcept {
		if (current)
			current = current->_prev();
		return *this;
	}
	DLL_PUBLIC BinEntC BinEntC::operator ++(int) noexcept {
		const BinEnt* prev = current;
		BinEntC::operator++();
		return prev;
	}
	DLL_PUBLIC BinEntC BinEntC::operator --(int) noexcept {
		const BinEnt* prev = current;
		BinEntC::operator--();
		return prev;
	}
	DLL_PUBLIC BinEntI BinEntI::next() noexcept {
		if (current)
			return current->_next();
		else
			return nullptr;
	}
	DLL_PUBLIC BinEntI BinEntI::prev() noexcept {
		if (current)
			return current->_prev();
		else
			return nullptr;
	}
	DLL_PUBLIC BinEntC BinEntI::next() const noexcept {
		if (current)
			return current->_next();
		else
			return nullptr;
	}
	DLL_PUBLIC BinEntC BinEntI::prev() const noexcept {
		if (current)
			return current->_prev();
		else
			return nullptr;
	}
	DLL_PUBLIC BinEntC BinEntC::next() const noexcept {
		if (current)
			return current->_next();
		else
			return nullptr;
	}
	DLL_PUBLIC BinEntC BinEntC::prev() const noexcept {
		if (current)
			return current->_prev();
		else
			return nullptr;
	}
	DLL_PUBLIC bool BinEntI::operator ==(const BinEntI& val) const noexcept {
		return current == val.current;
	}
	DLL_PUBLIC bool BinEntI::operator ==(const BinEntC& val) const noexcept {
		return current == val.current;
	}
	DLL_PUBLIC bool BinEntC::operator ==(const BinEntI& val) const noexcept {
		return current == val.current;
	}
	DLL_PUBLIC bool BinEntC::operator ==(const BinEntC& val) const noexcept {
		return current == val.current;
	}
}
DLL_RESTORE
