#pragma once
#include "system_internals.hpp"
namespace CPPExtensions {
	namespace Utils {
		enum OpenFlags : unsigned { //flags are for opening files
			F_CREATE = 1, //create file if non-existant
			F_READ = 2, //open file in read mode
			F_WRITE = 4, //open file in write mode
			F_RDWR = F_READ | F_WRITE, //open file with read and write attributes
			F_APPEND = 8, //open file for appending
			F_TRUNC = 16 //open file for truncation
		};
		enum ModeFlags : unsigned { //flags are for determining how others can access your file
			M_NONE = 0, //No access for other users
			M_EXEC = 1, //Sets exec (if applicable for the current system)
			M_READ = 2, //Read access for other users
			M_WRITE = 4, //Write access for other users
			M_RDWR = M_READ | M_WRITE, //Read-Write access for other users
		};
		enum SeekFlag : unsigned { //Seek flags are for seek 
			S_RES, //Seeks from beginning
			S_CUR, //Seeks from current position
			S_END //Seeks from end
		};
		enum ErrFlag : unsigned {
			NO_ERROR, //there is no active local error
			MEM_ERROR, //allocation failed in call
			NULL_ERROR, //null pointer passed to function where it wasn't expected
			TYPE_ERROR //type was set incorrectly
		};
		extern thread_local ErrFlag _locerr;
	}
}
