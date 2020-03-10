#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

PRIVATE void cleanup(struct proc* proc);

// fork syscall
PUBLIC int do_fork() {
    return 0;
}

// exit syscall
PUBLIC void do_exit(int status) {

}

PRIVATE void cleanup(struct proc* proc) {

}

PUBLIC void do_wait() {
    
}