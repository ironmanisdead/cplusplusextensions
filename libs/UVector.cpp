#include "headers/UVector.hpp"
#include <stdexcept>
//TODO: finish Vectors
namespace CustomUtils {
	UVector::UVector(const FullType* const type)
		noexcept : typeinfo(type), trulen(0), len(0) {}
	UVector::~UVector() { finalize(); }
	const char* UVector::id() noexcept { return typeinfo->text->id; }
	void UVector::allocate(Utils::size siz) {
		raw = Utils::downcast<char*>(::operator new(siz));
		trulen = siz;
		len = 0;
	}
	void UVector::save(UVector& val) noexcept { 
		trulen = val.trulen;
		val.trulen = 0;
	}
	void UVector::save(UVector& val, void (*const mover)(void*, void*)) {
		if (mover == nullptr) {
			const char* aserr = typeinfo->text->assign;
			const char* altvec = typeinfo->text->vecid;
			throw std::runtime_error(String(aserr, altvec, " at runtime"));
		}
		Utils::size target = val.len;
		Utils::size ot = val.typeinfo->data->elem;
		Utils::size cur = typeinfo->data->elem;
		finalize();
		allocate((target + 1) * cur);
		for (Utils::size idx = 0; idx < target; idx++)
			mover(&raw[idx * cur], &val.raw[idx * ot]);
		len = target;
	}
	void UVector::copy(const UVector& val) {
		if (typeinfo->data->copier == nullptr) {
			const char* coperr = typeinfo->text->copy;
			const char* altvec = val.typeinfo->text->vecid;
			throw std::runtime_error(String(coperr, altvec, " at runtime"));
		}
		Utils::size target = val.len;
		Utils::size cur = typeinfo->data->elem;
		finalize();
		allocate((target + 1) * cur);
		for (Utils::size idx = 0; idx < target; idx++)
			typeinfo->data->copier(&raw[idx * cur], &val.raw[idx * cur]);
		len = target;
	}
	void UVector::copy(const UVector& val, void (*const mobilize)(void*, const void*)) {
		if (mobilize == nullptr) {
			const char* conerr = typeinfo->text->conv;
			const char* altvec = val.typeinfo->text->vecid;
			throw std::runtime_error(String(conerr, altvec, " at runtime"));
		}
		Utils::size target = val.len;
		Utils::size ot = val.typeinfo->data->elem;
		Utils::size cur = typeinfo->data->elem;
		finalize();
		allocate((target + 1) * cur);
		Utils::size idx;
		try {
			for (idx = 0; idx < target; idx++)
				mobilize(&raw[idx * cur], &val.raw[idx * ot]);
		} catch (...) {
			len = idx - 1;
			finalize();
			throw;
		}
		len = target;
	}
	void UVector::finalize() noexcept {
		if (trulen > 0) {
			void (&del)(void*) = typeinfo->data->deleter;
			Utils::size elem = typeinfo->data->elem;
			for (Utils::size i = 0; i < len; i++)
				del(&raw[i * elem]);
			::operator delete (raw);
			trulen = 0;
			len = 0;
		}
	}
	void UVector::resize(Utils::size n1) {
		if (trulen == 0)
			return allocate(n1);
		if (trulen > n1)
			return;
		Utils::size ntru = trulen;
		while (ntru < n1)
			ntru *= 2;
		Utils::size elem = typeinfo->data->elem;
		char* temp = Utils::downcast<char*>(::operator new(ntru + elem));
		Utils::memcpy(temp, raw, len * elem);
		raw = temp;
		trulen = ntru + elem;
	}
	void UVector::remove(Utils::size n1, Utils::size n2) {
		const char* vecid = typeinfo->text->vecid;
		if (n2 == 0)
			return;
		if (trulen == 0)
			throw std::runtime_error(String(vecid, " error: Vector is empty"));
		if ((n1 + n2) > len)
			throw std::runtime_error(
					String(vecid, " error: ", n1 + n2 - 1,
						" is out of bounds for Vector of length ", len));
		void (&del)(void*) = typeinfo->data->deleter;
		Utils::size elem = typeinfo->data->elem;
		Utils::size start = elem * n1;
		Utils::size fin = start + (elem * n2);
		for (Utils::size n = start; n <= fin; n += elem)
			del(&raw[n]);
		Utils::memmove(&raw[start], &raw[fin], len - (n1 + n2));
	}
	void UVector::place(Utils::size n1, const char* val) noexcept {
		Utils::size elem = typeinfo->data->elem;
		Utils::size rem = elem * (len - n1);
		Utils::memcpy(&raw[(n1 + 1) * elem], &raw[n1 * elem], rem);
		Utils::memcpy(&raw[n1 * elem], val, elem);
		len++;
	}
}
