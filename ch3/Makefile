# Makefile for pmtext[1, 2, ...].asm

# To check the space and tabs for the correctness of Makefile, use: cat -e -t -v Makefile
# https://stackoverflow.com/questions/16931770/makefile4-missing-separator-stop

ASM:=pmtest1.asm
# no empty space among ".asm,.com,$(ASM)"
COM:=$(subst .asm,.com,$(ASM)) # this is string sub function, or simply: pmtest1.com 


.PHONY : all  # always execute the commands using PHONY

all : $(COM)
	sudo mount -o loop pm.img /mnt	
	sudo cp $(COM) /mnt -v
	sudo umount /mnt

$(COM) : $(ASM)
    # $<: first prerequisite ($(ASM)), $@: target of the rule ($(COM))
	nasm $< -o $@ 

.PHONY : clean

clean : 
	sudo umount /mnt
	rm -f $(COM)