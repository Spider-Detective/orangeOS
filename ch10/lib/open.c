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

PUBLIC int open(const char* pathname, int flags) {
    MESSAGE msg;
    msg.type = OPEN;
    msg.PATHNAME = (void*)pathname;
    msg.FLAGS = flags;
    msg.NAME_LEN = strlen(pathname);

    send_recv(BOTH, TASK_FS, &msg);
    assert(msg.type == SYSCALL_RET);

    return msg.FD;
}