#pragma once
#include "Utils.hpp"
#include "Find.hpp"
namespace CustomUtils {
	struct BinInfo {
		Utils::size keydist;
		Utils::size valdist;
		Utils::size total;
	};
	class BinTree;
	class BinEnt {
		public:
			union out {
				BinTree* tree;
				BinEnt* parent;
			};
		private:
			static constexpr BinEnt* nullEnt = nullptr;
			static constexpr BinTree* nullTree = nullptr;
			const BinInfo* const data;
			bool root;
			out upwards;
			BinEnt* left;
			BinEnt* right;
		protected:
			struct Imply {};
			static constexpr Imply cons = {};
			virtual void _rebalance() noexcept;
			virtual bool less(const void*) const = 0;
			virtual bool greater(const void*) const = 0;
			virtual BinEnt* _copy() const = 0;
			virtual BinEnt* _build() = 0;
			inline BinTree*& uTree() noexcept {
				return upwards.tree;
			}
			inline BinEnt*& uParent() noexcept {
				return upwards.parent;
			}
			bool unParent(BinEnt*) noexcept;
			void disownSelf(BinEnt*) noexcept;
			inline void setParent(BinEnt* val) noexcept {
				root = false;
				uParent() = val;
			}
			inline void setParent(BinTree* val) noexcept {
				root = true;
				uTree() = val;
			}
			inline void setParent(Utils::nullptr_t) noexcept {
				root = true;
				uTree() = nullptr;
			}
			void setParentFrom(BinEnt*) noexcept;
			void reSelf(BinEnt*) noexcept;
			BinEnt* clonemut(BinEnt*);
			BinEnt* clone(BinEnt*) const;
			BinEnt* clonemut(BinTree*);
			BinEnt* clone(BinTree*) const;
		public:
			BinEnt(const BinInfo* info, bool cent, out up, BinEnt* lef, BinEnt* rig) noexcept :
				data(info), root(cent), upwards(up), left(lef), right(rig) {}
			bool rotate(bool) noexcept;
			inline bool isroot() const noexcept { return root; }
			inline bool isattach() const noexcept {
				return root ? (bool)upwards.tree : (bool)upwards.parent;
			}
			bool detach() noexcept;
			inline const void* _key() const noexcept {
				return RECAST(const char*, this) + data->keydist;
			}
			inline void* _value() noexcept {
				return RECAST(char*, this) + data->valdist;
			}
			inline const void* _value() const noexcept {
				return const_cast<BinEnt*>(this)->_value();
			}
			inline BinEnt*& _parent() noexcept {
				if (root)
					return const_cast<BinEnt*&>(nullEnt);
				else
					return uParent();
			}
			inline const BinEnt* _parent() const noexcept {
				return const_cast<BinEnt*>(this)->_parent();
			}
			BinEnt* _root() noexcept;
			inline const BinEnt* _root() const noexcept {
				return const_cast<BinEnt*>(this)->_root();
			}
			inline BinTree* _tree() noexcept {
				return _root()->upwards.tree;
			}
			inline const BinTree* _tree() const noexcept {
				return _root()->upwards.tree;
			}
			inline BinEnt*& _left() noexcept { return left; }
			inline BinEnt*& _right() noexcept { return right; }
			inline const BinEnt* _left() const noexcept { return left; }
			inline const BinEnt* _right() const noexcept { return right; }
			BinEnt* _min() noexcept;
			BinEnt* _max() noexcept;
			BinEnt* _prev() noexcept;
			BinEnt* _next() noexcept;
			inline const BinEnt* _min() const noexcept { return const_cast<BinEnt*>(this)->_min(); }
			inline const BinEnt* _max() const noexcept { return const_cast<BinEnt*>(this)->_max(); }
			inline const BinEnt* _prev() const noexcept { return const_cast<BinEnt*>(this)->_prev(); }
			inline const BinEnt* _next() const noexcept { return const_cast<BinEnt*>(this)->_next(); }
			virtual ~BinEnt() = default;
			void deleter() noexcept;
			friend class BinTree;
	};
}
#include "BinEntI.hpp"
namespace CustomUtils {
	class BinTree {
		private:
			const BinInfo* const data;
		protected:
			BinEnt* root;
			using stronk = Utils::strongcmp_t;
			BinEnt* _place(BinEnt*, BinEnt*, stronk, bool = false) noexcept;
			BinEnt* _attach(BinEnt*);
			bool _add(BinEnt*, bool = false);
		public:
			inline BinTree(BinEnt* val, const BinInfo* info) noexcept :
				data(info), root(val) {}
			virtual BinTree* clone() = 0;
			virtual BinTree* clone() const = 0;
			BinEnt* attach(BinEnt*);
			inline BinEnt* attach(BinEntI& val) {
				return attach(&*val);
			}
			inline BinEnt* getRoot() noexcept {
				return root;
			}
			inline const BinEnt* getRoot() const noexcept {
				return root;
			}
			Find<BinEnt*> _find(const void*);
			inline Find<const BinEnt*> _find(const void* key) const {
				return const_cast<BinTree*>(this)->_find(key);
			}
			BinEntI begin() noexcept;
			BinEntI max() noexcept;
			BinEntC begin() const noexcept;
			BinEntC max() const noexcept;
			inline const BinEntC end() const noexcept {
				return nullptr;
			}
			BinEnt* _search(const void*);
			inline const BinEnt* _search(const void* key) const {
				return const_cast<BinTree*>(this)->_search(key);
			}
			BinEnt* _detach(const void*) noexcept;
			virtual ~BinTree() = default;
			friend class BinEnt;
	};
	template <class T, class = T>
		class BinNode;
	template <class T, class = T>
		class BinMap;
}
