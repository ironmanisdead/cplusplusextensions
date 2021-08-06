ifneq ($(call isundef,LDLIBS),)
 LDLIBS:=-lstdc++
endif
ifneq ($(call isundef,C_VER),)
 C_VER:=c++2a
endif
ifneq ($(call isundef,C_EXT),)
 C_EXT:=cpp
endif
ifneq ($(call isundef,H_EXT),)
 H_EXT:=hpp
endif
ifneq ($(call isundef,current_lib),)
 current_lib:=cppextensions
endif
