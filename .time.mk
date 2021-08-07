#this file is automatically created and modified,
#please do not modify or delete it yourself.

time.o: time.cpp /usr/local/include/stdc-predef.h \
 libs/include/CPPExtensions/system.hpp \
 libs/include/CPPExtensions/system_internals.hpp \
 libs/include/CPPExtensions/predefs.hpp \
 libs/include/CPPExtensions/.c_extern/internal-macros.h \
 libs/include/CPPExtensions/String.hpp \
 libs/include/CPPExtensions/types.hpp \
 libs/include/CPPExtensions/.part/StringView.hpp \
 libs/include/CPPExtensions/.part/../Utils.hpp \
 /usr/local/.mount_dir_point/crew/include/c++/11/compare \
 /usr/local/.mount_dir_point/crew/include/c++/11/concepts \
 /usr/local/.mount_dir_point/crew/include/c++/11/type_traits \
 /usr/local/.mount_dir_point/crew/include/c++/11/x86_64-cros-linux-gnu/bits/c++config.h \
 /usr/local/.mount_dir_point/crew/include/c++/11/x86_64-cros-linux-gnu/bits/os_defines.h \
 /usr/local/include/features.h /usr/local/include/sys/cdefs.h \
 /usr/local/include/bits/wordsize.h /usr/local/include/bits/long-double.h \
 /usr/local/include/gnu/stubs.h /usr/local/include/gnu/stubs-64.h \
 /usr/local/.mount_dir_point/crew/include/c++/11/x86_64-cros-linux-gnu/bits/cpu_defines.h \
 /usr/local/.mount_dir_point/crew/include/c++/11/pstl/pstl_config.h \
 libs/include/CPPExtensions/.part/../GString.hpp \
 libs/include/CPPExtensions/.part/../.part/GString.hpp \
 libs/include/CPPExtensions/.part/../.part/../Array.hpp \
 libs/include/CPPExtensions/.part/../.part/../Tuple.hpp \
 /usr/local/.mount_dir_point/crew/include/c++/11/typeinfo \
 /usr/local/.mount_dir_point/crew/include/c++/11/bits/exception.h \
 /usr/local/.mount_dir_point/crew/include/c++/11/bits/hash_bytes.h | libs
	$(shell test -f ./time.mk.2)
	$(if $(filter-out 0,$(.SHELLSTATUS)),.extra/depgen)
	$(CC) -o ./time.o ./time.cpp $(CFLAGS) $(LDFLAGS) $(LDLIBS) -I "libs/include/" -L "$$(pwd)/libs/" -Wl,-rpath,'$$ORIGIN/libs/' -Wl,-l:.shared.so
/usr/local/include/stdc-predef.h:
libs/include/CPPExtensions/system.hpp:
libs/include/CPPExtensions/system_internals.hpp:
libs/include/CPPExtensions/predefs.hpp:
libs/include/CPPExtensions/.c_extern/internal-macros.h:
libs/include/CPPExtensions/String.hpp:
libs/include/CPPExtensions/types.hpp:
libs/include/CPPExtensions/.part/StringView.hpp:
libs/include/CPPExtensions/.part/../Utils.hpp:
/usr/local/.mount_dir_point/crew/include/c++/11/compare:
/usr/local/.mount_dir_point/crew/include/c++/11/concepts:
/usr/local/.mount_dir_point/crew/include/c++/11/type_traits:
/usr/local/.mount_dir_point/crew/include/c++/11/x86_64-cros-linux-gnu/bits/c++config.h:
/usr/local/.mount_dir_point/crew/include/c++/11/x86_64-cros-linux-gnu/bits/os_defines.h:
/usr/local/include/features.h:
/usr/local/include/sys/cdefs.h:
/usr/local/include/bits/wordsize.h:
/usr/local/include/bits/long-double.h:
/usr/local/include/gnu/stubs.h:
/usr/local/include/gnu/stubs-64.h:
/usr/local/.mount_dir_point/crew/include/c++/11/x86_64-cros-linux-gnu/bits/cpu_defines.h:
/usr/local/.mount_dir_point/crew/include/c++/11/pstl/pstl_config.h:
libs/include/CPPExtensions/.part/../GString.hpp:
libs/include/CPPExtensions/.part/../.part/GString.hpp:
libs/include/CPPExtensions/.part/../.part/../Array.hpp:
libs/include/CPPExtensions/.part/../.part/../Tuple.hpp:
/usr/local/.mount_dir_point/crew/include/c++/11/typeinfo:
/usr/local/.mount_dir_point/crew/include/c++/11/bits/exception.h:
/usr/local/.mount_dir_point/crew/include/c++/11/bits/hash_bytes.h:
