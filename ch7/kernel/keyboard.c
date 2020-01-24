#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "keymap.h"

PRIVATE KB_INPUT       kb_in;

PUBLIC void keyboard_handler(int irq) {
    u8 scan_code = in_byte(KB_DATA);    // see spec for 8042 and 8048, Table 7.1
    if (kb_in.count < KB_IN_BYTES) {
        *(kb_in.p_head) = scan_code;
        kb_in.p_head++;
        if (kb_in.p_head == kb_in.buf + KB_IN_BYTES) {
            kb_in.p_head = kb_in.buf;
        }
        kb_in.count++;
    }
}

PUBLIC void keyboard_read() {
    u8     scan_code;
    char   output[2];    // first byte is the char mapped from keymap, second is \0 for now
    int    make;

    memset(output, 0, 2);

    if (kb_in.count > 0) {
        disable_int();
        scan_code = *(kb_in.p_tail);
        kb_in.p_tail++;
        if (kb_in.p_tail == kb_in.buf + KB_IN_BYTES) {
            kb_in.p_tail = kb_in.buf;
        }
        kb_in.count--;
        enable_int();

        if (scan_code == 0xE1) {

        } else if (scan_code == 0xE0) {

        } else {
            make = (scan_code & FLAG_BREAK) ? FALSE : TRUE;   // mask_code | 0x80 == break_code, see keymap.h
            if (make) {
                // get the char from keymap
                output[0] = keymap[(scan_code & 0x7F) * MAP_COLS];
                disp_str(output);
            }
        }

        // disp_int(scan_code);
    }
}

PUBLIC void init_keyboard() {
    // initialize the circular buffer
    kb_in.count = 0;
    kb_in.p_head = kb_in.p_tail = kb_in.buf;

    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}