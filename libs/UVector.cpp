#include "headers/UVector.hpp"
#include <stdexcept>
#include <new>
namespace CPPExtensions {
	UVector::UVector(const FullType* const type)
		noexcept : typeinfo(type), trulen(0), len(0) {}
	UVector::~UVector() { finalize(); }
	const char* UVector::id() noexcept { return typeinfo->text->id; }
	bool UVector::_allocate(Utils::size siz) noexcept {
		void* temp =
			Utils::downcast<char*>(::operator new(siz, std::nothrow_t {}));
		if (!temp)
			return false;
		raw = (char*)temp;
		trulen = siz;
		len = 0;
		return true;
	}
	void UVector::deinit() noexcept {
		if (trulen > 0) {
			void (*del)(void*) = typeinfo->data->deleter;
			if (del) {
				Utils::size elem = typeinfo->data->elem;
				for (Utils::size i = 0; i < len; i++)
					del(&raw[i * elem]);
			}
			len = 0;
		}
	}
	bool UVector::save(UVector& val) noexcept { 
		if (typeinfo != val.typeinfo)
			return false;
		finalize();
		raw = val.raw;
		len = val.len;
		trulen = val.trulen;
		val.trulen = 0;
		return true;
	}
	bool UVector::save(UVector& val, void (*const mover)(void*, void*)) {
		if (mover == nullptr)
			return false;
		Utils::size target = val.len;
		Utils::size ot = val.typeinfo->data->elem;
		Utils::size cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		for (Utils::size idx = 0; idx < target; idx++)
			mover(&raw[idx * cur], &val.raw[idx * ot]);
		len = target;
		return true;
	}
	bool UVector::save(UVector& val, void (*const mover)(void*, void*) noexcept) noexcept {
		if (mover == nullptr)
			return false;
		Utils::size target = val.len;
		Utils::size ot = val.typeinfo->data->elem;
		Utils::size cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		for (Utils::size idx = 0; idx < target; idx++)
			mover(&raw[idx * cur], &val.raw[idx * ot]);
		len = target;
		return true;
	}
	bool UVector::copy(const UVector& val) {
		if (typeinfo->data->copier == nullptr)
			return false;
		Utils::size target = val.len;
		Utils::size cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		for (Utils::size idx = 0; idx < target; idx++)
			typeinfo->data->copier(&raw[idx * cur], &val.raw[idx * cur]);
		len = target;
		return true;
	}
	bool UVector::copy(const UVector& val, void (*const mobilize)(void*, const void*)) {
		if (mobilize == nullptr)
			return false;
		Utils::size target = val.len;
		Utils::size ot = val.typeinfo->data->elem;
		Utils::size cur = typeinfo->data->elem;
		deinit();
		if (!resize((target + 1) * cur))
			return false;
		Utils::size idx;
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
	UVector& UVector::operator =(const UVector& val) {
		static_cast<void>(copy(val));
		return *this;
	}
	void UVector::finalize() noexcept {
		if (trulen > 0) {
			deinit();
			::operator delete (raw);
			trulen = 0;
		}
	}
	bool UVector::resize(Utils::size n1) noexcept {
		if (trulen == 0)
			return _allocate(n1);
		if (trulen > n1)
			return true;
		Utils::size ntru = trulen;
		while (ntru < n1)
			ntru *= 2;
		Utils::size elem = typeinfo->data->elem;
		char* temp = Utils::downcast<char*>(::operator new(ntru + elem));
		if (!temp)
			return false;
		Utils::memcpy(temp, raw, len * elem);
		::operator delete(raw);
		raw = temp;
		trulen = ntru + elem;
		return true;
	}
	bool UVector::remove(Utils::size n1, Utils::size n2) noexcept {
		if (n2 == 0)
			return false;
		if (trulen == 0)
			return false;
		if ((n1 + n2) > len)
			return false;
		void (*del)(void*) = typeinfo->data->deleter;
		Utils::size elem = typeinfo->data->elem;
		Utils::size start = elem * n1;
		Utils::size fin = start + (elem * n2);
		if (del)
			for (Utils::size n = start; n <= fin; n += elem)
				del(&raw[n]);
		Utils::memmove(&raw[start], &raw[fin], len - (n1 + n2));
		return true;
	}
	void UVector::place(Utils::size n1, const char* val) noexcept {
		Utils::size elem = typeinfo->data->elem;
		Utils::size rem = elem * (len - n1);
		Utils::memcpy(&raw[(n1 + 1) * elem], &raw[n1 * elem], rem);
		Utils::memcpy(&raw[n1 * elem], val, elem);
		len++;
	}
	bool UVector::allocate(Utils::size dat) noexcept {
		Utils::size elem = typeinfo->data->elem;
		Utils::size locsiz = elem * dat;
		if (trulen == 0)
			return _allocate(locsiz);
		if (trulen > locsiz)
			return true;
		char* temp = Utils::downcast<char*>(::operator new(locsiz));
		if (!temp)
			return false;
		Utils::memcpy(temp, raw, len * elem);
		::operator delete(raw);
		raw = temp;
		trulen = locsiz;
		return true;
	}
}
#include "headers/Vector.hpp"
#include <iostream>
namespace CPPExtensions {
	template <>
	void Vector<char>::create(String&& val) noexcept {
		len = val.view.len;
		trulen = val.trulen;
		raw = val.view.edit();
		val.trulen = 0;
	}
	template <>
	bool Vector<char>::create(const String& val) noexcept {
		if (!_allocate(val.view.len + 1))
			return false;
		Utils::memcpy(raw, val.view.read(), len = val.view.len);
		return true;
	}
	template <>
	void vecput(std::ostream& os, const Vector<char>* val) {
		if (val && (val->trulen > 0))
			return static_cast<void>(os.write(val->raw, val->len));
	}
}
