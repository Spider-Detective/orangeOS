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
 * Write to a given file descriptor
 * fd: file descriptor
 * buf: buffer storing the bytes to write
 * count: length of bytes to write
 * Return number of bytes written if success; otherwise return -1
 */
PUBLIC int write(int fd, const void *buf, int count) {
    MESSAGE msg;
    msg.type = WRITE;
    msg.FD = fd;
    msg.BUF = (void*)buf;
    msg.CNT = count;

    send_recv(BOTH, TASK_FS, &msg);

    return msg.CNT;
}