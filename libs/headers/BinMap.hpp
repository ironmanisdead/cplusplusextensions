#pragma once
#include "BinNode.hpp"
#include "BinNodeI.hpp"
#include "String.hpp"
namespace CustomUtils {
	template <class K, class V>
	class BinMap final : public BinTree, private Utils {
		private:
			static constexpr bool lessex =
				noexcept(declval<K>() < declval<K>());
			using Node = BinNode<K, V>;
			using NodeI = BinNodeI<K, V>;
			using NodeC = BinNodeC<K, V>;
			BinMap(BinEnt* val) noexcept : BinTree(val, &Node::info) {}
		public:
			BinMap() noexcept : BinTree(nullptr, &Node::info) {}
			BinMap(BinMap&& val) noexcept : BinTree(val.root, &Node::info) { val.root = nullptr; }
			BinMap(BinMap& val) : BinTree(nullptr, &Node::info) {
				if (val.root)
					root = downcast<Node*>(val.root)->clonemut(this);
			}
			BinMap(const BinMap& val) : BinTree(nullptr, &Node::info) {
				if (val.root)
					root = downcast<Node*>(val.root)->clone(this);
			}
			BinMap* clone() override {
				if constexpr (Node::keycop && Node::valbil) {
					if (root)
						return new BinMap(*this);
					else
						return new BinMap();
				}
				return nullptr;
			}
			BinMap* clone() const override {
				if constexpr (Node::keycop && Node::valcop) {
					if (root)
						return new BinMap(*this);
					else
						return new BinMap();
				}
				return nullptr;
			}
			template <class... L>
			Node* insert(const K& key, L&&... val) {
				char temp[sizeof(Node)];
				new (temp) Node(Node::cons, false, BinEnt::out { nullptr }, nullptr,
						nullptr, key, Utils::forward<L>(val)...);
				try {
					if (!_add(RECAST(Node*, temp))) {
						Node* result = downcast<Node*>(::operator new(sizeof(Node)));
						Utils::memcpy(result, temp, sizeof(Node));
						return result;
					}
					return nullptr;
				} catch (...) {
					RECAST(Node*, temp)->~Node();
					throw;
				}
			}
			Node* attach(BinEnt* val) noexcept(lessex) {
				return downcast<Node*>(BinTree::attach(val));
			}
			template <class X, class Y>
			Node* attach(BinNode<X, Y>* val) noexcept(lessex) {
				static_assert(is_same<K, X> && is_same<V, Y>, "type conversion is improper");
				return downcast<Node*>(BinTree::attach(val));
			}
			Node* detach(const K& key) noexcept(lessex) {
				return downcast<Node*>(BinTree::_detach(&key));
			}
			Node* getRoot() noexcept {
				return downcast<Node*>(root);
			}
			const Node* getRoot() const noexcept {
				return downcast<const Node*>(root);
			}
			NodeI begin() noexcept {
				BinEntI temp = BinTree::begin();
				return downcast<NodeI&>(temp);
			}
			NodeI max() noexcept {
				BinEntI temp = BinTree::max();
				return downcast<NodeI&>(temp);
			}
			NodeC begin() const noexcept {
				BinEntC temp = BinTree::begin();
				return downcast<const NodeC&>(temp);
			}
			NodeC max() const noexcept {
				BinEntC temp = BinTree::begin();
				return downcast<const NodeC&>(temp);
			}
			NodeC end() const noexcept {
				return nullptr;
			}
			V* find(const K& key) noexcept(lessex) {
				Find<BinEnt*> medi = _find(&key);
				Find<Node*> result = { medi.found, downcast<Node*>(medi.value) };
				if (result.value == nullptr)
					return nullptr;
				else if (result.found == 0)
					return addressof(result.value->getValue());
				else
					return nullptr;
			}
			const V* find(const K& key) const noexcept(lessex) {
				return const_cast<BinMap*>(this)->find(key);
			}
			Node* search(const K& key) noexcept(lessex) {
				return downcast<Node*>(_search(&key));
			}
			const Node* search(const K& key) const noexcept(lessex) {
				return downcast<const Node*>(_search(&key));
			}
		private:
			template <class, class>
				struct strlen_callable : lie {};
			template <class G>
				struct strlen_callable<none<
				decltype(GString::strlen(declval<G>()))>, G> {};
			template <class, class>
				struct strlen_noexcept_callable : lie {};
			template <class G>
				struct strlen_noexcept_callable<
				enable_if<noexcept(GString::strlen(declval<G>())), void>, G> : truth {};
		public:
			V& operator [](const K& key) {
				V* result = find(key);
				if (result)
					return *result;
				else if constexpr (strlen_noexcept_callable<void, const K&>::value)
					RangeError(String("Error: binary tree doesn't contain such key: ", key));
				else
					RangeError("Error: binary tree doesn't contain such key");
			}
			const V& operator [](const K& key) const {
				return const_cast<BinMap&>(*this)[key];
			}
			virtual ~BinMap() override {
				if (root) {
					root->deleter();
					delete root;
				}
			}
	};
	template <class K, class V>
		BinMap(BinMap<K, V>&&) -> BinMap<K, V>;
	template <class K, class V>
		BinMap(BinMap<K, V>&) -> BinMap<K, V>;
	template <class K, class V>
		BinMap(const BinMap<K, V>&) -> BinMap<K, V>;
}
