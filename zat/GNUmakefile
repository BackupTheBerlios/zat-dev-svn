# Makefile for GNU Make.
# $Id$

include ../Makedefs.faerion.include

TARGET = executable
PROG   = zat-$(VERSION)
SRCS   = $(wildcard *.cc support/*.cc)
LIBS  += stdc++

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
	./$(PROG) -c ../share/cpu/Z80 -dd ../test/01/Z80.asm
