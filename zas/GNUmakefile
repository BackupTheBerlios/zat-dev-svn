# Makefile for GNU Make.
# $Id$

include ../Makedefs.faerion.include

TARGET  = executable
INCDIR += errors ../lib
PROG    = zas-$(VERSION)
SRCS    = $(wildcard *.cc errors/*.cc)
ARCH   += ../lib/zalib.a
LIBS   += stdc++
CMDLINE = -c ../share/cpu/Z80 -d T -I ../share/tests/Z80 -s zas.sym -m zas.map 01.asm 01.asm 01.asm 01.asm 01.asm 01.asm 01.asm 01.asm 01.asm 01.asm

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
	./$(PROG) $(CMDLINE) > zas.log 2>&1
	ls -ld zas.obj zas.map zas.sym zas.log

debug: all
	echo "tb main" > zas.gdb
	echo "r $(CMDLINE)" >> zas.gdb
	gdb -x zas.gdb ./$(PROG)
