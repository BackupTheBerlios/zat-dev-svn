# Makefile for GNU Make.
# $Id$

include ../Makedefs.faerion.include

TARGET  = executable
INCDIR += errors
PROG    = zodump-$(VERSION)
SRCS    = $(wildcard *.cc)
LIBS   += stdc++

ifeq ($(MDF_OSNAME),NT)
  PROG := $(PROG).exe
endif

include ../Makedefs.faerion
