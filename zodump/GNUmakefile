# Makefile for GNU Make.
# $Id$

include ../Makedefs.faerion.include

TARGET  = executable
INCDIR += ../lib
PROG    = zodump-$(VERSION)
SRCS    = $(wildcard *.cc)
ARCH   += ../lib/zalib.a
LIBS   += stdc++

ifeq ($(MDF_OSNAME),NT)
  PROG := $(PROG).exe
endif

include ../Makedefs.faerion

test: all
	ls -ld ../zat/zat.obj
	./$(PROG) ../zat/zat.obj
	./$(PROG) -d ../zat/zat.obj
