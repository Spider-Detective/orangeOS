#!/bin/bash
# $0 the script name, $1: .com system file, $2: floppy name
COM_NAME="$1"
FLOPPY_NAME="$2"
echo "Start to copy $1 to $2"
sudo mount -o loop $2 /mnt
sudo cp $1 /mnt
sudo umount /mnt