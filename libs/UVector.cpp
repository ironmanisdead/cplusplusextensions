#include "headers/UVector.hpp"
#include <stdexcept>
#include "headers/system_internals.hpp"
DLL_HIDE
namespace CPPExtensions {
	DLL_PUBLIC UVector::UVector(const FullType* const type)
		noexcept : typeinfo(type), _status(NO_ERROR), trulen(0), len(0) {}
	DLL_PUBLIC UVector::~UVector() { finalize(); }
	DLL_PUBLIC const char* UVector::id() noexcept { return typeinfo->text->id; }
	DLL_PUBLIC bool UVector::_allocate(Utils::size_t siz) noexcept {
		_status = NO_ERROR;
		void* temp =
			Utils::downcast<char*>(Utils::malloc(siz));
		if (!temp) {
			_status = MEM_ERROR;
			return false;
		}
		raw = (char*)temp;
		trulen = siz;
		len = 0;
		return true;
	}
	DLL_PUBLIC void UVector::deinit() noexcept {
		if (trulen > 0) {
			void (*del)(void*) = typeinfo->data->deleter;
			if (del) {
				Utils::size_t elem = typeinfo->data->elem;
				for (Utils::size_t i = 0; i < len; i++)
					del(&raw[i * elem]);
			}
			len = 0;
		}
	}
	DLL_PUBLIC bool UVector::save(UVector& val) noexcept { 
		_status = NO_ERROR;
		if (typeinfo != val.typeinfo) {
			_status = TYPE_ERROR;
			return false;
		}
		finalize();
		raw = val.raw;
		len = val.len;
		trulen = val.trulen;
		if (val._status != TYPE_ERROR)
			_status = val._status;
		val.trulen = 0;
		return true;
	}
	DLL_PUBLIC bool UVector::save(UVector& val, void (*const mover)(void*, void*)) {
		_status = NO_ERROR;
		if (mover == nullptr) {
			_status = TYPE_ERROR;
			return false;
		}
		Utils::size_t target = val.len;
		Utils::size_t ot = val.typeinfo->data->elem;
		Utils::size_t cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		for (Utils::size_t idx = 0; idx < target; idx++)
			mover(&raw[idx * cur], &val.raw[idx * ot]);
		len = target;
		return true;
	}
	DLL_PUBLIC bool UVector::save(UVector& val, void (*const mover)(void*, void*) noexcept) noexcept {
		_status = NO_ERROR;
		if (mover == nullptr) {
			_status = TYPE_ERROR;
			return false;
		}
		Utils::size_t target = val.len;
		Utils::size_t ot = val.typeinfo->data->elem;
		Utils::size_t cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		for (Utils::size_t idx = 0; idx < target; idx++)
			mover(&raw[idx * cur], &val.raw[idx * ot]);
		len = target;
		return true;
	}
	DLL_PUBLIC bool UVector::copy(const UVector& val) {
		_status = NO_ERROR;
		if (typeinfo->data->copier == nullptr) {
			_status = TYPE_ERROR;
			return false;
		}
		Utils::size_t target = val.len;
		Utils::size_t cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		for (Utils::size_t idx = 0; idx < target; idx++)
			typeinfo->data->copier(&raw[idx * cur], &val.raw[idx * cur]);
		len = target;
		return true;
	}
	DLL_PUBLIC bool UVector::copy(const UVector& val, void (*const mobilize)(void*, const void*)) {
		_status = NO_ERROR;
		if (mobilize == nullptr) {
			_status = TYPE_ERROR;
			return false;
		}
		Utils::size_t target = val.len;
		Utils::size_t ot = val.typeinfo->data->elem;
		Utils::size_t cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		Utils::size_t idx;
		try {
			for (idx = 0; idx < target; idx++)
				mobilize(&raw[idx * cur], &val.raw[idx * ot]);
		} catch (...) {
			len = idx - 1;
			deinit();
			throw;
		}
		len = target;
		return true;
	}
	DLL_PUBLIC bool UVector::copy(const UVector& val, void (*const mobilize)(void*, const void*) noexcept) noexcept {
		if (mobilize == nullptr) {
			_status = TYPE_ERROR;
			return false;
		}
		Utils::size_t target = val.len;
		Utils::size_t ot = val.typeinfo->data->elem;
		Utils::size_t cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		Utils::size_t idx;
		for (idx = 0; idx < target; idx++)
			mobilize(&raw[idx * cur], &val.raw[idx * ot]);
		len = target;
		return true;
	}
	DLL_PUBLIC UVector& UVector::operator =(const UVector& val) {
		static_cast<void>(copy(val));
		return *this;
	}
	DLL_PUBLIC void UVector::finalize() noexcept {
		if (trulen > 0) {
			deinit();
			Utils::free(raw);
			trulen = 0;
		}
	}
	DLL_PUBLIC bool UVector::resize(Utils::size_t n1) noexcept {
		_status = NO_ERROR;
		if (trulen == 0)
			return _allocate(n1);
		if (trulen > n1)
			return true;
		Utils::size_t ntru = trulen;
		Utils::size_t elem = typeinfo->data->elem;
		n1 += elem;
		while (ntru < n1)
			ntru *= 2;
		char* temp = Utils::downcast<char*>(Utils::malloc(ntru));
		if (!temp) {
			_status = MEM_ERROR;
			return false;
		}
		Utils::memcpy(temp, raw, len * elem);
		Utils::free(raw);
		raw = temp;
		trulen = ntru + elem;
		return true;
	}
	DLL_PUBLIC bool UVector::remove(Utils::size_t n1, Utils::size_t n2) noexcept {
		if (n2 == 0)
			return false;
		if (trulen == 0)
			return false;
		if ((n1 + n2) > len)
			return false;
		void (*del)(void*) = typeinfo->data->deleter;
		Utils::size_t elem = typeinfo->data->elem;
		Utils::size_t start = elem * n1;
		Utils::size_t fin = start + (elem * n2);
		if (del)
			for (Utils::size_t n = start; n <= fin; n += elem)
				del(&raw[n]);
		Utils::memmove(&raw[start], &raw[fin], len - (n1 + n2));
		return true;
	}
	DLL_PUBLIC void UVector::place(Utils::size_t n1, const char* val) noexcept {
		Utils::size_t elem = typeinfo->data->elem;
		Utils::size_t rem = elem * (len - n1);
		Utils::memcpy(&raw[(n1 + 1) * elem], &raw[n1 * elem], rem);
		Utils::memcpy(&raw[n1 * elem], val, elem);
		len++;
	}
	DLL_PUBLIC bool UVector::allocate(Utils::size_t dat) noexcept {
		_status = NO_ERROR;
		Utils::size_t elem = typeinfo->data->elem;
		Utils::size_t locsiz = elem * dat;
		if (trulen == 0)
			return _allocate(locsiz);
		if (trulen > locsiz)
			return true;
		char* temp = Utils::downcast<char*>(Utils::malloc(locsiz));
		if (!temp) {
			_status = MEM_ERROR;
			return false;
		}
		Utils::memcpy(temp, raw, len * elem);
		Utils::free(raw);
		raw = temp;
		trulen = locsiz;
		return true;
	}
}
DLL_RESTORE
#include "headers/Vector.hpp"
#include <iostream>
DLL_HIDE
namespace CPPExtensions {
	template <>
	DLL_PUBLIC void Vector<char>::create(String&& val) noexcept {
		len = val.view.len;
		trulen = val.trulen;
		raw = val.view.edit();
		val.trulen = 0;
	}
	template <>
	DLL_PUBLIC bool Vector<char>::create(const String& val) noexcept {
		if (!_allocate(val.view.len + 1))
			return false;
		Utils::memcpy(raw, val.view.read(), len = val.view.len);
		return true;
	}
	template <>
	DLL_PUBLIC void vecput(std::ostream& os, const Vector<char>* val) {
		if (val && (val->trulen > 0))
			return static_cast<void>(os.write(val->raw, val->len));
	}
}
DLL_RESTORE
