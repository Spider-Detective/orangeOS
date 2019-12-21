# Makefile for pmtext[1, 2, ...].asm

# To check the space and tabs for the correctness of Makefile, use: cat -e -t -v Makefile
# https://stackoverflow.com/questions/16931770/makefile4-missing-separator-stop

ASM:=boot.asm
# no empty space among ".asm,.com,$(ASM)"
BIN:=$(subst .asm,.bin,$(ASM)) # this is string sub function, or simply: pmtest1.com 


.PHONY : all  # always execute the commands using PHONY

all : $(BIN)
	dd if=$(BIN) of=a.img bs=512 count=1 conv=notrunc

$(BIN) : $(ASM)
    # $<: first prerequisite ($(ASM)), $@: target of the rule ($(COM))
	nasm $< -o $@ 

.PHONY : clean

clean : 
	rm -f $(BIN)