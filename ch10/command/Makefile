# Makefile for command/, run-time C library
# Makefile cheatsheet: https://devhints.io/makefile

# Entry point of orangeS: same value as KRNL_ENT_PT_PHY_ADDR in load.inc
ENTRYPOINT           = 0x1000

HD                   = ../80m.img
ASM                  = nasm
DASM                 = objdump
CC                   = gcc
LD                   = ld
ASMFLAGS             = -I ../include/ -f elf
CFLAGS               = -I ../include/ -m32 -c -fno-stack-protector -fno-builtin -Wall
LDFLAGS              = -m elf_i386 -Ttext $(ENTRYPOINT)
DASMFLAGS            = -D

LIB                  = ../lib/orangescrt.a
BIN                  = echo pwd

.PHONY : everything install clean realclean

everything : $(BIN)

# tar kernel and apps into one file, then use dd to write into the hard drive
install : realclean everything clean
	cp ../kernel.bin ./ -v
	tar vcf inst.tar kernel.bin $(BIN)
	dd if=inst.tar of=$(HD) seek=`echo "obase=10;ibase=16;(\`egrep -e '^ROOT_BASE' ../boot/include/load.inc | sed -e 's/.*0x//g'\`+\`egrep -e '#define[[:space:]]*INSTALL_START_SECT' ../include/sys/config.h | sed -e 's/.*0x//g'\`)*200" | bc` bs=1 count=`ls -l inst.tar | awk -F " " '{print $$5}'` conv=notrunc

clean : 
	-rm -f *.o

realclean : 
	-rm -f $(BIN) *.o

# $^: all prerequisites, $?: all new prerequisites
kernel.bin : 
	cp ../kernel.bin ./

start.o : start.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

echo.o : echo.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

echo : echo.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

pwd.o : pwd.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<
	
pwd : pwd.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?