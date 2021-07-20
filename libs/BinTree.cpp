#include "headers/BinTree.hpp"
#include "headers/BinNode.hpp"
#include "headers/Utils.hpp"
#include <new>
DLL_HIDE
namespace CPPExtensions {
	DLL_PUBLIC void BinEnt::_rebalance() noexcept {}
	DLL_PUBLIC bool BinEnt::rotate(bool direction) noexcept {
		if (!isattach())
			return false; //cannot rotate node that isn't on a tree
		if (direction) { //right rotation
			BinEnt* A = left;
			if (!A)
				return false; //cannot rotate left if nothing's right
			A->setParentFrom(this);
			unParent(A);
			setParent(A);
			BinEnt* T2 = A->right;
			A->right = this;
			left = T2;
			if (T2)
				T2->setParent(this);
		} else { //left rotation
			BinEnt* B = right;
			if (!B)
				return false; //cannot rotate right if nothing's left
			B->setParentFrom(this);
			unParent(B);
			setParent(B);
			BinEnt* T2 = B->left;
			B->left = this;
			right = T2;
			if (T2)
				T2->setParent(this);
		}
		return true;
	}
	DLL_PUBLIC BinEnt* BinTree::_detach(const void* key) noexcept {
		Find<BinEnt*> result = _find(key);
		if (result.found == 0) {
			result.value->detach();
			return result.value;
		}
		return nullptr;
	}
	DLL_PUBLIC BinEnt* BinEnt::_root() noexcept {
		for (BinEnt* capture = this; capture; capture = capture->uParent()) {
			if (capture->root) {
				if (capture->uTree())
					return capture;
				else
					break;
			}
		}
		return nullptr;
	}
	DLL_PUBLIC BinEnt* BinTree::_search(const void* key) {
		Find<BinEnt*> result = _find(key);
		if (result.found == 0)
			return result.value;
		else
			return nullptr;
	}
	DLL_LOCAL void BinEnt::setParentFrom(BinEnt* val) noexcept {
		root = val->root;
		if (root)
			uTree() = val->uTree();
		else
			uParent() = val->uParent();
	}
	DLL_LOCAL bool BinEnt::unParent(BinEnt* val) noexcept {
		if (root) {
			if (uTree())
				uTree()->root = val;
			return true;
		}
		BinEnt* parent = uParent();
		if (parent) {
			if (parent->left == this)
				parent->left = val;
			else
				parent->right = val;
		}
		return false;
	}
	DLL_LOCAL void BinEnt::disownSelf(BinEnt* val) noexcept {
		if (unParent(val))
			uTree() = nullptr;
		else
			uParent() = nullptr;
	}
	DLL_PUBLIC bool BinEnt::detach() noexcept {
		if (!isattach())
			return false;
		if (left && right) { //node has both children
			BinEnt* next = right->_min();
			next->left = left;
			if (left)
				left->setParent(next);
			BinEnt* t1 = next->uParent();
			if (t1 != this) {
				t1->left = next->right;
				if (next->right)
					next->right->setParent(t1);
				next->right = right;
				if (right)
					right->setParent(next);
			}
			next->setParentFrom(this);
			disownSelf(next);
			next->_rebalance();
		} else if (left) { //node has left child
			left->setParentFrom(this);
			disownSelf(left);
			left->_rebalance();
		} else if (right) { //node has right child
			right->setParentFrom(this);
			disownSelf(right);
			right->_rebalance();
		} else { //node is a leaf
			disownSelf(nullptr);
			if (!root)
				uParent()->_rebalance();
		}
		return true;
	}
	DLL_PUBLIC BinEnt* BinEnt::_min() noexcept {
		BinEnt* current = this;
		while (current->left)
			current = current->left;
		return current;
	}
	DLL_PUBLIC BinEnt* BinEnt::_max() noexcept {
		BinEnt* current = this;
		while (current->right)
			current = current->right;
		return current;
	}
	DLL_PUBLIC BinEnt* BinEnt::_next() noexcept {
		if (right)
			return right->_min();
		BinEnt* current = this;
		while (true) {
			BinEnt* parent = current->_parent();
			if (!parent)
				return nullptr;
			if (parent->left == current)
				return parent;
			current = parent;
		}
	}
	DLL_PUBLIC BinEnt* BinEnt::_prev() noexcept {
		if (left)
			return left->_max();
		BinEnt* current = this;
		while (true) {
			BinEnt* parent = current->_parent();
			if (!parent)
				return nullptr;
			if (parent->right == current)
				return parent;
			current = parent;
		}
	}
	DLL_PUBLIC void BinEnt::deleter() noexcept {
		if (left) {
			left->deleter();
			delete left;
		}
		if (right) {
			right->deleter();
			delete right;
		}
	}
	DLL_PUBLIC BinEntI BinTree::begin() noexcept {
		if (root)
			return root->_min();
		else
			return nullptr;
	}
	DLL_PUBLIC BinEntI BinTree::max() noexcept {
		if (root)
			return root->_max();
		else
			return nullptr;
	}
	DLL_PUBLIC BinEntC BinTree::begin() const noexcept {
		if (root)
			return root->_min();
		else
			return nullptr;
	}
	DLL_PUBLIC BinEntC BinTree::max() const noexcept {
		if (root)
			return root->_max();
		else
			return nullptr;
	}
	DLL_LOCAL void BinEnt::reSelf(BinEnt* val) noexcept {
		if ((root = val->root))
			uTree() = val->uTree();
		else
			uParent() = val->uParent();
		left = val->left;
		right = val->right;
	}
	DLL_PUBLIC BinEnt* BinTree::_place(BinEnt* dest, BinEnt* src, stronk dir, bool pop) noexcept {
		if (dir < 0) {
			src->setParent(dest);
			dest->_left() = src;
		} else if (dir > 0) {
			src->setParent(dest);
			dest->_right() = src;
		} else if (pop) {
			src->reSelf(dest);
			dest->disownSelf(src);
			dest->setParent(nullptr);
			return dest;
		} else {
			src->reSelf(dest);
			src->_left() = dest->_left();
			src->_right() = dest->_right();
			dest->~BinEnt();
			Utils::memcpy(dest, src, src->data->total);
			return nullptr;
		}
		dest->_rebalance();
		return nullptr;
	}
	DLL_PUBLIC BinEnt* BinTree::_attach(BinEnt* ins) {
		if (ins->isattach())
			return ins;
		const void* key = ins->_key();
		if (!root) {
			root = ins;
			ins->setParent(this);
			return nullptr;
		}
		Find<BinEnt*> result = _find(key);
		return _place(result.value, ins, result.found, true);
	}
	DLL_PUBLIC BinEnt* BinTree::attach(BinEnt* ins) {
		if (ins->data != data)
			return ins;
		return _attach(ins);
	}
	DLL_PUBLIC BinTree::BinStat BinTree::_add(BinEnt* ins, bool force) {
		const void* key = ins->_key();
		Utils::size_t siz = ins->data->total;
		if (!root) {
			root = Utils::downcast<BinEnt*>(::operator new(siz, std::nothrow_t {}));
			if (!root)
				return { false, nullptr };
			ins->setParent(this);
			Utils::memcpy(root, ins, siz);
			return { true, root };
		}
		Find<BinEnt*> result = _find(key);
		if (result.found == 0) {
			if (force)
				_place(result.value, ins, 0 <=> 0);
			return { false, result.value };
		}
		BinEnt* store = Utils::downcast<BinEnt*>(::operator new(siz, std::nothrow_t {}));
		if (!store)
			return { false, nullptr };
		Utils::memcpy(store, ins, siz);
		_place(result.value, store, result.found);
		return { true, store };
	}
	DLL_PUBLIC Find<BinEnt*> BinTree::_find(const void* key) {
		if (!root)
			return { 0 <=> 1, nullptr };
		BinEnt* current = root;
		while (true) {
			if (current->greater(key)) {
				BinEnt* left = current->_left();
				if (left)
					current = left;
				else
					return { 0 <=> 1, current };
			} else if (current->less(key)) {
				BinEnt* right = current->_right();
				if (right)
					current = right;
				else
					return { 1 <=> 0, current };
			} else
				return { 0 <=> 0, current };
		}
	}
	DLL_PUBLIC BinEnt* BinEnt::clone(BinEnt* up) const {
		BinEnt* copy = _copy();
		if (copy) {
			copy->uParent() = up;
			if (left)
				copy->left = left->clone(copy);
			if (right)
				copy->right = right->clone(copy);
		}
		return copy;
	}
	DLL_PUBLIC BinEnt* BinEnt::clone(BinTree* top) const {
		BinEnt* copy = _copy();
		if (copy) {
			try {
				copy->setParent(top);
				if (left)
					copy->left = left->clone(copy);
				if (right)
					copy->right = right->clone(copy);
			} catch (...) {
				copy->deleter();
				delete copy;
				throw;
			}
		}
		return copy;
	}
	DLL_PUBLIC BinEnt* BinEnt::clonemut(BinEnt* up) {
		BinEnt* copy = _build();
		if (copy) {
			copy->uParent() = up;
			if (left)
				copy->left = left->clonemut(copy);
			if (right)
				copy->right = right->clonemut(copy);
		}
		return copy;
	}
	DLL_PUBLIC BinEnt* BinEnt::clonemut(BinTree* top) {
		BinEnt* copy = _build();
		if (copy) {
			copy->setParent(top);
			try {
				if (left)
					copy->left = left->clonemut(copy);
				if (right)
					copy->right = right->clonemut(copy);
			} catch (...) {
				copy->deleter();
				delete copy;
				throw;
			}
		}
		return copy;
	}
}
DLL_RESTORE
