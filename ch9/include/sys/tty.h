/*
 * Constants for tty
 */

#ifndef _ORANGES_TTY_H_
#define _ORANGES_TTY_H_

#define TTY_IN_BYTES    256    /* tty input curcular buffer size */

struct s_console;

/* TTY, still a circular buffer */
typedef struct s_tty {
    u32    in_buf[TTY_IN_BYTES];  /* tty input buffer */
    u32*   p_inbuf_head;          /* next pos for write */
    u32*   p_inbuf_tail;          /* next pos for read */
    int    inbuf_count;           /* size of used buffer */

    struct  s_console*    p_console;
} TTY;

#endif /* _ORANGES_TTY_H_ */