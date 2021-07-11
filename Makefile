.DEFAULT_GOAL:=help
null:=
slash:=\$(null)
open:=(
close:=)
comma:=,
semi:=;
colon:=:
not=$(if $(1),,true)
targetfile=$*
installdir?=
installabs?=
ifeq ($(installabs),)
 ifeq ($(installdir),)
  installdir:=/usr/local
 endif
 installabs:=$(abspath $(installdir))
endif
export installdir
export installabs
export targetfile
ifneq ($(findstring /$(shell pwd),/$(installabs)),)
 $(error cannot install libcustomutils inside source directory)
endif

PHON:=help remake unmake scrape clean reset install uninstall deps
SOURCES:=$(wildcard *.cpp)
RULES:=$(patsubst %.cpp,.%.mk,$(SOURCES))
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))
isdep:=$(call not,$(filter nodep,$(MAKECMDGOALS)))
ifneq ($(and $(filter-out $(PHON),$(MAKECMDGOALS)),$(isdep)),)
 no:=$(shell .extra/depcheck)
 no:=$(if $(filter-out 0,$(.SHELLSTATUS)),$(error depcheck failed with error $(.SHELLSTATUS)))
 ifeq ($(filter nodown,$(MAKECMDLIST)),)
  no:=$(shell cd libs && make >&2)
 endif
 no:=$(if $(filter-out 0,$(.SHELLSTATUS)),$(error libraries could not be generated properly))
 include $(RULES)
endif

.PHONY:tests nodep nodown $(PHON)

help:
	$(info please select 'install', 'tests', 'uninstall', or a specific '.o' file that you wish to generate)

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
