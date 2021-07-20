#pragma once
#include "Utils.hpp"
#include "Find.hpp"
DLL_HIDE
namespace CPPExtensions {
	struct BinInfo {
		Utils::size_t keydist;
		Utils::size_t valdist;
		Utils::size_t total;
	};
	class BinTree;
	class DLL_PUBLIC BinEnt {
		public:
			enum State { NO_ERROR, MEM_ERROR,
				ALLOC_ERROR, MISSING_ERROR };
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
			mutable State _status;
			struct Imply {};
			static constexpr Imply cons = {};
			DLL_PUBLIC virtual void _rebalance() noexcept;
			DLL_LOCAL virtual bool less(const void*) const = 0;
			DLL_LOCAL virtual bool greater(const void*) const = 0;
			DLL_LOCAL virtual BinEnt* _copy() const = 0;
			DLL_LOCAL virtual BinEnt* _build() = 0;
			inline BinTree*& uTree() noexcept {
				return upwards.tree;
			}
			inline BinEnt*& uParent() noexcept {
				return upwards.parent;
			}
			DLL_LOCAL bool unParent(BinEnt*) noexcept;
			DLL_LOCAL void disownSelf(BinEnt*) noexcept;
			inline void setParent(BinEnt* val) noexcept {
				root = false;
				uParent() = val;
			}
			inline void setParent(BinTree* val) noexcept {
				root = true;
				uTree() = val;
			}
			inline void setParent(Utils::nullpt) noexcept {
				root = true;
				uTree() = nullptr;
			}
			DLL_LOCAL void setParentFrom(BinEnt*) noexcept;
			DLL_LOCAL void reSelf(BinEnt*) noexcept;
			DLL_PUBLIC BinEnt* clonemut(BinEnt*);
			DLL_PUBLIC BinEnt* clone(BinEnt*) const;
			DLL_PUBLIC BinEnt* clonemut(BinTree*);
			DLL_PUBLIC BinEnt* clone(BinTree*) const;
		public:
			DLL_PUBLIC BinEnt(const BinInfo* info, bool cent, out up, BinEnt* lef, BinEnt* rig) noexcept :
				data(info), root(cent), upwards(up), left(lef), 
				right(rig), _status(NO_ERROR) {}
			DLL_PUBLIC bool rotate(bool) noexcept;
			inline State getStatus() noexcept { return _status; }
			inline bool isroot() const noexcept { return root; }
			inline bool isattach() const noexcept {
				return root ? (bool)upwards.tree : (bool)upwards.parent;
			}
			DLL_PUBLIC bool detach() noexcept;
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
			DLL_PUBLIC BinEnt* _root() noexcept;
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
			DLL_PUBLIC BinEnt* _min() noexcept;
			DLL_PUBLIC BinEnt* _max() noexcept;
			DLL_PUBLIC BinEnt* _prev() noexcept;
			DLL_PUBLIC BinEnt* _next() noexcept;
			inline const BinEnt* _min() const noexcept { return const_cast<BinEnt*>(this)->_min(); }
			inline const BinEnt* _max() const noexcept { return const_cast<BinEnt*>(this)->_max(); }
			inline const BinEnt* _prev() const noexcept { return const_cast<BinEnt*>(this)->_prev(); }
			inline const BinEnt* _next() const noexcept { return const_cast<BinEnt*>(this)->_next(); }
			DLL_PUBLIC virtual ~BinEnt() = default;
			DLL_PUBLIC void deleter() noexcept;
			friend class BinTree;
	};
}
DLL_RESTORE
#include "BinEntI.hpp"
DLL_HIDE
namespace CPPExtensions {
	class BinTree {
		public:
			enum State { NO_ERROR, TYPE_ERROR,
				MEM_ERROR, OVERRIDE_ERROR, SEARCH_ERROR };
		private:
			const BinInfo* const data;
		protected:
			struct BinStat {
				bool error;
				BinEnt* value;
			};
			BinEnt* root;
			mutable State _status;
			using stronk = Utils::strongcmp_t;
			DLL_PUBLIC BinEnt* _place(BinEnt*, BinEnt*, stronk, bool = false) noexcept;
			DLL_PUBLIC BinEnt* _attach(BinEnt*);
			DLL_PUBLIC BinStat _add(BinEnt*, bool = false);
		public:
			inline BinTree(BinEnt* val, const BinInfo* info) noexcept :
				data(info), root(val), _status(NO_ERROR) {}
			DLL_PUBLIC virtual BinTree* clone() = 0;
			DLL_PUBLIC virtual BinTree* clone() const = 0;
			DLL_PUBLIC BinEnt* attach(BinEnt*);
			inline BinEnt* attach(BinEntI& val) { return attach(&*val); }
			inline BinEnt* getRoot() noexcept { return root; }
			inline const BinEnt* getRoot() const noexcept { return root; }
			inline State getStatus() noexcept { return _status; }
			DLL_PUBLIC Find<BinEnt*> _find(const void*);
			inline Find<const BinEnt*> _find(const void* key) const {
				return const_cast<BinTree*>(this)->_find(key);
			}
			DLL_PUBLIC BinEntI begin() noexcept;
			DLL_PUBLIC BinEntI max() noexcept;
			DLL_PUBLIC BinEntC begin() const noexcept;
			DLL_PUBLIC BinEntC max() const noexcept;
			inline const BinEntC end() const noexcept { return nullptr; }
			DLL_PUBLIC BinEnt* _search(const void*);
			inline const BinEnt* _search(const void* key) const {
				return const_cast<BinTree*>(this)->_search(key);
			}
			DLL_PUBLIC BinEnt* _detach(const void*) noexcept;
			DLL_PUBLIC virtual ~BinTree() = default;
			friend class BinEnt;
	};
	template <class T, class = T>
		class BinNode;
	template <class T, class = T>
		class BinMap;
}
DLL_RESTORE
