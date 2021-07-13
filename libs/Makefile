PHON:=headers clean reset scrape unmake remake .reset install uninstall
.DEFAULT_GOAL:=all
not=$(if $(1),,true)
comma:=,
sources=$(wildcard *.cpp)
mk2:=$(wildcard *.mk.2)
isdep:=$(call not,$(filter nodep,$(MAKECMDGOALS)))

ifneq ($(if $(MAKECMDGOALS),$(and $(filter-out $(PHON),$(MAKECMDGOALS)),$(isdep)),true),)
 include $(patsubst %.cpp,.%.mk,$(wildcard *.cpp))
endif
headers:
	.extra/depcheck
	$(if $(filter-out nodown,$(MAKECMDGOALS)),,cd headers && make)

.PHONY: all nodep nodown $(PHON)

all: .shared.so .static.a

nodep: #means makefile won't generate dependencies for objects

nodown: #means makefile won't descend into another directory

install: headers
	.extra/install
	MAKELEVEL=$(MAKELEVEL) make nodown all

uninstall:
	.extra/uninstall

.%.mk: %.cpp
	.extra/depgen $^

.shared.so: $(patsubst %.cpp,.%.o,$(sources))
	$(if $^,gcc -shared -Wl$(comma)-z$(comma)defs -o .shared.so $^ -lstdc++)

.static.a: $(patsubst %.cpp,.%.o,$(sources))
	$(if $(wildcard .static.a),rm .static.a)
	$(if $^,ar rcs .static.a $^)

scrape:
	$(if $(wildcard .shared.so),rm .shared.so)
	$(if $(wildcard .static.a),rm .static.a)
	$(if $(wildcard .*.o),rm .*.o)
	$(if $(wildcard .*.mk),rm .*.mk)

reset: scrape
	cd headers && make reset
	MAKELEVEL=$(MAKELEVEL) make nodown all

clean: scrape
	cd headers && make clean

remake: clean
	MAKELEVEL=$(MAKELEVEL) make nodown all
