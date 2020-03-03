#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "global.h"
#include "proto.h"

// close a file
PUBLIC int close(int fd) {
    MESSAGE msg;
    msg.type = CLOSE;
    msg.FD = fd;

    send_recv(BOTH, TASK_FS, &msg);

    return msg.RETVAL;
}