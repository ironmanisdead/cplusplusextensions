#pragma once
#include "BinNode.hpp"
#include "BinNodeI.hpp"
#include "String.hpp"
DLL_HIDE
namespace CPPExtensions {
	template <class K, class V>
	class BinMap final : public BinTree {
		private:
			static constexpr bool lessex =
				noexcept(Utils::declval<K>() < Utils::declval<K>());
			using Node = BinNode<K, V>;
			using NodeI = BinNodeI<K, V>;
			using NodeC = BinNodeC<K, V>;
			BinMap(BinEnt* val) noexcept : BinTree(val, &Node::info) {}
		public:
			struct MapStat {
				bool status;
				Node* value;
			};
			BinMap() noexcept : BinTree(nullptr, &Node::info) {}
			BinMap(BinMap&& val) noexcept : BinTree(val.root, &Node::info) { val.root = nullptr; }
			BinMap(BinMap& val) : BinTree(nullptr, &Node::info) {
				if (val.root)
					root = Utils::downcast<Node*>(val.root)->clonemut(this);
			}
			BinMap(const BinMap& val) : BinTree(nullptr, &Node::info) {
				if (val.root)
					root = Utils::downcast<Node*>(val.root)->clone(this);
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
			MapStat insert(const K& key, L&&... val) {
				char temp[sizeof(Node)];
				new (temp) Node(Node::cons, false, BinEnt::out { nullptr }, nullptr,
						nullptr, key, Utils::forward<L>(val)...);
				try {
					BinStat test = _add(RECAST(Node*, temp));
					if (!test.value) {
						Node* result = Utils::downcast<Node*>(::operator new(sizeof(Node)));
						Utils::memcpy(result, temp, sizeof(Node));
						return { true, result };
					}
					return { false, Utils::downcast<Node*>(test.value) };
				} catch (...) {
					RECAST(Node*, temp)->~Node();
					throw;
				}
			}
			Node* attach(BinEnt* val) noexcept(lessex) {
				return Utils::downcast<Node*>(BinTree::attach(val));
			}
			template <class X, class Y>
			Node* attach(BinNode<X, Y>* val) noexcept(lessex) {
				static_assert(Utils::is_same<K, X> && Utils::is_same<V, Y>,
						"type conversion is improper");
				return Utils::downcast<Node*>(BinTree::attach(val));
			}
			Node* detach(const K& key) noexcept(lessex) {
				return Utils::downcast<Node*>(BinTree::_detach(&key));
			}
			Node* getRoot() noexcept {
				return Utils::downcast<Node*>(root);
			}
			const Node* getRoot() const noexcept {
				return Utils::downcast<const Node*>(root);
			}
			NodeI begin() noexcept {
				BinEntI temp = BinTree::begin();
				return Utils::downcast<NodeI&>(temp);
			}
			NodeI max() noexcept {
				BinEntI temp = BinTree::max();
				return Utils::downcast<NodeI&>(temp);
			}
			NodeC begin() const noexcept {
				BinEntC temp = BinTree::begin();
				return Utils::downcast<const NodeC&>(temp);
			}
			NodeC max() const noexcept {
				BinEntC temp = BinTree::begin();
				return Utils::downcast<const NodeC&>(temp);
			}
			NodeC end() const noexcept {
				return nullptr;
			}
			V* find(const K& key) noexcept(lessex) {
				Find<BinEnt*> medi = _find(&key);
				Find<Node*> result = { medi.found, Utils::downcast<Node*>(medi.value) };
				if (result.value == nullptr)
					return nullptr;
				else if (result.found == 0)
					return Utils::addressof(result.value->getValue());
				else
					return nullptr;
			}
			const V* find(const K& key) const noexcept(lessex) {
				return const_cast<BinMap*>(this)->find(key);
			}
			Node* search(const K& key) noexcept(lessex) {
				return Utils::downcast<Node*>(_search(&key));
			}
			const Node* search(const K& key) const noexcept(lessex) {
				return Utils::downcast<const Node*>(_search(&key));
			}
		private:
			template <class, class>
				struct strlen_callable : Utils::lie {};
			template <class G>
				struct strlen_callable<Utils::none<
				decltype(GString::strlen(Utils::declval<G>()))>, G> {};
			template <class, class>
				struct strlen_noexcept_callable : Utils::lie {};
			template <class G>
				struct strlen_noexcept_callable<
				Utils::enable_if<noexcept(GString::strlen(Utils::declval<G>())), void>, G> : Utils::truth {};
		public:
			V& operator [](const K& key) {
				V* result = find(key);
				if (result)
					return *result;
				else if constexpr (strlen_noexcept_callable<void, const K&>::value)
					Utils::RangeError(String("Error: binary tree doesn't contain such key: ", key));
				else
					Utils::RangeError("Error: binary tree doesn't contain such key");
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
DLL_RESTORE
