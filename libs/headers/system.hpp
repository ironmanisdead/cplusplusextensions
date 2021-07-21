#pragma once
#include "system_internals.hpp"
namespace CPPExtensions {
	namespace Utils {
		enum FileFlags : unsigned { //flags are for opening files
			F_CREATE = 1, //create file if non-existant
			F_READ = 2, //open file in read mode
			F_WRITE = 4, //open file in write mode
			F_RDWR = 6 //F_RDRW = F_READ | F_WRITE
		};
		enum SeekFlag : unsigned { //Seek flags are for seek 
			S_RES, //Seeks from beginning
			S_CUR, //Seeks from current position
			S_END //Seeks from end
		};
		enum ErrFlag : unsigned {
			NO_ERROR, //there is no active local error
			MEM_ERROR, //allocation failed in call
			NULL_ERROR //null pointer passed to function where it wasn't expected
		};
		DLL_LOCAL extern thread_local ErrFlag _locerr; //stores local error code 
		constexpr FileFlags operator |(FileFlags a, FileFlags b) noexcept {
			return (FileFlags)((int)a | (int) b);
		} //Allows merging file flags with or operator
	}
}
