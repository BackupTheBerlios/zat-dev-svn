# Makefile for GNU Make.
# $Id$

include Makedefs.faerion.include

DIRS = zat
LIST = configure configure.in GNUmakefile Makedefs.faerion \
       docs/README.* share/LICENSE share/cpu/Z80 zat/GNUmakefile \
       zat/*.cc zat/*.h zat/support/*.cc zat/errors/*.h \
       zat/errors/*.cc

custom-cleandist = custom-cleandist

include Makedefs.faerion

fake: all
	rm -rf fake-$(VERSION)
	mkdir -p fake-$(VERSION)/$(BINPATH)
	mkdir -p fake-$(VERSION)/$(SHAREPATH)/cpu
	cp zat/zat-$(VERSION) fake-$(VERSION)/$(BINPATH)/
	ln -s zat-$(VERSION) fake-$(VERSION)/$(BINPATH)/zat
	cp share/cpu/Z80 fake-$(VERSION)/$(SHAREPATH)/cpu/
	cp share/LICENSE fake-$(VERSION)/share/

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

dist:
	rm -rf zat-$(VERSION)
	mkdir zat-$(VERSION)
	tar cf tmp.tar $(LIST)
	cd zat-$(VERSION) && tar xf ../tmp.tar && cd ..
	rm -f tmp.tar
	tar cfz zat-$(VERSION).tar.gz zat-$(VERSION)
	rm -rf zat-$(VERSION)
