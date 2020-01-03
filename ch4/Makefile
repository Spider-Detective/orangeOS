# Makefile for loader.asm
# loader.asm: the simple loader printing 'L', Chapter 4
# loader2.asm: the loader printing random chars, Chapter 3.4

# To check the space and tabs for the correctness of Makefile, use: cat -e -t -v Makefile
# https://stackoverflow.com/questions/16931770/makefile4-missing-separator-stop

BOOT:=boot.asm
LDR:=loader.asm
LDR2:=loader2.asm
# no empty space among ".asm,.com,$(ASM)"
BOOT_BIN:=$(subst .asm,.bin,$(BOOT)) # this is string sub function, or simply: pmtest1.com 
LDR_BIN:=$(subst .asm,.bin,$(LDR))


.PHONY : all  # always execute the commands using PHONY

all : $(BOOT_BIN) $(LDR_BIN)
	dd if=$(BOOT_BIN) of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(LDR_BIN) /mnt/floppy/ -v
	sudo umount /mnt/floppy/

clean : 
	rm -f $(BOOT_BIN) $(LDR_BIN)
	sudo umount /mnt/

$(BOOT_BIN) : $(BOOT)
    # $<: first prerequisite ($(ASM)), $@: target of the rule ($(COM))
	nasm $< -o $@ 

$(LDR_BIN) : $(LDR)
	nasm $< -o $@ 