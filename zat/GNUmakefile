# Makefile for GNU Make.
# $Id$

include ../Makedefs.faerion.include

TARGET  = executable
INCDIR += errors
PROG    = zat-$(VERSION)
SRCS    = $(wildcard *.cc errors/*.cc)
LIBS   += stdc++

custom-clean = custom-clean
custom-cleandist = custom-cleandist

ifeq ($(MDF_OSNAME),NT)
  PROG := $(PROG).exe
endif

include ../Makedefs.faerion

custom-clean:
	rm -f z80.out z80.sym

custom-cleandist:
	rm -f $(PROG)

test: all
	./$(PROG) -c ../share/cpu/Z80 -d +T -I ../test/01 -s zat.sym -S +m -m zat.map Z80.asm
	ls -ld zat.*

benchmark: all
	./$(PROG) -c ../share/cpu/Z80 -d +T -I ../test/01 -s zat.sym -m zat.map Z80.asm
	ls -ld zat.*
