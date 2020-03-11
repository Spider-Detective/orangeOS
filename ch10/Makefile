# Makefile for orangeS

# To check the space and tabs for the correctness of Makefile, use: cat -e -t -v Makefile
# https://stackoverflow.com/questions/16931770/makefile4-missing-separator-stop

# To print out the dependency needed in Makefile for $filename$:
# gcc -M $filename$ -I include

# Entry point of orangeS: same value as KRNL_ENT_PT_PHY_ADDR in load.inc
ENTRYPOINT           = 0x1000
# Offset of entry point in kernel
ENTRYOFFSET          = 0x400

ASM                  = nasm
DASM                 = objdump
CC                   = gcc
LD                   = ld
ASMBFLAGS            = -I boot/include/      # for boot.bin
ASMKFLAGS            = -I include/ -I include/sys/ -f elf    # for kernel.bin
CFLAGS               = -I include/ -I include/sys/ -m32 -c -fno-stack-protector -fno-builtin -Wall
LDFLAGS              = -m elf_i386 -s -Ttext $(ENTRYPOINT) -Map krnl.map
DASMFLAGS            = -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)

ORANGESBOOT          = boot/boot.bin boot/loader.bin
ORANGESKERNEL        = kernel.bin
OBJS                 = kernel/kernel.o lib/syscall.o kernel/start.o kernel/main.o\
					   kernel/clock.o kernel/keyboard.o kernel/tty.o kernel/console.o\
					   kernel/i8259.o kernel/global.o kernel/protect.o kernel/proc.o\
                       kernel/systask.o kernel/hd.o\
					   lib/printf.o lib/vsprintf.o\
					   lib/klib.o lib/kliba.o lib/string.o lib/misc.o\
					   lib/open.o lib/close.o lib/read.o lib/write.o lib/unlink.o\
					   lib/getpid.o lib/syslog.o\
					   lib/fork.o lib/exit.o lib/wait.o\
					   mm/main.o mm/forkexit.o\
					   fs/main.o fs/open.o fs/misc.o fs/read_write.o\
					   fs/link.o fs/disklog.o
DASMOUTPUT           = kernel.bin.asm

FLOPPY               = /mnt/floppy/
IMG                  = a.img

.PHONY : everything final image clean realclean disasm all buildimg

everything : $(ORANGESBOOT) $(ORANGESKERNEL)

image : realclean everything clean buildimg

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
                    boot/include/fat12hdr.inc boot/include/pm.inc# boot/include/lib.inc
		$(ASM) $(ASMBFLAGS) -o $@ $<

$(ORANGESKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

kernel/kernel.o : kernel/kernel.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

lib/syscall.o : lib/syscall.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/start.o : kernel/start.c
	$(CC) $(CFLAGS) -o $@ $<

kernel/main.o : kernel/main.c
	$(CC) $(CFLAGS) -o $@ $<

kernel/clock.o: kernel/clock.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/keyboard.o: kernel/keyboard.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/tty.o: kernel/tty.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/console.o: kernel/console.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o : kernel/i8259.c
	$(CC) $(CFLAGS) -o $@ $<	

kernel/global.o : kernel/global.c
	$(CC) $(CFLAGS) -o $@ $<	

kernel/protect.o : kernel/protect.c
	$(CC) $(CFLAGS) -o $@ $<

kernel/proc.o : kernel/proc.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/systask.o: kernel/systask.c 
	$(CC) $(CFLAGS) -o $@ $<

kernel/hd.o : kernel/hd.c
	$(CC) $(CFLAGS) -o $@ $<

lib/printf.o: lib/printf.c 
	$(CC) $(CFLAGS) -o $@ $<

lib/vsprintf.o: lib/vsprintf.c 
	$(CC) $(CFLAGS) -o $@ $<

lib/klib.o : lib/klib.c
	$(CC) $(CFLAGS) -o $@ $<

lib/misc.o : lib/misc.c
	$(CC) $(CFLAGS) -o $@ $<

lib/kliba.o : lib/kliba.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

lib/string.o : lib/string.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

lib/open.o : lib/open.c
	$(CC) $(CFLAGS) -o $@ $<

lib/close.o : lib/close.c
	$(CC) $(CFLAGS) -o $@ $<

lib/read.o : lib/read.c
	$(CC) $(CFLAGS) -o $@ $<

lib/write.o : lib/write.c
	$(CC) $(CFLAGS) -o $@ $<

lib/unlink.o : lib/unlink.c
	$(CC) $(CFLAGS) -o $@ $<

lib/getpid.o : lib/getpid.c
	$(CC) $(CFLAGS) -o $@ $<

lib/syslog.o : lib/syslog.c
	$(CC) $(CFLAGS) -o $@ $<

lib/fork.o : lib/fork.c
	$(CC) $(CFLAGS) -o $@ $<

lib/exit.o : lib/exit.c
	$(CC) $(CFLAGS) -o $@ $<

lib/wait.o : lib/wait.c
	$(CC) $(CFLAGS) -o $@ $<

mm/main.o : mm/main.c
	$(CC) $(CFLAGS) -o $@ $<

mm/forkexit.o : mm/forkexit.c
	$(CC) $(CFLAGS) -o $@ $<

fs/main.o : fs/main.c
	$(CC) $(CFLAGS) -o $@ $<

fs/open.o : fs/open.c
	$(CC) $(CFLAGS) -o $@ $<

fs/read_write.o : fs/read_write.c
	$(CC) $(CFLAGS) -o $@ $<

fs/misc.o : fs/misc.c
	$(CC) $(CFLAGS) -o $@ $<

fs/link.o : fs/link.c
	$(CC) $(CFLAGS) -o $@ $<

fs/disklog.o : fs/disklog.c
	$(CC) $(CFLAGS) -o $@ $<