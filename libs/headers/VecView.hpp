#include <CPPExtensions/>
namespace CPPExtensions {
	class DataView {
		Utils::size len;
	};
	template <class T>
	class VecView : public DataView {
	};
}
