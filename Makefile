.DEFAULT_GOAL:=libs
not=$(if $(1),,true)
targetfile=$*
include .extra/rulemk/char.mk
isundef=$(filter undefined default,$(origin $(1)))
ifneq ($(call isundef,installabs),)
 ifneq ($(call isundef,installdir),)
  installdir:=/usr/local
 endif
 installabs:=$(abspath $(installdir))
endif
ifneq ($(call isundef,CC),)
 CC:=gcc
endif
ifneq ($(call isundef,LDFLAGS),)
 LDFLAGS:=-Wl,--disable-new-dtags
endif
ifneq ($(call isundef,LDLIBS),)
 LDLIBS:=-lstdc++
endif
ifneq ($(call isundef,CFLAGS),)
 CFLAGS:=-std=c++2a -O3 -Wall -Wextra
endif
ifneq ($(call isundef,OFLAGS),)
 OFLAGS:=-fPIC
endif
export installdir
export installabs
export targetfile
export CC
export LDFLAGS
export LDLIBS
export CFLAGS
export OFLAGS
ifneq ($(findstring /$(shell pwd),/$(installabs)),)
 $(error cannot install libcppextensions inside source directory)
endif

PHON:=libs remake unmake scrape clean reset install uninstall deps
.PHONY:tests nodep nodown $(PHON)
SOURCES:=$(wildcard *.cpp)
RULES:=$(patsubst %.cpp,.%.mk,$(SOURCES))
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))
isdep:=$(call not,$(filter nodep,$(MAKECMDGOALS)))

ifneq ($(and $(filter-out $(PHON),$(MAKECMDGOALS)),$(isdep)),)
 no:=$(shell .extra/namecheck)
 ifneq ($(.SHELLSTATUS),0)
  $(error Makefile detected an invalid name)
 endif
 include $(RULES)
endif

libs:
	.extra/depcheck
	$(if $(filter nodown,$(MAKECMDGOALS)),,cd libs && make)

install:
	.extra/install

uninstall:
	.extra/uninstall

tests: $(OBJECTS)

nodep: #means make will not generate dependencies

nodown: #means make will not descend into subdirectories to remake things

.%.mk:
	.extra/depgen

unmake: #removes all test object files
	.extra/rulemk/rmobj

remake: unmake #remakes all test object files
	MAKELEVEL=$(MAKELEVEL) make tests

scrape: unmake #removes all test object files and also generated rules
	.extra/rulemk/rmgen

clean: scrape
	cd libs && make clean

reset: scrape
	cd libs && make reset
	MAKELEVEL= make nodown tests
