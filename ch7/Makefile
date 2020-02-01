# Makefile for orangeS

# To check the space and tabs for the correctness of Makefile, use: cat -e -t -v Makefile
# https://stackoverflow.com/questions/16931770/makefile4-missing-separator-stop

# To print out the dependency needed in Makefile for $filename$:
# gcc -M $filename$ -I include

# Entry point of orangeS: same value as KernelEntryPointPhyAddr in load.inc
ENTRYPOINT           = 0x30400
# Offset of entry point in kernel
ENTRYOFFSET          = 0x400

ASM                  = nasm
DASM                 = ndisasm
CC                   = gcc
LD                   = ld
ASMBFLAGS            = -I boot/include/      # for boot.bin
ASMKFLAGS            = -I include/ -f elf    # for kernel.bin
CFLAGS               = -I include/ -m32 -c -fno-stack-protector -fno-builtin 
LDFLAGS              = -m elf_i386 -s -Ttext $(ENTRYPOINT)
DASMFLAGS            = -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)

ORANGESBOOT          = boot/boot.bin boot/loader.bin
ORANGESKERNEL        = kernel.bin
OBJS                 = kernel/kernel.o kernel/syscall.o kernel/start.o kernel/main.o kernel/proc.o\
                               kernel/i8259.o kernel/global.o kernel/protect.o kernel/clock.o kernel/console.o\
							   kernel/keyboard.o kernel/tty.o lib/klib.o lib/kliba.o lib/string.o\
							   kernel/printf.o kernel/vsprintf.o
DASMOUTPUT           = kernel.bin.asm
FLOPPY               = /mnt/floppy/
IMG                  = a.img

.PHONY : everything final image clean realclean disasm all buildimg

everything : $(ORANGESBOOT) $(ORANGESKERNEL)

all : realclean everything

final : all clean

image : final buildimg

clean : 
	-rm -f $(OBJS)

realclean : 
	-rm -f $(OBJS) $(ORANGESBOOT) $(ORANGESKERNEL)

disasm : 
	$(DASM) $(DASMFLAGS) $(ORANGESKERNEL) > $(DASMOUTPUT)

buildimg : 
	dd if=boot/boot.bin of=$(IMG) bs=512 count=1 conv=notrunc
	sudo mount -o loop $(IMG) $(FLOPPY)
	sudo cp -fv boot/loader.bin $(FLOPPY) -v
	sudo cp -fv kernel.bin $(FLOPPY) -v
	sudo umount $(FLOPPY)

# $<: first prerequisite (boot/boot.asm), $@: target of the rule (boot/boot.bin)
boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
		$(ASM) $(ASMBFLAGS) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/load.inc \
                    boot/include/fat12hdr.inc boot/include/pm.inc boot/include/lib.inc
		$(ASM) $(ASMBFLAGS) -o $@ $<

$(ORANGESKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

kernel/kernel.o : kernel/kernel.asm include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/syscall.o : kernel/syscall.asm include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/start.o : kernel/start.c include/type.h include/const.h include/protect.h \
				   include/proto.h include/proc.h include/string.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/main.o : kernel/main.c include/type.h include/const.h include/protect.h \
				   include/proto.h include/proc.h include/string.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o : kernel/i8259.c include/type.h include/const.h include/protect.h \
				   include/proto.h
	$(CC) $(CFLAGS) -o $@ $<	

kernel/global.o : kernel/global.c include/type.h include/const.h include/protect.h \
				   include/proto.h include/proc.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<	

kernel/protect.o : kernel/protect.c include/type.h include/const.h include/protect.h \
				   include/proto.h include/proc.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/proc.o : kernel/proc.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/clock.o: kernel/clock.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/keyboard.o: kernel/keyboard.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/tty.o: kernel/tty.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/console.o: kernel/console.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/printf.o: kernel/printf.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/vsprintf.o: kernel/vsprintf.c 
	$(CC) $(CFLAGS) -o $@ $<

lib/klib.o : lib/klib.c include/type.h include/const.h include/protect.h \
				   include/proto.h include/proc.h include/string.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

lib/kliba.o : lib/kliba.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

lib/string.o : lib/string.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<	