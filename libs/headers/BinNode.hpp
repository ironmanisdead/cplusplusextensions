#pragma once
#include "BinTree.hpp"
#include "Function.hpp"
namespace CPPExtensions {
	template <class K, class V>
	class BinNode final : public BinEnt {
		public:
			static constexpr bool keycop = Utils::is_constructible<K, const K&>;
			static constexpr bool valcop = Utils::is_constructible<V, const V&>;
			static constexpr bool valbil = Utils::is_constructible<V, V&>;
		private:
			K key;
			V value;
		protected:
			bool less(const void* val) const override {
				return (key < *Utils::downcast<const K*>(val));
			}
			bool greater(const void* val) const override {
				return (*Utils::downcast<const K*>(val) < key);
			}
			BinNode* _copy() const noexcept override {
				if constexpr (keycop && valcop)
					return new BinNode(cons, _root(), out { nullptr },
							nullptr, nullptr, key, value);
				else
					return nullptr;
			}
			BinNode* _build() noexcept override {
				if constexpr (keycop && valbil)
					return new BinNode(cons, _root(), out { nullptr },
							nullptr, nullptr, key, value);
				else
					return nullptr;
			}
		public:
			static inline const BinInfo info = {
				Utils_offset(BinNode, key),
				Utils_offset(BinNode, value),
				sizeof(BinNode),
			};
			template <class... L>
			BinNode(Imply, bool root, out up, BinNode* lef, BinNode* rig, const K& ke, L&&... val) :
				BinEnt(&info, root, up, lef, rig), key(ke),
				value(Utils::forward<L>(val)...) {}
			template <class... L>
			BinNode(const K& ke, L&&... val)
			noexcept(noexcept( ( K(ke), V(Utils::forward<L>(val)...) ) )) :
				BinEnt(&info, false, out { nullptr }, nullptr, nullptr), key(ke),
				value(Utils::forward<L>(val)...) {}
			BinNode*& getParent() noexcept { return Utils::downcast<BinNode*&>(_parent()); }
			BinNode*& getLeft() noexcept { return Utils::downcast<BinNode*&>(_left()); }
			BinNode*& getRight() noexcept { return Utils::downcast<BinNode*&>(_right()); }
			const BinNode* getParent() const noexcept { return (const BinNode*)_parent(); }
			const BinNode* getLeft() const noexcept { return (const BinNode*)_left(); }
			const BinNode* getRight() const noexcept { return (const BinNode*)_right(); }
			const K& getKey() const noexcept { return key; }
			V& getValue() noexcept { return value; }
			const V& getValue() const noexcept { return value; }
			BinNode* min() noexcept { return (BinNode*)_min(); }
			BinNode* max() noexcept { return (BinNode*)_max(); }
			const BinNode* min() const noexcept { return (const BinNode*)_min(); }
			const BinNode* max() const noexcept { return (const BinNode*)_max(); }
			BinNode* prev() noexcept { return (BinNode*)_prev(); }
			BinNode* next() noexcept { return (BinNode*)_next(); }
			const BinNode* prev() const noexcept { return (const BinNode*)_prev(); }
			const BinNode* next() const noexcept { return (const BinNode*)_next(); }
			~BinNode() override = default;
			friend class BinMap<K, V>;
	};
}
