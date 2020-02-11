#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

#define TTY_FIRST       (tty_table)
#define TTY_END         (tty_table + NR_CONSOLES)

PRIVATE void init_tty(TTY* p_tty);
PRIVATE void tty_do_read(TTY* p_tty);
PRIVATE void tty_do_write(TTY* p_tty);
PRIVATE void put_key(TTY* p_tty, u32 key);

// loop over all consoles and do write and read if necessary
PUBLIC void task_tty() {
    TTY*    p_tty;

    // panic("in TTY");
    // assert(0);

    init_keyboard();
    for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++) {
        init_tty(p_tty);
    }
    select_console(0);
    while (1) {
        for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++) {
            tty_do_read(p_tty);
            tty_do_write(p_tty); 
        }
    }
}

// init the circular buffer and console in tty struct
PRIVATE void init_tty(TTY* p_tty) {
    p_tty->inbuf_count = 0;
    p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;

    init_screen(p_tty);
}

PUBLIC void in_process(TTY* p_tty, u32 key) {
    char output[2] = {'\0', '\0'};

    if (!(key & FLAG_EXT)) {   // FLAG_EXT means it is not a printable char
        put_key(p_tty, key);
    } else {
        int raw_code = key & MASK_RAW;  // remove the non-functional key flags
        switch(raw_code) {
            case ENTER:
                put_key(p_tty, '\n');
                break;
            case BACKSPACE:
                put_key(p_tty, '\b');
                break;
            case UP:
                if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                    // scroll DOWN the console by one line
                    scroll_screen(p_tty->p_console, SCR_DN);
                }
                break;
            case DOWN:
                if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                    scroll_screen(p_tty->p_console, SCR_UP);
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
                if ((key & FLAG_CTRL_L) || (key & FLAG_CTRL_R)) {
                    select_console(raw_code - F1);
                } else {
                    if (raw_code == F12) {
                        disable_int();
                        dump_proc(proc_table + 4);
                        for (;;);
                    }
                }
                break;
            default:
                break;
        }
    }
}

// write the char into the circular buffer of tty for reading
PRIVATE void put_key(TTY* p_tty, u32 key) {
    if (p_tty->inbuf_count < TTY_IN_BYTES) {
        *(p_tty->p_inbuf_head) = key;
        p_tty->p_inbuf_head++;
        if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES) {
            p_tty->p_inbuf_head = p_tty->in_buf;
        }
        p_tty->inbuf_count++;
    }
}

// only read when the console is current active console
PRIVATE void tty_do_read(TTY* p_tty) {
    if (is_current_console(p_tty->p_console)) {
        keyboard_read(p_tty);
    }
}

PRIVATE void tty_do_write(TTY* p_tty) {
    if (p_tty->inbuf_count) {
        char ch = *(p_tty->p_inbuf_tail);
        p_tty->p_inbuf_tail++;
        if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES) {
            p_tty->p_inbuf_tail = p_tty->in_buf;
        }
        p_tty->inbuf_count--;

        out_char(p_tty->p_console, ch);
    }
}

// write the buf into console of tty
PUBLIC void tty_write(TTY* p_tty, char* buf, int len) {
    char* p = buf;
    int i = len;

    while (i) {
        out_char(p_tty->p_console, *p++);
        i--;
    }
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
                while (((int)v - V_MEM_BASE) % (SCREEN_WIDTH * 16)) {
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
        out_char(tty_table[p_proc->nr_tty].p_console, ch);
    }

    return 0;
}