.DEFAULT_GOAL:=install
not=$(if $(1),,true)
installdir:=/usr/local
installabs:=$(abspath $(installdir))
PHON:=help remake unmake scrape clean reset .refresh install uninstall
SOURCES:=$(wildcard *.cpp)
DOTMK:=$(wildcard .*.mk)
DOTMK2:=$(wildcard .*.mk.2)
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))
GENO:=$(wildcard $(OBJECTS))
GENERATED:=$(strip $(DOTMK) $(GENO))
RULES:=$(patsubst %.cpp,.%.mk,$(SOURCES))
isdep:=$(call not,$(filter nodep,$(MAKECMDGOALS)))
ifneq ($(and $(filter-out $(PHON),$(MAKECMDGOALS)),$(isdep)),)
 no:=$(shell .extra/depcheck)
 no:=$(if $(filter-out, 0,$(.SHELLSTATUS)),$(error depcheck failed with error $(.SHELLSTATUS)))
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
	installabs="$(installabs)" .extra/install

uninstall:
	installabs="$(installabs)" .extra/uninstall

tests: $(OBJECTS)

nodep: #means make will not generate dependencies

nodown: #means make will not descend into subdirectories to remake things

.refresh: #for removing stray .mk.2 files after make is done
	$(if $(DOTMK2),rm $(DOTMK2))

.%.mk: %.cpp
	.extra/depgen $*

unmake: .refresh #removes all test object files
	$(if $(GENO),rm $(GENO))

remake: unmake #remakes all test object files
	MAKELEVEL=$(MAKELEVEL) make tests

scrape: .refresh #removes all test object files and also generated rules
	$(if $(GENERATED),rm $(GENERATED))

clean: scrape
	cd libs && make clean

reset: scrape
	cd libs && make reset
	MAKELEVEL= make nodown tests
