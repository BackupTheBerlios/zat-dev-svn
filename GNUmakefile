# Makefile for GNU Make.
# $Id: GNUmakefile,v 1.1.1.1 2004/09/13 21:16:29 vhex Exp $

include Makedefs.faerion.include

DIRS = zat zatp

custom-cleandist = custom-cleandist

include Makedefs.faerion

fake: all
	rm -rf fake-$(VERSION)
	mkdir -p fake-$(VERSION)/$(BINPATH)
	mkdir -p fake-$(VERSION)/$(SHAREPATH)/cpu
	cp zat/zat-$(VERSION) fake-$(VERSION)/$(BINPATH)/
	ln -s zat-$(VERSION) fake-$(VERSION)/$(BINPATH)/zat
	cp zatp/zatp-$(VERSION) fake-$(VERSION)/$(BINPATH)
	ln -s zatp-$(VERSION) fake-$(VERSION)/$(BINPATH)/zatp
	cp share/cpu/Z80 fake-$(VERSION)/$(SHAREPATH)/cpu
	cp share/LICENSE fake-$(VERSION)/share

custom-cleandist:
	rm -rf fake-$(VERSION)

install: fake
  ifeq ($(PREFIX),)
	@echo "The PREFIX envar is not defined."
  else
	cp -R fake-$(VERSION)/* $(PREFIX)/ && rm -rf fake-$(VERSION)
  endif

test: install
	cd test/01 && $(MAKE)

debug: install
	cd test/01 && $(MAKE) debug
