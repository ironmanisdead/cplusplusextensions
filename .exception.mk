#this file is automatically created and modified,
#please do not modify or delete it yourself.

exception.o: exception.cpp /usr/local/include/stdc-predef.h \
 libs/include/CPPExtensions/system.hpp \
 libs/include/CPPExtensions/system_internals.hpp \
 libs/include/CPPExtensions/predefs.hpp \
 libs/include/CPPExtensions/.c_extern/internal-macros.h | libs
	$(shell test -f ./exception.mk.2)
	$(if $(filter-out 0,$(.SHELLSTATUS)),.extra/depgen)
	$(CC) -o ./exception.o ./exception.cpp $(CFLAGS) $(LDFLAGS) $(LDLIBS) -I "libs/include/" -L "$$(pwd)/libs/" -Wl,-rpath,'$$ORIGIN/libs/' -Wl,-l:.shared.so
/usr/local/include/stdc-predef.h:
libs/include/CPPExtensions/system.hpp:
libs/include/CPPExtensions/system_internals.hpp:
libs/include/CPPExtensions/predefs.hpp:
libs/include/CPPExtensions/.c_extern/internal-macros.h:
