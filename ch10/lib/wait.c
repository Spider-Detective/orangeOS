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
#include "proto.h"

/*
 * wait for the child process to terminate
 * status: the value returned by child
 * return: the PID of the terminated child
 */
PUBLIC int wait(int* status) {
    MESSAGE msg;
    msg.type = WAIT;

    send_recv(BOTH, TASK_MM, &msg);
    *status = msg.STATUS;

    return (msg.PID == NO_TASK ? -1 : msg.PID);
}