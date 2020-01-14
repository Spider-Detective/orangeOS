# orangeOS

Git repo on learning to make a toy OS. 

## Install Bochs on Ubuntu:
### Without debugger:
```bash
sudo apt-get install vgabios bochs bochs-x bximage
```

### With debugger:
1. Download Bochs from http://bochs.sourceforge.net/
2. In terminal, run below:
```shell
tar -vxzf bochs-2.6.9.tar.gz
cd bochs-2.6.9
./configure --enable-debugger --enable-disasm
make
sudo make install
```

## Create floppy image in Bochs:
* In terminal, run:
```bximage```.
* Follow the instruction and select the option **fd**, then select a name for the floppy

## Run toy OS in each ch*/ folder
### For ch1/
* To compile: 
```nasm boot.asm -o boot.bin```
* Create floppy image **a.img** by ```bximage```
* To write into floppy 
```dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc```
* To run in Bochs **with debugger**:
```bochs```.
Then type enter, type c

### For ch3/ and ch4/, [FreeDos](http://bochs.sourceforge.net/diskimages.html) is used as the boot image
* Create floppy image **pm.img** by ```bximage```
* To compile and write into floppy: ```make all``` (```make clean``` also available)
* To start the FreeDos image: ```bochs```.
* To start the OS image: 
    * in Bochs terminal, clean up floppy b by ``` format B: ``` (for the first time only) 
    * type ```B:\[image_name].com```
    
### For ch5/ and on, boot.bin is ready
* Create floppy image **a.img** by ```bximage```
* To compile and write into floppy, please review the Makefile, or simply: 
```make image```
* To run in Bochs **with debugger**:
```bochs```.
Then type enter, type c
