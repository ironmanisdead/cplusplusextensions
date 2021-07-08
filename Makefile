.DEFAULT_GOAL:=help
.SECONDEXPANSION:
null:=
slash:=\$(null)
open:=(
close:=)
comma:=,
semi:=;
colon:=:
not=$(if $(1),,true)
installdir:=/usr/local
installabs:=$(abspath $(installdir))
ifneq ($(findstring /$(shell pwd),/$(installabs)),)
 $(error cannot install libcustomutils inside source directory)
endif
include .extra/list.mk
PHON:=.extra/list.mk help remake unmake scrape clean reset install uninstall deps
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

.extra/list.mk:
	@echo '#this file is automatically generated by make' >.extra/list.mk
	@echo -n 'SOURCES:=' >>.extra/list.mk
	.extra/makelist '*.cpp' >>.extra/list.mk

deps:
	make nodep $(RULES)

help:
	$(info please select 'install', 'tests', 'uninstall', or a specific '.o' file that you wish to generate)

install:
	installabs='$(subst ','$(slash)'',$(installabs))' .extra/install

uninstall:
	installabs='$(subst ','$(slash)'',$(installabs))' .extra/uninstall

tests: $(OBJECTS)

nodep: #means make will not generate dependencies

nodown: #means make will not descend into subdirectories to remake things

.%.mk: %.cpp
	installabs='$(subst ','$(slash)'',$(installabs))' $\
		   .extra/depgen '$*'

unmake: #removes all test object files
	.extra/rmobj

remake: unmake #remakes all test object files
	MAKELEVEL=$(MAKELEVEL) make $\
		  installabs='$(subst ','$(slash)'',$(installabs))' tests

scrape: unmake #removes all test object files and also generated rules
	.extra/rmgen

clean: scrape
	cd libs && make clean

reset: scrape
	cd libs && make $\
		installabs='$(subst ','$(slash)'',$(installabs))' reset
	MAKELEVEL= make $\
		   installabs='$(subst ','$(slash)'',$(installabs))' nodown tests
