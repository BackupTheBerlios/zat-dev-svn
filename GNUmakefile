# Makefile for GNU Make.
# $Id$

include Makedefs.faerion.include

DIRS     = lib zat zodump
PACK     = zat
DISTLIST = share/LICENSE share/cpu/Z80
DISTMASK = *.cc *.h GNUmakefile

custom-cleandist = custom-cleandist

include Makedefs.faerion

fake: all
	rm -rf fake-$(VERSION)
	mkdir -p fake-$(VERSION)/$(BINPATH)
	mkdir -p fake-$(VERSION)/$(SHAREPATH)/cpu
	cp zat/zat-$(VERSION) fake-$(VERSION)/$(BINPATH)/
	ln -s zat-$(VERSION) fake-$(VERSION)/$(BINPATH)/zat
	cp zodump/zodump-$(VERSION) fake-$(VERSION)/$(BINPATH)/
	ln -s zodump-$(VERSION) fake-$(VERSION)/$(BINPATH)/zodump
	cp share/cpu/Z80 fake-$(VERSION)/$(SHAREPATH)/cpu/
	cp share/LICENSE fake-$(VERSION)/share/

custom-cleandist:
	rm -rf fake-$(VERSION) configure.h

install: fake
  ifeq ($(PREFIX),)
	@echo "The PREFIX envar is not defined."
  else
	$(SUDO) cp -R $(wildcard fake-$(VERSION)/*) $(PREFIX)/ && rm -rf fake-$(VERSION)
  endif

test: install
	cd test/01 && $(MAKE)

debug: install
	cd test/01 && $(MAKE) debug

dist: dist-default
