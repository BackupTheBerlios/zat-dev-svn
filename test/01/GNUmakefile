include ../../Makedefs.faerion.include

all::
	zat -o Z80.out -s Z80.sym -ddd Z80.asm > Z80.log 2>&1

fine::
	zat -o Z80.out -s Z80.sym Z80.asm

debug::
ifneq ($(wildcard Z80.gdb),)
	gdb -x Z80.gdb zat
else
	@echo "Please make a Z80.gdb file in this directory."
endif
