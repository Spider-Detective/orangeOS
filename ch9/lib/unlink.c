#include "type.h"
#include "config.h"
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
#include "hd.h"

PUBLIC int unlink(const char* pathname) {
    MESSAGE msg;
    msg.type = UNLINK;

    msg.PATHNAME = (void*)pathname;
    msg.NAME_LEN = strlen(pathname);

    send_recv(BOTH, TASK_FS, &msg);

    return msg.RETVAL;
}