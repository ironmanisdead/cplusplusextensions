.DEFAULT_GOAL:=libs
null:=
slash:=\$(null)
open:=(
close:=)
comma:=,
semi:=;
colon:=:
not=$(if $(1),,true)
targetfile=$*
ifeq ($(origin installabs),undefined)
 ifeq ($(origin installdir),undefined)
  installdir:=/usr/local
 endif
 installabs:=$(abspath $(installdir))
endif
ifeq ($(origin CC),default)
 CC:=gcc
endif
ifeq ($(origin CFLAGS),undefined)
 CFLAGS:=-std=c++2a -O3 -Wall -Wextra -lstdc++ -Wl,--disable-new-dtags
endif
ifeq ($(origin OFLAGS),undefined)
 OFLAGS:=-std=c++2a -O3 -Wall -Wextra -lstdc++ -fPIC
endif
ifeq ($(origin REINSTALL_ON_CHANGE),undefined)
 REINSTALL_ON_CHANGE:=true
endif
export installdir
export installabs
export targetfile
export CC
export CFLAGS
export OFLAGS
export REINSTALL_ON_CHANGE
ifneq ($(findstring /$(shell pwd),/$(installabs)),)
 $(error cannot install libcppextensions inside source directory)
endif

PHON:=libs remake unmake scrape clean reset install uninstall deps
SOURCES:=$(wildcard *.cpp)
RULES:=$(patsubst %.cpp,.%.mk,$(SOURCES))
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))
isdep:=$(call not,$(filter nodep,$(MAKECMDGOALS)))
ifneq ($(and $(filter-out $(PHON),$(MAKECMDGOALS)),$(isdep)),)
 no:=$(shell .extra/depcheck)
 no:=$(if $(filter-out 0,$(.SHELLSTATUS)),$(error depcheck failed with error $(.SHELLSTATUS)))
 ifeq ($(filter nodown,$(MAKECMDGOALS)),)
  no:=$(shell cd libs && make >&2)
 endif
 no:=$(if $(filter-out 0,$(.SHELLSTATUS)),$(error libraries could not be generated properly))
 include $(RULES)
endif

.PHONY:tests nodep nodown $(PHON)

libs:
	cd libs && make

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
	.extra/rmobj

remake: unmake #remakes all test object files
	MAKELEVEL=$(MAKELEVEL) make tests

scrape: unmake #removes all test object files and also generated rules
	.extra/rmgen

clean: scrape
	cd libs && make clean

reset: scrape
	cd libs && make reset
	MAKELEVEL= make nodown tests
