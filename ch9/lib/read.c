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

/*
 * Read from a given file descriptor
 * fd: file descriptor
 * buf: buffer to store the read bytes
 * count: length of bytes to read
 * Return number of bytes read if success; otherwise return -1
 */
PUBLIC int read(int fd, void *buf, int count) {
    MESSAGE msg;
    msg.type = READ;
    msg.FD = fd;
    msg.BUF = buf;
    msg.CNT = count;

    send_recv(BOTH, TASK_FS, &msg);

    return msg.CNT;
}