#pragma once
#include "BinEntI.hpp"
namespace CustomUtils {
	template <class K, class V>
		class BinNodeI;
	template <class K, class V>
	class BinNodeC : public BinEntC {
		private:
			using Node = BinNode<K, V>;
		public:
			constexpr BinNodeC() noexcept : BinEntC() {}
			constexpr BinNodeC(const Node* val) noexcept : BinEntC(val) {}
			constexpr BinNodeC(const BinNodeC& val) : BinEntC(val) {}
			constexpr BinNodeC(const BinNodeI<K, V>& val);
			constexpr BinNodeC& operator =(const BinNodeC& val) noexcept {
				current = val.current;
				return *this;
			}
			BinNodeC& operator ++() noexcept {
				BinEntC::operator++();
				return *this;
			}
			BinNodeC& operator --() noexcept {
				BinEntC::operator--();
				return *this;
			}
			BinNodeC& operator ++(int) noexcept {
				const Node* prev = Utils::downcast<const Node*>(current);
				BinEntC::operator++();
				return prev;
			}
			BinNodeC& operator --(int) noexcept {
				const Node* prev = Utils::downcast<const Node*>(current);
				BinEntC::operator--();
				return prev;
			}
			BinNodeC next() const noexcept {
				if (current)
					return Utils::downcast<const Node*>(current->_next());
				return nullptr;
			}
			BinNodeC prev() const noexcept {
				if (current)
					return Utils::downcast<const Node*>(current->_prev());
				return nullptr;
			}
			const Node* operator ->() const noexcept {
				return Utils::downcast<const Node*>(current);
			}
			const Node& operator *() const noexcept {
				return *Utils::downcast<const Node*>(current);
			}
	};
	template <class K, class V>
		BinNodeC(const BinNodeC<K, V>&) -> BinNodeC<K, V>;
	template <class K, class V>
		BinNodeC(const BinNodeI<K, V>&) -> BinNodeC<K, V>;
	template <class K, class V>
		BinNodeC(const BinNode<K, V>*) -> BinNodeC<K, V>;
	template <class K, class V>
	class BinNodeI : public BinEntI {
		private:
			using Node = BinNode<K, V>;
		public:
			constexpr BinNodeI() noexcept : BinEntI() {}
			constexpr BinNodeI(Node* val) noexcept : BinEntI(val) {}
			constexpr BinNodeI(BinNodeI& val) noexcept : BinEntI(val) {}
			constexpr BinNodeI(BinNodeI&& val) noexcept : BinEntI(val) {}
			constexpr BinNodeI& operator =(BinNodeI& val) noexcept {
				current = val.current;
				return *this;
			}
			constexpr BinNodeI& operator =(BinNodeI&& val) noexcept {
				current = val.current;
				return *this;
			}
			BinNodeI& operator ++() noexcept {
				BinEntI::operator++();
				return *this;
			}
			BinNodeI& operator --() noexcept {
				BinEntI::operator--();
				return *this;
			}
			BinNodeI operator ++(int) noexcept {
				Node* prev = Utils::downcast<Node*>(current);
				BinEntI::operator++();
				return prev;
			}
			BinNodeI operator --(int) noexcept {
				Node* prev = Utils::downcast<Node*>(current);
				BinEntI::operator--();
				return prev;
			}
			BinNodeI next() noexcept {
				if (current)
					return Utils::downcast<Node*>(current->_next());
				return nullptr;
			}
			BinNodeI prev() noexcept {
				if (current)
					return Utils::downcast<Node*>(current->_prev());
				return nullptr;
			}
			BinNodeC<K, V> next() const noexcept {
				if (current)
					return Utils::downcast<const Node*>(current->_next());
				return nullptr;
			}
			BinNodeC<K, V> prev() const noexcept {
				if (current)
					return Utils::downcast<const Node*>(current->_prev());
				return nullptr;
			}
			Node* operator ->() noexcept {
				return Utils::downcast<Node*>(current);
			}
			const Node* operator ->() const noexcept {
				return Utils::downcast<const Node*>(current);
			}
			Node& operator *() noexcept {
				return *Utils::downcast<Node*>(current);
			}
			const Node& operator *() const noexcept {
				return *Utils::downcast<const Node*>(current);
			}
	};
	template <class K, class V>
		constexpr BinNodeC<K, V>::BinNodeC(const BinNodeI<K, V>& val) : BinEntC(val) {}
	template <class K, class V>
		BinNodeI(BinNodeI<K, V>&) -> BinNodeI<K, V>;
	template <class K, class V>
		BinNodeI(BinNodeI<K, V>&&) -> BinNodeI<K, V>;
	template <class K, class V>
		BinNodeI(BinNode<K, V>*) -> BinNodeI<K, V>;
}
