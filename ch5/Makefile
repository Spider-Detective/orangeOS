# Makefile

# To check the space and tabs for the correctness of Makefile, use: cat -e -t -v Makefile
# https://stackoverflow.com/questions/16931770/makefile4-missing-separator-stop

BOOT:=boot.asm
LDR:=loader.asm
KERNEL:=kernel.asm
STRING:=string.asm
KLIBA:=kliba.asm
START:=start.c
# no empty space among ".asm,.com,$(ASM)"
BOOT_BIN:=$(subst .asm,.bin,$(BOOT)) # this is string sub function, or simply: pmtest1.com 
LDR_BIN:=$(subst .asm,.bin,$(LDR))
KERNEL_O:=$(subst .asm,.o,$(KERNEL))
STRING_O:=$(subst .asm,.o,$(STRING))
KLIBA_O:=$(subst .asm,.o,$(KLIBA))
START_O:=$(subst .c,.o,$(START))
KERNEL_BIN:=$(subst .asm,.bin,$(KERNEL))

FLOPPY:=/mnt/floppy/

.PHONY : all  # always execute the commands using PHONY

all : $(BOOT_BIN) $(LDR_BIN) $(KERNEL_BIN)
	dd if=$(BOOT_BIN) of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img $(FLOPPY)
	sudo cp $(LDR_BIN) $(FLOPPY) -v
	sudo cp $(KERNEL_BIN) $(FLOPPY) -v
	sudo umount $(FLOPPY)

.PHONY : clean
clean : 
	-rm -f $(BOOT_BIN) $(LDR_BIN) $(KERNEL_BIN) *.o
	-sudo umount /mnt/

$(BOOT_BIN) : $(BOOT)
    # $<: first prerequisite ($(BOOT)), $@: target of the rule ($(BOOT_BIN))
	nasm $< -o $@ 

$(LDR_BIN) : $(LDR)
	nasm $< -o $@ 

$(KERNEL_BIN) : $(KERNEL_O) $(STRING_O) $(START_O) $(KLIBA_O)
	ld -m elf_i386 -s -Ttext 0x30400 -o $@ $(KERNEL_O) $(STRING_O) $(START_O) $(KLIBA_O)

$(KERNEL_O) : $(KERNEL)
	nasm -f elf -o $@ $<

$(STRING_O) : $(STRING)
	nasm -f elf -o $@ $<

$(KLIBA_O) : $(KLIBA)
	nasm -f elf -o $@ $<

$(START_O) : $(START)
	gcc -m32 -c -fno-builtin -o $@ $<	