# Makefile for GNU Make.
# $Id$

include Makedefs.faerion.include

DIRS     = lib zas zodump
PACK     = zat
DISTLIST = share/LICENSE share/cpu/Z80
DISTMASK = *.cc *.h GNUmakefile

custom-cleandist = custom-cleandist

include Makedefs.faerion

fake: all
	rm -rf fake-$(VERSION)
	mkdir -p fake-$(VERSION)/$(BINPATH)
	mkdir -p fake-$(VERSION)/$(SHAREPATH)/cpu
	cp zas/zas-$(VERSION) fake-$(VERSION)/$(BINPATH)/
	ln -s zas-$(VERSION) fake-$(VERSION)/$(BINPATH)/zas
	cp zodump/zodump-$(VERSION) fake-$(VERSION)/$(BINPATH)/
	ln -s zodump-$(VERSION) fake-$(VERSION)/$(BINPATH)/zodump
	cp share/cpu/Z80 fake-$(VERSION)/$(SHAREPATH)/cpu/
	cp share/LICENSE fake-$(VERSION)/$(SHAREPATH)/

custom-cleandist:
	rm -rf fake-$(VERSION) configure.h

ifeq ($(MDF_OSNAME),openbsd)
install: dist
	cd dist/openbsd && CDROM_SITE="$(SRCPATH)" make
else
install: fake
  ifeq ($(PREFIX),)
	@echo "The PREFIX envar is not defined."
  else
	$(SUDO) cp -R $(wildcard fake-$(VERSION)/*) $(PREFIX)/ && rm -rf fake-$(VERSION)
  endif
endif

dist: dist-default
