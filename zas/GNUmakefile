# Makefile for GNU Make.
# $Id$

include ../Makedefs.faerion.include

TARGET  = executable
INCDIR += errors ../lib
PROG    = zas-$(VERSION)
SRCS    = $(wildcard *.cc errors/*.cc)
ARCH   += ../lib/zalib.a
LIBS   += stdc++

custom-clean = custom-clean
custom-cleandist = custom-cleandist

ifeq ($(MDF_OSNAME),NT)
  PROG := $(PROG).exe
endif

include ../Makedefs.faerion

custom-clean:
	rm -f zas.obj zas.sym zas.map

custom-cleandist:
	rm -f $(PROG)

test: all
	./$(PROG) -c ../share/cpu/Z80 -d +T -I ../test/01 -s zas.sym -S +m -m zas.map Z80.asm
	ls -ld zas.*

benchmark: all
	./$(PROG) -c ../share/cpu/Z80 -d +T -I ../test/01 -s zas.sym -m zas.map Z80.asm
	ls -ld zas.*
