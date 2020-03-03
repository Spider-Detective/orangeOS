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

#define TTY_FIRST       (tty_table)
#define TTY_END         (tty_table + NR_CONSOLES)

PRIVATE void init_tty(TTY* tty);
PRIVATE void tty_dev_read(TTY* tty);
PRIVATE void tty_dev_write(TTY* tty);
PRIVATE void tty_do_read(TTY* tty, MESSAGE* msg);
PRIVATE void tty_do_write(TTY* tty, MESSAGE* msg);
PRIVATE void put_key(TTY* tty, u32 key);

// loop over all consoles and do write and read if necessary
PUBLIC void task_tty() {
    TTY*     tty;
    MESSAGE  msg;

    init_keyboard();
    for (tty = TTY_FIRST; tty < TTY_END; tty++) {
        init_tty(tty);
    }
    select_console(0);
    while (1) {
        for (tty = TTY_FIRST; tty < TTY_END; tty++) {
            do {
                tty_dev_read(tty);
                tty_dev_write(tty);
            } while (tty->ibuf_cnt);
        }

        send_recv(RECEIVE, ANY, &msg);

        int src = msg.source;
        assert(src != TASK_TTY);

        TTY* ptty = &tty_table[msg.DEVICE];

        switch(msg.type) {
            case DEV_OPEN:
                reset_msg(&msg);
                msg.type = SYSCALL_RET;
                send_recv(SEND, src, &msg);
                break;
            case DEV_READ:
                tty_do_read(ptty, &msg);
                break;
            case DEV_WRITE:
                tty_do_write(ptty, &msg); 
                break;
            case HARD_INT:
                // triggered by clock handler for key pressed
                key_pressed = 0;
                continue;
            default:
                dump_msg("TTY::unknown msg", &msg);
                break;
        }

    }
}

// init the circular buffer and console in tty struct
PRIVATE void init_tty(TTY* tty) {
    tty->ibuf_cnt = 0;
    tty->ibuf_head = tty->ibuf_tail = tty->ibuf;

    init_screen(tty);
}

PUBLIC void in_process(TTY* tty, u32 key) {
    if (!(key & FLAG_EXT)) {   // FLAG_EXT means it is not a printable char
        put_key(tty, key);
    } else {
        int raw_code = key & MASK_RAW;  // remove the non-functional key flags
        switch(raw_code) {
            case ENTER:
                put_key(tty, '\n');
                break;
            case BACKSPACE:
                put_key(tty, '\b');
                break;
            case UP:
                if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                    // scroll DOWN the console by one line
                    scroll_screen(tty->console, SCR_DN);
                }
                break;
            case DOWN:
                if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                    scroll_screen(tty->console, SCR_UP);
                }
                break;
            case F1:
            case F2:
            case F3:
            case F4:
            case F5:
            case F6:
            case F7:
            case F8:
            case F9:
            case F10:
            case F11:
            case F12:
                // to shift between consoles on Mac, use fn + control + F1/F2/F3
                if ((key & FLAG_CTRL_L) || (key & FLAG_CTRL_R)) {
                    select_console(raw_code - F1);
                } 
                // else {
                //     if (raw_code == F12) {
                //         disable_int();
                //         dump_proc(proc_table + 4);
                //         for (;;);
                //     }
                // }
                break;
            default:
                break;
        }
    }
}

// write the char into the circular buffer of tty for reading
PRIVATE void put_key(TTY* tty, u32 key) {
    if (tty->ibuf_cnt < TTY_IN_BYTES) {
        *(tty->ibuf_head) = key;
        tty->ibuf_head++;
        if (tty->ibuf_head == tty->ibuf + TTY_IN_BYTES) {
            tty->ibuf_head = tty->ibuf;
        }
        tty->ibuf_cnt++;
    }
}

// get input from keyboard of current console
PRIVATE void tty_dev_read(TTY* tty) {
    if (is_current_console(tty->console)) {
        keyboard_read(tty);
    }
}

// echo the input chars and send it to the waiting process
PRIVATE void tty_dev_write(TTY* tty) {
    while (tty->ibuf_cnt) {
        char ch = *(tty->ibuf_tail);
        tty->ibuf_tail++;
        if (tty->ibuf_tail == tty->ibuf + TTY_IN_BYTES) {
            tty->ibuf_tail = tty->ibuf;
        }
        tty->ibuf_cnt--;

        // if there is still chars needed to write
        if (tty->tty_left_cnt) {
            if (ch >= ' ' && ch <= '~') {  // if printable
                out_char(tty->console, ch);
                void* p = tty->tty_req_buf + tty->tty_trans_cnt;
                phys_copy(p, (void*)va2la(TASK_TTY, &ch), 1);
                tty->tty_trans_cnt++;
                tty->tty_left_cnt--;
            // if backspace, remove the last char
            } else if (ch == '\b' && tty->tty_trans_cnt) {
                out_char(tty->console, ch);
                tty->tty_trans_cnt--;
                tty->tty_left_cnt++;
            }

            // if pressed enter or no more char left, finish writing and send msg to fs
            if (ch == '\n' || tty->tty_left_cnt == 0) {
                out_char(tty->console, '\n');
                MESSAGE msg;
                msg.type = RESUME_PROC;
                msg.PROC_NR = tty->tty_procnr;
                msg.CNT = tty->tty_trans_cnt;
                send_recv(SEND, tty->tty_caller, &msg);
                tty->tty_left_cnt = 0;
            }
        }
    }
}

/* simply read the msg info and set into tty
 * then send FS SUSPEND_PROC immediately to suspend the process
 * the subsequent reading will be handled by tty_dev_read and tty_dev_write 
 * in while loop of task_tty
 */ 
PRIVATE void tty_do_read(TTY* tty, MESSAGE* msg) {
    // update tty, see def in tty,h
    tty->tty_caller = msg->source;
    tty->tty_procnr = msg->PROC_NR;
    tty->tty_req_buf = va2la(tty->tty_procnr, msg->BUF);
    tty->tty_left_cnt = msg->CNT;
    tty->tty_trans_cnt = 0;

    msg->type = SUSPEND_PROC;
    msg->CNT = tty->tty_left_cnt;
    send_recv(SEND, tty->tty_caller, msg);
}

// simply write the chars in msg.BUF to console
PRIVATE void tty_do_write(TTY* tty, MESSAGE* msg) {
    char buf[TTY_OUT_BUF_LEN];
    char* p = (char*)va2la(msg->PROC_NR, msg->BUF);
    int i = msg->CNT;
    int j;

    while (i) {
        int bytes = min(TTY_OUT_BUF_LEN, i);
        phys_copy(va2la(TASK_TTY, buf), (void*)p, bytes);
        for (j = 0; j < bytes; j++) {
            out_char(tty->console, buf[j]);
        }
        i -= bytes;
        p += bytes;
    }
    msg->type = SYSCALL_RET;
    send_recv(SEND, msg->source, msg);
}

PUBLIC int sys_printx(int _unused1, int _unused2, char* s, struct proc* p_proc) {
    const char* p;
    char ch;

    char reenter_err[] = "? k_reenter is incorrect for unknown reason";
    reenter_err[0] = MAG_CH_PANIC;
    /*
     * printx() can be called in Ring 0 or Ring 1-3
     * When it is called in Ring 0, printx() generates a interrupt, 
     *  thus k_reenter will be increased, so k_reenter > 0
     * When it is called in Ring 1-3, k_reenter == 0, and we need to do
     *  linear-physical address mapping
     */
    if (k_reenter == 0) {          // printx() called in Ring 1-3
        p = va2la(proc2pid(p_proc), s);
    } else if (k_reenter > 0) {
        p = s;
    } else {
        p = reenter_err;
    }

    // If panic, or task's assert failure, hlt the whole system
    if ((*p == MAG_CH_PANIC) ||
        (*p == MAG_CH_ASSERT && p_proc_ready < &proc_table[NR_TASKS])) {
        disable_int();
        char* v = (char*)V_MEM_BASE;     // directly write into video mem
        const char* q = p + 1;           // skip the magic char

        // write to whole screen, 
        while (v < (char*)(V_MEM_BASE + V_MEM_SIZE)) {
            *v++ = *q++;
            *v++ = RED_CHAR;
            // if q reaches the end, turn the existing char on screen to be gray
            // in 16 lines, then rollback q to the beginning of p
            if (!*q) {  
                while (((int)v - V_MEM_BASE) % (SCR_WIDTH * 16)) {
                    v++;
                    *v++ = GRAY_CHAR;
                }
                q = p + 1;
            }
        }

        // The __volatile__ modifier on an __asm__ block forces the 
        // compiler's optimizer to execute the code as-is
        __asm__ __volatile__("hlt");
    }

    // otherwise simply print the string s (or p)
    while ((ch = *p++) != 0) {
        if (ch == MAG_CH_PANIC || ch == MAG_CH_ASSERT) {
            continue;      // skip the magic char
        }
        out_char(TTY_FIRST->console, ch);
    }

    return 0;
}

PUBLIC void dump_tty_buf() {
    TTY* tty = &tty_table[1];

    static char sep[] = "--------------------------------\n";

	printl(sep);

	printl("head: %d\n", tty->ibuf_head - tty->ibuf);
	printl("tail: %d\n", tty->ibuf_tail - tty->ibuf);
	printl("cnt: %d\n", tty->ibuf_cnt);

	int pid = tty->tty_caller;
	printl("caller: %s (%d)\n", proc_table[pid].name, pid);
	pid = tty->tty_procnr;
	printl("caller: %s (%d)\n", proc_table[pid].name, pid);

	printl("req_buf: %d\n", (int)tty->tty_req_buf);
	printl("left_cnt: %d\n", tty->tty_left_cnt);
	printl("trans_cnt: %d\n", tty->tty_trans_cnt);

	printl("--------------------------------\n");

	strcpy(sep, "\n");
}