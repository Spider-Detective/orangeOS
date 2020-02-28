/*
 * Constants for tty
 */

#ifndef _ORANGES_TTY_H_
#define _ORANGES_TTY_H_

#define TTY_IN_BYTES       256    /* tty input curcular buffer size */
#define TTY_OUT_BUF_LEN    2      /* tty output buffer */

struct s_tty;
struct s_console;

/* TTY, still a circular buffer */
typedef struct s_tty {
    u32    ibuf[TTY_IN_BYTES];  /* tty input buffer */
    u32*   ibuf_head;          /* next pos for write */
    u32*   ibuf_tail;          /* next pos for read */
    int    ibuf_cnt;           /* size of used buffer */

    int    tty_caller;         // who called, usually FS
    int    tty_procnr;         // which process wants the chars
    void*  tty_req_buf;        // the addr where the chars to put
    int    tty_left_cnt;       // requested chars
    int    tty_trans_cnt;      // transferred chars

    struct  s_console*    console;
} TTY;

#endif /* _ORANGES_TTY_H_ */