# This script is to solve the issue encountered when using git on VM:
# >error: object file .git/objects/ce/theRef is empty 
# >error: object file .git/objects/ce/theRef is empty fatal: loose object theRef (stored in .git/objects/ce/theRef) is corrupt

find .git/objects/ -size 0 -exec rm -f {} \;
git fetch origin
