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

PRIVATE void set_cursor(u32 position);

PUBLIC void init_screen(TTY* p_tty) {
    int nr_tty = p_tty - tty_table;
    p_tty->p_console = console_table + nr_tty;

    int v_mem_size = V_MEM_SIZE >> 1;   // size in unit of WORD

    int con_v_mem_size                   = v_mem_size / NR_CONSOLES;
    p_tty->p_console->original_addr      = nr_tty * con_v_mem_size;
    p_tty->p_console->v_mem_limit        = con_v_mem_size;
    p_tty->p_console->current_start_addr = p_tty->p_console->original_addr;
    p_tty->p_console->cursor             = p_tty->p_console->original_addr; 

    if (nr_tty == 0) {
        p_tty->p_console->cursor = disp_pos / 2;
        disp_pos = 0;
    } else {
        out_char(p_tty->p_console, nr_tty + '0');
        out_char(p_tty->p_console, '#');
    }

    set_cursor(p_tty->p_console->cursor);
}

PUBLIC int is_current_console(CONSOLE* p_con) {
    return (p_con == &console_table[nr_current_console]);
}

// print out ch and set cursor
PUBLIC void out_char(CONSOLE* p_con, char ch) {
    u8* p_vmem = (u8*)(V_MEM_BASE + disp_pos);

    *p_vmem++ = ch;
    *p_vmem++ = DEFAULT_CHAR_COLOR;
    disp_pos += 2;

    set_cursor(disp_pos / 2);  // divided by 2 since each console position contains 2 bytes
}

// make the cursor follows console's I/O
PRIVATE void set_cursor(u32 position) {
    disable_int();
    out_byte(CRTC_ADDR_REG, CURSOR_H);    // write into the addr reg port the reg you want to manipulate
    out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);   // then write into the data reg port
    out_byte(CRTC_ADDR_REG, CURSOR_L);
    out_byte(CRTC_DATA_REG, position & 0xFF);   
    enable_int();
}