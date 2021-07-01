#include "headers/BinEntI.hpp"
#include "headers/BinTree.hpp"
namespace CustomUtils {
	void BinEntI::erase() noexcept {
		if (!current)
			return;
		current->detach();
		delete current;
		current = nullptr;
	}
	BinEntI BinTree::_search(const void* key) {
		Find<BinEnt*> result = _find(key);
		if (result.found == 0)
			return result.value;
		else
			return nullptr;
	}
	BinEntI& BinEntI::operator ++() noexcept {
		if (current)
			current = current->_next();
		return *this;
	}
	BinEntI& BinEntI::operator --() noexcept {
		if (current)
			current = current->_prev();
		return *this;
	}
	BinEntI BinEntI::operator ++(int) noexcept {
		BinEnt* prev = current;
		BinEntI::operator++();
		return prev;
	}
	BinEntI BinEntI::operator --(int) noexcept {
		BinEnt* prev = current;
		BinEntI::operator--();
		return prev;
	}
	BinEntC& BinEntC::operator ++() noexcept {
		if (current)
			current = current->_next();
		return *this;
	}
	BinEntC& BinEntC::operator --() noexcept {
		if (current)
			current = current->_prev();
		return *this;
	}
	BinEntC BinEntC::operator ++(int) noexcept {
		const BinEnt* prev = current;
		BinEntC::operator++();
		return prev;
	}
	BinEntC BinEntC::operator --(int) noexcept {
		const BinEnt* prev = current;
		BinEntC::operator--();
		return prev;
	}
	BinEntI BinEntI::next() noexcept {
		if (current)
			return current->_next();
		else
			return nullptr;
	}
	BinEntI BinEntI::prev() noexcept {
		if (current)
			return current->_prev();
		else
			return nullptr;
	}
	BinEntC BinEntI::next() const noexcept {
		if (current)
			return current->_next();
		else
			return nullptr;
	}
	BinEntC BinEntI::prev() const noexcept {
		if (current)
			return current->_prev();
		else
			return nullptr;
	}
	BinEntC BinEntC::next() const noexcept {
		if (current)
			return current->_next();
		else
			return nullptr;
	}
	BinEntC BinEntC::prev() const noexcept {
		if (current)
			return current->_prev();
		else
			return nullptr;
	}
	bool BinEntI::operator ==(const BinEntI& val) const noexcept {
		return current == val.current;
	}
	bool BinEntI::operator ==(const BinEntC& val) const noexcept {
		return current == val.current;
	}
	bool BinEntC::operator ==(const BinEntI& val) const noexcept {
		return current == val.current;
	}
	bool BinEntC::operator ==(const BinEntC& val) const noexcept {
		return current == val.current;
	}
}
