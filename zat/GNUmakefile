# Makefile for GNU Make.
# $Id: GNUmakefile,v 1.2 2004/09/14 08:28:46 vhex Exp $

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
	./zaa -i z80.asm -o z80.out -s z80.sym -dd
